/******************************************************************************
 *
 * Button interface
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.1
 * - AppNote:			N/A
 *
 * - Last Author:		Ryan David ( ryan.david@redline-electronics.com )
 *
 *
 * Copyright (c) 2012 Redline Electronics LLC.
 *
 * This file is part of traq|paq.
 *
 * traq|paq is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * traq|paq is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with traq|paq. If not, see http://www.gnu.org/licenses/.
 *
 ******************************************************************************/

#include <asf.h>
#include "drivers.h"
#include "lcd/menu.h"

extern struct tMenu mainMenu;

static xQueueHandle buttonPress;

__attribute__((__interrupt__)) static void ISR_button0(void) {
	unsigned char button = 0;
	eic_clear_interrupt_line(&AVR32_EIC, EXTINT_BUTTON0);
	xQueueSendFromISR(buttonPress, &button, pdFALSE);
}  

__attribute__((__interrupt__)) static void ISR_button1(void) {
	unsigned char button = 1;
	eic_clear_interrupt_line(&AVR32_EIC, EXTINT_BUTTON1);
	xQueueSendFromISR(buttonPress, &button, pdFALSE);
}  

__attribute__((__interrupt__)) static void ISR_button2(void) {
	unsigned char button = 2;
	eic_clear_interrupt_line(&AVR32_EIC, EXTINT_BUTTON2);
	xQueueSendFromISR(buttonPress, &button, pdFALSE);
}  

__attribute__((__interrupt__)) static void ISR_button3(void) {
	unsigned char button = 3; 
	eic_clear_interrupt_line(&AVR32_EIC, EXTINT_BUTTON3);
	xQueueSendFromISR(buttonPress, &button, pdFALSE);
}  

void buttons_task_init( void ){
	buttonPress = xQueueCreate(1, sizeof(unsigned char));
	
	INTC_register_interrupt(&ISR_button0, EXTINT_BUTTON0_IRQ, EXTINT_BUTTON0); 
	INTC_register_interrupt(&ISR_button1, EXTINT_BUTTON1_IRQ, EXTINT_BUTTON1); 
	INTC_register_interrupt(&ISR_button2, EXTINT_BUTTON2_IRQ, EXTINT_BUTTON2); 
	INTC_register_interrupt(&ISR_button3, EXTINT_BUTTON3_IRQ, EXTINT_BUTTON3); 
	
	xTaskCreate(buttons_task, configTSK_BUTTONS_TASK_NAME, configTSK_BUTTONS_TASK_STACK_SIZE, NULL, configTSK_BUTTONS_TASK_PRIORITY, NULL);
}

// LCD GUI Task
void buttons_task( void *pvParameters ){
	unsigned char button;
	
	while(1){
		asm volatile ("nop");
		//vTaskDelay( (portTickType)TASK_DELAY_MS(1000) );

		if( xQueueReceive(buttonPress, &button, portMAX_DELAY) == pdTRUE){
			switch(button){
				case(0):
					menu_scrollUp(&mainMenu);
					break;
				case(1):
					menu_scrollDown(&mainMenu);
					break;
				case(2):
					asm("nop");
					break;
				case(3):
					asm("nop");
					break;

			}
			vTaskDelay( (portTickType)TASK_DELAY_MS(BUTTON_DEBOUNCE_TIME) );
			xQueueReceive(buttonPress, &button, 0);		// Flush queue
		}			
			
	}
}
