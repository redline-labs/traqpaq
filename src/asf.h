/**
 * \file
 *
 * \brief Autogenerated API include file for the AVR Software Framework (ASF)
 *
 * Copyright (C) 2011 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#ifndef ASF_H
#define ASF_H

/*
 * This file includes all API header files for the selected drivers from ASF.
 * Note: There might be duplicate includes required by more than one driver.
 *
 * The file is automatically generated and will be re-written when
 * running the ASF driver selector tool. Any changes will be discarded.
 */

// From module: ANALOG - ADC - Analog to Digital Converter
#include <adc.h>

// From module: CPU - Cycle Counter
#include <cycle_counter.h>

// From module: CPU - INTC - Interrupt Controller
#include <intc.h>

// From module: CPU - Interrupt management - UC3 implementation
#include <interrupt.h>

// From module: CPU - Reset Cause
#include <avr32_reset_cause.h>

// From module: CPU - SLEEP - Sleep manager - UC3 implementation
#include <sleepmgr.h>
#include <uc3/sleepmgr.h>

// From module: GPIO - General-Purpose Input/Output
#include <gpio.h>

// From module: MEMORY - EBI Static Memory Controller
#include <smc.h>

// From module: MEMORY - EIC - External Interrupt Controller
#include <eic.h>

// From module: MEMORY - FLASHC - Flash Controller
#include <flashc.h>

// From module: PM - UC3 A0/A1/A3/A4/B0/B1 implementation
#include <power_clocks_lib.h>
#include <sleep.h>

// From module: PWM - UC3 A/B implementation
#include <pwm.h>

// From module: SPI - Serial Peripheral Interface
#include <spi.h>

// From module: TIMING - Clock Control - UC3 A implementation
#include <sysclk.h>

// From module: TWI - Two-Wire Interface
#include <twi.h>

// From module: USART - Universal Synchronous/Asynchronous Receiver/Transmitter
#include <usart.h>

// From module: UTILITY - Compiler abstraction layer and code utilities
#include <compiler.h>
#include <status_codes.h>

// From module: UTILITY - Generic board support
#include <board.h>

// From module: WDT - UC3 A0/A1/A3/A4/B0/B1 implementation
#include <wdt.h>

#endif // ASF_H
