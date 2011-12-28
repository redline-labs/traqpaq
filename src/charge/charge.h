/******************************************************************************
 *
 * Charge Control Include
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.1
 * - AppNote:			N/A
 *
 * - Last Author:		Ryan David ( ryan.david@redline-electronics.com )
 *
 *
 * Copyright (c) 2011 Redline Electronics LLC.
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

#ifndef CHARGE_H_
#define CHARGE_H_

#include "board.h"

#define CHARGE_VERSION				"1.00"


#define CHARGE_STAT2_COMPLETE		0
#define CHARGE_STAT2_INCOMPLETE		1

#define CHARGE_PG_TRUE				0
#define CHARGE_PG_FALSE				1

#define CHARGE_STAT1_CHARGING		0
#define CHARGE_STAT1_NOTCHARGING	1

#define CHARGE_RATE_HIGH			1
#define CHARGE_RATE_LOW				0

//	Cycle State			Stat1	Stat2	PG
//	----------------------------------------
//	Shutdown			Hi-Z	Hi-Z	Hi-Z
//	Standby				Hi-Z	Hi-Z	L
//	Preconditioning		L		Hi-Z	L
//	Constant Current	L		Hi-Z	L
//	Constant Voltage	L		Hi-Z	L
//	Charge Complete		Hi-Z	L		L
//	Temperature Fault	Hi-Z	Hi-Z	L
//	Timer Fault			Hi-Z	Hi-Z	L
//	System Test Mode	L		L		L

#define CHARGE_STATUS_SHUTDOWN		0b111	// 0x07
#define CHARGE_STATUS_STANDBY		0b110	// 0x06: Grouped Temperature/Timer faults and Standby together
#define CHARGE_STATUS_COMPLETE		0b100	// 0x04
#define CHARGE_STATUS_CHARGING		0b010	// 0x02: Grouped Constant Current, Constant Voltage, and Preconditioning together
#define CHARGE_STATUS_SYS_TEST		0b000	// 0x00




void charge_task_init( void );
void charge_task( void );

void charge_setRate(unsigned char rate);
unsigned char charge_powerGood(void);
unsigned char charge_state(void);

#endif /* CHARGE_H_ */
