/*! *********************************************************************************
* \addtogroup Temperature Collector
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Temperature Collector application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework / Drivers */
#include "EmbeddedTypes.h"
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "Panic.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "SerialManager.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"
#if !defined(MULTICORE_APPLICATION_CORE) || (!MULTICORE_APPLICATION_CORE)
#include "gatt_db_handles.h"
#else
#define UUID128(name, ...) uint8_t name[16] = { __VA_ARGS__ };
#include "gatt_uuid128.h"
#undef UUID128
#endif

/* Profile / Services */
#include "ble_conn_manager.h"
#include "ble_service_discovery.h"
#include "ApplMain.h"
#include "temperature_collector.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
#include "erpc_host.h"
#endif

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum appEvent_tag{
    mAppEvt_PeerConnected_c,
    mAppEvt_PairingComplete_c,
    mAppEvt_ServiceDiscoveryComplete_c,
    mAppEvt_ServiceDiscoveryFailed_c,
    mAppEvt_GattProcComplete_c,
    mAppEvt_GattProcError_c,
}appEvent_t;

typedef enum appState_tag{
    mAppIdle_c,
    mAppExchangeMtu_c,
    mAppServiceDisc_c,
    mAppReadDescriptor_c,
    mAppRunning_c,
}appState_t;

typedef struct appLockerKeyServiceConfig_tag
{
	uint16_t hLockerKeyService;
	uint16_t hAccessKeyCharacteristic;
	uint16_t hMaterialKeyCharacteristic;
	uint16_t hNameCharacteristic;
	uint16_t hAvailableACharacteristic;
	uint16_t hAvailableBCharacteristic;
	uint16_t hAvailableCCharacteristic;
	uint16_t hDummyCharacteristic;
} appLockerKeyServiceConfig_t;

typedef struct appCustomInfo_tag
{
    appLockerKeyServiceConfig_t 	lockerkeyClientConfig;
}appCustomInfo_t;

typedef struct appPeerInfo_tag
{
    deviceId_t      deviceId;
    appCustomInfo_t customInfo;
    bool_t          isBonded;
    appState_t      appState;
}appPeerInfo_t;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static appPeerInfo_t mPeerInformation;
static bool_t   mScanningOn = FALSE;
static bool_t   mFoundDeviceToConnect = FALSE;

tmrTimerID_t mRfidUartTimerId;
tmrTimerID_t mBleLockerStatusTimerId;

bool_t isKeyAvailable = FALSE;
bool_t mRequestBLEStatusReceived = FALSE;
uint8_t mRFIDName[18];			//16 bytes
uint8_t mRFIDRegister[18];		//8 bytes
uint8_t mLockerKey[4];
uint8_t mMaterialKey[4];

typedef enum rfid_state_tag
{
	RFID_ReadUserNameAndRegister = 0,
	RFID_WriteUserNameAndRegister,
	RFID_WriteLockerUserKey,
	RFID_WriteLockerMaterialKey,
	RFID_RemoveCard
} rfid_state_t;

typedef enum ble_user_request_state_tag
{
	BLE_Idle = 0,
	BLE_WriteLockerMaterialKeyCharacteristic,
	BLE_WriteNameCharacteristic,
	BLE_WriteAvailableCharacteristic,
	BLE_WriteDummySyncCharacteristic,
	BLE_WrteUpdateInformationAfterPOR,
	BLE_ReadUserLockerKeyA,
	BLE_ReadUserLockerKeyB,
	BLE_ReadUserLockerKeyC
} ble_state_t;

extern uint8_t RFID_State;
static uint8_t BLE_UserRequestState = BLE_Idle;
static uint8_t BLE_UserReadUserKey = BLE_Idle;
uint8_t BLE_UserReadUserKeyList[4] = {0xFF, 0xFF, 0xFF, 0x00};
static uint8_t BLE_UserRequestLockerId;

