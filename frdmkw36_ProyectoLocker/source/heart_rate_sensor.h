/*! *********************************************************************************
 * \defgroup Heart Rate Sensor
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Heart Rate Sensor application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _APP_H_
#define _APP_H_

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/
/* Profile Parameters */
#ifndef gAppDeepSleepMode_c
#define gAppDeepSleepMode_c             1
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern gapAdvertisingData_t         gAppAdvertisingData;
extern gapScanResponseData_t        gAppScanRspData;
extern gapAdvertisingParameters_t   gAdvParams;
extern gapSmpKeys_t                 gSmpKeys;
extern gapPairingParameters_t       gPairingParameters;
extern gapDeviceSecurityRequirements_t deviceSecurityRequirements;
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void BleApp_Init(void);
void BleApp_Start(void);
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent);

#ifdef __cplusplus
}
#endif 


#endif /* _APP_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
