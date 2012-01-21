/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief Management of the USB high-level applicative device task.
 *
 * This file manages the USB high-level applicative device task.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a USB module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ***************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */

#include "asf.h"
#include "drivers.h"

#include "conf_usb.h"
#include "usb_drv.h"
#include "usb_descriptors.h"
#include "usb_standard_request.h"
#include "device_template_task.h"

#define Enable_usb_dma_tx_send(start_address, length); { \ 
  AVR32_USBB.uddma1_addr = (uint32_t)(start_address);   \ 
  AVR32_USBB.uddma1_control = ((length) << 16) |        \ 
    AVR32_USBB_UXDMAX_CONTROL_DMAEND_EN_MASK |          \ 
    AVR32_USBB_UXDMAX_CONTROL_CH_EN_MASK;               \ 
}

static unsigned char data_length = 0;


void device_template_task_init(void){

	xTaskCreate(device_template_task, configTSK_USB_DTP_NAME, configTSK_USB_DTP_STACK_SIZE, NULL, configTSK_USB_DTP_PRIORITY, NULL);
}


void device_template_task(void *pvParameters){
	unsigned char i;
	static U8 rxBuf[EP_SIZE_TEMP2];
	static U8 txBuf[EP_SIZE_TEMP2];
	
	unsigned char responseU8;
	unsigned short responseU16;
	unsigned int responseU32;

	portTickType xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();
	while (true){
		vTaskDelayUntil(&xLastWakeTime, configTSK_USB_DTP_PERIOD);

		// First, check the device enumeration state
		if (!Is_device_enumerated()) continue;

		// If we receive something in the OUT endpoint, just store it in the RAM buffer
		if (Is_usb_out_received(EP_TEMP_OUT)){
			Usb_reset_endpoint_fifo_access(EP_TEMP_OUT);
			data_length = Usb_byte_count(EP_TEMP_OUT);
			usb_read_ep_rxpacket(EP_TEMP_OUT, rxBuf, data_length, NULL);
			Usb_ack_out_received_free(EP_TEMP_OUT);
		}

		// Load the IN endpoint with the contents of the RAM buffer
		if (data_length && Is_usb_in_ready(EP_TEMP_IN)){
			
			data_length = 0;
			
			switch(rxBuf[0]){
				case(USB_CMD_REQ_APPL_VER):
					txBuf[data_length++] = TRAQPAQ_SW_LEVEL;
					break;
					
				case(USB_CMD_REQ_HARDWARE_VER):
					txBuf[data_length++] = TRAQPAQ_HW_LEVEL;
					break;
					
				case(USB_CMD_REQ_SERIAL_NUMBER):
					txBuf[data_length++] = 'T';
					txBuf[data_length++] = 'R';
					txBuf[data_length++] = 'A';
					txBuf[data_length++] = 'Q';
					txBuf[data_length++] = '2';
					txBuf[data_length++] = '3';
					txBuf[data_length++] = '9';
					txBuf[data_length++] = '1';
					txBuf[data_length++] = '1';
					txBuf[data_length++] = '0';
					txBuf[data_length++] = '0';
					txBuf[data_length++] = '0';
					txBuf[data_length++] = '1';
					break;
					
				case(USB_CMD_REQ_BATTINFO):
					responseU16 = fuel_read_voltage();
					txBuf[data_length++] = (responseU16 >> 8) & 0xFF;	// Battery voltage
					txBuf[data_length++] = (responseU16 >> 0) & 0xFF ;
					
					responseU16 = fuel_read_temperature();
					txBuf[data_length++] = (responseU16 >> 8) & 0xFF;	// Temperature
					txBuf[data_length++] = (responseU16 >> 0) & 0xFF;
					
					responseU16 = fuel_read_current(FUEL_CURRENT_INSTANTANEOUS);
					txBuf[data_length++] = (responseU16 >> 8) & 0xFF;	// Instantaneous Current
					txBuf[data_length++] = (responseU16 >> 0) & 0xFF;
					
					responseU16 = fuel_read_current(FUEL_CURRENT_ACCUMULATED);
					txBuf[data_length++] = (responseU16 >> 8) & 0xFF;	// Accumulated Current
					txBuf[data_length++] = (responseU16 >> 0) & 0xFF;
					break;
					
	
				case(USB_CMD_READ_OTP):
					if(rxBuf[1] == 0){
						for(i = 0; i <= 63; i++){
							txBuf[i] = i;
						}
						data_length = 64;
						
					}else{
						for(i = 0; i <= 63; i++){
							txBuf[i] = i+63;
						}
						data_length = 64;
					}
					
				case(USB_CMD_WRITE_OTP):
					
					
					
				default:
					// Unknown command!
					debug_log("ERROR [USB]: Unknown command received");

			}
			
			
			Usb_reset_endpoint_fifo_access(EP_TEMP_IN);
			usb_write_ep_txpacket(EP_TEMP_IN, &(txBuf), data_length, NULL);
			Usb_ack_in_ready_send(EP_TEMP_IN);
			data_length = 0;	
		}
	}
}