/* Buffer used for Characteristic related procedures */
static gattAttribute_t      *mpCharProcBuffer = NULL;
static uint8_t gAppSerMgrIf;
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Host Stack callbacks */
static void BleApp_ScanningCallback
(
    gapScanningEvent_t* pScanningEvent
);

static void BleApp_ConnectionCallback
(
    deviceId_t peerDeviceId,
    gapConnectionEvent_t* pConnectionEvent
);

static void BleApp_GattClientCallback
(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
);

static void BleApp_ServiceDiscoveryCallback
(
    deviceId_t peerDeviceId,
    servDiscEvent_t* pEvent
);

static void BleApp_Config(void);

void BleApp_StateMachineHandler
(
    deviceId_t peerDeviceId,
    appEvent_t event
);

static bool_t CheckScanEvent(gapScannedDevice_t* pData);

static void BleApp_StoreServiceHandles
(
    gattService_t   *pService
);

static void BleApp_WriteLockerAccessKey (uint8_t KeyAccessN, uint8_t *Key);
static void BleApp_WriteName (uint8_t LockerN);
static void BleApp_WriteUnavailableLocker (uint8_t LockerN);
static void BleApp_WriteLockerMaterialTag (uint8_t LockerN, uint8_t MaterialN, uint8_t *Tag);
static void BleApp_ReadAvailableLocker (uint8_t LockerN);
static void BleApp_WriteDummySyncByte (void);

