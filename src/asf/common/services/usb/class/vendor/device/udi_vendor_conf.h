/**
 * \file
 *
 * \brief Default Vendor class configuration for a USB Device
 * with a single interface
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

#ifndef _UDI_VENDOR_CONF_H_
#define _UDI_VENDOR_CONF_H_

#include "conf_usb.h"

/**
 * \ingroup udi_vendor_group
 * \defgroup udi_vendor_group_conf Default configuration for a USB Device
 * with a single interface based on vendor class
 *
 * @{
 */

//! Control endpoint size
#define  USB_DEVICE_EP_CTRL_SIZE       64

//! Endpoint numbers used by vendor interface
#define  UDI_VENDOR_EP_INTERRUPT_IN  (1 | USB_EP_DIR_IN)
#define  UDI_VENDOR_EP_INTERRUPT_OUT (2 | USB_EP_DIR_OUT)
#define  UDI_VENDOR_EP_BULK_IN       ((((UDI_VENDOR_EPS_SIZE_INT_FS)?2:0)+1)\
		| USB_EP_DIR_IN)
#define  UDI_VENDOR_EP_BULK_OUT      ((((UDI_VENDOR_EPS_SIZE_INT_FS)?2:0)+2)\
		| USB_EP_DIR_OUT)
#define  UDI_VENDOR_EP_ISO_IN        ((((UDI_VENDOR_EPS_SIZE_INT_FS)?2:0)+ \
	((UDI_VENDOR_EPS_SIZE_BULK_FS)?2:0)+1)\
		| USB_EP_DIR_IN)
#define  UDI_VENDOR_EP_ISO_OUT       ((((UDI_VENDOR_EPS_SIZE_INT_FS)?2:0)+ \
	((UDI_VENDOR_EPS_SIZE_BULK_FS)?2:0)+2)\
		| USB_EP_DIR_OUT)

//! Interface number is 0 because it is the unique interface
#define  UDI_VENDOR_IFACE_NUMBER 0

/**
 * \name UDD Configuration
 */
//@{
//! Maximum 6 endpoints used by vendor interface
#define UDI_VENDOR_EP_NB_INT  ((UDI_VENDOR_EPS_SIZE_INT_FS)?2:0)
#define UDI_VENDOR_EP_NB_BULK ((UDI_VENDOR_EPS_SIZE_BULK_FS)?2:0)
#define UDI_VENDOR_EP_NB_ISO  ((UDI_VENDOR_EPS_SIZE_ISO_FS)?2:0)
#define USB_DEVICE_MAX_EP     (UDI_VENDOR_EP_NB_INT+UDI_VENDOR_EP_NB_BULK+UDI_VENDOR_EP_NB_ISO)
//@}

//@}

#include "udi_vendor.h"

#endif // _UDI_VENDOR_CONF_H_
