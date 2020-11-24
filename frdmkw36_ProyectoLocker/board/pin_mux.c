/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v7.0
processor: MKW36Z512xxx4
package_id: MKW36Z512VHT4
mcu_data: ksdk2_0
processor_version: 7.0.1
board: FRDM-KW36
pin_labels:
- {pin_num: '23', pin_signal: ADC0_SE4/CMP0_IN2/PTB18/LPUART1_CTS_b/I2C1_SCL/TPM_CLKIN0/TPM0_CH0/NMI_b, label: SW2, identifier: SW2}
- {pin_num: '38', pin_signal: PTC2/LLWU_P10/TX_SWITCH/I2C1_SCL/LPUART0_RX/CMT_IRO/DTM_RX/SPI1_SOUT/BSM_FRAME, label: SW3, identifier: SW3}
- {pin_num: '18', pin_signal: ADC0_SE3/CMP0_IN3/PTB2/RF_NOT_ALLOWED/DTM_TX/TPM1_CH0, label: LEDRGB_BLUE, identifier: LEDRGB_BLUE}
- {pin_num: '4', pin_signal: PTA16/LLWU_P4/SPI1_SOUT/LPUART1_RTS_b/TPM0_CH0, label: LEDRGB_GREEN, identifier: SPI_RFID_MOSI}
- {pin_num: '37', pin_signal: PTC1/RF_EARLY_WARNING/I2C0_SDA/LPUART0_RTS_b/TPM0_CH2/SPI1_SCK/BSM_CLK, label: LEDRGB_RED, identifier: LEDRGB_RED}
- {pin_num: '42', pin_signal: PTC6/LLWU_P14/RF_RFOSC_EN/I2C1_SCL/LPUART0_RX/TPM2_CH0/BSM_FRAME, label: UART_SDA_RX, identifier: UART_SDA_RX}
- {pin_num: '43', pin_signal: PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/LPUART0_TX/TPM2_CH1/BSM_DATA, label: UART_SDA_TX, identifier: UART_SDA_TX}
- {pin_num: '16', pin_signal: PTB0/LLWU_P8/RF_RFOSC_EN/RF_RESET/I2C0_SCL/CMP0_OUT/TPM0_CH1/CLKOUT/CAN0_TX, label: I2C_SENSOR_SCL, identifier: I2C_SENSOR_SCL}
- {pin_num: '17', pin_signal: ADC0_SE1/CMP0_IN5/PTB1/RF_PRIORITY/DTM_RX/I2C0_SDA/LPTMR0_ALT1/TPM0_CH2/CMT_IRO/CAN0_RX, label: I2C_SENSOR_SCL, identifier: I2C_SENSOR_SCL}
- {pin_num: '46', pin_signal: PTC17/LLWU_P1/RF_EXT_OSC_EN/SPI0_SOUT/I2C1_SCL/LPUART0_RX/BSM_FRAME/DTM_RX/LPUART1_RX, label: SPI_FLASH_SI, identifier: SPI_FLASH_SI}
- {pin_num: '45', pin_signal: PTC16/LLWU_P0/RF_STATUS/SPI0_SCK/I2C0_SDA/LPUART0_RTS_b/TPM0_CH3/LPUART1_RTS_b, label: SPI_FLASH_SCK, identifier: SPI_FLASH_SCK}
- {pin_num: '48', pin_signal: PTC19/LLWU_P3/RF_EARLY_WARNING/SPI0_PCS0/I2C0_SCL/LPUART0_CTS_b/BSM_CLK/LPUART1_CTS_b, label: SPI_FLASH_CS, identifier: SPI_FLASH_CS}
- {pin_num: '47', pin_signal: PTC18/LLWU_P2/SPI0_SIN/I2C1_SDA/LPUART0_TX/BSM_DATA/DTM_TX/LPUART1_TX, label: SPI_FLASH_SO, identifier: SPI_FLASH_SO}
- {pin_num: '5', pin_signal: PTA17/LLWU_P5/RF_RESET/SPI1_SIN/LPUART1_RX/CAN0_TX/TPM_CLKIN1, label: 'J1[5]/U10[1]/U12[4]/D4/LIN_RX_LS', identifier: SPI_RFID_MISO}
- {pin_num: '6', pin_signal: PTA18/LLWU_P6/SPI1_SCK/LPUART1_TX/CAN0_RX/TPM2_CH0, label: 'J1[7]/U7[2]/U10[4]/D6/LIN_TX_LS', identifier: SPI_RFID_SCK}
- {pin_num: '7', pin_signal: ADC0_SE5/PTA19/LLWU_P7/SPI1_PCS0/LPUART1_CTS_b/TPM2_CH1, label: 'J2[3]/U11[11]/D10/INT1_COMBO', identifier: SPI_RFID_SS}
- {pin_num: '19', pin_signal: ADC0_SE2/CMP0_IN4/PTB3/LPUART1_RTS_b/CLKOUT/TPM1_CH1/RTC_CLKOUT, label: LED3, identifier: LED3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitButtons:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '38', peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: PTC2/LLWU_P10/TX_SWITCH/I2C1_SCL/LPUART0_RX/CMT_IRO/DTM_RX/SPI1_SOUT/BSM_FRAME, direction: INPUT,
    pull_select: up, pull_enable: enable}
  - {pin_num: '23', peripheral: GPIOB, signal: 'GPIO, 18', pin_signal: ADC0_SE4/CMP0_IN2/PTB18/LPUART1_CTS_b/I2C1_SCL/TPM_CLKIN0/TPM0_CH0/NMI_b, direction: INPUT,
    pull_select: up, pull_enable: enable, passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitButtons
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitButtons(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t SW2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTB18 (pin 23)  */
    GPIO_PinInit(BOARD_INITBUTTONS_SW2_GPIO, BOARD_INITBUTTONS_SW2_PIN, &SW2_config);

    gpio_pin_config_t SW3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC2 (pin 38)  */
    GPIO_PinInit(BOARD_INITBUTTONS_SW3_GPIO, BOARD_INITBUTTONS_SW3_PIN, &SW3_config);

    /* PORTB18 (pin 23) is configured as PTB18 */
    PORT_SetPinMux(BOARD_INITBUTTONS_SW2_PORT, BOARD_INITBUTTONS_SW2_PIN, kPORT_MuxAsGpio);

    PORTB->PCR[18] = ((PORTB->PCR[18] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | PORT_PCR_PS(PCR_PS_0b1)

                      /* Pull Enable: Internal pull resistor is enabled on the corresponding pin, if the pin is
                       * configured as a digital input. */
                      | PORT_PCR_PE(PCR_PE_0b1)

                      /* Passive Filter Enable: Passive input filter is disabled on the corresponding pin. */
                      | PORT_PCR_PFE(PCR_PFE_0b0));

    /* PORTC2 (pin 38) is configured as PTC2 */
    PORT_SetPinMux(BOARD_INITBUTTONS_SW3_PORT, BOARD_INITBUTTONS_SW3_PIN, kPORT_MuxAsGpio);

    PORTC->PCR[2] = ((PORTC->PCR[2] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | PORT_PCR_PS(PCR_PS_0b1)

                     /* Pull Enable: Internal pull resistor is enabled on the corresponding pin, if the pin is
                      * configured as a digital input. */
                     | PORT_PCR_PE(PCR_PE_0b1));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDs:
- options: {coreID: core0, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLEDs(void)
{
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitRGB:
- options: {coreID: core0, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitRGB
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitRGB(void)
{
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSPI:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '4', peripheral: SPI1, signal: SOUT, pin_signal: PTA16/LLWU_P4/SPI1_SOUT/LPUART1_RTS_b/TPM0_CH0}
  - {pin_num: '5', peripheral: SPI1, signal: SIN, pin_signal: PTA17/LLWU_P5/RF_RESET/SPI1_SIN/LPUART1_RX/CAN0_TX/TPM_CLKIN1}
  - {pin_num: '6', peripheral: SPI1, signal: SCK, pin_signal: PTA18/LLWU_P6/SPI1_SCK/LPUART1_TX/CAN0_RX/TPM2_CH0}
  - {pin_num: '7', peripheral: SPI1, signal: PCS0_SS, pin_signal: ADC0_SE5/PTA19/LLWU_P7/SPI1_PCS0/LPUART1_CTS_b/TPM2_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSPI
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitSPI(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA16 (pin 4) is configured as SPI1_SOUT */
    PORT_SetPinMux(BOARD_INITSPI_SPI_RFID_MOSI_PORT, BOARD_INITSPI_SPI_RFID_MOSI_PIN, kPORT_MuxAlt2);

    /* PORTA17 (pin 5) is configured as SPI1_SIN */
    PORT_SetPinMux(BOARD_INITSPI_SPI_RFID_MISO_PORT, BOARD_INITSPI_SPI_RFID_MISO_PIN, kPORT_MuxAlt2);

    /* PORTA18 (pin 6) is configured as SPI1_SCK */
    PORT_SetPinMux(BOARD_INITSPI_SPI_RFID_SCK_PORT, BOARD_INITSPI_SPI_RFID_SCK_PIN, kPORT_MuxAlt2);

    /* PORTA19 (pin 7) is configured as SPI1_PCS0 */
    PORT_SetPinMux(BOARD_INITSPI_SPI_RFID_SS_PORT, BOARD_INITSPI_SPI_RFID_SS_PIN, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLPUART:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '42', peripheral: LPUART0, signal: RX, pin_signal: PTC6/LLWU_P14/RF_RFOSC_EN/I2C1_SCL/LPUART0_RX/TPM2_CH0/BSM_FRAME, slew_rate: fast}
  - {pin_num: '43', peripheral: LPUART0, signal: TX, pin_signal: PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/LPUART0_TX/TPM2_CH1/BSM_DATA, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLPUART
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLPUART(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC6 (pin 42) is configured as LPUART0_RX */
    PORT_SetPinMux(BOARD_INITLPUART_UART_SDA_RX_PORT, BOARD_INITLPUART_UART_SDA_RX_PIN, kPORT_MuxAlt4);

    PORTC->PCR[6] = ((PORTC->PCR[6] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_SRE_MASK | PORT_PCR_ISF_MASK)))

                     /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is
                      * configured as a digital output. */
                     | PORT_PCR_SRE(PCR_SRE_0b0));

    /* PORTC7 (pin 43) is configured as LPUART0_TX */
    PORT_SetPinMux(BOARD_INITLPUART_UART_SDA_TX_PORT, BOARD_INITLPUART_UART_SDA_TX_PIN, kPORT_MuxAlt4);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 Transmit Data Source Select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 Receive Data Source Select: LPUART_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitI2C:
- options: {coreID: core0, enableClock: 'false'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitI2C
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitI2C(void)
{
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/