static void Uart_RxCallBack (void *pData);
void AppPrintString (const char* pBuff);
void RfidUartTimerCallback (void * pParam);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void)
{
#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    /* Get the serial interface in use */
    gAppSerMgrIf = BOARD_GetSerialManagerInterface();
#else
    SerialManager_Init();
    Serial_InitInterface(&gAppSerMgrIf, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    Serial_SetBaudRate(gAppSerMgrIf, APP_SERIAL_INTERFACE_SPEED);
    Serial_SetRxCallBack(gAppSerMgrIf, Uart_RxCallBack, NULL);
#endif

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
    /* Init eRPC host */
    init_erpc_host();
#endif
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    if (!mScanningOn)
    {
        /* Start scanning */
        (void)App_StartScanning(&gScanParams, BleApp_ScanningCallback, gGapDuplicateFilteringEnable_c, gGapScanContinuously_d, gGapScanPeriodicDisabled_d);
    }
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    switch (events)
    {
        /* Start on button press if low-power is disabled */
        case gKBD_EventPressPB1_c:
        {
            BleApp_Start();
            break;
        }

        /* Disconnect on long button press */
        case gKBD_EventLongPB1_c:
        {
            if (mPeerInformation.deviceId != gInvalidDeviceId_c)
            {
                (void)Gap_Disconnect(mPeerInformation.deviceId);
            }
            break;
        }

        case gKBD_EventPressPB2_c:  /* Fall-through */
        case gKBD_EventLongPB2_c:   /* Fall-through */
        default:
        {
            ; /* No action required */
            break;
        }
    }
}

/*! *********************************************************************************
* \brief        Handles BLE generic callback.
*
* \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
********************************************************************************** */
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    /* Call BLE Conn Manager */
    BleConnManager_GenericEvent(pGenericEvent);

    switch (pGenericEvent->eventType)
    {
    	case gInitializationComplete_c:
        {
            BleApp_Config();
        }
        break;
    	default:
        {
            ; /* No action required */
        }
        break;
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief        Configures BLE Stack after initialization. Usually used for
*               configuring advertising, scanning, white list, services, et al.
*
********************************************************************************** */
static void BleApp_Config(void)
{
    /* Configure as GAP Central */
    BleConnManager_GapCommonConfig();

    /* Register for callbacks*/
    (void)App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    BleServDisc_RegisterCallback(BleApp_ServiceDiscoveryCallback);

    /* Allocate Timer for UART Events */
    mRfidUartTimerId = TMR_AllocateTimer();
    /* Initialize private variables */
    mPeerInformation.appState = mAppIdle_c;
    mPeerInformation.deviceId = gInvalidDeviceId_c;
    mScanningOn = FALSE;
    mFoundDeviceToConnect = FALSE;
}

/*! *********************************************************************************
* \brief        Handles BLE Scanning callback from host stack.
*
* \param[in]    pScanningEvent    Pointer to gapScanningEvent_t.
********************************************************************************** */
static void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
    switch (pScanningEvent->eventType)
    {
        case gDeviceScanned_c:
        {
            /* Check if the scanned device implements the Temperature Custom Profile */
            if( FALSE == mFoundDeviceToConnect )
            {
                mFoundDeviceToConnect = CheckScanEvent(&pScanningEvent->eventData.scannedDevice);

                if (mFoundDeviceToConnect)
                {
                    /* Set connection parameters and stop scanning. Connect on gScanStateChanged_c. */
                    gConnReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;
                    FLib_MemCpy(gConnReqParams.peerAddress,
                                pScanningEvent->eventData.scannedDevice.aAddress,
                                sizeof(bleDeviceAddress_t));

                    (void)Gap_StopScanning();
                }
            }
        }
        break;

        case gScanStateChanged_c:
        {
            mScanningOn = !mScanningOn;

            /* Node starts scanning */
            if (mScanningOn)
            {
                mFoundDeviceToConnect = FALSE;
            }
            /* Node is not scanning */
            else
            {
                /* Connect with the previously scanned peer device */
                if (mFoundDeviceToConnect)
                {
                    (void)App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
                }
            }
        }
        break;

        case gScanCommandFailed_c:
        {
            ; /* No action required */
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles BLE Connection callback from host stack.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    pConnectionEvent    Pointer to gapConnectionEvent_t.
********************************************************************************** */
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    /* Connection Manager to handle Host Stack interactions */
    BleConnManager_GapCentralEvent(peerDeviceId, pConnectionEvent);

    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
        	/* BLE connected message */
        	//AppPrintString("F0:1\r\n");

        	/* Update application status */
            mPeerInformation.deviceId = peerDeviceId;
            mPeerInformation.isBonded = FALSE;
            BleApp_StateMachineHandler(mPeerInformation.deviceId, mAppEvt_PeerConnected_c);
        }
        break;

        case gConnEvtDisconnected_c:
        {
        	/* Update the BLE Status of the Java Database */
        	/* BLE disconnected message */
        	AppPrintString("F0:0\r\n");

        	/* Update application status */
            mPeerInformation.deviceId = gInvalidDeviceId_c;
            mPeerInformation.appState = mAppIdle_c;
            /* Reset Service Discovery to be sure*/
            BleServDisc_Stop(peerDeviceId);
        }
        break;
        default:
        ; /* No action required */
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles discovered services.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    pEvent              Pointer to servDiscEvent_t.
********************************************************************************** */
static void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t* pEvent)
{
    switch(pEvent->eventType)
    {
        /* Store the discovered handles for later use. */
        case gServiceDiscovered_c:
        {
            BleApp_StoreServiceHandles(pEvent->eventData.pService);
        }
        break;

        /* Service discovery has finished, run the state machine. */
        case gDiscoveryFinished_c:
        {
            if (pEvent->eventData.success)
            {
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_ServiceDiscoveryComplete_c);
            }
            else
            {
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_ServiceDiscoveryFailed_c);
            }
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Stores handles for the specified service.
*
* \param[in]    pService    Pointer to gattService_t.
********************************************************************************** */
static void BleApp_StoreServiceHandles
(
    gattService_t   *pService
)
{
    uint8_t i;

    if ((pService->uuidType == gBleUuidType128_c) &&
    		FLib_MemCmp(pService->uuid.uuid128, uuid_LockerKey_service, 16))
    {
    	/* Found Locker Key Service */
    	mPeerInformation.customInfo.lockerkeyClientConfig.hLockerKeyService = pService->startHandle;

    	if ((pService->cNumCharacteristics > 0U) &&
    		(pService->aCharacteristics != NULL))
		{
    		/* Found Access Key Characteristics */
    		for (i = 0; i < pService->cNumCharacteristics; i++)
    		{
    			if (FLib_MemCmp(&pService->aCharacteristics[i].value.uuid, &uuid_AccessKey_char, 16))
    			{
    				mPeerInformation.customInfo.lockerkeyClientConfig.hAccessKeyCharacteristic = pService->aCharacteristics[i].value.handle;
    			}
    			else if(FLib_MemCmp(&pService->aCharacteristics[i].value.uuid, &uuid_MaterialKey_char, 16))
    			{
    				mPeerInformation.customInfo.lockerkeyClientConfig.hMaterialKeyCharacteristic = pService->aCharacteristics[i].value.handle;
    			}
    			else if(FLib_MemCmp(&pService->aCharacteristics[i].value.uuid, &uuid_Name_char, 16))
    			{
    				mPeerInformation.customInfo.lockerkeyClientConfig.hNameCharacteristic = pService->aCharacteristics[i].value.handle;
    			}
    			else if(FLib_MemCmp(&pService->aCharacteristics[i].value.uuid, &uuid_AvailableA_char, 16))
    			{
    				mPeerInformation.customInfo.lockerkeyClientConfig.hAvailableACharacteristic = pService->aCharacteristics[i].value.handle;
    			}
    			else if(FLib_MemCmp(&pService->aCharacteristics[i].value.uuid, &uuid_AvailableB_char, 16))
    			{
    				mPeerInformation.customInfo.lockerkeyClientConfig.hAvailableBCharacteristic = pService->aCharacteristics[i].value.handle;
    			}
    			else if(FLib_MemCmp(&pService->aCharacteristics[i].value.uuid, &uuid_AvailableC_char, 16))
    			{
    				mPeerInformation.customInfo.lockerkeyClientConfig.hAvailableCCharacteristic = pService->aCharacteristics[i].value.handle;
    			}
    			else if(FLib_MemCmp(&pService->aCharacteristics[i].value.uuid, &uuid_Dummy_char, 16))
    			{
    				mPeerInformation.customInfo.lockerkeyClientConfig.hDummyCharacteristic = pService->aCharacteristics[i].value.handle;
    			}
    			else
    			{
    				break;
    			}
    		}
		}
    }
}

/*! *********************************************************************************
* \brief        Handles GATT client callback from host stack.
*
* \param[in]    serverDeviceId      GATT Server device ID.
* \param[in]    procedureType       Procedure type.
* \param[in]    procedureResult     Procedure result.
* \param[in]    error               Callback result.
********************************************************************************** */
static void BleApp_GattClientCallback(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
)
{
	uint8_t i=0;

    if (procedureResult == gGattProcError_c)
    {
        attErrorCode_t attError = (attErrorCode_t)(uint8_t)(error);

        if (attError == gAttErrCodeInsufficientEncryption_c     ||
            attError == gAttErrCodeInsufficientAuthorization_c  ||
            attError == gAttErrCodeInsufficientAuthentication_c)
        {
            /* Start Pairing Procedure */
            (void)Gap_Pair(serverDeviceId, &gPairingParameters);
        }

        BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcError_c);
    }
    else
    {
        if (procedureResult == gGattProcSuccess_c)
        {
            switch(procedureType)
            {
                case gGattProcWriteCharacteristicValue_c:
                {
                	if(BLE_UserRequestState == BLE_WriteLockerMaterialKeyCharacteristic)
                	{
                		BLE_UserRequestState = (uint8_t)BLE_Idle;
                	}
                	else if(BLE_UserRequestState == BLE_WriteNameCharacteristic)
                	{
                		isKeyAvailable = TRUE;
                		BLE_UserRequestState = (uint8_t)BLE_WriteAvailableCharacteristic;
                		BleApp_WriteName(BLE_UserRequestLockerId);

                	}
                	else if(BLE_UserRequestState == BLE_WriteAvailableCharacteristic)
                	{
                		BLE_UserRequestState = (uint8_t)BLE_Idle;
                		BleApp_WriteUnavailableLocker(BLE_UserRequestLockerId);
                	}
                	else if(BLE_UserRequestState == BLE_WriteDummySyncCharacteristic)
                	{
            			BleApp_ReadAvailableLocker('A');
            			BLE_UserRequestState = (uint8_t)BLE_Idle;
                    	BLE_UserReadUserKey = (uint8_t)BLE_ReadUserLockerKeyA;
                	}
                }
                break;
                case gGattProcReadCharacteristicValue_c:
                {
                	if(BLE_UserReadUserKey == BLE_ReadUserLockerKeyA)
                	{
                		BleApp_ReadAvailableLocker('B');
                		BLE_UserReadUserKey = (uint8_t)BLE_ReadUserLockerKeyB;
                	}
                	else if(BLE_UserReadUserKey == BLE_ReadUserLockerKeyB)
                	{
                		BleApp_ReadAvailableLocker('C');
                		BLE_UserReadUserKey = (uint8_t)BLE_ReadUserLockerKeyC;
                	}
                	else if(BLE_UserReadUserKey == BLE_ReadUserLockerKeyC)
                	{
                		for(i=0; i<3; i++)
                		{
                			BLE_UserReadUserKeyList[i] = BLE_UserReadUserKeyList[i] + '0';
                		}

                		AppPrintString("F9:");
                		AppPrintString((const char*)&BLE_UserReadUserKeyList[0]);
                		AppPrintString("\r\n");

                		BLE_UserReadUserKey = (uint8_t)BLE_Idle;
                	}
                }
                break;

                default:
                {
                	/* No action required */
                }
                break;
            }

            BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcComplete_c);
        }
    }

    /* Signal Service Discovery Module */
    BleServDisc_SignalGattClientEvent(serverDeviceId, procedureType, procedureResult, error);

}

/*! *********************************************************************************
* \brief        Process scanning events to search for the Temperature Custom Service.
*               This function is called from the scanning callback.
*
* \param[in]    pData                   Pointer to gapScannedDevice_t.
*
* \return       TRUE if the scanned device implements the Temperature Custom Service,
                FALSE otherwise
********************************************************************************** */
static bool_t CheckScanEvent(gapScannedDevice_t* pData)
{
    uint32_t index = 0;
    bool_t foundMatch = FALSE;

    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;

        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1U];
        adElement.aData = &pData->data[index + 2U];

         /* Search for Locker Key Service */
        if(pData->advEventType == gBleAdvRepScanRsp_c)
        {
        	if(adElement.adType == gAdIncomplete128bitServiceList_c)
        	{
        		if(FLib_MemCmp(adElement.aData, &uuid_LockerKey_service, (adElement.length - 1)))
        		{
        			foundMatch = TRUE;
        		}
        	}
        }
        /* Move on to the next AD element type */
        index += (uint32_t)adElement.length + sizeof(uint8_t);
    }
    return foundMatch;
}

