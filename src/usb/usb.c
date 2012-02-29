/******************************************************************************
 *
 * USB Interface
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

#include "asf.h"
#include "drivers.h"
#include "dataflash/dataflash_manager_request.h"
#include "dataflash/dataflash_otp_layout.h"

void main_vendor_bulk_in_received(udd_ep_status_t status, iram_size_t nb_transfered);
//void main_vendor_bulk_out_received(udd_ep_status_t status, iram_size_t nb_transfered);
//void main_vendor_int_in_received(udd_ep_status_t status, iram_size_t nb_transfered);
//void main_vendor_int_out_received(udd_ep_status_t status, iram_size_t nb_transfered);
//void main_vendor_iso_in_received(udd_ep_status_t status, iram_size_t nb_transfered);
//void main_vendor_iso_out_received(udd_ep_status_t status, iram_size_t nb_transfered);
void main_vendor_read_callback(udd_ep_status_t status, iram_size_t nb_transfered);


// Buffer for loopback
static uint8_t usbRxBuffer[USB_RX_BUFFER_SIZE];
static uint8_t usbTxBuffer[USB_TX_BUFFER_SIZE];

xQueueHandle usbManagerQueue;
extern xQueueHandle dataflashManagerQueue;

extern struct tDataflashOTP dataflashOTP;

// Create task for FreeRTOS
void usb_task_init( void ){
	udc_start();
	
	usbManagerQueue = xQueueCreate(USB_MANAGER_QUEUE_SIZE, sizeof(char));
	
	xTaskCreate(usb_task, configTSK_USB_TASK_NAME, configTSK_USB_TASK_STACK_SIZE, NULL, configTSK_USB_TASK_PRIORITY, configTSK_USB_TASK_HANDLE);
}

void usb_task( void *pvParameters ){
	unsigned char flag;
	unsigned short data_length;
	unsigned char responseU8;
	unsigned short responseU16;
	unsigned short i;
	
	struct tDataflashRequest dataflashRequest;
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_USB, "Task Started");
	
	while(TRUE){
		xQueueReceive(usbManagerQueue, &flag, portMAX_DELAY);
		
		data_length = 0;
		
		switch(usbRxBuffer[0]){
			case(USB_CMD_REQ_APPL_VER):
				usbTxBuffer[data_length++] = TRAQPAQ_SW_LEVEL_MAJOR;
				usbTxBuffer[data_length++] = TRAQPAQ_SW_LEVEL_MINOR;
				break;
				
			case(USB_CMD_REQ_HARDWARE_VER):
				usbTxBuffer[data_length++] = dataflashOTP.pcb_rev;
				break;
				
			case(USB_CMD_REQ_SERIAL_NUMBER):
				for(i = 0; i < OTP_SERIAL_LENGTH; i++){
					usbTxBuffer[data_length++] = dataflashOTP.serial[i];
				}					
				break;
				
			case(USB_CMD_REQ_TESTER_ID):
				usbTxBuffer[data_length++] = dataflashOTP.tester_id;				
				break;

			case(USB_CMD_REQ_BATTINFO):
				responseU16 = fuel_read_voltage();
				usbTxBuffer[data_length++] = (responseU16 >> 8) & 0xFF;	// Battery voltage
				usbTxBuffer[data_length++] = (responseU16 >> 0) & 0xFF ;

				responseU16 = fuel_read_temperature();
				usbTxBuffer[data_length++] = (responseU16 >> 8) & 0xFF;	// Temperature
				usbTxBuffer[data_length++] = (responseU16 >> 0) & 0xFF;

				responseU16 = fuel_read_current(FUEL_CURRENT_INSTANTANEOUS);
				usbTxBuffer[data_length++] = (responseU16 >> 8) & 0xFF;	// Instantaneous Current
				usbTxBuffer[data_length++] = (responseU16 >> 0) & 0xFF;

				responseU16 = fuel_read_current(FUEL_CURRENT_ACCUMULATED);
				usbTxBuffer[data_length++] = (responseU16 >> 8) & 0xFF;	// Accumulated Current
				usbTxBuffer[data_length++] = (responseU16 >> 0) & 0xFF;
				break;
					
			case(USB_CMD_READ_OTP):
				dataflashRequest.command	= DFMAN_REQUEST_READ_OTP;
				dataflashRequest.length	= usbRxBuffer[1];
				dataflashRequest.index	= usbRxBuffer[2];
				dataflashRequest.pointer	= &usbTxBuffer;
				dataflashRequest.resume	= xTaskGetCurrentTaskHandle();
				data_length = usbRxBuffer[1];
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
				break;

			case(USB_CMD_READ_RECORDTABLE):
				dataflashRequest.command	= DFMAN_REQUEST_READ_RECORDTABLE;
				dataflashRequest.length	= usbRxBuffer[1];
				dataflashRequest.index	= usbRxBuffer[2];
				dataflashRequest.pointer	= &usbTxBuffer;
				dataflashRequest.resume	= xTaskGetCurrentTaskHandle();
				data_length = usbRxBuffer[1];
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
				break;
				
			case(USB_CMD_WRITE_RECORDTABLE):
				dataflashRequest.command = DFMAN_REQUEST_END_CURRENT_RECORD;
				dataflashRequest.resume = NULL;
				usbTxBuffer[data_length++] = TRUE;
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				break;
					
			case(USB_CMD_READ_RECORDDATA):
				dataflashRequest.command	= DFMAN_REQUEST_READ_RECORDDATA;
				dataflashRequest.length	= DATAFLASH_PAGE_SIZE;
				dataflashRequest.index	= (usbRxBuffer[2] << 8) + (usbRxBuffer[3] << 0);
				dataflashRequest.pointer	= &usbTxBuffer;
				dataflashRequest.resume	= xTaskGetCurrentTaskHandle();
				data_length = usbRxBuffer[1];
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
				break;
					
			case(USB_CMD_WRITE_RECORDDATA):
				for(i = 0; i < 256; i++){
					usbTxBuffer[i] = i;
				}

				dataflashRequest.command	= DFMAN_REQUEST_ADD_RECORDDATA;
				dataflashRequest.length	= usbRxBuffer[1];
				dataflashRequest.pointer	= &usbTxBuffer;
				dataflashRequest.resume	= xTaskGetCurrentTaskHandle();
				data_length = 1;
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
				break;
				
			case(USB_DBG_DF_SECTOR_ERASE):
				dataflashRequest.command	= DFMAN_REQUEST_SECTOR_ERASE;
				dataflashRequest.index = usbRxBuffer[1];
				dataflashRequest.resume = NULL;
				usbTxBuffer[0] = TRUE;
				data_length = 1;
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				break;
				
			case(USB_DBG_DF_BUSY):
				dataflashRequest.command	= DFMAN_REQUEST_BUSY;
				dataflashRequest.pointer	= &usbTxBuffer;
				dataflashRequest.resume	= xTaskGetCurrentTaskHandle();
				data_length = 1;
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
				break;
					
			case(USB_DBG_DF_CHIP_ERASE):
				dataflashRequest.command	= DFMAN_REQUEST_CHIP_ERASE;
				dataflashRequest.pointer = &usbTxBuffer;
				dataflashRequest.resume = xTaskGetCurrentTaskHandle();
				data_length = 1;
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
				break;
					
			case(USB_DBG_DF_IS_FLASH_FULL):
				dataflashRequest.command	= DFMAN_REQUEST_IS_FLASH_FULL;
				dataflashRequest.pointer = &usbTxBuffer;
				dataflashRequest.resume	= xTaskGetCurrentTaskHandle();
				data_length = 1;
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
				break;
					
			case(USB_DBG_DF_USED_SPACE):
				dataflashRequest.command	= DFMAN_REQUEST_USED_SPACE;
				dataflashRequest.pointer = &usbTxBuffer;
				dataflashRequest.resume	= xTaskGetCurrentTaskHandle();
				data_length = 1;
				xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
				vTaskSuspend(NULL);		// Wait until the dataflash manager is completed processing request
				break;
				
			case(USB_CMD_WRITE_OTP):
				usbTxBuffer[data_length++] = 'T';
				usbTxBuffer[data_length++] = 'R';
				usbTxBuffer[data_length++] = 'A';
				usbTxBuffer[data_length++] = 'Q';
				usbTxBuffer[data_length++] = '0';
				usbTxBuffer[data_length++] = '3';
				usbTxBuffer[data_length++] = '6';
				usbTxBuffer[data_length++] = '1';
				usbTxBuffer[data_length++] = '2';
				usbTxBuffer[data_length++] = '0';
				usbTxBuffer[data_length++] = '0';
				usbTxBuffer[data_length++] = '0';
				usbTxBuffer[data_length++] = '1';

				usbTxBuffer[data_length++] = 0x12;	// PCB Rev
				usbTxBuffer[data_length++] = 0x30;	// ID of test fixture
				usbTxBuffer[data_length++] = 0xAA;	// Reserved

				responseU16 = 0;
				for(i = 0; i < 16; i++){
					responseU16 = update_crc_ccitt(responseU16, usbTxBuffer[i]);
				}

				usbTxBuffer[data_length++] = (responseU16 >> 8) & 0xFF;
				usbTxBuffer[data_length++] = (responseU16 >> 0) & 0xFF;

				dataflash_WriteOTP(0, 18, &usbTxBuffer);
				break;


			default:
				// Unknown command!
				debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_USB, "Invalid Command Received");
				break;
		}
		
		// Send the response
		if(data_length){
			udi_vendor_bulk_in_run(&usbTxBuffer, data_length, main_vendor_bulk_in_received);
		}		
	}
}










void main_vbus_action(bool b_high){
	if (b_high) {
		// Attach USB Device
		udc_attach();
	} else {
		// VBUS not present
		udc_detach();
	}
}

void main_suspend_action(void){
	asm("nop");
}

void main_resume_action(void){
	asm("nop");
}

bool main_vendor_enable(void){
	// Start data reception on OUT endpoints
	main_vendor_bulk_in_received(UDD_EP_TRANSFER_OK, 0);
	return true;
}

void main_vendor_disable(void){
	asm("nop");
}

bool main_setup_out_received(void){

	udd_g_ctrlreq.payload = usbRxBuffer;
	udd_g_ctrlreq.payload_size = min(
			udd_g_ctrlreq.req.wLength,
			sizeof(usbRxBuffer));
	return true;
}

bool main_setup_in_received(void){

	udd_g_ctrlreq.payload = usbRxBuffer;
	udd_g_ctrlreq.payload_size =
			min( udd_g_ctrlreq.req.wLength,
			sizeof(usbRxBuffer) );
	return true;
}



// Receive data into module
void main_vendor_bulk_in_received(udd_ep_status_t status, iram_size_t nb_transfered) {
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Transfer aborted, then stop loopback
	}
	
	udi_vendor_bulk_out_run(usbRxBuffer, sizeof(usbRxBuffer), main_vendor_read_callback);
}


void main_vendor_read_callback(udd_ep_status_t status, iram_size_t nb_transfered){
	unsigned char flag;
	
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Transfer aborted, then stop loopback
	}	
	
	flag = TRUE;
	xQueueSendFromISR(usbManagerQueue, &flag, pdFALSE);
}



// Send Data out of module
/*void main_vendor_bulk_out_received(udd_ep_status_t status, iram_size_t nb_transfered) {
	
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Transfer aborted, then stop loopback
	}
	
	// Send on IN endpoint the data received on endpoint OUT
	udi_vendor_bulk_in_run(usbRxBuffer, nb_transfered, main_vendor_bulk_in_received);
}*/







