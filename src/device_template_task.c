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
#include "dataflash/dataflash_manager_request.h"


/*#define Enable_usb_dma_tx_send(start_address, length) { \ 
  AVR32_USBB.uddma1_addr = (uint32_t)(start_address);   \ 
  AVR32_USBB.uddma1_control = ((length) << 16) |        \ 
    AVR32_USBB_UXDMAX_CONTROL_DMAEND_EN_MASK |          \ 
    AVR32_USBB_UXDMAX_CONTROL_CH_EN_MASK;               \ 
} */


static unsigned long data_length = 0;

extern xQueueHandle dataflashManagerQueue;


void device_template_task_init(void){

	xTaskCreate(device_template_task, configTSK_USB_DTP_NAME, configTSK_USB_DTP_STACK_SIZE, NULL, configTSK_USB_DTP_PRIORITY, configTSK_USB_DTP_HANDLE);
}


void device_template_task(void *pvParameters){
	static U8 rxBuf[EP_BUFFER_TEMP2];
	static U8 txBuf[EP_BUFFER_TEMP1];
	
	unsigned char responseU8;
	unsigned short responseU16;
	unsigned int responseU32;
	
	unsigned long i;
	
	portTickType xLastWakeTime;
	
	struct tDataflashRequest request;

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
				
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_CMD_REQ_HARDWARE_VER):
					txBuf[data_length++] = dataflashOTP.pcb_rev;
					break;
				#endif
				
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_CMD_REQ_SERIAL_NUMBER):
					for(i = 0; i < OTP_SERIAL_LENGTH; i++){
						txBuf[data_length++] = dataflashOTP.serial[i];
					}					
					break;
				#endif
				
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_CMD_REQ_TESTER_ID):
					txBuf[data_length++] = dataflashOTP.tester_id;				
					break;
				#endif
					
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
					
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_CMD_READ_OTP):
					request.command	= DFMAN_REQUEST_READ_OTP;
					request.length	= rxBuf[1];
					request.index	= rxBuf[2];
					request.pointer	= &txBuf;
					request.resume	= xTaskGetCurrentTaskHandle();
					data_length = rxBuf[1];
					xQueueSend(dataflashManagerQueue, &request, 20);
					vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
					break;
				#endif

				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_CMD_READ_RECORDTABLE):
					request.command	= DFMAN_REQUEST_READ_RECORDTABLE;
					request.length	= rxBuf[1];
					request.index	= rxBuf[2];
					request.pointer	= &txBuf;
					request.resume	= xTaskGetCurrentTaskHandle();
					data_length = rxBuf[1];
					xQueueSend(dataflashManagerQueue, &request, 20);
					vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
					break;
				#endif
				
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_CMD_WRITE_RECORDTABLE):
					request.command = DFMAN_REQUEST_UPDATE_RECORDTABLE;
					request.resume = NULL;
					txBuf[data_length++] = TRUE;
					xQueueSend(dataflashManagerQueue, &request, 20);
					break;
				#endif
					
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_CMD_READ_RECORDDATA):
					request.command	= DFMAN_REQUEST_READ_RECORDDATA;
					request.length	= rxBuf[1];
					request.index	= rxBuf[2];
					request.pointer	= &txBuf;
					request.resume	= xTaskGetCurrentTaskHandle();
					data_length = rxBuf[1];
					xQueueSend(dataflashManagerQueue, &request, 20);
					vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
					break;
				#endif
					
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_CMD_WRITE_RECORDDATA):
					for(i = 0; i < 256; i++){
						txBuf[i] = i;
					}
					
					request.command	= DFMAN_REQUEST_ADD_RECORDDATA;
					request.length	= rxBuf[1];
					request.pointer	= &txBuf;
					request.resume	= xTaskGetCurrentTaskHandle();
					data_length = 1;
					xQueueSend(dataflashManagerQueue, &request, 20);
					vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
					break;
				#endif
				
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_DBG_DF_SECTOR_ERASE):
					request.command	= DFMAN_REQUEST_SECTOR_ERASE;
					request.index = rxBuf[1];
					request.resume = NULL;
					txBuf[0] = TRUE;
					data_length = 1;
					xQueueSend(dataflashManagerQueue, &request, 20);
					break;
				#endif
				
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_DBG_DF_BUSY):
					request.command	= DFMAN_REQUEST_BUSY;
					request.pointer	= &txBuf;
					request.resume	= xTaskGetCurrentTaskHandle();
					data_length = 1;
					xQueueSend(dataflashManagerQueue, &request, 20);
					vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
					break;
				#endif
					
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_DBG_DF_CHIP_ERASE):
					request.command	= DFMAN_REQUEST_CHIP_ERASE;
					request.pointer = &txBuf;
					request.resume = NULL;
					data_length = 1;
					xQueueSend(dataflashManagerQueue, &request, 20);
					break;
				#endif
					
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_DBG_DF_IS_FLASH_FULL):
					request.command	= DFMAN_REQUEST_IS_FLASH_FULL;
					request.pointer = &txBuf;
					request.resume	= xTaskGetCurrentTaskHandle();
					data_length = 1;
					xQueueSend(dataflashManagerQueue, &request, 20);
					vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
					break;
				#endif
					
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_DBG_DF_USED_SPACE):
					request.command	= DFMAN_REQUEST_USED_SPACE;
					request.pointer = &txBuf;
					request.resume	= xTaskGetCurrentTaskHandle();
					data_length = 1;
					xQueueSend(dataflashManagerQueue, &request, 20);
					vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
					break;
					
				#endif
				
				#if( TRAQPAQ_HW_SPI_ENABLED )
				case(USB_CMD_WRITE_OTP):
					txBuf[data_length++] = 'T';
					txBuf[data_length++] = 'R';
					txBuf[data_length++] = 'A';
					txBuf[data_length++] = 'Q';
					txBuf[data_length++] = '0';
					txBuf[data_length++] = '3';
					txBuf[data_length++] = '6';
					txBuf[data_length++] = '1';
					txBuf[data_length++] = '2';
					txBuf[data_length++] = '0';
					txBuf[data_length++] = '0';
					txBuf[data_length++] = '0';
					txBuf[data_length++] = '1';
					
					txBuf[data_length++] = 0x12;	// PCB Rev
					txBuf[data_length++] = 0x30;	// ID of test fixture
					txBuf[data_length++] = 0xAA;	// Reserved
					
					responseU16 = 0;
					for(i = 0; i < 16; i++){
						responseU16 = update_crc_ccitt(responseU16, txBuf[i]);
					}
					
					txBuf[data_length++] = (responseU16 >> 8) & 0xFF;
					txBuf[data_length++] = (responseU16 >> 0) & 0xFF;
					
					dataflash_WriteOTP(0, 18, &txBuf);
					break;
				#endif
					
				#if( TRAQPAQ_HW_TWI_ENABLED )
				case(USB_DBG_SEND_FUEL_CMD):
					responseU16 = 0;
					fuel_write_register(FUEL_ADDRESS_ACCUM_CURRENT_REGISTER_MSB, &responseU16, 2);
					fuel_read_current(FUEL_CURRENT_ACCUMULATED);
					break;
				#endif
					
					
				default:
					// Unknown command!
					debug_log("ERROR [USB]: Unknown command received");	
					break;
			}
			
			i = 0;
			while (data_length){
			  while (!Is_usb_in_ready(EP_TEMP_IN));

			  Usb_reset_endpoint_fifo_access(EP_TEMP_IN);
			  data_length = usb_write_ep_txpacket(EP_TEMP_IN, &(txBuf[i]), data_length, NULL);
			  i += 64;
			  Usb_ack_in_ready_send(EP_TEMP_IN);
			}
			

		}
	}
}