/*! *********************************************************************************
* \brief        State machine handler of the Temperature Collector application.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    event               Event type.
********************************************************************************** */
void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event)
{
    switch (mPeerInformation.appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
            	/* Moving to Exchange MTU State */
            	mPeerInformation.appState = mAppExchangeMtu_c;
                (void)GattClient_ExchangeMtu(peerDeviceId, gAttMaxMtu_c);
            }
        }
        break;

        case mAppExchangeMtu_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                /* Moving to Service Discovery State*/
                mPeerInformation.appState = mAppServiceDisc_c;
                /* Start Service Discovery*/
                (void)BleServDisc_Start(peerDeviceId);
            }
            else
            {
                if (event == mAppEvt_GattProcError_c)
                {
                    (void)Gap_Disconnect(peerDeviceId);
                }
            }
        }
        break;

        case mAppServiceDisc_c:
        {
            if (event == mAppEvt_ServiceDiscoveryComplete_c)
            {
                /* Moving to Running State*/
            	mPeerInformation.appState = mAppRunning_c;
            	/* BLE connected message */
            	AppPrintString("F0:1\r\n");
            }
            else
            {
                if (event == mAppEvt_ServiceDiscoveryFailed_c)
                {
                    (void)Gap_Disconnect(peerDeviceId);
                }
            }
        }
        break;

        case mAppRunning_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                if (mpCharProcBuffer != NULL)
                {
                    (void)MEM_BufferFree(mpCharProcBuffer);
                    mpCharProcBuffer = NULL;
                }
            }
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Enable temperature notifications by writing peer's CCCD of the
                Temperature characteristic.
