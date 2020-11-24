/*
 * NVIC.h
 *
 *  Created on: Sep 9, 2017
 *      Author: Edgar
 */

#ifndef NVIC_H_
#define NVIC_H_

#include "derivative.h"

void Enable_IRQ (unsigned char IRQ);
void Disable_IRQ (unsigned char IRQ);
void Priority_IRQ (unsigned char IRQ, unsigned char LEVEL);

#endif /* NVIC_H_ */
