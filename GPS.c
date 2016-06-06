#include "GPS.h"


int Init_GPS(UART_HandleTypeDef *huartptr, uint8_t GPS_Received[], uint8_t** GPS_Data)
{
	*GPS_Data = &GPS_Received[6];
    FRAMEUNION POSLLH_Frame;
	FRAMEUNION PVT_Frame;
	FRAMEUNION POSECEF_Frame;
	
	POSLLH_Frame.fS.header[0] = 0xB5;
    POSLLH_Frame.fS.header[1] = 0x62;
    POSLLH_Frame.fS.cls = 0x01;
    POSLLH_Frame.fS.id = 0x02;
    POSLLH_Frame.fS.len[0] = 0x1c;
    POSLLH_Frame.fS.len[1] = 0x00;
	
		PVT_Frame.fS.header[0] = 0xB5;
    PVT_Frame.fS.header[1] = 0x62;
    PVT_Frame.fS.cls = 0x01;
    PVT_Frame.fS.id = 0x07;
    PVT_Frame.fS.len[0] = 0x5c;
    PVT_Frame.fS.len[1] = 0x00;
	
	POSECEF_Frame.fS.header[0] = 0xB5;
    POSECEF_Frame.fS.header[1] = 0x62;
    POSECEF_Frame.fS.cls = 0x01;
    POSECEF_Frame.fS.id = 0x01;
    POSECEF_Frame.fS.len[0] = 0x14;
    POSECEF_Frame.fS.len[1] = 0x00;
		
		
	HAL_UART_Receive(huartptr, GPS_Received, 1, 1000);
	for(int loopcounter = 0; loopcounter < 3; loopcounter++)
	{
		HAL_UART_Receive(huartptr, &GPS_Received[loopcounter], 1, 1000);
		if(GPS_Received[loopcounter] != PVT_Frame.fstr[loopcounter])
		{
			loopcounter = -1;
		}
	}
	//HAL_UART_Receive(huartptr, &GPS_Received[3], 3, 1000);

	HAL_UART_Receive_DMA(huartptr, (uint8_t *)&GPS_Received[3], 97);
	return 1;
}

void GPS_receive( UART_HandleTypeDef *huartptr, uint8_t GPS_Received[])
{
	HAL_UART_Receive_DMA(huartptr, (uint8_t *)GPS_Received, GPS_Received[4]+8);
}

void GPS_zero( UART_HandleTypeDef *huartptr, uint8_t GPS_Received[], NAV_zeroed * GPS_Zeroed)
{
	


		double lon;
		double lat;
		double height;
		double hAcc;
		double vAcc;
			
	if(GPS_Received[3] == 2)
	{
		lon = ((NAV_POSLLH*)&GPS_Received[6])->lon;
		lat = ((NAV_POSLLH*)&GPS_Received[6])->lat;
		height = ((NAV_POSLLH*)&GPS_Received[6])->height;
		hAcc = ((NAV_POSLLH*)&GPS_Received[6])->hAcc;
		vAcc = ((NAV_POSLLH*)&GPS_Received[6])->vAcc;
	}
	
		else if(GPS_Received[3] == 7)
	{
		lon = ((NAV_PVT*)&GPS_Received[6])->lon;
		lat = ((NAV_PVT*)&GPS_Received[6])->lat;
		height = ((NAV_PVT*)&GPS_Received[6])->height;
		hAcc = ((NAV_PVT*)&GPS_Received[6])->hAcc;
		vAcc = ((NAV_PVT*)&GPS_Received[6])->vAcc;
	}
		
	if(GPS_Zeroed->lon == 0)
	{
		
	GPS_Zeroed->lat = lat;
	GPS_Zeroed->lon = lon;
	GPS_Zeroed->hMSL = height;
	GPS_Zeroed->hAcc = hAcc;
	GPS_Zeroed->vAcc = vAcc;
	}
	else{
	double vGain = GPS_Zeroed->vAcc / (GPS_Zeroed->vAcc + vAcc);
	GPS_Zeroed->hMSL += vGain * ( height - GPS_Zeroed->hMSL);
	GPS_Zeroed->vAcc *= 1 - vGain;
	
	double hGain = GPS_Zeroed->hAcc / (GPS_Zeroed->hAcc + hAcc);
	GPS_Zeroed->lat += hGain * ( lat - GPS_Zeroed->lat);
	GPS_Zeroed->lon += hGain * ( lon - GPS_Zeroed->lon);
	GPS_Zeroed->hAcc *= 1 - hGain;
	}

	
}


/*int GPS_calcChecksum(void) {
  unsigned char GPS_checksum_calc[2];
  int i;
  for (i = 2; i < 0x5c+6; i++) {
    GPS_checksum_calc[0] += ((unsigned char*)(&working))[i];
    GPS_checksum_calc[1] += GPS_checksum[0];
  }
	return memcmp(GPS_checksum_calc, GPS_checksum,2);
}*/
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
  
            //if(GPS_calcChecksum())
						//{
							
//}