*
* \return       gBleSuccess_c or gBleOutOfMemory_c
********************************************************************************** */
static void BleApp_WriteLockerAccessKey (uint8_t KeyAccessN, uint8_t *Key)
{
	uint8_t BleLockerAccessKey[5], i=0;
    gattCharacteristic_t characteristic;

    /* Verify if there is a valid peer */
    if (gInvalidDeviceId_c != mPeerInformation.deviceId)
    {
        BleLockerAccessKey[0] = KeyAccessN;

        for(i = 1; i < 5; i++)
        {
        	BleLockerAccessKey[i] = *Key++;
        }

        characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hAccessKeyCharacteristic;

    	(void)GattClient_WriteCharacteristicValue(mPeerInformation.deviceId, &characteristic, 5, &BleLockerAccessKey[0], FALSE, FALSE, FALSE, NULL);
    }
}

/*! *********************************************************************************
* \brief        Enable temperature notifications by writing peer's CCCD of the
                Temperature characteristic.
*
* \return       gBleSuccess_c or gBleOutOfMemory_c
********************************************************************************** */
static void BleApp_WriteLockerMaterialTag (uint8_t LockerN, uint8_t MaterialN, uint8_t *Tag)
{
	uint8_t BleLockerMaterialTag[6], i=0;
    gattCharacteristic_t characteristic;

    /* Verify if there is a valid peer */
    if (gInvalidDeviceId_c != mPeerInformation.deviceId)
    {
    	BleLockerMaterialTag[0] = LockerN;
    	BleLockerMaterialTag[1] = MaterialN;

		for(i = 2; i < 6; i++)
        {
			BleLockerMaterialTag[i] = *Tag++;
        }

        characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hMaterialKeyCharacteristic;

    	(void)GattClient_WriteCharacteristicValue(mPeerInformation.deviceId, &characteristic, 6, &BleLockerMaterialTag[0], FALSE, FALSE, FALSE, NULL);
    }
}

