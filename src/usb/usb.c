/******************************************************************************
 *
 * USB Interface
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.2
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
#include "hal.h"
#include "string.h"

void main_vendor_bulk_in_received(udd_ep_status_t status, iram_size_t nb_transfered);
void main_vendor_read_callback(udd_ep_status_t status, iram_size_t nb_transfered);


// Buffer for loopback
static uint8_t usbRxBuffer[USB_RX_BUFFER_SIZE];
static uint8_t usbTxBuffer[USB_TX_BUFFER_SIZE];

xQueueHandle usbManagerQueue;

extern struct tFlashOTP flashOTP;
extern struct tFlashFlags flashFlags;
extern struct tUserPrefs userPrefs;

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
	unsigned int responseU32;
	signed int responseS32;
	unsigned short i;
	
	struct tTracklist trackList;
	
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
				usbTxBuffer[data_length++] = flashOTP.pcb_rev;
				break;
				
			case(USB_CMD_REQ_SERIAL_NUMBER):
				for(i = 0; i < OTP_SERIAL_LENGTH; i++){
					usbTxBuffer[data_length++] = flashOTP.serial[i];
				}					
				break;
				
			case(USB_CMD_REQ_TESTER_ID):
				usbTxBuffer[data_length++] = flashOTP.tester_id;				
				break;
				
				

			case(USB_CMD_REQ_BATTERY_VOLTAGE):
				fuel_send_request(FUEL_MGR_REQUEST_VOLTAGE, NULL, &responseU16, TRUE, NULL);
				usbTxBuffer[data_length++] = (responseU16 >> 8) & 0xFF;	// Battery voltage
				usbTxBuffer[data_length++] = (responseU16 >> 0) & 0xFF ;
				break;
				
			case(USB_CMD_REQ_BATTERY_TEMPERATURE):
				fuel_send_request(FUEL_MGR_REQUEST_TEMPERATURE, NULL, &responseU16, TRUE, NULL);
				usbTxBuffer[data_length++] = (responseU16 >> 8) & 0xFF;	// Battery voltage
				usbTxBuffer[data_length++] = (responseU16 >> 0) & 0xFF ;
				break;
				
			case(USB_CMD_REQ_BATTERY_ACCUM):
				fuel_send_request(FUEL_MGR_REQUEST_ACCUM_CURRENT, NULL, &responseU16, TRUE, NULL);
				usbTxBuffer[data_length++] = (responseU16 >> 8) & 0xFF;	// Battery voltage
				usbTxBuffer[data_length++] = (responseU16 >> 0) & 0xFF ;
				break;
				
				
			case(USB_CMD_REQ_BATTERY_INSTANT):
				fuel_send_request(FUEL_MGR_REQUEST_INSTANT_CURRENT, NULL, &responseU16, TRUE, NULL);
				usbTxBuffer[data_length++] = (responseU16 >> 8) & 0xFF;	// Battery voltage
				usbTxBuffer[data_length++] = (responseU16 >> 0) & 0xFF ;
				break;
				
			case(USB_CMD_REQ_BATTERY_UPDATE):	// 8d
				fuel_send_request(FUEL_MGR_REQUEST_UPDATE_ACCUM, NULL, NULL, TRUE, NULL);
				usbTxBuffer[data_length++] = TRUE;
				break;
				
					
			case(USB_CMD_READ_OTP):
				data_length = usbRxBuffer[1];
				//flash_send_request(FLASH_REQUEST_READ_OTP, &usbTxBuffer, usbRxBuffer[1], usbRxBuffer[2], TRUE, pdFALSE);
				flash_send_request(FLASH_MGR_READ_OTP, &usbTxBuffer, usbRxBuffer[1], usbRxBuffer[2], TRUE, pdFALSE);
				break;

			case(USB_CMD_READ_RECORDTABLE):
				data_length = usbRxBuffer[1];
				//flash_send_request(FLASH_REQUEST_READ_RECORDTABLE, &usbTxBuffer, usbRxBuffer[1], usbRxBuffer[2], TRUE, pdFALSE);
				flash_send_request(FLASH_MGR_READ_RECORDTABLE, &usbTxBuffer, usbRxBuffer[1], usbRxBuffer[2], TRUE, pdFALSE);
				break;
					
			case(USB_CMD_READ_RECORDDATA):	// 19d
				data_length = (usbRxBuffer[1] << 8) + usbRxBuffer[2];
				//flash_send_request(FLASH_REQUEST_READ_RECORDDATA, &usbTxBuffer, (usbRxBuffer[1] << 8) + usbRxBuffer[2], (usbRxBuffer[3] << 8) + usbRxBuffer[4], TRUE, pdFALSE);
				flash_send_request(FLASH_MGR_READ_RECORDATA, &usbTxBuffer, (usbRxBuffer[1] << 8) + usbRxBuffer[2], (usbRxBuffer[3] << 8) + usbRxBuffer[4], TRUE, pdFALSE);
				break;
				
			case(USB_DBG_DF_SECTOR_ERASE):
				usbTxBuffer[0] = TRUE;
				data_length = 1;
				//flash_send_request(FLASH_REQUEST_SECTOR_ERASE, NULL, NULL, usbRxBuffer[1], FALSE, pdFALSE);
				flash_send_request(FLASH_MGR_SECTOR_ERASE, NULL, NULL, usbRxBuffer[1], FALSE, pdFALSE);
				break;
				
			case(USB_DBG_DF_BUSY):
				data_length = 1;
				usbTxBuffer[0] = flash_is_busy();
				break;
				
			case(USB_CMD_ERASE_RECORDDATA):
				data_length = 1;
				//flash_send_request(FLASH_REQUEST_ERASE_RECORDED_DATA, NULL, NULL, NULL, FALSE, pdFALSE);
				flash_send_request(FLASH_MGR_ERASE_RECORDED_DATA, NULL, NULL, NULL, FALSE, pdFALSE);
				usbTxBuffer[0] = TRUE;
				break;
					
			case(USB_DBG_DF_IS_FLASH_FULL):
				data_length = 1;
				usbTxBuffer[0] = flash_full_flag();
				break;
					
			case(USB_DBG_DF_USED_SPACE):
				data_length = 1;
				usbTxBuffer[0] = TRUE;
				//flash_send_request(FLASH_REQUEST_USED_SPACE, NULL, NULL, NULL, FALSE, pdFALSE);
				flash_send_request(FLASH_MGR_USED_SPACE, NULL, NULL, NULL, FALSE, pdFALSE);
				break;
				
			case(USB_DBG_DF_CHIP_ERASE):
				data_length = 1;
				//flash_send_request(FLASH_REQUEST_CHIP_ERASE, &usbTxBuffer, NULL, NULL, TRUE, pdFALSE);
				flash_send_request(FLASH_MGR_CHIP_ERASE, &usbTxBuffer, NULL, NULL, TRUE, pdFALSE);
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

				flash_WriteOTP(0, 18, &usbTxBuffer);	// TODO: Change this to a flash manager request
				break;
				
			case(USB_CMD_WRITE_SAVEDTRACKS):	// 25d
				//flash_send_request(FLASH_REQUEST_ERASE_TRACKS, NULL, NULL, NULL, TRUE, pdFALSE);
				flash_send_request(FLASH_MGR_ERASE_TRACKS, NULL, NULL, NULL, TRUE, pdFALSE);
			
				strlcpy(&trackList.name, "Burn Pit", TRACKLIST_MAX_STRLEN);
				trackList.heading = 900;
				trackList.longitude = -83472585;
				trackList.latitude = 42558330;
				trackList.isEmpty = FALSE;
				trackList.reserved = 0xA5;
				//flash_send_request(FLASH_REQUEST_ADD_TRACK, &trackList, NULL, NULL, TRUE, pdFALSE);
				flash_send_request(FLASH_MGR_ADD_TRACK, &trackList, NULL, NULL, TRUE, pdFALSE);
				
				data_length = 1;
				usbTxBuffer[0] = TRUE;
				break;
				
			case(USB_CMD_READ_SAVEDTRACKS):		// 17d
				data_length = sizeof(trackList);
				//flash_send_request(FLASH_REQUEST_READ_TRACK, &usbTxBuffer, NULL, (usbRxBuffer[1] << 8) + (usbRxBuffer[2] << 0), TRUE, pdFALSE);
				flash_send_request(FLASH_MGR_READ_TRACK, &usbTxBuffer, NULL, (usbRxBuffer[1] << 8) + (usbRxBuffer[2] << 0), TRUE, pdFALSE);
				break;
				
			case(USB_CMD_WRITE_USERPREFS):	// 24d
				data_length = 1;
				usbTxBuffer[0] = TRUE;
				userPrefs.screenFadeTime = BACKLIGHT_DEFAULT_FADETIME;
				userPrefs.screenOffTime = BACKLIGHT_DEFAULT_OFFTIME;
				userPrefs.screenPWMMax = BACKLIGHT_DEFAULT_MAX;
				userPrefs.screenPWMMin = BACKLIGHT_DEFAULT_MIN;
				//flash_send_request(FLASH_REQUEST_WRITE_USER_PREFS, NULL, NULL, NULL, FALSE, pdFALSE);
				flash_send_request(FLASH_MGR_WRITE_USER_PREFS, NULL, NULL, NULL, FALSE, pdFALSE);
				break;
				
			case(USB_DBG_GPS_LATITUDE):		// 64d
				data_length = 4;
				gps_send_request(GPS_MGR_REQUEST_LATITUDE, &usbTxBuffer[0], NULL, pdFALSE, pdTRUE);
				break;
				
			case(USB_DBG_GPS_LONGITUDE):	// 65d
				data_length = 4;
				gps_send_request(GPS_MGR_REQUEST_LONGITUDE, &usbTxBuffer[0], NULL, pdFALSE, pdTRUE);
				break;
				
			case(USB_DBG_GPS_COURSE):	// 66d
				data_length = 4;
				gps_send_request(GPS_MGR_REQUEST_COURSE, &usbTxBuffer[0], NULL, pdFALSE, pdTRUE);
				break;
				
			case(USB_DBG_START_RECORDING):
				data_length = 1;
				gps_send_request(GPS_MGR_REQUEST_START_RECORDING, NULL, NULL, pdFALSE, pdTRUE);
				usbTxBuffer[0] = TRUE;
				break;
			
			case(USB_DBG_STOP_RECORDING):
				data_length = 1;
				gps_send_request(GPS_MGR_REQUEST_STOP_RECORDING, NULL, NULL, pdFALSE, pdTRUE);
				usbTxBuffer[0] = TRUE;
				break;
			
			case(USB_DBG_RECORDING_STATUS):
				data_length = 1;
				gps_send_request(GPS_MGR_REQUEST_RECORD_STATUS, &usbTxBuffer, NULL, pdFALSE, pdTRUE);
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
	if (status != UDD_EP_TRANSFER_OK) {
		return; // Transfer aborted, then stop loopback
	}
	
	udi_vendor_bulk_out_run(usbRxBuffer, sizeof(usbRxBuffer), main_vendor_read_callback);
}


void main_vendor_read_callback(udd_ep_status_t status, iram_size_t nb_transfered){
	unsigned char flag;
	
	if (status != UDD_EP_TRANSFER_OK) {
		return; // Transfer aborted, then stop loopback
	}	
	
	flag = TRUE;
	xQueueSendFromISR(usbManagerQueue, &flag, pdFALSE);
}