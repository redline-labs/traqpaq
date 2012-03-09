/*
 * COPYRIGHT:   See COPYING in the top level directory
 * PROJECT:     ReactOS system libraries
 * FILE:        lib/crtdll/stdlib/itoa.c
 * PURPOSE:     converts a integer to ascii
 * PROGRAMER:   
 * UPDATE HISTORY:
 *              1995: Created
 *              1998: Added ltoa Boudewijn Dekker
 */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */

#ifndef ITOA_H_
#define ITOA_H_

char *itoa(int value, char *string, unsigned short radix, unsigned char pad);



#endif /* ITOA_H_ */