/*! *********************************************************************************
* \addtogroup Heart Rate Sensor
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Heart Rate Sensor application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework / Drivers */
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "SerialManager.h"
#include "Panic.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_db_app_interface.h"
#include "gap_interface.h"

#if MULTICORE_APPLICATION_CORE
#include "dynamic_gatt_database.h"
#else
#include "gatt_db_handles.h"
#endif

/* Profile / Services */
#include "device_info_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"
#include "ble_service_discovery.h"

/* BLE Application */
#include "board.h"
#include "ApplMain.h"
#include "heart_rate_sensor.h"

#if MULTICORE_APPLICATION_CORE
#include "erpc_host.h"
#include "dynamic_gatt_database.h"
#endif

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
/* Adv State */
static bool_t  	   mAdvOn = FALSE;
static deviceId_t  mPeerDeviceId = gInvalidDeviceId_c;

/* Service Data*/
static uint16_t cpWHandles[6] = {AccessKey_value, MaterialKey_value, Name_value, AvailableA_value, AvailableB_value, AvailableC_value};
static uint8_t gAppSerMgrIf;
static uint8_t mBuffUartLockerKeyMes[8] = {0};
static uint8_t mBuffUartMaterialKeyMes[9] = {0};
static uint8_t mBuffUartNameMes[20] = {0};
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent);
static void BleApp_Config(void);
static void BleApp_Advertise(void);
void AppPrintString(const char* pBuff);
static void Uart_RxCallBack(void *pData);

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

    /* Initialize application support for drivers */
    BOARD_InitAdc();

#if MULTICORE_APPLICATION_CORE
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
    /* Device is not connected and not advertising*/
    if (!mAdvOn)
    {
        BleApp_Advertise();
    }
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
	uint8_t ClearAttribute = 0;

    switch (events)
    {
        case gKBD_EventPressPB1_c:
        {
        	GattDb_WriteAttribute(AvailableA_value, 1, (const uint8_t*)&ClearAttribute);
        }
        break;

        case gKBD_EventPressPB2_c:
        {
        	GattDb_WriteAttribute(AvailableB_value, 1, (const uint8_t*)&ClearAttribute);
        }
        break;

        case gKBD_EventLongPB1_c:
        {
        	GattDb_WriteAttribute(AvailableC_value, 1, (const uint8_t*)&ClearAttribute);
        }
        break;

        case gKBD_EventLongPB2_c:
        {

        }
        break;

        default:
            break;
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

        case gAdvertisingParametersSetupComplete_c:
        {
            (void)Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
        }
        break;

        case gAdvertisingDataSetupComplete_c:
        {
            (void)App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
        }
        break;

        case gAdvertisingSetupFailed_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
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
static void BleApp_Config()
{
#if MULTICORE_APPLICATION_CORE
    if (GattDbDynamic_CreateDatabase() != gBleSuccess_c)
    {
        panic(0,0,0,0);
        return;
    }
#endif /* MULTICORE_APPLICATION_CORE */

    /* Common GAP configuration */
    BleConnManager_GapCommonConfig();

    /* Register for callbacks*/
    GattServer_RegisterHandlesForWriteNotifications(NumberOfElements(cpWHandles), cpWHandles);
    App_RegisterGattServerCallback(BleApp_GattServerCallback);
    BleApp_Start();
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will start after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    /* Set advertising parameters*/
    Gap_SetAdvertisingParameters(&gAdvParams);
}

/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
    switch (pAdvertisingEvent->eventType)
    {
        case gAdvertisingStateChanged_c:
        case gExtAdvertisingStateChanged_c:
        {
            mAdvOn = !mAdvOn;

            if (!mAdvOn)
            {
                BleApp_Advertise();
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
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
    BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);

    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
        	/* Subscribe client */
            mPeerDeviceId = peerDeviceId;
            /* Advertising stops when connected */
            mAdvOn = FALSE;
        }
        break;

        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            mPeerDeviceId = gInvalidDeviceId_c;
            /* Restart advertising*/
            BleApp_Start();
        }
        break;
    default:
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles GATT server callback from host stack.
*
* \param[in]    deviceId        Peer device ID.
* \param[in]    pServerEvent    Pointer to gattServerEvent_t.
********************************************************************************** */
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent)
{
    uint16_t handle;
    uint8_t status;
    uint8_t TempBuffLockerKey[5];
    uint8_t TempBuffMaterialKey[6];
    uint8_t TempBuffName[17];
    uint8_t i = 0;

    switch (pServerEvent->eventType)
    {
        case gEvtAttributeWritten_c:
        {
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;
            status = gAttErrCodeNoError_c;

            GattDb_WriteAttribute( pServerEvent->eventData.attributeWrittenEvent.handle,
                                   pServerEvent->eventData.attributeWrittenEvent.cValueLength,
                                   pServerEvent->eventData.attributeWrittenEvent.aValue );

            if (handle == AccessKey_value)
            {
            	for (i = 0; i < 5; i++, pServerEvent->eventData.attributeWrittenEvent.aValue++)
            	{
            		TempBuffLockerKey[i] = *pServerEvent->eventData.attributeWrittenEvent.aValue;
            	}

            	mBuffUartLockerKeyMes[0] = '2';
            	mBuffUartLockerKeyMes[1] = TempBuffLockerKey[0];
            	mBuffUartLockerKeyMes[2] = ':';

            	for (i = 3; i < 7; i++)
            	{
            		mBuffUartLockerKeyMes[i] = TempBuffLockerKey[i-2];
            	}

                AppPrintString((const char*)&mBuffUartLockerKeyMes[0]);
            	AppPrintString("\n");
            }
            else if (handle == MaterialKey_value)
            {
            	for (i = 0; i < 6; i++, pServerEvent->eventData.attributeWrittenEvent.aValue++)
            	{
            		TempBuffMaterialKey[i] = *pServerEvent->eventData.attributeWrittenEvent.aValue;
            	}

            	mBuffUartMaterialKeyMes[0] = '3';
            	mBuffUartMaterialKeyMes[1] = TempBuffMaterialKey[0];
				mBuffUartMaterialKeyMes[2] = TempBuffMaterialKey[1];
				mBuffUartMaterialKeyMes[3] = ':';

				for (i = 4; i < 8; i++)
				{
					mBuffUartMaterialKeyMes[i] = TempBuffMaterialKey[i-2];
				}

                AppPrintString((const char*)&mBuffUartMaterialKeyMes[0]);
                AppPrintString("\n");
            }
            else if (handle == Name_value)
            {

            	for (i = 0; i < 20; i++)
            	{
            		mBuffUartNameMes[i] = 0;
            	}

            	for (i = 0; i < pServerEvent->eventData.attributeWrittenEvent.cValueLength; i++, pServerEvent->eventData.attributeWrittenEvent.aValue++)
            	{
            		TempBuffName[i] = *pServerEvent->eventData.attributeWrittenEvent.aValue;
            	}

            	mBuffUartNameMes[0] = '1';
            	mBuffUartNameMes[1] = TempBuffName[0];
            	mBuffUartNameMes[2] = ':';

            	for(i = 3; i < 19; i++)
            	{
            		mBuffUartNameMes[i] = TempBuffName[i-2];
            	}

            	AppPrintString((const char*)&mBuffUartNameMes[0]);
                AppPrintString("\n");
            }

            GattServer_SendAttributeWrittenStatus(deviceId, handle, status);
        }
        break;
        default:
        break;
    }
}