static void BleApp_WriteName (uint8_t LockerN)
{
	uint8_t BleLockerName[17], i=0;
    gattCharacteristic_t characteristic;

    /* Verify if there is a valid peer */
    if (gInvalidDeviceId_c != mPeerInformation.deviceId)
    {
    	BleLockerName[0] = LockerN;

		for(i = 1; i < 17; i++)
        {
			BleLockerName[i] = mRFIDName[i-1];
        }

        characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hNameCharacteristic;

    	(void)GattClient_WriteCharacteristicValue(mPeerInformation.deviceId, &characteristic, 17, &BleLockerName[0], FALSE, FALSE, FALSE, NULL);
    }
}

static void BleApp_WriteUnavailableLocker (uint8_t LockerN)
{
	uint8_t BleLockerBusy[1]={0x01};
    gattCharacteristic_t characteristic;

    /* Verify if there is a valid peer */
    if (gInvalidDeviceId_c != mPeerInformation.deviceId)
    {
    	if(LockerN == 'A')
    	{
    		characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hAvailableACharacteristic;
    	}
    	else if(LockerN == 'B')
    	{
    		characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hAvailableBCharacteristic;
    	}
    	else if(LockerN == 'C')
    	{
    		characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hAvailableCCharacteristic;
    	}

    	(void)GattClient_WriteCharacteristicValue(mPeerInformation.deviceId, &characteristic, 1, &BleLockerBusy[0], FALSE, FALSE, FALSE, NULL);
    }
}

