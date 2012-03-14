/**
 * \file
 *
 * \brief USB Vendor class interface definitions.
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

#ifndef _UDI_VENDOR_H_
#define _UDI_VENDOR_H_

#include "conf_usb.h"
#include "usb_protocol.h"
#include "usb_protocol_vendor.h"
#include "udd.h"
#include "udc_desc.h"
#include "udi.h"

#ifdef __cplusplus
extern "C" {
#endif

// Configuration Full Speed check
#ifndef UDI_VENDOR_EPS_SIZE_INT_FS
# error UDI_VENDOR_EPS_SIZE_INT_FS must be defined in conf_usb.h file.
#endif
#ifndef UDI_VENDOR_EPS_SIZE_BULK_FS
# error UDI_VENDOR_EPS_SIZE_BULK_FS must be defined in conf_usb.h file.
#endif
#ifndef UDI_VENDOR_EPS_SIZE_ISO_FS
# error UDI_VENDOR_EPS_SIZE_ISO_FS must be defined in conf_usb.h file.
#endif

// Configuration High Speed check
#ifdef USB_DEVICE_HS_SUPPORT
# ifndef UDI_VENDOR_EPS_SIZE_INT_HS
#   error UDI_VENDOR_EPS_SIZE_INT_HS must be defined in conf_usb.h file.
# endif
# ifndef UDI_VENDOR_EPS_SIZE_BULK_HS
#   error UDI_VENDOR_EPS_SIZE_BULK_HS must be defined in conf_usb.h file.
# endif
# ifndef UDI_VENDOR_EPS_SIZE_ISO_HS
#   error UDI_VENDOR_EPS_SIZE_ISO_HS must be defined in conf_usb.h file.
# endif
#endif

/**
 * \ingroup udi_group
 * \defgroup udi_vendor_group UDI for vendor Class
 *
 * @{
 */

/**
 * \name Interface Descriptor
 *
 * The following structures provide the interface descriptor.
 * It must be implemented in USB configuration descriptor.
 * @{
 */

/**
 * \name Enpoint descriptors
 * @{
 */

#if UDI_VENDOR_EPS_SIZE_INT_FS
# define UDI_VENDOR_EPS_INT_DESC \
	.ep_interrupt_in.bLength           = sizeof(usb_ep_desc_t), \
	.ep_interrupt_in.bDescriptorType   = USB_DT_ENDPOINT, \
	.ep_interrupt_in.bEndpointAddress  = UDI_VENDOR_EP_INTERRUPT_IN, \
	.ep_interrupt_in.bmAttributes      = USB_EP_TYPE_INTERRUPT, \
	.ep_interrupt_in.bInterval         = 0, \
	.ep_interrupt_out.bLength          = sizeof(usb_ep_desc_t), \
	.ep_interrupt_out.bDescriptorType  = USB_DT_ENDPOINT,\
	.ep_interrupt_out.bEndpointAddress = UDI_VENDOR_EP_INTERRUPT_OUT, \
	.ep_interrupt_out.bmAttributes     = USB_EP_TYPE_INTERRUPT, \
	.ep_interrupt_out.bInterval        = 0

# define UDI_VENDOR_EPS_INT_DESC_FS \
	.ep_interrupt_in.wMaxPacketSize    = LE16(UDI_VENDOR_EPS_SIZE_INT_FS), \
	.ep_interrupt_out.wMaxPacketSize   = LE16(UDI_VENDOR_EPS_SIZE_INT_FS) \

# define UDI_VENDOR_EPS_INT_DESC_HS \
	.ep_interrupt_in.wMaxPacketSize    = LE16(UDI_VENDOR_EPS_SIZE_INT_HS), \
	.ep_interrupt_out.wMaxPacketSize   = LE16(UDI_VENDOR_EPS_SIZE_INT_HS) \

#else
# define UDI_VENDOR_EPS_INT_DESC
# define UDI_VENDOR_EPS_INT_DESC_FS
# define UDI_VENDOR_EPS_INT_DESC_HS
#endif

