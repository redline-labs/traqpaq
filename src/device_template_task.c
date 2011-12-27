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

//_____  I N C L U D E S ___________________________________________________

#include "asf.h"
#include "drivers.h"

#include "conf_usb.h"
#include "usb_drv.h"
#include "usb_descriptors.h"
#include "usb_standard_request.h"
#include "device_template_task.h"


//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________


//_____ D E C L A R A T I O N S ____________________________________________
static U8   data_length;


//!
//! @brief This function initializes the hardware/software resources required for device applicative task.
//!
void device_template_task_init(void){
  data_length = 0;
  xTaskCreate(device_template_task, configTSK_USB_DTP_NAME, configTSK_USB_DTP_STACK_SIZE, NULL, configTSK_USB_DTP_PRIORITY, NULL);
}


//!
//! @brief Entry point of the device applicative task management
//!
//! This function links the device application to the USB bus.
//!

void device_template_task(void *pvParameters){
  static U8 buf[EP_SIZE_TEMP2];

  portTickType xLastWakeTime;

  xLastWakeTime = xTaskGetTickCount();
  while (true)
  {
    vTaskDelayUntil(&xLastWakeTime, configTSK_USB_DTP_PERIOD);

    // First, check the device enumeration state
    if (!Is_device_enumerated()) continue;

    // HERE STARTS THE USB DEVICE APPLICATIVE CODE
    // The example below just performs a loopback transmission/reception.
    // All data received with the OUT endpoint is stored in a RAM buffer and
    // sent back to the IN endpoint.


    // If we receive something in the OUT endpoint, just store it in the RAM buffer
    if (Is_usb_out_received(EP_TEMP_OUT)){
      Usb_reset_endpoint_fifo_access(EP_TEMP_OUT);
      data_length = Usb_byte_count(EP_TEMP_OUT);
      usb_read_ep_rxpacket(EP_TEMP_OUT, buf, data_length, NULL);
      Usb_ack_out_received_free(EP_TEMP_OUT);
    }

    // Load the IN endpoint with the contents of the RAM buffer
    if (data_length && Is_usb_in_ready(EP_TEMP_IN)){
      Usb_reset_endpoint_fifo_access(EP_TEMP_IN);
      usb_write_ep_txpacket(EP_TEMP_IN, buf, data_length, NULL);
      data_length = 0;
      Usb_ack_in_ready_send(EP_TEMP_IN);
    }
  }
}