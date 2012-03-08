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


char *itoa(int value, char *string, unsigned short radix){
	char tmp[33];
	char *tp = tmp;
	int i;
	unsigned int v;
	int sign;
	char *sp;

	if (radix > 36 || radix <= 1){
		return 0;
	}

	sign = (radix == 10 && value < 0);
	if (sign){
		v = -value;
	}else{
		v = (unsigned)value;
	}
			
	while (v || tp == tmp){
		i = v % radix;
		v = v / radix;
		if (i < 10){
			*tp++ = i+'0';
		}else{
			*tp++ = i + 'a' - 10;
		}		
	}
			
	sp = string;

	if (sign){
		*sp++ = '-';
	}
			
	while (tp > tmp){
		*sp++ = *--tp;
	}
			
	*sp = 0;
	return string;
}

char *itoa_paddded(int value, char *string, unsigned short radix){
	char tmp[33];
	char *tp = tmp;
	int i;
	unsigned v;
	int sign;
	char *sp;

	if (radix > 36 || radix <= 1){
		return 0;
	}

	sign = (radix == 10 && value < 0);
	if (sign){
		v = -value;
	}else{
		v = (unsigned)value;
	}
			
	while (v || tp == tmp){
		i = v % radix;
		v = v / radix;
		if (i < 10){
			*tp++ = i+'0';
		}else{
			*tp++ = i + 'a' - 10;
		}		
	}
			
	sp = string;

	if (sign){
		*sp++ = '-';
	}
	
	if(value < 10){
		*sp++ = '0';
	}		
			
	while (tp > tmp){
		*sp++ = *--tp;
	}
			
	*sp = 0;
	return string;
}