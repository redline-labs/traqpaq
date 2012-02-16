/******************************************************************************
 *
 * Main Application for traq|paq
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
static bool main_b_vendor_enable = false;

/**
 * \name Buffer for loopback
 */
//@{
//! Size of buffer used for the loopback
#define  MAIN_LOOPBACK_SIZE    1024
static uint8_t main_buf_loopback[MAIN_LOOPBACK_SIZE];
static uint8_t main_buf_iso_sel;
//@}

// check configuration
#if UDI_VENDOR_EPS_SIZE_ISO_FS>(MAIN_LOOPBACK_SIZE/2)
# error UDI_VENDOR_EPS_SIZE_ISO_FS must be <= MAIN_LOOPBACK_SIZE/2 in cond_usb.h
#endif
#ifdef USB_DEVICE_HS_SUPPORT
# if UDI_VENDOR_EPS_SIZE_ISO_HS>(MAIN_LOOPBACK_SIZE/2)
#   error UDI_VENDOR_EPS_SIZE_ISO_HS must be <= MAIN_LOOPBACK_SIZE/2 in cond_usb.h
# endif
#endif

void main_vendor_int_in_received(udd_ep_status_t status,
		iram_size_t nb_transfered);
void main_vendor_int_out_received(udd_ep_status_t status,
		iram_size_t nb_transfered);
void main_vendor_bulk_in_received(udd_ep_status_t status,
		iram_size_t nb_transfered);
void main_vendor_bulk_out_received(udd_ep_status_t status,
		iram_size_t nb_transfered);
void main_vendor_iso_in_received(udd_ep_status_t status,
		iram_size_t nb_transfered);
void main_vendor_iso_out_received(udd_ep_status_t status,
		iram_size_t nb_transfered);


// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main( void ){
	unsigned short temp = 0;
	
	wdt_disable();
	
	// Initialization ---------------------------------------------
	Disable_global_exception();
	Disable_global_interrupt();
	
	INTC_init_interrupts();
	board_init();
	
	// Check to see if we got reset from a watchdog timeout
	if( wdt_triggered() ){
		debug_log("WARNING [WDT]: Recovering from watchdog reset!");
	}
	
	if( !gpio_get_pin_value(GPIO_BUTTON2) ){
		debug_log("INFO [PM]: Powered on via USB");
	}
	
	// Schedule Tasks ---------------------------------------------
	#if( TRAQPAQ_HW_SPI_ENABLED )
	dataflash_task_init();
	#endif
	
	#if( TRAQPAQ_HW_EBI_ENABLED )
	lcd_task_init();
	#endif
		
	#if( TRAQPAQ_HW_PWM_ENABLED )
	pwm_task_init();
	#endif
	
	#if( TRAQPAQ_HW_USB_ENABLED )
	udc_start();
	#endif
	
	#if( TRAQPAQ_HW_TWI_ENABLED )
	fuel_task_init();
	#endif
	
	#if( TRAQPAQ_HW_GPS_ENABLED )
	gps_task_init();
	#else
	// Kick the GPS out of reset so we can use it for external datalogging
	gpio_set_gpio_pin(GPS_RESET);
	#endif
	
	#if( TRAQPAQ_HW_EXINT_ENABLED )
	buttons_task_init();
	// If switches are enabled, then keep the main power supply on!
	gpio_set_gpio_pin(PM_ENABLE);
	#endif
	
	#if( TRAQPAQ_HW_CHARGE_ENABLED )
	charge_task_init();
	#endif
	
	#if( TRAQPAQ_HW_ADC_ENABLED )
	adc_task_init();
	#endif
	
	#if( TRAQPAQ_HW_WDT_ENABLED )
	wdt_task_init();
	#endif
	
	Enable_global_interrupt();

	// Start the scheduler! ---------------------------------------
	vTaskStartScheduler();
	
	debug_log("CRITICAL [RTOS]: Scheduler has ended");
	
	// Should never reach this!
	while (true){
		asm("nop");
	};			
}
void main_vbus_action(bool b_high)
{
	if (b_high) {
		// Attach USB Device
		udc_attach();
	} else {
		// VBUS not present
		udc_detach();
	}
}

void main_suspend_action(void)
{
	asm("nop");
}

