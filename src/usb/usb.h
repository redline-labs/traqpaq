/******************************************************************************
 *
 * USB Includes
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


#ifndef USB_H_
#define USB_H_

#include "compiler.h"

void main_vendor_int_in_received(udd_ep_status_t status, iram_size_t nb_transfered);

void main_vendor_int_out_received(udd_ep_status_t status, iram_size_t nb_transfered);

void main_vendor_bulk_in_received(udd_ep_status_t status, iram_size_t nb_transfered);

void main_vendor_bulk_out_received(udd_ep_status_t status, iram_size_t nb_transfered);

void main_vendor_iso_in_received(udd_ep_status_t status, iram_size_t nb_transfered);

void main_vendor_iso_out_received(udd_ep_status_t status, iram_size_t nb_transfered);

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

#endif /* USB_H_ */