static void BleApp_ReadAvailableLocker (uint8_t LockerN)
{
    gattCharacteristic_t characteristic;

    /* Verify if there is a valid peer */
    if (gInvalidDeviceId_c != mPeerInformation.deviceId)
    {
    	if(LockerN == 'A')
    	{
    		characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hAvailableACharacteristic;
    		characteristic.value.paValue = &BLE_UserReadUserKeyList[0];
    	}
    	else if(LockerN == 'B')
    	{
    		characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hAvailableBCharacteristic;
    		characteristic.value.paValue = &BLE_UserReadUserKeyList[1];
    	}
    	else if(LockerN == 'C')
    	{
    		characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hAvailableCCharacteristic;
    		characteristic.value.paValue = &BLE_UserReadUserKeyList[2];
    	}

    	(void)GattClient_ReadCharacteristicValue(mPeerInformation.deviceId, &characteristic, (uint16_t)1);
    }
}

static void BleApp_WriteDummySyncByte (void)
{
	uint8_t BleDummyArray[1]={0x01};
    gattCharacteristic_t characteristic;

    /* Verify if there is a valid peer */
    if (gInvalidDeviceId_c != mPeerInformation.deviceId)
    {
    	characteristic.value.handle = mPeerInformation.customInfo.lockerkeyClientConfig.hDummyCharacteristic;
    	(void)GattClient_WriteCharacteristicValue(mPeerInformation.deviceId, &characteristic, 1, &BleDummyArray[0], FALSE, FALSE, FALSE, NULL);
    }
}

/*! *********************************************************************************
* \brief        Prints a string.
*
********************************************************************************** */
void AppPrintString(const char* pBuff)
{
    (void)Serial_Print(gAppSerMgrIf, pBuff, gAllowToBlock_d);
}

static void KeysFormat (uint8_t *pAscii, uint8_t *pHex)
{
	while(*pAscii && *pAscii != '\r')
	{
		*pHex = (((*pAscii++ - '0') << 4) & 0xF0);
		*pHex++ |= ((*pAscii++ - '0') & 0x0F);
	}
}

