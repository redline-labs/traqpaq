/******************************************************************************
 *
 * Hardware Abstraction Includes
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


#ifndef HAL_H_
#define HAL_H_

#define TASK_MODE_NORMAL		0
#define TASK_MODE_USB			1


// Debug
#include "adc/adc.h"

// Dataflash
#include "dataflash/dataflash.h"
#include "dataflash/dataflash_layout.h"

// Fuel Gauge
#include "fuel/fuel.h"

// Watchdog
#include "wdt/wdt.h"

// Idle Task Hook
#include "idle/idle.h"

// LCD
#include "lcd/lcd.h"

// GPS
#include "gps/gps.h"

// PWM
#include "lcd/backlight/backlight.h"

// Charge
#include "charge/charge.h"

// Buttons
#include "button/button.h"

// Debug
#include "debug/debug.h"

// USB
#include "usb/usb.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// CRC
#include "crc/crc.h"


#endif /* HAL_H_ */