/**
 * \file
 *
 * \brief Declaration of main function used by example
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 *    AVR product.
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
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef _USB_H_
#define _USB_H_



#define USB_RX_BUFFER_SIZE			256
#define USB_TX_BUFFER_SIZE			256

#define USB_MANAGER_QUEUE_SIZE		5



// Module info commands
#define USB_CMD_REQ_APPL_VER			0x00
#define USB_CMD_REQ_HARDWARE_VER		0x01
#define USB_CMD_REQ_SERIAL_NUMBER		0x02
#define USB_CMD_REQ_TESTER_ID			0x03
#define USB_CMD_REQ_BATTERY_VOLTAGE		0x04
#define USB_CMD_REQ_BATTERY_TEMPERATURE	0x05
#define USB_CMD_REQ_BATTERY_INSTANT		0x06
#define USB_CMD_REQ_BATTERY_ACCUM		0x07
#define USB_CMD_REQ_BATTERY_UPDATE		0x08

// Commands to interact with the dataflash
#define USB_CMD_READ_USERPREFS			0x10
#define USB_CMD_READ_SAVEDTRACKS		0x11
#define USB_CMD_READ_RECORDTABLE		0x12
#define USB_CMD_READ_RECORDDATA			0x13
#define USB_CMD_ERASE_USERPREFS			0x14
#define USB_CMD_ERASE_SAVEDTRACKS		0x15
#define USB_CMD_ERASE_RECORDTABLE		0x16
#define USB_CMD_ERASE_RECORDDATA		0x17
#define USB_CMD_WRITE_USERPREFS			0x18
#define USB_CMD_WRITE_SAVEDTRACKS		0x19
#define USB_CMD_WRITE_RECORDTABLE		0x1A
#define USB_CMD_WRITE_RECORDDATA		0x1B
#define USB_CMD_READ_OTP				0x1C
#define USB_CMD_WRITE_OTP				0x1D

// Debug Commands
#define USB_DBG_SEND_DF_CMD				0x30	// Send a command to the dataflash
#define USB_DBG_READ_PM_PGOOD1			0x31	// Read the PM_PGOOD1 for the power supply
#define USB_DBG_READ_PM_PGOOD3			0x32	// Read the PM_PGOOD3 for the power supply
#define USB_DBG_READ_ADC_VCC			0x33	// Read the ADC value for VCC
#define USB_DBG_READ_ADC_VEE			0x34	// Read the ADC value for VEE
#define USB_DBG_READ_ADC_3V3			0x35	// Read the ADC value for 3V3
#define USB_DBG_SEND_GPS_CMD			0x36	// Send a command to the GPS
#define USB_DBG_READ_CHG_STAT			0x37	// Read the status for the charging
#define USB_DBG_SEND_FUEL_CMD			0x38	// Send a command to the fuel gauge

#define USB_DBG_DF_SECTOR_ERASE			0x39
#define USB_DBG_DF_BUSY					0x3A
#define USB_DBG_DF_CHIP_ERASE			0x3B
#define USB_DBG_DF_IS_FLASH_FULL		0x3C
#define USB_DBG_DF_USED_SPACE			0x3D	

#define USB_DBG_GPS_LATITUDE			0x40
#define USB_DBG_GPS_LONGITUDE			0x41
#define USB_DBG_GPS_COURSE				0x42


#define INDEX_CMD						0
#define INDEX_LENGTH					1
#define INDEX_DATA						2






void usb_task_init( void );
void usb_task( void *pvParameters );



/*! \brief Notify via user interface that enumeration is ok
 * This is called by vendor interface when USB Host enable it.
 *
 * \retval true if vendor startup is successfully done
 */
bool main_vendor_enable(void);

/*! \brief Notify via user interface that enumeration is disabled
 * This is called by vendor interface when USB Host disable it.
 */
void main_vendor_disable(void);

/*! \brief Attach or detach USB device
 * Called by UDC when Vbus line state changes
 *
 * \param b_high  True if VBus is present
 */
void main_vbus_action(bool b_high);

/*! \brief Manages the leds behaviors
 * Called when a start of frame is received on USB line each 1ms.
 */
void main_sof_action(void);

/*! \brief Enters the application in low power mode
 * Callback called when USB host sets USB line in suspend state
 */
void main_suspend_action(void);

/*! \brief Turn on a led to notify active mode
 * Called when the USB line is resumed from the suspend state
 */
void main_resume_action(void);

/*! \brief Manage the reception of setup request OUT
 *
 * \retval true if request accepted
 */
bool main_setup_out_received(void);

/*! \brief Manage the reception of setup request IN
 *
 * \retval true if request accepted
 */
bool main_setup_in_received(void);

#endif // _USB_H_