/*void main_vendor_iso_in_received(udd_ep_status_t status, iram_size_t nb_transfered) {
	asm("nop");
}

void main_vendor_iso_out_received(udd_ep_status_t status, iram_size_t nb_transfered) {
	uint8_t *buf_ptr;

	if (UDD_EP_TRANSFER_OK != status) {
		return; // Tranfert aborted, then stop loopback
	}

	if (nb_transfered) {
		asm("nop");
		// Send on IN endpoint the data received on endpoint OUT
		buf_ptr = &main_buf_loopback[ main_buf_iso_sel
				*(sizeof(main_buf_loopback)/2) ];
		udi_vendor_iso_in_run(
				buf_ptr,
				nb_transfered,
				main_vendor_iso_in_received);
	}

	// Switch of buffer
	main_buf_iso_sel = main_buf_iso_sel? 0:1;

	// Immediately enable a transfer on next USB isochronous OUT packet
	// to avoid to skip a USB packet.
	// NOTE:
	// Here the expected buffer size is equal to endpoint size.
	// Thus, this transfer request will end after reception of
	// one USB packet.
	//
	// When using buffer size larger than endpoint size,
	// the requested transfer is stopped when the buffer is = full*.
	// *on USBC and XMEGA USB driver, the buffer is full
	// when "number of data transfered" > "buffer size" - "endppoint size".
	buf_ptr = &main_buf_loopback[ main_buf_iso_sel
			*(sizeof(main_buf_loopback)/2) ];

	// Send on IN endpoint the data received on endpoint OUT
	udi_vendor_iso_out_run(
			buf_ptr,
			udd_is_high_speed()?
				UDI_VENDOR_EPS_SIZE_ISO_HS:UDI_VENDOR_EPS_SIZE_ISO_FS,
			main_vendor_iso_out_received);
}*/


/*
void main_vendor_int_in_received(udd_ep_status_t status, iram_size_t nb_transfered){
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Tranfert aborted, then stop loopback
	}
	asm("nop");
	// Wait a full buffer
	udi_vendor_interrupt_out_run(
			main_buf_loopback,
			sizeof(main_buf_loopback),
			main_vendor_int_out_received);
}
*/
/*
void main_vendor_int_out_received(udd_ep_status_t status, iram_size_t nb_transfered) {
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Tranfert aborted, then stop loopback
	}
	asm("nop");
	// Send on IN endpoint the data received on endpoint OUT
	udi_vendor_interrupt_in_run(
			main_buf_loopback,
			nb_transfered,
			main_vendor_int_in_received);
}
*/