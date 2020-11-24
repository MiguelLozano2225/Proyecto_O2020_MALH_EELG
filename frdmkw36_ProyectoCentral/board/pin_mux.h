#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*!
 * @brief
 * Pull Enable: Internal pull resistor is enabled on the corresponding pin, if the pin is configured as a digital
 * input.
 */
#define PCR_PE_0b1 0x01u
/*!
 * @brief Passive Filter Enable: Passive input filter is disabled on the corresponding pin. */
#define PCR_PFE_0b0 0x00u
/*!
 * @brief
 * Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set.
 */
#define PCR_PS_0b1 0x01u

/*! @name PORTC2 (number 38), SW3
  @{ */
#define BOARD_INITBUTTONS_SW3_FGPIO FGPIOC /*!<@brief FGPIO device name: FGPIOC */
#define BOARD_INITBUTTONS_SW3_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_INITBUTTONS_SW3_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITBUTTONS_SW3_PIN 2U     /*!<@brief PORTC pin index: 2 */
                                         /* @} */

/*! @name PORTB18 (number 23), SW2
  @{ */
#define BOARD_INITBUTTONS_SW2_FGPIO FGPIOB /*!<@brief FGPIO device name: FGPIOB */
#define BOARD_INITBUTTONS_SW2_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITBUTTONS_SW2_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITBUTTONS_SW2_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                         /* @} */
void BOARD_InitButtons(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDs(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitRGB(void);

/*! @name PORTA16 (number 4), LEDRGB_GREEN
  @{ */
#define BOARD_INITSPI_SPI_RFID_MOSI_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITSPI_SPI_RFID_MOSI_PIN 16U    /*!<@brief PORTA pin index: 16 */
                                               /* @} */

/*! @name PORTA17 (number 5), J1[5]/U10[1]/U12[4]/D4/LIN_RX_LS
  @{ */
#define BOARD_INITSPI_SPI_RFID_MISO_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITSPI_SPI_RFID_MISO_PIN 17U    /*!<@brief PORTA pin index: 17 */
                                               /* @} */

/*! @name PORTA18 (number 6), J1[7]/U7[2]/U10[4]/D6/LIN_TX_LS
  @{ */
#define BOARD_INITSPI_SPI_RFID_SCK_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITSPI_SPI_RFID_SCK_PIN 18U    /*!<@brief PORTA pin index: 18 */
                                              /* @} */

/*! @name PORTA19 (number 7), J2[3]/U11[11]/D10/INT1_COMBO
  @{ */
#define BOARD_INITSPI_SPI_RFID_SS_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITSPI_SPI_RFID_SS_PIN 19U    /*!<@brief PORTA pin index: 19 */
                                             /* @} */
/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSPI(void);

#define BOARD_INITLPUART_LPUART0_TX_DIRECTION           kPIN_MUX_DirectionOutput   /*!< Direction of LPUART0_TX signal */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPUART(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2C(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