#if UDI_VENDOR_EPS_SIZE_BULK_FS
# define UDI_VENDOR_EPS_BULK_DESC \
	.ep_bulk_in.bLength                = sizeof(usb_ep_desc_t), \
	.ep_bulk_in.bDescriptorType        = USB_DT_ENDPOINT, \
	.ep_bulk_in.bEndpointAddress       = UDI_VENDOR_EP_BULK_IN, \
	.ep_bulk_in.bmAttributes           = USB_EP_TYPE_BULK, \
	.ep_bulk_in.bInterval              = 0, \
	.ep_bulk_out.bLength               = sizeof(usb_ep_desc_t), \
	.ep_bulk_out.bDescriptorType       = USB_DT_ENDPOINT, \
	.ep_bulk_out.bEndpointAddress      = UDI_VENDOR_EP_BULK_OUT, \
	.ep_bulk_out.bmAttributes          = USB_EP_TYPE_BULK, \
	.ep_bulk_out.bInterval             = 0

# define UDI_VENDOR_EPS_BULK_DESC_FS \
	.ep_bulk_in.wMaxPacketSize         = LE16(UDI_VENDOR_EPS_SIZE_BULK_FS), \
	.ep_bulk_out.wMaxPacketSize        = LE16(UDI_VENDOR_EPS_SIZE_BULK_FS) \

# define UDI_VENDOR_EPS_BULK_DESC_HS \
	.ep_bulk_in.wMaxPacketSize         = LE16(UDI_VENDOR_EPS_SIZE_BULK_HS), \
	.ep_bulk_out.wMaxPacketSize        = LE16(UDI_VENDOR_EPS_SIZE_BULK_HS) \

#else
# define UDI_VENDOR_EPS_BULK_DESC
# define UDI_VENDOR_EPS_BULK_DESC_FS
# define UDI_VENDOR_EPS_BULK_DESC_HS
#endif


#if UDI_VENDOR_EPS_SIZE_ISO_FS
# define UDI_VENDOR_EPS_ISO_DESC \
	.ep_iso_in.bLength                 = sizeof(usb_ep_desc_t), \
	.ep_iso_in.bDescriptorType         = USB_DT_ENDPOINT, \
	.ep_iso_in.bEndpointAddress        = UDI_VENDOR_EP_ISO_IN, \
	.ep_iso_in.bmAttributes            = USB_EP_TYPE_ISOCHRONOUS, \
	.ep_iso_in.bInterval               = 0, \
	.ep_iso_out.bLength                = sizeof(usb_ep_desc_t), \
	.ep_iso_out.bDescriptorType        = USB_DT_ENDPOINT, \
	.ep_iso_out.bEndpointAddress       = UDI_VENDOR_EP_ISO_OUT, \
	.ep_iso_out.bmAttributes           = USB_EP_TYPE_ISOCHRONOUS, \
	.ep_iso_out.bInterval              = 0

# define UDI_VENDOR_EPS_ISO_DESC_FS \
	.ep_iso_in.wMaxPacketSize          = LE16(UDI_VENDOR_EPS_SIZE_ISO_FS), \
	.ep_iso_out.wMaxPacketSize         = LE16(UDI_VENDOR_EPS_SIZE_ISO_FS) \

# define UDI_VENDOR_EPS_ISO_DESC_HS \
	.ep_iso_in.wMaxPacketSize          = LE16(UDI_VENDOR_EPS_SIZE_ISO_HS), \
	.ep_iso_out.wMaxPacketSize         = LE16(UDI_VENDOR_EPS_SIZE_ISO_HS) \

#else
# define UDI_VENDOR_EPS_ISO_DESC
# define UDI_VENDOR_EPS_ISO_DESC_FS
# define UDI_VENDOR_EPS_ISO_DESC_HS
#endif

//@}

