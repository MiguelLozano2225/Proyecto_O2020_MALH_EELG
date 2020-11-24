/*
 * NVIC.c
 *
 *  Created on: Sep 9, 2017
 *      Author: Edgar
 */

#include "NVIC.h"

void Enable_IRQ (unsigned char IRQ){
	
	IRQ-=16;
	NVIC_BASE_PTR->ICPR[IRQ/32]=(1<<IRQ%32); 
	NVIC_BASE_PTR->ISER[IRQ/32]=(1<<IRQ%32);
	
}

void Disable_IRQ (unsigned char IRQ){
	
	IRQ-=16;
	NVIC_BASE_PTR->ICER[IRQ/32]=(1<<IRQ%32); 
	
}

void Priority_IRQ (unsigned char IRQ, unsigned char LEVEL){
	
	IRQ-=16;
	NVIC_BASE_PTR->IP[IRQ/4]=((LEVEL&0xF)<<(8*(IRQ%4)+4));

}
