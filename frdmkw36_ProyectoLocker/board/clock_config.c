/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/
/*
 * How to setup clock using clock driver functions:
 *
 * 1. CLOCK_SetSimSafeDivs, to make sure core clock, bus clock, flexbus clock
 *    and flash clock are in allowed range during clock mode switch.
 *
 * 2. Call CLOCK_Osc0Init to setup OSC clock, if it is used in target mode.
 *
 * 3. Set MCG configuration, MCG includes three parts: FLL clock, PLL clock and
 *    internal reference clock(MCGIRCLK). Follow the steps to setup:
 *
 *    1). Call CLOCK_BootToXxxMode to set MCG to target mode.
 *
 *    2). If target mode is FBI/BLPI/PBI mode, the MCGIRCLK has been configured
 *        correctly. For other modes, need to call CLOCK_SetInternalRefClkConfig
 *        explicitly to setup MCGIRCLK.
 *
 *    3). Don't need to configure FLL explicitly, because if target mode is FLL
 *        mode, then FLL has been configured by the function CLOCK_BootToXxxMode,
 *        if the target mode is not FLL mode, the FLL is disabled.
 *
 *    4). If target mode is PEE/PBE/PEI/PBI mode, then the related PLL has been
 *        setup by CLOCK_BootToXxxMode. In FBE/FBI/FEE/FBE mode, the PLL could
 *        be enabled independently, call CLOCK_EnablePll0 explicitly in this case.
 *
 * 4. Call CLOCK_SetSimConfig to set the clock configuration in SIM.
 */

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Clocks v7.0
processor: MKW36Z512xxx4
package_id: MKW36Z512VHT4
mcu_data: ksdk2_0
processor_version: 7.0.1
board: FRDM-KW36
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

#include "fsl_smc.h"
#include "fsl_rtc.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RTC_OSC_CAP_LOAD_0PF                            0x0U  /*!< RTC oscillator capacity load: 0pF */
#define SIM_LPUART_CLK_SEL_OSCERCLK_CLK                   2U  /*!< LPUART clock select: OSCERCLK clock */
#define SIM_OSC32KSEL_OSC32KCLK_CLK                       0U  /*!< OSC32KSEL select: OSC32KCLK clock */
#define SIM_TPM_CLK_SEL_OSCERCLK_CLK                      2U  /*!< TPM clock select: OSCERCLK clock */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_SetRtcClock
 * Description   : This function is used to configuring RTC clock
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_SetRtcClock(void)
{
    /* RTC clock gate enable */
    CLOCK_EnableClock(kCLOCK_Rtc0);
    /* Set the XTAL32/RTC_CLKIN frequency based on board setting. */
    CLOCK_SetXtal32Freq(BOARD_XTAL32K_CLK_HZ);
    /* Set RTC_TSR if there is fault value in RTC */
    if (RTC->SR & RTC_SR_TIF_MASK) {
        RTC -> TSR = RTC -> TSR;
    }
    /* RTC clock gate disable */
    CLOCK_DisableClock(kCLOCK_Rtc0);
}

