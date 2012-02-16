/*
 * usb.c
 *
 * Created: 2/16/2012 3:44:28 PM
 *  Author: Ryan
 */ 

#include "asf.h"
#include "drivers.h"
#include "usb/usb.h"

static bool main_b_vendor_enable = false;


void main_vendor_int_in_received(udd_ep_status_t status, iram_size_t nb_transfered);
void main_vendor_int_out_received(udd_ep_status_t status, iram_size_t nb_transfered);
void main_vendor_bulk_in_received(udd_ep_status_t status, iram_size_t nb_transfered);
void main_vendor_bulk_out_received(udd_ep_status_t status, iram_size_t nb_transfered);
void main_vendor_iso_in_received(udd_ep_status_t status, iram_size_t nb_transfered);
void main_vendor_iso_out_received(udd_ep_status_t status, iram_size_t nb_transfered);

/**
 * \name Buffer for loopback
 */
//@{
//! Size of buffer used for the loopback
static uint8_t main_buf_loopback[MAIN_LOOPBACK_SIZE];
static uint8_t main_buf_iso_sel;
//@}

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