static void Uart_RxCallBack(void *pData)
{
	static uint8_t SerialRxBuff[30];
	static uint16_t SerialRxBuffCount = 0;
	uint16_t SerialBytesRead = 0;
	uint8_t i = 0;

	(void)Serial_Read(gAppSerMgrIf, &SerialRxBuff[SerialRxBuffCount], 30, &SerialBytesRead);
	SerialRxBuffCount += SerialBytesRead;

	/* This character indicates the end of a valid frame*/
	if(SerialRxBuff[SerialRxBuffCount-1] == '\n')
	{
		/* BLE Status Request Packet Received */
		if(FLib_MemCmp(&SerialRxBuff[0], "F0:", 3))
		{
			if(mPeerInformation.deviceId != gInvalidDeviceId_c)
			{
				AppPrintString("F0:1\r\n");
			}
		}
		/* BLE Write New User Name Packet Received */
		else if(FLib_MemCmp(&SerialRxBuff[0], "F1:", 3))
		{
			/* Flush the RFID Name Buffer */
			for(i = 0; i < 18; i++)
			{
				mRFIDName[i] = 0;
			}

			/* Store the Name */
			i = 0;
			while(SerialRxBuff[3+i] != '\r')
			{
				mRFIDName[i] = SerialRxBuff[3+i];
				i++;
			}
		}
		/* BLE Write New User Register Packet Received */
		else if(FLib_MemCmp(&SerialRxBuff[0], "F2:", 3))
		{
			/* Flush the RFID Name Buffer */
			for(i = 0; i < 18; i++)
			{
				mRFIDRegister[i] = 0;
			}

			/* Store the Register*/
			i = 0;
			while(SerialRxBuff[3+i] != '\r')
			{
				mRFIDRegister[i] = SerialRxBuff[3+i];
				i++;
			}

			RFID_State = (uint8_t)RFID_WriteUserNameAndRegister;
		}
		/* BLE Write Locker Key Packet Received */
		else if(FLib_MemCmp(&SerialRxBuff[0], "F3:", 3))
		{
			KeysFormat(&SerialRxBuff[4], &mLockerKey[0]);
			BleApp_WriteLockerAccessKey(SerialRxBuff[3], &mLockerKey[0]);
			BLE_UserRequestLockerId = SerialRxBuff[3];
			BLE_UserRequestState = (uint8_t)BLE_WriteNameCharacteristic;
			RFID_State = (uint8_t)RFID_WriteLockerUserKey;
		}
		/* BLE Write Material Key Packet Received */
		else if(FLib_MemCmp(&SerialRxBuff[0], "F4:", 3))
		{
			KeysFormat(&SerialRxBuff[5], &mMaterialKey[0]);
			BleApp_WriteLockerMaterialTag(SerialRxBuff[3], SerialRxBuff[4], &mMaterialKey[0]);
			RFID_State = (uint8_t)RFID_WriteLockerMaterialKey;
		}
		else if(FLib_MemCmp(&SerialRxBuff[0], "F7", 2))
		{
			if(SerialRxBuff[2] == 'A')
			{
				KeysFormat(&SerialRxBuff[5], &mLockerKey[0]);
				BleApp_WriteLockerAccessKey(SerialRxBuff[4], &mLockerKey[0]);
			}
			else if(SerialRxBuff[2] == 'B')
			{
				/* Flush the RFID Name Buffer */
				for(i = 0; i < 18; i++)
				{
					mRFIDName[i] = 0;
				}
				/* Store the Name */
				i = 0;
				while(SerialRxBuff[5+i] != '\r')
				{
					mRFIDName[i] = SerialRxBuff[5+i];
					i++;
				}
				BleApp_WriteName(SerialRxBuff[4]);
				BLE_UserRequestLockerId = SerialRxBuff[4];
				BLE_UserRequestState = (uint8_t)BLE_WriteAvailableCharacteristic;
			}
			else if (SerialRxBuff[2] == 'C')
			{
				KeysFormat(&SerialRxBuff[6], &mMaterialKey[0]);

				if(SerialRxBuff[5] == '1')
				{
					BleApp_WriteLockerMaterialTag(SerialRxBuff[4], 'A', &mMaterialKey[0]);
				}
				else if(SerialRxBuff[5] == '2')
				{
					BleApp_WriteLockerMaterialTag(SerialRxBuff[4], 'B', &mMaterialKey[0]);
				}
				else
				{
					BleApp_WriteLockerMaterialTag(SerialRxBuff[4], 'C', &mMaterialKey[0]);
				}
			}

		}
		/* BLE Read Material Key Packet Received */
		else if(FLib_MemCmp(&SerialRxBuff[0], "F8:", 3))
		{
			BleApp_WriteDummySyncByte();
			BLE_UserRequestState = (uint8_t)BLE_WriteDummySyncCharacteristic;
		}

		/* Flush the Serial Rx Buffer and reset the byte counter */
		for(i = 0; i < 30; i++)
		{
			SerialRxBuff[i] = 0;
		}

		SerialRxBuffCount = 0;
	}

}

void RfidUartTimerCallback (void * pParam)
{
	static uint8_t mToggle = 0;
	uint8_t mBuffName[20] = {"F5:"};
	uint8_t mBuffRegister[20] = {"F6:"};
	uint8_t i = 0;

	if(mToggle & 1)
	{
		while(mRFIDRegister[i] && i < 8)
		{
			mBuffRegister[i+3] = mRFIDRegister[i];
			i++;
		}
		AppPrintString((const char*)&mBuffRegister[0]);
		TMR_StopTimer(mRfidUartTimerId);
	}
	else
	{
		while(mRFIDName[i] && i < 16)
		{
			mBuffName[i+3] = mRFIDName[i];
			i++;
		}
		AppPrintString((const char*)&mBuffName[0]);
	}
	AppPrintString("\r\n");
	mToggle++;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
