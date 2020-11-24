/*
 * i2c.h
 *
 *  Created on: Oct 23, 2019
 *      Author: migue
 */

#ifndef I2C_H_
#define I2C_H_


#define WRITE 0
#define READ  1

#define I2C_Start            I2C0_C1 |= I2C_C1_TX_MASK;\
                             I2C0_C1 |= I2C_C1_MST_MASK

#define I2C_Stop             I2C0_C1 &= ~I2C_C1_MST_MASK;\
                             I2C0_C1 &= ~I2C_C1_TX_MASK

#define I2C_Wait           while(!(I2C0_S & I2C_S_IICIF_MASK));\
                             I2C0_S |= I2C_S_IICIF_MASK

#define I2C_EnterRxMode      I2C0_C1 &= ~I2C_C1_TX_MASK
#define I2C_EnableAck        I2C0_C1 &= ~I2C_C1_TXAK_MASK
#define I2C_DisableAck        I2C0_C1 |= I2C_C1_TXAK_MASK

void Init_I2C(void);
void Transmit_data(uint8_t address);
void delay_7u(uint16_t a);
void Read_data(uint8_t address,uint16_t mem_add2, uint8_t *rx_data);

#endif /* I2C_H_ */