/*FUNCTION**********************************************************************
*
* Function Name  : CLOCK_CONFIG_SysTickWaitMs
* Description    : This function is used for waiting by SysTick
* Param delay_ms : Delay [milliseconds]
*
*END**************************************************************************/
static void CLOCK_CONFIG_SysTickWaitMs(uint32_t delay_ms)
{
	uint32_t ticks = 0UL;
	uint32_t count = delay_ms;
	/* Make a 1 milliseconds counter. */
	ticks = CLOCK_GetCoreSysClkFreq() / 1000UL;
	assert((ticks - 1UL) <= SysTick_LOAD_RELOAD_Msk);
	/* Enable the SysTick for counting. */
	SysTick->LOAD = (ticks > 1) ? (uint32_t)(ticks - 1UL) : 1;
	SysTick->VAL  = 0UL;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	for (; count > 0U; count--)
	{
		/* Wait for the SysTick counter reach 0. */
		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
		{
		}
	}
	/* Disable SysTick. */
	SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
	SysTick->LOAD = 0UL;
	SysTick->VAL  = 0UL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_FllStableDelay
 * Description   : This function is used to delay for FLL stable.
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_FllStableDelay(void)
{
    /* Wait 1ms for FLL become stable. */
    CLOCK_CONFIG_SysTickWaitMs(1UL);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_EnableRtcOsc
 * Description   : This function is used to enabling RTC oscillator
 * Param capLoad : Oscillator capacity load
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_EnableRtcOsc(uint32_t capLoad)
{
	#if !(defined(FSL_FEATURE_RTC_HAS_NO_CR_OSCE) && FSL_FEATURE_RTC_HAS_NO_CR_OSCE)
    	rtc_config_t rtcConfig;
    	/*Init the RTC with default configuration*/
    	RTC_GetDefaultConfig(&rtcConfig);
     	RTC_Init(RTC, &rtcConfig);
     	/* If the oscillator has not been enabled. */
     	if ((RTC->CR & RTC_CR_OSCE_MASK) == 0u)
     	{
			/* Set the specified capacitor configuration for the RTC oscillator */
			RTC_SetOscCapLoad(RTC, capLoad);
			/* Enable the RTC 32kHz oscillator */
			RTC_SetClockSource(RTC);
			/* Start the RTC time counter */
			RTC_StartTimer(RTC);
			/* Wait 1s for OSC clock steady. */
			CLOCK_CONFIG_SysTickWaitMs(1000UL);
    	}
   	RTC_Deinit(RTC);
	#endif /* FSL_FEATURE_RTC_HAS_NO_CR_OSCE */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_RfOscInit
 * Description   : This function is used to setup Ref oscillator.
 *
 *END**************************************************************************/
void BOARD_RfOscInit(void)
{
    /* Turn on clocks for the XCVR */

    /* Enable RF OSC in RSIM and wait for ready */
    RSIM->CONTROL = ((RSIM->CONTROL & ~RSIM_CONTROL_RF_OSC_EN_MASK) | RSIM_CONTROL_RF_OSC_EN(1));

    /* ERR010224 */
    RSIM->RF_OSC_CTRL |= RSIM_RF_OSC_CTRL_RADIO_EXT_OSC_OVRD_EN_MASK;   /* Prevent XTAL_OUT_EN from generating XTAL_OUT request */

    while((RSIM->CONTROL & RSIM_CONTROL_RF_OSC_READY_MASK) == 0)
    {
       ;                                                                /* Wait for RF_OSC_READY */
    }

    /* Enable clock gate to write to the XCVR registers */
    SIM->SCGC5 |= SIM_SCGC5_PHYDIG_MASK;


}

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitOsc0
 * Description   : This function is used to setup MCG OSC with Ref oscillator for KW40_512.
 *
 *END**************************************************************************/
void BOARD_InitOsc0(void)
{
    const osc_config_t oscConfig = {
      .freq = BOARD_XTAL0_CLK_HZ, .workMode = kOSC_ModeExt,
    };

    /* Initializes OSC0 according to previous configuration to meet Ref OSC needs. */
    CLOCK_InitOsc0(&oscConfig);

    /* Passing the XTAL0 frequency to clock driver. */
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
}

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    BOARD_BootClockRUN();
}

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockRUN
called_from_default_init: true
outputs:
- {id: Bus_clock.outFreq, value: 10.48576 MHz}
- {id: Core_clock.outFreq, value: 20.97152 MHz}
- {id: ERCLK32K.outFreq, value: 32.768 kHz}
- {id: Flash_clock.outFreq, value: 10.48576 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: LPUART0CLK.outFreq, value: 32 MHz}
- {id: LPUART1CLK.outFreq, value: 32 MHz}
- {id: MCGFLLCLK.outFreq, value: 20.97152 MHz}
- {id: MCGIRCLK.outFreq, value: 32.768 kHz}
- {id: OSCERCLK.outFreq, value: 32 MHz}
- {id: System_clock.outFreq, value: 20.97152 MHz}
- {id: TPMCLK.outFreq, value: 32 MHz}
settings:
- {id: LPUART0ClkConfig, value: 'yes'}
- {id: LPUART1ClkConfig, value: 'yes'}
- {id: MCG.FCRDIV.scale, value: '1'}
- {id: MCG.FLL_mul.scale, value: '640', locked: true}
- {id: MCG.OSCSEL.sel, value: RTC_32K.OSCCLK32K}
- {id: MCG_C1_IRCLKEN_CFG, value: Enabled}
- {id: MCG_C2_RANGE0_FRDIV_CFG, value: Very_high}
- {id: MCG_C2_RANGE_CFG, value: Very_high}
- {id: RTC_CR_OSCE_CFG, value: Oscillator_enabled}
- {id: SIM.LPUART0SRCSEL.sel, value: REFOSC.OSCCLK}
- {id: SIM.LPUART1SRCSEL.sel, value: REFOSC.OSCCLK}
- {id: SIM.TPMSRCSEL.sel, value: REFOSC.OSCCLK}
- {id: TPMClkConfig, value: 'yes'}
sources:
- {id: REFOSC.OSC.outFreq, value: 32 MHz, enabled: true}
- {id: RTC_32K.OSC32kHz.outFreq, value: 32.768 kHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockRUN configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockRUN =
    {
        .mcgMode = kMCG_ModeFEI,                  /* FEI - FLL Engaged Internal */
        .irclkEnableMode = kMCG_IrclkEnable,      /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
        .ircs = kMCG_IrcSlow,                     /* Slow internal reference clock selected */
        .fcrdiv = 0x0U,                           /* Fast IRC divider: divided by 1 */
        .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 1 */
        .drs = kMCG_DrsLow,                       /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
        .oscsel = kMCG_OscselRtc,                 /* Selects 32 kHz RTC Oscillator */
    };
const sim_clock_config_t simConfig_BOARD_BootClockRUN =
    {
        .er32kSrc = SIM_OSC32KSEL_OSC32KCLK_CLK,  /* OSC32KSEL select: OSC32KCLK clock */
        .clkdiv1 = 0x10000U,                      /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV4: /2 */
    };

/*******************************************************************************
 * Code for BOARD_BootClockRUN configuration
 ******************************************************************************/
void BOARD_BootClockRUN(void)
{
    /* Setup the reference oscillator. */
    BOARD_RfOscInit();
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_SetSimSafeDivs();
    /* Enable RTC oscillator. */
    CLOCK_CONFIG_EnableRtcOsc(RTC_OSC_CAP_LOAD_0PF);
    /* Initializes OSC0 according to Ref OSC needs. */
    BOARD_InitOsc0();
    /* Set MCG to FEI mode. */
#if FSL_CLOCK_DRIVER_VERSION >= MAKE_VERSION(2, 2, 0)
    CLOCK_BootToFeiMode(mcgConfig_BOARD_BootClockRUN.dmx32,
                        mcgConfig_BOARD_BootClockRUN.drs,
                        CLOCK_CONFIG_FllStableDelay);
#else
    CLOCK_BootToFeiMode(mcgConfig_BOARD_BootClockRUN.drs,
                        CLOCK_CONFIG_FllStableDelay);
#endif
    /* Configure the Internal Reference clock (MCGIRCLK). */
    CLOCK_SetInternalRefClkConfig(mcgConfig_BOARD_BootClockRUN.irclkEnableMode,
                                  mcgConfig_BOARD_BootClockRUN.ircs, 
                                  mcgConfig_BOARD_BootClockRUN.fcrdiv);
    /* Select the MCG external reference clock. */
    CLOCK_SetExternalRefClkConfig(mcgConfig_BOARD_BootClockRUN.oscsel);
    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig_BOARD_BootClockRUN);
    /* Configure RTC clock. */
    CLOCK_CONFIG_SetRtcClock();
    /* Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;
    /* Set LPUART0 clock source. */
    CLOCK_SetLpuart0Clock(SIM_LPUART_CLK_SEL_OSCERCLK_CLK);
    /* Set LPUART1 clock source. */
    CLOCK_SetLpuart1Clock(SIM_LPUART_CLK_SEL_OSCERCLK_CLK);
    /* Set TPM clock source. */
    CLOCK_SetTpmClock(SIM_TPM_CLK_SEL_OSCERCLK_CLK);
}

/*******************************************************************************
 ********************* Configuration BOARD_BootClockVLPR ***********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockVLPR
outputs:
- {id: Bus_clock.outFreq, value: 800 kHz}
- {id: Core_clock.outFreq, value: 4 MHz}
- {id: Flash_clock.outFreq, value: 800 kHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: MCGIRCLK.outFreq, value: 4 MHz}
- {id: System_clock.outFreq, value: 4 MHz}
settings:
- {id: MCGMode, value: BLPI}
- {id: powerMode, value: VLPR}
- {id: MCG.CLKS.sel, value: MCG.IRCS}
- {id: MCG.FCRDIV.scale, value: '1'}
- {id: MCG.IRCS.sel, value: MCG.FCRDIV}
- {id: MCG_C1_IRCLKEN_CFG, value: Enabled}
- {id: SIM.OUTDIV4.scale, value: '5'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockVLPR configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockVLPR =
    {
        .mcgMode = kMCG_ModeBLPI,                 /* BLPI - Bypassed Low Power Internal */
        .irclkEnableMode = kMCG_IrclkEnable,      /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
        .ircs = kMCG_IrcFast,                     /* Fast internal reference clock selected */
        .fcrdiv = 0x0U,                           /* Fast IRC divider: divided by 1 */
        .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 1 */
        .drs = kMCG_DrsLow,                       /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
        .oscsel = kMCG_OscselOsc,                 /* Selects System Oscillator (OSCCLK) */
    };
const sim_clock_config_t simConfig_BOARD_BootClockVLPR =
    {
        .er32kSrc = SIM_OSC32KSEL_OSC32KCLK_CLK,  /* OSC32KSEL select: OSC32KCLK clock */
        .clkdiv1 = 0x40000U,                      /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV4: /5 */
    };

/*******************************************************************************
 * Code for BOARD_BootClockVLPR configuration
 ******************************************************************************/
void BOARD_BootClockVLPR(void)
{
    /* ERR010224 */
    RSIM->RF_OSC_CTRL |= RSIM_RF_OSC_CTRL_RADIO_EXT_OSC_OVRD_EN_MASK;   /* Prevent XTAL_OUT_EN from generating XTAL_OUT request */
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_SetSimSafeDivs();
    /* Set MCG to BLPI mode. */
    CLOCK_BootToBlpiMode(mcgConfig_BOARD_BootClockVLPR.fcrdiv,
                         mcgConfig_BOARD_BootClockVLPR.ircs,
                         mcgConfig_BOARD_BootClockVLPR.irclkEnableMode);
    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig_BOARD_BootClockVLPR);
    /* Set VLPR power mode. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
#if (defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI)
    SMC_SetPowerModeVlpr(SMC, false);
#else
    SMC_SetPowerModeVlpr(SMC);
#endif
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr)
    {
    }
    /* Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKVLPR_CORE_CLOCK;
}
