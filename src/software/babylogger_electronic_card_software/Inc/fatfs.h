/**
  ******************************************************************************
  * @file   fatfs.h
  * @brief  Header for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
 extern "C" {
#endif

// Header include 

#include "stdint.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h" 
#include "Acquisition_Audio.h"
#include "Crypt.h"
#include "IHM.h"
#include "sensor_acquisition.h"


/*Prototype  -------------------------------------------------------------------------------- */

void Test_FATFS_Init(void);
void FATFS_Init(void); 
__weak void FATFS_Stop_System_SD_Space_PB (void);
uint8_t FATFS_SD_Enough_Space (uint32_t Free_Space);
//uint8_t FATFS_Format_uSD (void);
void Save_Context (void);
void Restor_Context (void);
uint8_t Audio_Svgrd_Pending (void);
void FATFS_Force_Stop(void);
void Set_Tick_Start_Time (void);
void Set_Tick_End_Time (void);	 
//void write_csv(void);
void Svgrd_Process(void);
void Acti_Svgrd_Process(void);
void Audio_Svgrd_Process (void);
void Swap_File_Process (void);
__weak void Sd_DeInit_Callback(void);
void Write_dev_crypt_test_report(int16_t* pointeur1, int16_t* pointeur2);

/*Application parameters define  ------------------------------------------------------------ */

/* Use log file for restor context */

//#define USE_LOG_FILE

/* File size define */
#define CounterBuffMax 								28125 //56250
#define ActiCounterBuffMax						1463 //2925
#define CounterBuffDelay							100

/* Swap process states */
#define	Swap_file_state_Off							0
#define Swap_file_state_Init						1
#define Swap_file_state_Run							2
#define Swap_file_state_New_file_rqst		3
#define Swap_file_state_Close_file_rqst	4


/*Structure defenition  --------------------------------------------------------------------- */

/* Audio custom header 84 Bytes */
typedef struct
{
	// device serial number : 2 octet 
	
	uint16_t 	BL_serial_Number;
	
	// Date start : 4 octet 
	
	uint16_t 	year;
	uint8_t 	month;
	uint8_t 	day;
	
	// Time start : 4 octet  
	
	uint8_t 	hoursS;
	uint8_t 	minutesS;
	uint8_t 	secondsS;
	uint8_t 	subsecondsS;

	// time end : 4 octet  

	uint8_t 	hoursE;
	uint8_t 	minutesE;
	uint8_t 	secondsE;
	uint8_t 	subsecondsE;
	
	// Audio informations : 4 octet 
	
	uint8_t 	audio_resolution;
	uint16_t	audio_freq; 
	uint8_t 	NbrChannel;

	// debeug variable : 56 octet 
	
	uint32_t SysTickS;
	uint32_t SysTickE;
	
	uint32_t 	hFlt1;
	uint32_t 	hFlt2;
	uint32_t 	hFlt3;
	uint32_t 	hFlt4;
	uint32_t 	cFlt1;
	uint32_t 	cFlt2;
	uint32_t 	cFlt3;
	uint32_t 	cFlt4;	
	uint32_t 	hRecBuff;
	uint32_t 	cRecBuff;	
	uint32_t  Buff_crypted;
	uint32_t  Buff_saved;
	
	// reserved for futur 
	uint8_t   Reserved[10];
	
}AUDIO_Cust_Header_Struct;


/* Audio custom header 72 Bytes */
typedef struct
{
	// device serial number : 2 octet 
	
	uint16_t 	BL_serial_Number;
	
	// Date start : 4 octet 
	
	uint16_t 	year;
	uint8_t 	month;
	uint8_t 	day;
	
	// Time start : 4 octet  
	
	uint8_t 	hoursS;
	uint8_t 	minutesS;
	uint8_t 	secondsS;
	uint8_t 	subsecondsS;

	// time end : 4 octet  

	uint8_t 	hoursE;
	uint8_t 	minutesE;
	uint8_t 	secondsE;
	uint8_t 	subsecondsE;
	
	// Sensors informations : 8 octet 
	uint16_t	Gyr_freq;
	uint16_t	Acc_freq;
	uint8_t		Gyr_mode; 		// 0 for low power, 1 for normal, 3 for High performance.
	uint8_t		Acc_mode;			// 0 for low power, 1 for normal, 3 for High performance.
	uint8_t		Gyr_scale;		// 0: 2g, 1: 16g, 3: 4g, 4 : 8g. 
	uint8_t		Acc_scale;		// 0: 125dsp 1: 250dps, 3: 500dps, 4 : 1000dps, 5: 2000 dps.

	// debeug variable : 40 octet 
	
	uint32_t SysTickS;
	uint32_t SysTickE;
	
	uint32_t Gyr_half;
	uint32_t Acc_half;

	uint32_t Gyr_cplt;
	uint32_t Acc_cplt;

	uint32_t GyrAcc_half;
	uint32_t GyrAcc_cplt;
	
	uint32_t INT_Motion;
	
	uint32_t Buff_saved;
	
	// reserved for futur 10 octets
	uint8_t   Reserved[10];
	
}ACTI_Cust_Header_Struct;





/* USER CODE END Prototypes */
#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


