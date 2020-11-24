/*
 * clocks_and_pin.h
 *
 *  Created on: Oct 25, 2020
 *      Author: migue
 */

#ifndef CLOCKS_AND_PIN_H_
#define CLOCKS_AND_PIN_H_

#include "derivative.h"

#define OPEN_LOCKER_A			GPIOB_PDOR |= (1<<2)
#define OPEN_LOCKER_B			GPIOB_PDOR |= (1<<3)
#define OPEN_LOCKER_C			GPIOB_PDOR |= (1<<10)

#define CLOSE_LOCKER_A			GPIOB_PDOR &= ~(1<<2)
#define CLOSE_LOCKER_B			GPIOB_PDOR &= ~(1<<3)
#define CLOSE_LOCKER_C			GPIOB_PDOR &= ~(1<<10)

#define LED_W_ON 	GPIOB_PDOR &= ~(1<<22);\
					GPIOB_PDOR &= ~(1<<21);\
					GPIOE_PDOR &= ~(1<<26);\
	
#define LED_W_OFF	GPIOB_PDOR |= (1<<22);\
					GPIOB_PDOR |= (1<<21);\
					GPIOE_PDOR |= (1<<26);\	



void Init_pin_clocks(void);

#endif /* CLOCKS_AND_PIN_H_ */
