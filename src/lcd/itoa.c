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
#include "itoa.h"

char *itoa(signed int value, char *string, unsigned short radix, unsigned char pad){
	char tmp[12]; // Max int value is 10 places, plus potential negative sign and end with a null
	char *tp = tmp;
	int i;
	char *sp;

	if (radix > 36 || radix <= 1){
		return 0;
	}
	
	sp = string;

	if (value < 0){
		*sp++ = '-';
	}
	
	if( (value < 10) && pad ){
		*sp++ = '0';
	}
			
	while (value || tp == tmp){
		i = value % radix;
		value = value / radix;
		
		if (i < 10){
			*tp++ = i+'0';
		}else{
			*tp++ = i + 'a' - 10;
		}		
	}	
			
	while (tp > tmp){
		*sp++ = *--tp;
	}
			
	*sp = 0;
	return string;
}