
#ifndef GPS_H
#define GPS_H
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"

    

            typedef struct {
                unsigned char header[2];
                unsigned char cls;
                unsigned char id;
                unsigned char len[2];
            } FRAME;
						
        typedef union{
            char fstr[6];
					FRAME fS;
        } FRAMEUNION;

        typedef struct {
            uint32_t iTOW;
            int32_t lon; // (units 0.01115m)
            int32_t lat; // units 0.011132*cos(lon/180*3.145926)m
            long height;
            long hMSL;
            unsigned long hAcc;
            unsigned long vAcc;
        unsigned char GPS_checksum[2];
        } NAV_POSLLH;
				
				typedef struct {
            uint32_t test;
            double lon; // (units 0.01115m)
            double lat; // units 0.011132*cos(lon/180*3.145926)m
            double hMSL;
            double hAcc;
            double vAcc;
        } NAV_zeroed;
        
        typedef struct {            
            uint32_t iTOW;
            uint16_t year;
            uint8_t month;
            uint8_t day;
            uint8_t hour;
            uint8_t minute;
            uint8_t second;
            uint8_t valid;
            uint32_t tAcc;//ns
            int32_t nano;//not sure, don't care
            uint8_t fixType;
            uint8_t flags;
            uint8_t reserved1;
            uint8_t numSV;// num of satalites
            int32_t lon;//0.01115m
            int32_t lat;//0.011132*cos(lon/180*3.145926)
            int32_t height;//mm above elipsoid
            int32_t hMSL;//mm above sea level
            uint32_t hAcc;//mm horizontal
            uint32_t vAcc;//mm vertical
            int32_t velN;// mm/s
            int32_t velE;// mm/s
            int32_t velD;// mm/s
            int32_t gSpeed;// ground speed
            int32_t headMot;// deg heading of motion
            uint32_t sAcc;// mm/s speed accuracy
            uint16_t pDOP;// position dilution of precision
            uint8_t reserved2[6];
            int32_t headVeh;//deg heading of vehicle?
            uint8_t reserved3[4];
        unsigned char GPS_checksum[2];
        } NAV_PVT;
        typedef struct {
            unsigned long iTOW;
            int32_t ecefX; // (cm)
            int32_t ecefY; // (cm)
            int32_t ecefZ; // (cm)
            uint32_t pAcc; // (cm)
        unsigned char GPS_checksum[2];
        } NAV_POSECEF;
        typedef union {
            uint8_t valid;
            struct VALIDFIELD{
                unsigned int blank : 5;
                unsigned int validDate : 1;
                unsigned int validTime : 1;
                unsigned int fullyResolved : 1;
            } validField;
        } VALIDUNION;//sounds a little try-hard
        typedef union{
            uint8_t flags;
            struct FLAGFIELD{
                unsigned int blank : 2;
                unsigned int headVehValid : 1;
                unsigned int psmState : 3;
                unsigned int difSoln : 1;
                unsigned int gnssFixOK : 1;
            } flagField;
        } FLAGUNION;
int Init_GPS(UART_HandleTypeDef *huartptr, uint8_t GPS_Received[], uint8_t** GPS_Data);
void GPS_receive( UART_HandleTypeDef *huartptr, uint8_t GPS_Received[]);
void GPS_zero( UART_HandleTypeDef *huartptr, uint8_t GPS_Received[], NAV_zeroed * GPS_Zeroed);// this is a filter designed to narrow down the location for a STATIONARY GPS
//        int GPS_calcChecksum(void);

#endif
