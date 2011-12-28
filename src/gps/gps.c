/******************************************************************************
 *
 * GPS Interface
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

// Green = RXD on FT232
// Blue = TXD on FT232

#include "asf.h"
#include "drivers.h"

void gps_task_init( void ){
	xTaskCreate(gps_task, configTSK_GPS_TASK_NAME, configTSK_GPS_TASK_STACK_SIZE, NULL, configTSK_GPS_TASK_PRIORITY, NULL);
}


void gps_task( void ){
	
	while(TRUE){
		usart_write_char(GPS_USART, 'H');
		vTaskDelay( (portTickType)TASK_DELAY_MS(1000) );
	}
}

void gps_irq_task( void ){
	
	
}

/*
void gps_processMsg(struct tGPSRXDBuffer *GPSRXDBuffer, unsigned char index, struct tGPSMsgGGA *GPSMsgGGA, struct tGPSMsgGSV *GPSMsgGSV){
	unsigned char i, j;
	
	if( (GPSRXDBuffer[index].raw[3] == 'G') && (GPSRXDBuffer[index].raw[4] == 'G') && (GPSRXDBuffer[index].raw[5] == 'A')){
		j = GPS_PACKET_START;
		// --- UTC -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgGGA->utc)) && (GPSRXDBuffer[index].raw[j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgGGA->utc[i++] = GPSRXDBuffer[index].raw[j];
			}
			j++;
		}
		GPSMsgGGA->utc[i] = 0x00;
			
			
		// --- Latitude -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgGGA.latitude)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgGGA.latitude[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgGGA.latitude[i] = 0x00;
			
			
		// --- North Or South -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgGGA.latitudeNorS)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgGGA.latitudeNorS[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgGGA.latitudeNorS[i] = 0x00;
			
			
		// --- Longitude -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgGGA.longitude)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgGGA.longitude[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgGGA.longitude[i] = 0x00;
			
			
		// --- East Or West -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgGGA.longitudeEorW)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgGGA.longitudeEorW[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgGGA.longitudeEorW[i] = 0x00;
			
			
		// --- Quality -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgGGA.quality)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgGGA.quality[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgGGA.quality[i] = 0x00;
			
			
		// --- Satelites In View -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgGGA.satelites)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgGGA.satelites[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgGGA.satelites[i] = 0x00;
			
			
		// --- HDOP -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgGGA.hdop)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgGGA.hdop[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgGGA.hdop[i] = 0x00;
			
			
		// --- Altitude -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgGGA.altitude)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgGGA.altitude[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgGGA.altitude[i] = 0x00;
			
	}
		
	if( (GPSRXDBuffer[index].raw[3] == 'R') && (GPSRXDBuffer[index].raw[4] == 'M') && (GPSRXDBuffer[index].raw[5] == 'C')){
		j = GPS_PACKET_START;
			
		// --- UTC -------------------------------------------------------
		while( GPSRXDBuffer[index].raw[j++] != ',' );
			
			
		// --- Status -------------------------------------------------------
		//while( GPSRXDBuffer[GPSBufferTemp].raw[j++] != ',' );
			
			
		// --- Latitude -------------------------------------------------------
		while( GPSRXDBuffer[index].raw[j++] != ',' );
			
			
		// --- Latitude North or South -------------------------------------------------------
		while( GPSRXDBuffer[index].raw[j++] != ',' );
			
			
		// --- Longitude -------------------------------------------------------
		while( GPSRXDBuffer[index].raw[j++] != ',' );
			
			
		// --- Longitude East or West -------------------------------------------------------
		while( GPSRXDBuffer[index].raw[j++] != ',' );
		j--;	//Use to realign
			
		// --- Speed -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgRMC.speed)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgRMC.speed[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgRMC.speed[i] = 0x00;
			
			
		// --- Track -------------------------------------------------------
		i = 0;
		while( (i < sizeof(GPSMsgRMC.track)) && (GPSRXDBuffer[index].raw[++j] != ',') ){
			if(GPSRXDBuffer[index].raw[j] != '.'){
				GPSMsgRMC.track[i++] = GPSRXDBuffer[index].raw[j];
			}
		}
		GPSMsgRMC.track[i] = 0x00;
			
	}			
}*/

