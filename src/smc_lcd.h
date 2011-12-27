/*
 * smc_lcd.h
 *
 * Created: 9/13/2011 11:03:38 PM
 *  Author: Ryan
 */ 

#ifndef SMC_LCD_H_
#define SMC_LCD_H_

//! SMC Peripheral Memory Size in Bytes
#define EXT_SM_SIZE            0x200001

//! SMC Data Bus Width
#define SMC_DBW                16

//! Whether 8-bit SM chips are connected on the SMC
#define SMC_8_BIT_CHIPS        false



// NCS setup time. Unit: ns.
#define NCS_WR_SETUP            10		//50

// NCS pulse time. Unit: ns.
#define NCS_WR_PULSE            10		//80

// NCS hold time. Unit: ns.
#define NCS_WR_HOLD             10

// NWE setup time. Unit: ns.
#define NWE_SETUP               10		//100

// NWE pulse time. Unit: ns.
#define NWE_PULSE               10		//50

// NWE hold time. Unit: ns.
#define NWE_HOLD                10		//0

// Write cycle time. Unit: ns.
#define NWE_CYCLE               Max((NCS_WR_SETUP + NCS_WR_PULSE + NCS_WR_HOLD),(NWE_SETUP + NWE_PULSE + NWE_HOLD))

// NCS setup time. Unit: ns.
#define NCS_RD_SETUP            0

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