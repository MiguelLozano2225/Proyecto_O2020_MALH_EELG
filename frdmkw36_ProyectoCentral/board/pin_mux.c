/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKW36Z512xxx4
package_id: MKW36Z512VHT4
mcu_data: ksdk2_0
processor_version: 0.0.4
board: FRDM-KW36
pin_labels:
- {pin_num: '19', pin_signal: ADC0_SE2/CMP0_IN4/PTB3/LPUART1_RTS_b/CLKOUT/TPM1_CH1/RTC_CLKOUT, label: LED3, identifier: LED3}
- {pin_num: '23', pin_signal: ADC0_SE4/CMP0_IN2/PTB18/LPUART1_CTS_b/I2C1_SCL/TPM_CLKIN0/TPM0_CH0/NMI_b, label: SW2, identifier: SW2}
- {pin_num: '38', pin_signal: PTC2/LLWU_P10/TX_SWITCH/I2C1_SCL/LPUART0_RX/CMT_IRO/DTM_RX/SPI1_SOUT/BSM_FRAME, label: SW3, identifier: SW3}
- {pin_num: '18', pin_signal: ADC0_SE3/CMP0_IN3/PTB2/RF_NOT_ALLOWED/DTM_TX/TPM1_CH0, label: LEDRGB_BLUE, identifier: LEDRGB_BLUE}
- {pin_num: '37', pin_signal: PTC1/RF_EARLY_WARNING/I2C0_SDA/LPUART0_RTS_b/TPM0_CH2/SPI1_SCK/BSM_CLK, label: LEDRGB_RED, identifier: LEDRGB_RED}
- {pin_num: '4', pin_signal: PTA16/LLWU_P4/SPI1_SOUT/LPUART1_RTS_b/TPM0_CH0, label: LEDRGB_GREEN, identifier: LEDRGB_GREEN}
- {pin_num: '45', pin_signal: PTC16/LLWU_P0/RF_STATUS/SPI0_SCK/I2C0_SDA/LPUART0_RTS_b/TPM0_CH3/LPUART1_RTS_b, label: SPI_FLASH_SCK, identifier: SPI_FLASH_SCK}
- {pin_num: '46', pin_signal: PTC17/LLWU_P1/RF_EXT_OSC_EN/SPI0_SOUT/I2C1_SCL/LPUART0_RX/BSM_FRAME/DTM_RX/LPUART1_RX, label: SPI_FLASH_SI, identifier: SPI_FLASH_SI}
- {pin_num: '47', pin_signal: PTC18/LLWU_P2/SPI0_SIN/I2C1_SDA/LPUART0_TX/BSM_DATA/DTM_TX/LPUART1_TX, label: SPI_FLASH_SO, identifier: SPI_FLASH_SO}
- {pin_num: '48', pin_signal: PTC19/LLWU_P3/RF_EARLY_WARNING/SPI0_PCS0/I2C0_SCL/LPUART0_CTS_b/BSM_CLK/LPUART1_CTS_b, label: SPI_FLASH_CS, identifier: SPI_FLASH_CS}
- {pin_num: '42', pin_signal: PTC6/LLWU_P14/RF_RFOSC_EN/I2C1_SCL/LPUART0_RX/TPM2_CH0/BSM_FRAME, label: UART_SDA_RX, identifier: UART_SDA_RX}
- {pin_num: '43', pin_signal: PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/LPUART0_TX/TPM2_CH1/BSM_DATA, label: UART_SDA_TX, identifier: UART_SDA_TX}
- {pin_num: '16', pin_signal: PTB0/LLWU_P8/RF_RFOSC_EN/RF_RESET/I2C0_SCL/CMP0_OUT/TPM0_CH1/CLKOUT/CAN0_TX, label: I2C_SENSOR_SCL, identifier: I2C_SENSOR_SCL}
- {pin_num: '17', pin_signal: ADC0_SE1/CMP0_IN5/PTB1/RF_PRIORITY/DTM_RX/I2C0_SDA/LPTMR0_ALT1/TPM0_CH2/CMT_IRO/CAN0_RX, label: I2C_SENSOR_SCL, identifier: I2C_SENSOR_SCL}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/*FUNCTION**********************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 *END**************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
}

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

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitButtons
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
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

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDs:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '19', peripheral: GPIOB, signal: 'GPIO, 3', pin_signal: ADC0_SE2/CMP0_IN4/PTB3/LPUART1_RTS_b/CLKOUT/TPM1_CH1/RTC_CLKOUT, direction: OUTPUT, slew_rate: slow,
    pull_select: down}
  - {pin_num: '18', peripheral: GPIOB, signal: 'GPIO, 2', pin_signal: ADC0_SE3/CMP0_IN3/PTB2/RF_NOT_ALLOWED/DTM_TX/TPM1_CH0, direction: OUTPUT, pull_select: down}
  - {pin_num: '37', peripheral: GPIOC, signal: 'GPIO, 1', pin_signal: PTC1/RF_EARLY_WARNING/I2C0_SDA/LPUART0_RTS_b/TPM0_CH2/SPI1_SCK/BSM_CLK, direction: OUTPUT, pull_select: down}
  - {pin_num: '4', peripheral: GPIOA, signal: 'GPIO, 16', pin_signal: PTA16/LLWU_P4/SPI1_SOUT/LPUART1_RTS_b/TPM0_CH0, direction: OUTPUT, slew_rate: slow, pull_select: down}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitLEDs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitLEDs(void)
{
    gpio_pin_config_t LED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };

	CLOCK_EnableClock(kCLOCK_PortB);
	PORT_SetPinMux(PORTB, 3U, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOB, 3U, &LED_config);
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitRGB:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '37', peripheral: TPM0, signal: 'CH, 2', pin_signal: PTC1/RF_EARLY_WARNING/I2C0_SDA/LPUART0_RTS_b/TPM0_CH2/SPI1_SCK/BSM_CLK, direction: OUTPUT, slew_rate: fast}
  - {pin_num: '4', peripheral: TPM0, signal: 'CH, 0', pin_signal: PTA16/LLWU_P4/SPI1_SOUT/LPUART1_RTS_b/TPM0_CH0, direction: OUTPUT, slew_rate: fast}
  - {pin_num: '18', peripheral: TPM1, signal: 'CH, 0', pin_signal: ADC0_SE3/CMP0_IN3/PTB2/RF_NOT_ALLOWED/DTM_TX/TPM1_CH0, direction: OUTPUT, slew_rate: fast}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitRGB
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitRGB(void)
{
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSPI:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '45', peripheral: SPI0, signal: SCK, pin_signal: PTC16/LLWU_P0/RF_STATUS/SPI0_SCK/I2C0_SDA/LPUART0_RTS_b/TPM0_CH3/LPUART1_RTS_b, direction: OUTPUT}
  - {pin_num: '46', peripheral: SPI0, signal: SOUT, pin_signal: PTC17/LLWU_P1/RF_EXT_OSC_EN/SPI0_SOUT/I2C1_SCL/LPUART0_RX/BSM_FRAME/DTM_RX/LPUART1_RX}
  - {pin_num: '47', peripheral: SPI0, signal: SIN, pin_signal: PTC18/LLWU_P2/SPI0_SIN/I2C1_SDA/LPUART0_TX/BSM_DATA/DTM_TX/LPUART1_TX}
  - {pin_num: '48', peripheral: SPI0, signal: PCS0_SS, pin_signal: PTC19/LLWU_P3/RF_EARLY_WARNING/SPI0_PCS0/I2C0_SCL/LPUART0_CTS_b/BSM_CLK/LPUART1_CTS_b, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitSPI
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
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


#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define SOPT5_LPUART0RXSRC_LPUART_RX  0x00u   /*!< LPUART0 Receive Data Source Select: LPUART_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX  0x00u   /*!< LPUART0 Transmit Data Source Select: LPUART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLPUART:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '42', peripheral: LPUART0, signal: RX, pin_signal: PTC6/LLWU_P14/RF_RFOSC_EN/I2C1_SCL/LPUART0_RX/TPM2_CH0/BSM_FRAME, slew_rate: fast}
  - {pin_num: '43', peripheral: LPUART0, signal: TX, pin_signal: PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/LPUART0_TX/TPM2_CH1/BSM_DATA, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitLPUART
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitLPUART(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt4);            /* PORTC6 (pin 42) is configured as LPUART0_RX */
  PORTC->PCR[6] = ((PORTC->PCR[6] &
    (~(PORT_PCR_SRE_MASK | PORT_PCR_ISF_MASK)))              /* Mask bits to zero which are setting */
      | PORT_PCR_SRE(0x00u)                                  /* Slew Rate Enable: 0x00u */
    );
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt4);            /* PORTC7 (pin 43) is configured as LPUART0_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX) /* LPUART0 Transmit Data Source Select: LPUART0_TX pin */
      | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX) /* LPUART0 Receive Data Source Select: LPUART_RX pin */
    );
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitI2C:
- options: {coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '16', peripheral: I2C0, signal: CLK, pin_signal: PTB0/LLWU_P8/RF_RFOSC_EN/RF_RESET/I2C0_SCL/CMP0_OUT/TPM0_CH1/CLKOUT/CAN0_TX, slew_rate: fast}
  - {pin_num: '17', peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE1/CMP0_IN5/PTB1/RF_PRIORITY/DTM_RX/I2C0_SDA/LPTMR0_ALT1/TPM0_CH2/CMT_IRO/CAN0_RX, identifier: '',
    slew_rate: fast}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitI2C
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitI2C(void)
{
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