void AppPrintString(const char* pBuff)
{
    (void)Serial_Print(gAppSerMgrIf, pBuff, gAllowToBlock_d);
}

static void Uart_RxCallBack(void *pData)
{
	static uint8_t SerialRxBuff[6];
	static uint16_t SerialRxBuffCount = 0;
	uint16_t SerialBytesRead = 0;
	uint8_t i = 0;
	uint8_t ClearAttribute = 0;

	(void)Serial_Read(gAppSerMgrIf, &SerialRxBuff[SerialRxBuffCount], 6, &SerialBytesRead);
	SerialRxBuffCount += SerialBytesRead;

	if(SerialRxBuff[SerialRxBuffCount-1] == '\n')
	{
		if(FLib_MemCmp(&SerialRxBuff[0], "4A", 2))
		{
			GattDb_WriteAttribute(AvailableA_value, 1, (const uint8_t*)&ClearAttribute);
		}
		else if(FLib_MemCmp(&SerialRxBuff[0], "4B", 2))
		{
			GattDb_WriteAttribute(AvailableB_value, 1, (const uint8_t*)&ClearAttribute);
		}
		else if(FLib_MemCmp(&SerialRxBuff[0], "4C", 2))
		{
			GattDb_WriteAttribute(AvailableC_value, 1, (const uint8_t*)&ClearAttribute);
		}

		/* Flush the Serial Rx Buffer and reset the byte counter */
		for(i = 0; i < 6; i++)
		{
			SerialRxBuff[i] = 0;
		}

		SerialRxBuffCount = 0;
	}
}
/*! *********************************************************************************
* @}
********************************************************************************** */
