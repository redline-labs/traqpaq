/**
 * \file
 *
 * \brief Standard board header file.
 *
 * This file includes the appropriate board header file according to the
 * defined board (parameter BOARD).
 *
 * Copyright (C) 2009-2011 Atmel Corporation. All rights reserved.
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

#ifndef _BOARD_H_
#define _BOARD_H_

/**
 * \defgroup group_common_boards Generic board support
 *
 * The generic board support module includes board-specific definitions
 * and function prototypes, such as the board initialization function.
 *
 * \{
 */

#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif


/*! \name Base Boards
 */
//! @{
#define EVK1100               1   //!< AT32UC3A EVK1100 board.
#define TRAQPAQ				  2   //!< traq|paq board
#define USER_BOARD            99  //!< User-reserved board (if any).
#define DUMMY_BOARD          100  //!< Dummy board to support board-independent applications (e.g. bootloader)
//! @}

/*! \name Extension Boards
 */
//! @{
//! @}

#if BOARD == EVK1100
  #include "evk1100/evk1100.h"
#elif BOARD == TRAQPAQ
  // User-reserved area: #include the header file of your board here (if any).
  #include "traqpaq/traqpaq.h"
#elif BOARD == DUMMY_BOARD
  #include "dummy/dummy_board.h"
#else
  #error No known AVR board defined
#endif

#if (defined EXT_BOARD)
  #if EXT_BOARD == MC300
    #include "mc300/mc300.h"
  #elif (EXT_BOARD == SENSORS_XPLAINED_INERTIAL_1)  || \
        (EXT_BOARD == SENSORS_XPLAINED_INERTIAL_2)  || \
        (EXT_BOARD == SENSORS_XPLAINED_INERTIAL_A1) || \
        (EXT_BOARD == SENSORS_XPLAINED_PRESSURE_1)  || \
        (EXT_BOARD == SENSORS_XPLAINED_LIGHTPROX_1)
    #include "sensors_xplained/sensors_xplained.h"
  #elif EXT_BOARD == USER_EXT_BOARD
    // User-reserved area: #include the header file of your extension board here
    // (if any).
  #endif
#endif


#if (defined(__GNUC__) && defined(__AVR32__)) || (defined(__ICCAVR32__) || defined(__AAVR32__))
#ifdef __AVR32_ABI_COMPILER__ // Automatically defined when compiling for AVR32, not when assembling.

/*! \brief This function initializes the board target resources
 *
 * This function should be called to ensure proper initialization of the target
 * board hardware connected to the part.
 */
extern void board_init(void);

#endif  // #ifdef __AVR32_ABI_COMPILER__
#else
/*! \brief This function initializes the board target resources
 *
 * This function should be called to ensure proper initialization of the target
 * board hardware connected to the part.
 */
extern void board_init(void);
#endif


#ifdef __cplusplus
}
#endif

/**
 * \}
 */

#endif  // _BOARD_H_
