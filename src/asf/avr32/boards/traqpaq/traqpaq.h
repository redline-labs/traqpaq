/******************************************************************************
 *
 * Board defines
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

#ifndef USER_BOARD_H
#define USER_BOARD_H

#define TRUE						1
#define FALSE						0

#include "compiler.h"
#include "conf_board.h"

// ------------------------------------------------------------
// Versions
// ------------------------------------------------------------
#define TRAQPAQ_SW_LEVEL			0x0100
#define TRAQPAQ_SW_LEVEL_ASCII		"1.00"
#define TRAQPAQ_SW_BUILD_DATE		__DATE__
#define TRAQPAQ_SW_SUPPORT_VER		__VERSION__


// ------------------------------------------------------------
// Clock Definitions
// ------------------------------------------------------------
#define FOSC0						12000000
#define OSC0_STARTUP				AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC

#define APPL_CPU_SPEED				48000000
#define APPL_CPU_SPEED_ASCII		"48000000"
#define APPL_PBA_SPEED				24000000
#define APPL_PBA_SPEED_ASCII		"24000000"

#define BOARD_OSC0_HZ				FOSC0
#define BOARD_OSC0_STARTUP_US		17000
#define BOARD_OSC0_IS_XTAL			TRUE


// ------------------------------------------------------------
// USB Definitions
// ------------------------------------------------------------
#define USB_ID                      AVR32_USBB_USB_ID_0_0
#define USB_VBOF                    AVR32_USBB_USB_VBOF_0_1
#define USB_VBOF_ACTIVE_LEVEL       LOW
#define USB_OVERCURRENT_DETECT_PIN  AVR32_PIN_PX33


// ------------------------------------------------------------
// Backlight and Status LED Definitions (PWM)
// ------------------------------------------------------------
#define BACKLIGHT_PIN				AVR32_PIN_PB28
#define BACKLIGHT_PWM_PIN			AVR32_PWM_5_1_PIN
#define BACKLIGHT_PWM_FUNCTION		AVR32_PWM_5_1_FUNCTION
#define BACKLIGHT_PWM_CHANNEL		5
#define BACKLIGHT_PWM_ID			1

#define BACKLIGHT_PWM_MIN			0
#define BACKLIGHT_PWM_MAX			255



// ------------------------------------------------------------
// Button Definitions (EXTINT)
// ------------------------------------------------------------
#define GPIO_BUTTON0				AVR32_PIN_PA04
#define GPIO_BUTTON1				AVR32_PIN_PA08
#define GPIO_BUTTON2				AVR32_PIN_PA10
#define GPIO_BUTTON3				AVR32_PIN_PA21

#define EXTINT_NUMBER_LINES			4

#define EXTINT_BUTTON0				EXT_INT5
#define EXTINT_BUTTON1				EXT_INT7
#define EXTINT_BUTTON2				EXT_INT6
#define EXTINT_BUTTON3				EXT_INT0
#define EXTINT_GPSPPS				EXT_INT4

#define EXTINT_BUTTON0_IRQ			AVR32_EIC_IRQ_5
#define EXTINT_BUTTON1_IRQ			AVR32_EIC_IRQ_7
#define EXTINT_BUTTON2_IRQ			AVR32_EIC_IRQ_6
#define EXTINT_BUTTON3_IRQ			AVR32_EIC_IRQ_0
#define EXTINT_GPSPPS_IRQ			AVR32_EIC_IRQ_4

#define EXTINT_BUTTON0_PIN			AVR32_EIC_EXTINT_5_PIN
#define EXTINT_BUTTON1_PIN			AVR32_EIC_EXTINT_7_PIN
#define EXTINT_BUTTON2_PIN			AVR32_EIC_EXTINT_6_PIN
#define EXTINT_BUTTON3_PIN			AVR32_EIC_EXTINT_0_PIN
#define EXTINT_GPSPPS_PIN			AVR32_EIC_EXTINT_4_PIN

#define EXTINT_BUTTON0_FUNCTION		AVR32_EIC_EXTINT_5_FUNCTION
#define EXTINT_BUTTON1_FUNCTION		AVR32_EIC_EXTINT_7_FUNCTION
#define EXTINT_BUTTON2_FUNCTION		AVR32_EIC_EXTINT_6_FUNCTION
#define EXTINT_BUTTON3_FUNCTION		AVR32_EIC_EXTINT_0_FUNCTION
#define EXTINT_BUTTON4_FUNCTION		AVR32_EIC_EXTINT_4_FUNCTION


// ------------------------------------------------------------
// Dataflash Definitions (SPI)
// ------------------------------------------------------------
#define DATAFLASH_SPI					(&AVR32_SPI0)
#define DATAFLASH_SPI_SCK_PIN			AVR32_SPI0_SCK_0_1_PIN
#define DATAFLASH_SPI_SCK_FUNCTION		AVR32_SPI0_SCK_0_1_FUNCTION
#define DATAFLASH_SPI_MISO_PIN			AVR32_SPI0_MISO_0_1_PIN
#define DATAFLASH_SPI_MISO_FUNCTION		AVR32_SPI0_MISO_0_1_FUNCTION
#define DATAFLASH_SPI_MOSI_PIN			AVR32_SPI0_MOSI_0_1_PIN
#define DATAFLASH_SPI_MOSI_FUNCTION		AVR32_SPI0_MOSI_0_1_FUNCTION
#define DATAFLASH_SPI_NPCS0_PIN			AVR32_SPI0_NPCS_0_1_PIN
#define DATAFLASH_SPI_NPCS0_FUNCTION	AVR32_SPI0_NPCS_0_1_FUNCTION

#define DATAFLASH_SPI_NPCS				0
#define SPI_BAUDRATE					12000000

#define SPI_BITS_PER_XFER			8			// 8 Bits per transfer
#define SPI_SCLK_DELAY				0			// Delay from CS to first CLK
#define SPI_XFER_DELAY				0			// Delay between transfers
#define SPI_STAY_ACTIVE				1			// Keep CS active between transfers (manually control CS)
#define SPI_MODE					0			// SPI Mode of Device
#define SPI_FAULT_DETECT			1			// Hardware Fault Detection

#define SPI_MASTER_DELAY_BCT		0
#define SPI_MASTER_DELAY_BS			0

#define SPI_VARIABLE_PS				0			// SPI Variable PS
#define SPI_PCS_DECODE				0			// SPI PCS Decode
#define SPI_PCS_DELAY				0			// SPI PCS Delay

#define SPI_TX_PDCA_CHANNEL			0
#define SPI_RX_PDCA_CHANNEL			1


// ------------------------------------------------------------
// Fuel Gauge Definitions (TWI)
// ------------------------------------------------------------
#define FUEL_TWI					(&AVR32_TWI)
#define FUEL_TWI_SCL_PIN			AVR32_TWI_SCL_0_0_PIN
#define FUEL_TWI_SCL_FUNCTION		AVR32_TWI_SCL_0_0_FUNCTION
#define FUEL_TWI_SDA_PIN			AVR32_TWI_SDA_0_0_PIN
#define FUEL_TWI_SDA_FUNCTION		AVR32_TWI_SDA_0_0_FUNCTION

#define FUEL_TWI_BAUDRATE			100000		// 100kHz MAX
#define FUEL_ADDRESS				0b0110100	// 7 bits
#define FUEL_ADDRESS_LENGTH			1
#define FUEL_RESPONSE_LENGTH		2


// ------------------------------------------------------------
// GPIO Definitions
// ------------------------------------------------------------
#define CHARGE_RATE					AVR32_PIN_PA18
#define CHARGE_STAT1				AVR32_PIN_PA16
#define CHARGE_STAT2				AVR32_PIN_PA17
#define CHARGE_PG					AVR32_PIN_PA19

#define PM_SHDN1					AVR32_PIN_PA06
#define PM_PGOOD1					AVR32_PIN_PA05
#define PM_PGOOD3					AVR32_PIN_PX11
#define PM_ENABLE					AVR32_PIN_PA02

#define LCD_RESET					AVR32_PIN_PA07
#define LCD_RESET_TIME				10				// CFAF240400DT-030T Datasheet pg 10, double the min spec

#define GPS_RESET					AVR32_PIN_PC05

#define ADC_VREF_EN					AVR32_PIN_PA28


// ------------------------------------------------------------
// GPS Definitions (USART3)
// ------------------------------------------------------------
#define GPS_USART               	(&AVR32_USART3)
#define GPS_TXD						AVR32_USART3_TXD_0_0_PIN
#define GPS_TXD_FUNCTION			AVR32_USART3_TXD_0_0_FUNCTION
#define GPS_RXD						AVR32_USART3_RXD_0_0_PIN
#define GPS_RXD_FUNCTION			AVR32_USART3_RXD_0_0_FUNCTION
#define GPS_USART_BAUD				115200
#define GPS_USART_IRQ           	AVR32_USART3_IRQ


// ------------------------------------------------------------
// Debug Definitions (USART2)
// ------------------------------------------------------------
#define DEBUG_USART               	(&AVR32_USART2)
#define DEBUG_TXD					AVR32_USART2_TXD_0_0_PIN
#define DEBUG_TXD_FUNCTION			AVR32_USART2_TXD_0_0_FUNCTION
#define DEBUG_RXD					AVR32_USART2_RXD_0_0_PIN
#define DEBUG_RXD_FUNCTION			AVR32_USART2_RXD_0_0_FUNCTION
#define DEBUG_USART_BAUD			57600
#define DEBUG_USART_IRQ           	AVR32_USART2_IRQ


// ------------------------------------------------------------
// Fuel Gauge Definitions (TWI)
// ------------------------------------------------------------
#define ADC							(&AVR32_ADC)

#define ADC_3V3_CHANNEL				1
#define ADC_3V3_PIN					AVR32_ADC_AD_1_PIN
#define ADC_3V3_FUNCTION			AVR32_ADC_AD_1_FUNCTION

#define ADC_VCC_CHANNEL				2
#define ADC_VCC_PIN					AVR32_ADC_AD_2_PIN
#define ADC_VCC_FUNCTION			AVR32_ADC_AD_2_FUNCTION

#define ADC_VEE_CHANNEL				3
#define ADC_VEE_PIN					AVR32_ADC_AD_3_PIN
#define ADC_VEE_FUNCTION			AVR32_ADC_AD_3_FUNCTION


// Function Prototypes
void board_init(void);


#endif // USER_BOARD_H
