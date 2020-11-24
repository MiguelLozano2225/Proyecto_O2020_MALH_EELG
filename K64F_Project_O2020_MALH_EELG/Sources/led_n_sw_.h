/*
 * led_n_sw_.h
 *
 *  Created on: Sep 12, 2020
 *      Author: migue
 */

#ifndef LED_N_SW__H_
#define LED_N_SW__H_
#include "derivative.h"


#define LED_RED_ON			GPIOB_PDOR &= ~(1<<22)	
#define LED_RED_OFF			GPIOB_PDOR |=  (1<<22)	
#define LED_RED_TOG			GPIOB_PDOR ^=  (1<<22)

#define LED_BLUE_ON			GPIOB_PDOR &= ~(1<<21)	
#define LED_BLUE_OFF		GPIOB_PDOR |=  (1<<21)	
#define LED_BLUE_TOG		GPIOB_PDOR ^=  (1<<21)

#define LED_GREEN_ON		GPIOE_PDOR &= ~(1<<26)	
#define LED_GREEN_OFF		GPIOE_PDOR |=  (1<<26)	
#define LED_GRENN_TOG		GPIOE_PDOR ^=  (1<<26)





#endif /* LED_N_SW__H_ */
