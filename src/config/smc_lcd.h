/******************************************************************************
 *
 * SMC Defines for Display
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

#ifndef SMC_LCD_H_
#define SMC_LCD_H_

//! SMC Peripheral Memory Size in Bytes
#define EXT_SM_SIZE            0x200000

//! SMC Data Bus Width
#define SMC_DBW                16

//! Whether 8-bit SM chips are connected on the SMC
#define SMC_8_BIT_CHIPS        false



// NCS setup time. Unit: ns.
#define NCS_WR_SETUP            50		//50

// NCS pulse time. Unit: ns.
#define NCS_WR_PULSE            80		//80

// NCS hold time. Unit: ns.
#define NCS_WR_HOLD             10

// NWE setup time. Unit: ns.
#define NWE_SETUP               100		//100

// NWE pulse time. Unit: ns.
#define NWE_PULSE               50		//50

// NWE hold time. Unit: ns.
#define NWE_HOLD                10		//0

// Write cycle time. Unit: ns.
#define NWE_CYCLE               Max((NCS_WR_SETUP + NCS_WR_PULSE + NCS_WR_HOLD),(NWE_SETUP + NWE_PULSE + NWE_HOLD))

// NCS setup time. Unit: ns.
#define NCS_RD_SETUP            100

// NCS pulse time. Unit: ns.
#define NCS_RD_PULSE            240

// NCS hold time. Unit: ns.
#define NCS_RD_HOLD             30

// NRD setup time. Unit: ns.
#define NRD_SETUP               30

// NRD pulse time. Unit: ns.
#define NRD_PULSE               210

// NRD hold time. Unit: ns.
#define NRD_HOLD                30

// Read cycle time. Unit: ns.
#define NRD_CYCLE               Max((NCS_RD_SETUP + NCS_RD_PULSE + NCS_RD_HOLD),(NRD_SETUP + NRD_PULSE + NRD_HOLD))



// Data float time
#define TDF_CYCLES              0
#define TDF_OPTIM               false

// Page mode
#define PAGE_MODE               false
#define PAGE_SIZE               0

//! Whether read is controlled by NCS or by NRD
#define NCS_CONTROLLED_READ     false

//! Whether write is controlled by NCS or by NWE
#define NCS_CONTROLLED_WRITE    false

//! Whether to use the NWAIT pin
#define NWAIT_MODE              AVR32_SMC_EXNW_MODE_DISABLED



#endif /* SMC_LCD_H_ */