//! Interface descriptor struture for vendor Class interface
typedef struct {
	usb_iface_desc_t iface;
#if UDI_VENDOR_EPS_SIZE_INT_FS
	usb_ep_desc_t ep_interrupt_in;
	usb_ep_desc_t ep_interrupt_out;
#endif
#if UDI_VENDOR_EPS_SIZE_BULK_FS
	usb_ep_desc_t ep_bulk_in;
	usb_ep_desc_t ep_bulk_out;
#endif
#if UDI_VENDOR_EPS_SIZE_ISO_FS
	usb_ep_desc_t ep_iso_in;
	usb_ep_desc_t ep_iso_out;
#endif
} udi_vendor_desc_t;

//! By default no string associated to this interface
#ifndef UDI_VENDOR_STRING_ID
#define UDI_VENDOR_STRING_ID     0
#endif



//! Content of vendor interface descriptor for all speeds
#define UDI_VENDOR_DESC      \
	.iface.bLength                     = sizeof(usb_iface_desc_t),\
	.iface.bDescriptorType             = USB_DT_INTERFACE,\
	.iface.bInterfaceNumber            = UDI_VENDOR_IFACE_NUMBER,\
	.iface.bAlternateSetting           = 0,\
	.iface.bNumEndpoints               = USB_DEVICE_MAX_EP,\
	.iface.bInterfaceClass             = VENDOR_CLASS,\
	.iface.bInterfaceSubClass          = VENDOR_SUBCLASS,\
	.iface.bInterfaceProtocol          = VENDOR_PROTOCOL,\
	.iface.iInterface                  = UDI_VENDOR_STRING_ID,\
	UDI_VENDOR_EPS_INT_DESC, \
	UDI_VENDOR_EPS_BULK_DESC, \
	UDI_VENDOR_EPS_ISO_DESC, \

//! Content of vendor interface descriptor for full speed only
/*#define UDI_VENDOR_DESC_FS {\
	UDI_VENDOR_DESC \
	UDI_VENDOR_EPS_INT_DESC_FS, \
	UDI_VENDOR_EPS_BULK_DESC_FS, \
	UDI_VENDOR_EPS_ISO_DESC_FS, \
   }*/
#define UDI_VENDOR_DESC_FS {\
	.iface.bLength                     = sizeof(usb_iface_desc_t),\
	.iface.bDescriptorType             = USB_DT_INTERFACE,\
	.iface.bInterfaceNumber            = UDI_VENDOR_IFACE_NUMBER,\
	.iface.bAlternateSetting           = 0,\
	.iface.bNumEndpoints               = USB_DEVICE_MAX_EP,\
	.iface.bInterfaceClass             = VENDOR_CLASS,\
	.iface.bInterfaceSubClass          = VENDOR_SUBCLASS,\
	.iface.bInterfaceProtocol          = VENDOR_PROTOCOL,\
	.iface.iInterface                  = UDI_VENDOR_STRING_ID,\
	.ep_bulk_in.bLength                = sizeof(usb_ep_desc_t),\
	.ep_bulk_in.bDescriptorType        = USB_DT_ENDPOINT,\
	.ep_bulk_in.bEndpointAddress       = UDI_VENDOR_EP_BULK_IN,\
	.ep_bulk_in.bmAttributes           = USB_EP_TYPE_BULK,\
	.ep_bulk_in.bInterval              = 0,\
	.ep_bulk_out.bLength               = sizeof(usb_ep_desc_t),\
	.ep_bulk_out.bDescriptorType       = USB_DT_ENDPOINT,\
	.ep_bulk_out.bEndpointAddress      = UDI_VENDOR_EP_BULK_OUT,\
	.ep_bulk_out.bmAttributes          = USB_EP_TYPE_BULK,\
	.ep_bulk_out.bInterval             = 0, \
	UDI_VENDOR_EPS_BULK_DESC_FS \
   }

//! Content of vendor interface descriptor for high speed only
#define UDI_VENDOR_DESC_HS   {\
	UDI_VENDOR_DESC \
	UDI_VENDOR_EPS_INT_DESC_HS, \
	UDI_VENDOR_EPS_BULK_DESC_HS, \
	UDI_VENDOR_EPS_ISO_DESC_HS, \
   }
//@}