void main_resume_action(void)
{
	asm("nop");
}

void main_sof_action(void)
{
	if (!main_b_vendor_enable)
		return;
	asm("nop");
}

bool main_vendor_enable(void)
{
	main_b_vendor_enable = true;
	// Start data reception on OUT endpoints
	main_vendor_int_in_received(UDD_EP_TRANSFER_OK, 0);
	main_vendor_bulk_in_received(UDD_EP_TRANSFER_OK, 0);
	main_buf_iso_sel=0;
	main_vendor_iso_out_received(UDD_EP_TRANSFER_OK, 0);
	return true;
}

void main_vendor_disable(void)
{
	main_b_vendor_enable = false;
}

bool main_setup_out_received(void)
{
	asm("nop");
	udd_g_ctrlreq.payload = main_buf_loopback;
	udd_g_ctrlreq.payload_size = min(
			udd_g_ctrlreq.req.wLength,
			sizeof(main_buf_loopback));
	return true;
}

bool main_setup_in_received(void)
{
	asm("nop");
	udd_g_ctrlreq.payload = main_buf_loopback;
	udd_g_ctrlreq.payload_size =
			min( udd_g_ctrlreq.req.wLength,
			sizeof(main_buf_loopback) );
	return true;
}

void main_vendor_int_in_received(udd_ep_status_t status,
		iram_size_t nb_transfered)
{
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

void main_vendor_int_out_received(udd_ep_status_t status,
		iram_size_t nb_transfered)
{
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

void main_vendor_bulk_in_received(udd_ep_status_t status,
		iram_size_t nb_transfered)
{
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Tranfert aborted, then stop loopback
	}
	asm("nop");
	// Wait a full buffer
	udi_vendor_bulk_out_run(
			main_buf_loopback,
			sizeof(main_buf_loopback),
			main_vendor_bulk_out_received);
}

void main_vendor_bulk_out_received(udd_ep_status_t status,
		iram_size_t nb_transfered)
{
	if (UDD_EP_TRANSFER_OK != status) {
		return; // Tranfert aborted, then stop loopback
	}
	asm("nop");
	// Send on IN endpoint the data received on endpoint OUT
	udi_vendor_bulk_in_run(
			main_buf_loopback,
			nb_transfered,
			main_vendor_bulk_in_received);
}

void main_vendor_iso_in_received(udd_ep_status_t status,
		iram_size_t nb_transfered)
{
	asm("nop");
}

void main_vendor_iso_out_received(udd_ep_status_t status,
		iram_size_t nb_transfered)
{
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

	// Immediatly enable a transfer on next USB isochronous OUT packet
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
}

/**
 * \mainpage ASF USB Device Vendor Example
 *
 * \section intro Introduction
 * This example shows how to implement a USB Device implementing Vendor Class
 * on Atmel MCU with USB module.
 *
 * \section startup Startup
 * The example uses a vendor class which implements a loopback on
 * all endpoints types: control, interrupt, bulk and isochronous.
 * After loading firmware, connect the board
 * (EVKxx,XPlain,...) to the USB Host. A Host applciation developped
 * on libusb library is provided in application note AVR4001.

 * \note
 * When the application is connected for the first time to the PC,
 * the operating system will detect a new peripheral:
 * - This will open a new hardware installation wizard on Windows operating systems.
 * - Choose "No, not this time" to connect to Windows Update for this installation
 * - click "Next"
 * - When requested to search the INF file, browse the avr4901\drivers\ folder
 *   provided package of the AVR4901 applicatinon note.
 * - click "Next"
*
 * \copydoc UI
 *
 * \section example About example
 *
 * The example uses the following module groups:
 * - Basic modules:
 *   Startup, board, clock, interrupt, power management
 * - USB Device stack and vendor modules:
 *   <br>services/usb/
 *   <br>services/usb/udc/
 *   <br>services/usb/class/vendor/
 * - Specific implementation:
 *    - main.c,
 *      <br>initializes clock
 *      <br>initializes interrupt
 *      <br>manages UI
 *    - specific implementation for each target "./examples/product_board/":
 *       - conf_foo.h   configuration of each module
 *       - ui.c        implement of user's interface (leds)
 */
