/******************************************************************************
 *
 * USB Interface
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.4
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
#include "usb_commands.h"

void main_vendor_bulk_in_received(udd_ep_status_t status, iram_size_t nb_transfered);
void main_vendor_read_callback(udd_ep_status_t status, iram_size_t nb_transfered);

// Storage for Tx/Rx USB frames
struct tUsbRxFrame usbRx;
struct tUsbTxFrame usbTx;

xQueueHandle usbManagerQueue;

extern struct tFlashOTP flashOTP;
extern struct tFlashFlags flashFlags;
extern struct tUserPrefs userPrefs;
extern struct tGPSInfo gpsInfo;
extern struct tFlash flash;

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
		
		// Assume the command is valid now, catch this in default case
		usbTx.command = usbRx.command;
		
		switch(usbRx.command){
			case(USB_CMD_REQ_APPL_VER):
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_APPL_VER);
				usbTx.message.CMD_REQ_APPL_VER.swVerMajor = TRAQPAQ_SW_LEVEL_MAJOR;
				usbTx.message.CMD_REQ_APPL_VER.swVerMinor = TRAQPAQ_SW_LEVEL_MINOR;
				break;
				
			case(USB_CMD_REQ_HARDWARE_VER):
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_HARDWARE_VER);
				usbTx.message.CMD_REQ_HARDWARE_VER.hwVer = flashOTP.pcb_rev;
				break;
				
			case(USB_CMD_REQ_SERIAL_NUMBER):
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_SERIAL_NUMBER);
				memcpy(&usbTx.message.CMD_REQ_SERIAL_NUMBER.serialNumber, &flashOTP.serial, OTP_SERIAL_LENGTH);				
				break;
				
			case(USB_CMD_REQ_TESTER_ID):
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_TESTER_ID);
				usbTx.message.CMD_REQ_TESTER_ID.testerId = flashOTP.tester_id;				
				break;

			case(USB_CMD_REQ_BATTERY_VOLTAGE):
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_BATTERY_VOLTAGE);
				fuel_send_request(FUEL_MGR_REQUEST_VOLTAGE, NULL, &usbTx.message.CMD_REQ_BATTERY_VOLTAGE.batteryVoltage, TRUE, NULL);
				break;
				
			case(USB_CMD_REQ_BATTERY_TEMPERATURE):
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_BATTERY_TEMPERATURE);
				fuel_send_request(FUEL_MGR_REQUEST_TEMPERATURE, NULL, &usbTx.message.CMD_REQ_BATTERY_TEMPERATURE.batteryTemperature, TRUE, NULL);
				break;
				
			case(USB_CMD_REQ_BATTERY_ACCUM):
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_BATTERY_ACCUMULATED);
				fuel_send_request(FUEL_MGR_REQUEST_ACCUM_CURRENT, NULL, &usbTx.message.CMD_REQ_BATTERY_ACCUMULATED.accumulatedCurrent, TRUE, NULL);
				break;
				
			case(USB_CMD_REQ_BATTERY_INSTANT):
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_BATTERY_INSTANT);
				fuel_send_request(FUEL_MGR_REQUEST_INSTANT_CURRENT, NULL, &usbTx.message.CMD_REQ_BATTERY_INSTANT.instantCurrent, TRUE, NULL);
				break;
				
			case(USB_CMD_REQ_BATTERY_UPDATE):	// 8d
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_BATTERY_UPDATE);
				fuel_send_request(FUEL_MGR_REQUEST_UPDATE_ACCUM, NULL, NULL, TRUE, NULL);
				usbTx.message.CMD_REQ_BATTERY_UPDATE.success = TRUE;		// TODO: Have the pass/fail determined by the fuel task
				break;
				
			case(USB_CMD_WRITE_BATTERY_INFO):
				usbTx.msgLength = sizeof(usbTx.message.CMD_REQ_WRITE_BATTERY_INFO);
				fuel_send_request(FUEL_MGR_REQUEST_WRITE_BATTERY_INFO, NULL, &usbTx.message.CMD_REQ_WRITE_BATTERY_INFO.success, TRUE, NULL);
				break;
				
			case(USB_CMD_READ_OTP):
				usbTx.msgLength = sizeof(usbTx.message.CMD_READ_OTP);
				flash_send_request(FLASH_MGR_READ_OTP, &usbTx.message.CMD_READ_OTP, usbRx.message.CMD_READ_OTP.length, usbRx.message.CMD_READ_OTP.index, TRUE, pdFALSE);
				break;

			case(USB_CMD_READ_RECORDTABLE):
				usbTx.msgLength = sizeof(usbTx.message.CMD_READ_RECORD_TABLE);	// TODO: Look at requesting size
				flash_send_request(FLASH_MGR_READ_RECORDTABLE, &usbTx.message.CMD_READ_RECORD_TABLE, usbRx.message.CMD_READ_RECORD_TABLE.length, usbRx.message.CMD_READ_RECORD_TABLE.index, TRUE, pdFALSE);
				break;
					
			case(USB_CMD_READ_RECORDDATA):	// 19d
				usbTx.msgLength = sizeof(usbTx.message.CMD_READ_RECORD_DATA);	// TODO: Look at requesting size
				flash_send_request(FLASH_MGR_READ_RECORDATA, &usbTx.message.CMD_READ_RECORD_DATA, usbRx.message.CMD_READ_RECORD_DATA.length, usbRx.message.CMD_READ_RECORD_DATA.index, TRUE, pdFALSE);
				break;
				
			case(USB_DBG_READ_ADC):			// 51d
				usbTx.msgLength = sizeof(usbTx.message.DBG_READ_ADC_VALUES);
				fuel_send_request(FUEL_MGR_REQUEST_ADC_VALUES, NULL, &usbTx.message.DBG_READ_ADC_VALUES, TRUE, NULL);
				break;
				
			case(USB_DBG_DF_SECTOR_ERASE):
				usbTx.msgLength = sizeof(usbTx.message.DBG_FLASH_SECTOR_ERASE);
				flash_send_request(FLASH_MGR_SECTOR_ERASE, NULL, NULL, usbRx.message.DBG_FLASH_SECTOR_ERASE.index, FALSE, pdFALSE);
				usbTx.message.DBG_FLASH_SECTOR_ERASE.success = TRUE;		// TODO: Have flash task set success
				break;
				
			case(USB_DBG_DF_BUSY):
				usbTx.msgLength = sizeof(usbTx.message.DBG_FLASH_BUSY);
				usbTx.message.DBG_FLASH_BUSY.busy = flash_is_busy();
				break;
				
			case(USB_CMD_ERASE_RECORDDATA):
				usbTx.msgLength = sizeof(usbTx.message.CMD_ERASE_RECORD_DATA);
				flash_send_request(FLASH_MGR_ERASE_RECORDED_DATA, NULL, NULL, NULL, FALSE, pdFALSE);
				usbTx.message.CMD_ERASE_RECORD_DATA.success = TRUE;			// TODO: Have flash task set success
				break;
					
			case(USB_DBG_DF_IS_FLASH_FULL):
				usbTx.msgLength = sizeof(usbTx.message.DBG_FLASH_IS_FULL);
				usbTx.message.DBG_FLASH_IS_FULL.isFull = flash_full_flag();
				break;
					
			case(USB_DBG_DF_USED_SPACE):
				usbTx.msgLength = sizeof(usbTx.message.DBG_FLASH_USED_SPACE);
				flash_send_request(FLASH_MGR_USED_SPACE, NULL, NULL, NULL, FALSE, pdFALSE);		// TODO: Fix this request!
				usbTx.message.DBG_FLASH_USED_SPACE.usedPercent = TRUE;
				break;
				
			case(USB_DBG_DF_CHIP_ERASE):
				usbTx.msgLength = sizeof(usbTx.message.DBG_FLASH_CHIP_ERASE);
				flash_send_request(FLASH_MGR_CHIP_ERASE, &usbTx.message.DBG_FLASH_CHIP_ERASE.success, NULL, NULL, FALSE, pdFALSE);
				break;
				
			case(USB_CMD_WRITE_OTP):
				usbTx.msgLength = sizeof(usbTx.message.CMD_WRITE_OTP);
				flash_send_request(FLASH_MGR_WRITE_OTP, &usbRx.message.CMD_WRITE_OTP, NULL, NULL, TRUE, pdFALSE);	// TODO: Need to implement this flash mgr request
				usbTx.message.CMD_WRITE_OTP.success = TRUE;		// TODO: Have flash mgr set this flag properly
				break;
				
			case(USB_CMD_WRITE_SAVEDTRACKS):	// 25d
				usbTx.msgLength = sizeof(usbTx.message.CMD_WRITE_SAVED_TRACKS);
				flash_send_request(FLASH_MGR_ERASE_TRACKS, NULL, NULL, NULL, TRUE, pdFALSE);
				flash_send_request(FLASH_MGR_ADD_TRACK, &usbRx.message.CMD_WRITE_SAVED_TRACKS, NULL, NULL, TRUE, pdFALSE);

				usbTx.message.CMD_WRITE_SAVED_TRACKS.success = TRUE;
				break;
		
			case(USB_CMD_READ_SAVEDTRACKS):		// 17d
				usbTx.msgLength = sizeof(usbTx.message.CMD_READ_SAVED_TRACKS);
				flash_send_request(FLASH_MGR_READ_TRACK, &usbTx.message.CMD_READ_SAVED_TRACKS, NULL, usbRx.message.CMD_READ_SAVED_TRACKS.index, TRUE, pdFALSE);
				break;
				
			case(USB_CMD_WRITE_USERPREFS):	// 24d
				usbTx.msgLength = sizeof(usbTx.message.CMD_WRITE_USER_PREFS);
				//userPrefs.screenFadeTime = BACKLIGHT_DEFAULT_FADETIME;
				//userPrefs.screenOffTime = BACKLIGHT_DEFAULT_OFFTIME;
				//userPrefs.screenPWMMax = BACKLIGHT_DEFAULT_MAX;
				//userPrefs.screenPWMMin = BACKLIGHT_DEFAULT_MIN;
				flash_send_request(FLASH_MGR_WRITE_USER_PREFS, NULL, NULL, NULL, FALSE, pdFALSE);	// TODO: Change to usbRx data
				usbTx.message.CMD_WRITE_USER_PREFS.success = TRUE;		// TODO: Have Flash mgr set success
				break;
				
			case(USB_DBG_DF_ARB_READ):
				usbTx.msgLength = sizeof(usbTx.message.DBG_FLASH_ARB_READ);	//TODO: Have it set by the usbRx request
				flash_send_request(FLASH_MGR_READ_PAGE, &usbTx.message.DBG_FLASH_ARB_READ.data, usbRx.message.DBG_FLASH_ARB_READ.length, usbRx.message.DBG_FLASH_ARB_READ.index, TRUE, pdFALSE);
				break;
				
			case(USB_DBG_GPS_CURRENT_POSITION):		// 64d
				usbTx.msgLength = sizeof(usbTx.message.DBG_GPS_CURRENT_POSITION);
				usbTx.message.DBG_GPS_CURRENT_POSITION.latitude = gpsInfo.current_location.latitude;
				usbTx.message.DBG_GPS_CURRENT_POSITION.longitude = gpsInfo.current_location.longitude;
				usbTx.message.DBG_GPS_CURRENT_POSITION.course = gpsInfo.current_location.heading;
				break;
				
			case(USB_DBG_GPS_CURRENT_MODE):
				usbTx.msgLength = sizeof(usbTx.message.DBG_GPS_CURRENT_MODE);
				usbTx.message.DBG_GPS_CURRENT_MODE.mode = gpsInfo.mode;
				usbTx.message.DBG_GPS_CURRENT_MODE.satellites = gpsInfo.satellites;
				break;
				
			case(USB_DBG_START_RECORDING):
				usbTx.msgLength = sizeof(usbTx.message.DBG_GPS_START_RECORDING);
			
				// Set an arbitrary finish point first
				gps_send_request(GPS_MGR_REQUEST_SET_FINISH_POINT, NULL, 0, pdFALSE, pdTRUE);
				gps_send_request(GPS_MGR_REQUEST_START_RECORDING, NULL, NULL, pdFALSE, pdTRUE);
				
				usbTx.message.DBG_GPS_START_RECORDING.success = TRUE;
				break;
			
			case(USB_DBG_STOP_RECORDING):
				usbTx.msgLength = sizeof(usbTx.message.DBG_GPS_STOP_RECORDING);
				gps_send_request(GPS_MGR_REQUEST_STOP_RECORDING, NULL, NULL, pdFALSE, pdTRUE);
				usbTx.message.DBG_GPS_START_RECORDING.success = TRUE;
				break;
			
			case(USB_DBG_RECORDING_STATUS):
				usbTx.msgLength = sizeof(usbTx.message.DBG_GPS_RECORDING_STATUS);
				usbTx.message.DBG_GPS_RECORDING_STATUS.status = gpsInfo.record_flag;
				break;
				
			case(USB_DBG_GPS_INFO_SN):	// 67d
				usbTx.msgLength = sizeof(usbTx.message.DBG_GPS_SERIAL_NUMBER);
				usbTx.message.DBG_GPS_SERIAL_NUMBER.valid = gpsInfo.serial_number_valid;
				memcpy(&usbTx.message.DBG_GPS_SERIAL_NUMBER.serialNumber, gpsInfo.serial_number, sizeof(usbTx.message.DBG_GPS_SERIAL_NUMBER.serialNumber));
				break;
				
			case(USB_DBG_GPS_INFO_PN):	// 68d
				usbTx.msgLength = sizeof(usbTx.message.DBG_GPS_PART_NUMBER);
				usbTx.message.DBG_GPS_PART_NUMBER.valid = gpsInfo.part_number_valid;
				memcpy(&usbTx.message.DBG_GPS_PART_NUMBER.partNumber, gpsInfo.part_number, sizeof(usbTx.message.DBG_GPS_PART_NUMBER.partNumber));
				break;
				
			case(USB_DBG_GPS_INFO_SW_VER):	// 69d
				usbTx.msgLength = sizeof(usbTx.message.DBG_GPS_SW_VER);
				usbTx.message.DBG_GPS_SW_VER.valid = gpsInfo.sw_version_valid;
				memcpy(&usbTx.message.DBG_GPS_SW_VER.swVer, gpsInfo.sw_version, sizeof(usbTx.message.DBG_GPS_SW_VER.swVer));
				break;
				
			case(USB_DBG_GPS_INFO_SW_DATE):	// 70d
				usbTx.msgLength = sizeof(usbTx.message.DBG_GPS_SW_DATE);
				usbTx.message.DBG_GPS_SW_DATE.valid = gpsInfo.sw_date_valid;
				memcpy(&usbTx.message.DBG_GPS_SW_DATE.swDate, gpsInfo.sw_date, sizeof(usbTx.message.DBG_GPS_SW_DATE.swDate));
				break;

			default:
				// Unknown command!
				usbTx.command = USB_UNKNOWN_CMD;
				usbTx.msgLength = 0;
				debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_USB, "Invalid Command Received");
				break;
		}
		
		// Send the response
		udi_vendor_bulk_in_run(&usbTx, usbTx.msgLength + sizeof(usbTx.command) + sizeof(usbTx.msgLength), main_vendor_bulk_in_received);	
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

	udd_g_ctrlreq.payload = &usbRx;
	udd_g_ctrlreq.payload_size = min(
			udd_g_ctrlreq.req.wLength,
			sizeof(usbRx));
	return true;
}

bool main_setup_in_received(void){

	udd_g_ctrlreq.payload = &usbRx;
	udd_g_ctrlreq.payload_size =
			min( udd_g_ctrlreq.req.wLength,
			sizeof(usbRx) );
	return true;
}



// Receive data into module
void main_vendor_bulk_in_received(udd_ep_status_t status, iram_size_t nb_transfered) {
	if (status != UDD_EP_TRANSFER_OK) {
		return; // Transfer aborted, then stop loopback
	}
	
	udi_vendor_bulk_out_run(&usbRx, sizeof(usbRx), main_vendor_read_callback);
}


void main_vendor_read_callback(udd_ep_status_t status, iram_size_t nb_transfered){
	unsigned char flag;
	
	if (status != UDD_EP_TRANSFER_OK) {
		return; // Transfer aborted, then stop loopback
	}	
	
	flag = TRUE;
	xQueueSendFromISR(usbManagerQueue, &flag, pdFALSE);
}