//! Global struture which contains standard UDI interface for UDC
extern UDC_DESC_STORAGE udi_api_t udi_api_vendor;


#if UDI_VENDOR_EPS_SIZE_INT_FS
/**
 * \brief Start a transfer on interrupt IN
 *
 * When the transfer is finished or aborted (stall, reset, ...), the \a callback is called.
 * The \a callback returns the transfer status and eventually the number of byte transfered.
 *
 * \param buf           Buffer on Internal RAM to send or fill.
 *                      It must be align, then use COMPILER_WORD_ALIGNED.
 * \param buf_size      Buffer size to send or fill
 * \param callback      NULL or function to call at the end of transfer
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_vendor_interrupt_in_run(uint8_t * buf, iram_size_t buf_size,
		udd_callback_trans_t callback);

/**
 * \brief Start a transfer on interrupt OUT
 *
 * When the transfer is finished or aborted (stall, reset, ...), the \a callback is called.
 * The \a callback returns the transfer status and eventually the number of byte transfered.
 *
 * \param buf           Buffer on Internal RAM to send or fill.
 *                      It must be align, then use COMPILER_WORD_ALIGNED.
 * \param buf_size      Buffer size to send or fill
 * \param callback      NULL or function to call at the end of transfer
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_vendor_interrupt_out_run(uint8_t * buf, iram_size_t buf_size,
		udd_callback_trans_t callback);
#endif

#if UDI_VENDOR_EPS_SIZE_BULK_FS
/**
 * \brief Start a transfer on bulk IN
 *
 * When the transfer is finished or aborted (stall, reset, ...), the \a callback is called.
 * The \a callback returns the transfer status and eventually the number of byte transfered.
 *
 * \param buf           Buffer on Internal RAM to send or fill.
 *                      It must be align, then use COMPILER_WORD_ALIGNED.
 * \param buf_size      Buffer size to send or fill
 * \param callback      NULL or function to call at the end of transfer
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_vendor_bulk_in_run(uint8_t * buf, iram_size_t buf_size,
		udd_callback_trans_t callback);

/**
 * \brief Start a transfer on bulk OUT
 *
 * When the transfer is finished or aborted (stall, reset, ...), the \a callback is called.
 * The \a callback returns the transfer status and eventually the number of byte transfered.
 *
 * \param buf           Buffer on Internal RAM to send or fill.
 *                      It must be align, then use COMPILER_WORD_ALIGNED.
 * \param buf_size      Buffer size to send or fill
 * \param callback      NULL or function to call at the end of transfer
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_vendor_bulk_out_run(uint8_t * buf, iram_size_t buf_size,
		udd_callback_trans_t callback);
#endif


#if UDI_VENDOR_EPS_SIZE_ISO_FS
/**
 * \brief Start a transfer on isochronous IN
 *
 * When the transfer is finished or aborted (stall, reset, ...), the \a callback is called.
 * The \a callback returns the transfer status and eventually the number of byte transfered.
 *
 * \param buf           Buffer on Internal RAM to send or fill.
 *                      It must be align, then use COMPILER_WORD_ALIGNED.
 * \param buf_size      Buffer size to send or fill
 * \param callback      NULL or function to call at the end of transfer
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_vendor_iso_in_run(uint8_t * buf, iram_size_t buf_size,
		udd_callback_trans_t callback);

/**
 * \brief Start a transfer on isochronous OUT
 *
 * When the transfer is finished or aborted (stall, reset, ...), the \a callback is called.
 * The \a callback returns the transfer status and eventually the number of byte transfered.
 *
 * \param buf           Buffer on Internal RAM to send or fill.
 *                      It must be align, then use COMPILER_WORD_ALIGNED.
 * \param buf_size      Buffer size to send or fill
 * \param callback      NULL or function to call at the end of transfer
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_vendor_iso_out_run(uint8_t * buf, iram_size_t buf_size,
		udd_callback_trans_t callback);
#endif

//@}

#ifdef __cplusplus
}
#endif
#endif // _UDI_VENDOR_H_
