/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include "fatfs.h"



/* Private FatSs objects ------------------------------------------------------*/

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FATFS SDFatFS1;	  /* File system object for audio partition SD logical drive */
FATFS SDFatFS2;	  /* File system object for acti  partition SD logical drive */
FIL SDFile;       /* File object for Context File */
FIL Audio_File_1; /* File object for Audio file */
FIL Audio_File_2; /* File object for Audio file */
FIL	Acti_File; /* File object for Motion file */

/* Private FatFs variable ------------------------------------------------------*/

DWORD Clust_free 		= 0; 
WORD 	cluster_size 		= 0;
WORD 	Sector_size 		= 0;
DWORD Size_free_SD 		= 0;

/* FatFs partition variable ----------------------------------------------------*/

    PARTITION VolToPart[] = {
        {0, 1},    /* "0:" ==> Physical drive 0, 1st partition */
        {0, 2},    /* "1:" ==> Physical drive 0, 2nd partition */
        //{0, 3}     /* "2:" ==> Physical drive 1, auto detection */
    };

	  
DWORD plist[] = {95,5, 0, 0};  /* Divide drive into two partitions */    
BYTE work[_MAX_SS];

/* Static prototype ------------------------------------------------------------*/

static void Force_close_Files_Process(void);
static void FATFS_DeInit(void);
static void FATFS_Error_Handel(void);
static void Set_Audio_Cust_Header2(uint8_t* CustHeader );
static void Set_Audio_Cust_Header_Start(uint8_t* CustHeader );
static void Set_Acti_Cust_Header(uint8_t* CustHeader );

/* Buffer custom header --------------------------------------------------------*/

uint8_t My_Buff_Cust_Header[84] = {0};
uint8_t My_Buff_Acti_Cust_Header[72] = {0};

/* Flags -----------------------------------------------------------------------*/

/* Application state */
uint8_t 									End_Init_Filter = 0;
uint8_t										Wait_Close_Rqst = 0;

/* File state */ 
uint8_t 									Audio_File_1_State = 0;
uint8_t 									Audio_File_2_State = 0;
uint8_t										Actim_File_State   = 0;
uint8_t										Swap_file_state    = 0;
uint8_t 									Swap_Timeout_State = 0;

/* Buffer State */
uint8_t 									HalfCryptSvgrdBuffCplt  = 0;
uint8_t 									CryptSvgrdBuffCplt      = 0;
uint8_t 									HalfActiSvgrdBuffCplt   = 0;
uint8_t										ActiSvgrdBuffCplt	= 0;

/* Structure -------------------------------------------------------------------*/

AUDIO_Cust_Header_Struct My_Audio_Header = {0};

/* Pointers --------------------------------------------------------------------*/

/* File swapping */
FIL *pAudio_File;
FIL *pAudio_Next_File;
FIL *pAudio_Old_File;

/* External buffer to save */
extern int16_t*             pCryptSvgrdHalfBuff;
extern int16_t*             pCryptSvgrdCpltBuff;

extern int16_t*		    pAccGyrBuff_Half;
extern int16_t*		    pAccGyrBuff_Cplt;

extern uint8_t*		    pWavHeader;

/* Time & Date buffer (RTC) ---------------------------------------------------*/

uint8_t			    Rtc_Date_Start[3] 		= {0};
uint8_t			    Rtc_Acti_Date_Start[3] 	= {0};

uint8_t 		    Rtc_Time_Start[4] 		= {0};
uint8_t 		    Rtc_Acti_Time_Start[4] 	= {0};

uint8_t 		    Rtc_Time_End[4] 		= {0};
uint8_t 		    Rtc_Acti_Time_End[4] 	= {0};

extern uint8_t		    RTC_Buff[7];

/* variables of debeug  -------------------------------------------------------*/

uint32_t		   CounterBuff 		         = 0;
uint32_t 		   Acti_CounterBuff 		 = 0;

/* File name buffer -----------------------------------------------------------*/

uint8_t			WavFileName[14]			= {0};
uint8_t			ActFileName[14]                 = {0};

/* File process request -------------------------------------------------------*/

uint8_t			OpenFileRqst			= 0; 
uint8_t		        CloseFileRqst			= 0; 
uint8_t			ForceCloseFileRqst		= 0;

uint8_t 		OpenActiFileRqst		= 0;
uint8_t 		CloseActiFileRqst		= 0;
											 
/* Output management variables ------------------------------------------------*/

extern uint16_t 	 BL_Serial_Number;

uint32_t 		 Audio_Rec_Number = 0;
uint32_t 		 Actim_Fil_Number = 0;

/* Systick time variables -----------------------------------------------------*/

uint32_t 		Time_start		= 0; 
uint32_t 		Time_end		= 0xFFFFFFFF; 

uint32_t 		Time_Acti_start		= 0; 
uint32_t 		Time_Acti_end		= 0xFFFFFFFF; 


/*******************************************************************************
* Function Name  : HalfCryptSvgrBuffCpltCallback 
* Description    : Redefined to notify that audio data are encrypted and ready to save.
* Input          : No input
* Return         : No return value
*******************************************************************************/
void HalfCryptSvgrBuffCpltCallback(void)
{
	HalfCryptSvgrdBuffCplt  = 1;
}

/*******************************************************************************
* Function Name  : CryptSvgrBuffCpltCallback 
* Description    : Redefined to notify that audio data are encrypted and ready to save.
* Input          : No input
* Return         : No return value
*******************************************************************************/
void CryptSvgrBuffCpltCallback(void)
{
	CryptSvgrdBuffCplt      = 1;
}

/*******************************************************************************
* Function Name  : Motion_Buff_Half_Ready 
* Description    : Redefined to notify that motion data are ready to save.
* Input          : No input
* Return         : No return value
*******************************************************************************/
void Motion_Buff_Half_Ready(void)
{
	HalfActiSvgrdBuffCplt		= 1;
}

/*******************************************************************************
* Function Name  : Motion_Buff_Cplt_Ready 
* Description    : Redefined to notify that motion data are ready to save.
* Input          : No input
* Return         : No return value
*******************************************************************************/
void Motion_Buff_Cplt_Ready(void)
{
	ActiSvgrdBuffCplt				= 1;
}

/*

---------------------------Fonctions d'initialisation-----------------------------------------

*/ 


void Test_FATFS_Init(void) 
{
	FRESULT res;                                          
  uint32_t byteswritten=0;
	
	retSD = FATFS_LinkDriver(&SD_Driver,SDPath );
	retSD = FATFS_LinkDriver(&SD_Driver,SDPath);

    
	if(f_fdisk(0, plist, work) != FR_OK)	                /* Divide physical drive 0 */
	{
		while(1);
	}
	// err in doc the f_mkfs take the size of the cluster in Byte on the third argument
  if(f_mkfs("0:/", FM_FAT32, 65536, work, 512) != FR_OK)		 /* Create FAT volume on the logical drive 0 */
		{
			while(1);
		}
	if(f_mkfs("1:/", FM_FAT32, 0, work, 512) != FR_OK) /* Create FAT volume on the logical drive 1 */
		{
			while(1); 
		}

		
	if(f_mount(&SDFatFS1,"0:", 1) != FR_OK)
	{
				//PWR_Go_To_Shutdown_Mode();
		while(1);
	}
		

	if(f_mount(&SDFatFS2,"1:", 1) != FR_OK)
		{
			//PWR_Go_To_Shutdown_Mode();
			while(1);
		}
		
		if(f_open(&SDFile,"0:/Context0.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
		{
				while(1);
		}
		
	HAL_Delay(150);	
		
	res = f_write(&SDFile, "A", 1 , (void *)&byteswritten);
	
  if((byteswritten == 0) || (res != FR_OK))
  {
		//FATFS_Error_Handel();
		while(1);
	}	
		
	HAL_Delay(150);
		
	f_close(&SDFile);
		
	HAL_Delay(150);
		
	if(f_open(&SDFile,"1:/Context1.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
		{
				while(1);
		}

	HAL_Delay(150);

	//	byteswritten=0;
		
	res = f_write(&SDFile, "B", 1 , (void *)&byteswritten);
	
  if((byteswritten == 0) || (res != FR_OK))
  {
		//FATFS_Error_Handel();
		while(1);
	}
		
		HAL_Delay(150);
		
		f_close(&SDFile);
		
		HAL_Delay(150);
		
		f_mount(NULL, "0:/", 0);
		f_mount(NULL, "1:/", 0);
		
		FATFS_UnLinkDriver("0:/");
		FATFS_UnLinkDriver("1:/");
		
		while(1);
}

/*******************************************************************************
* Function Name  : FATFS_Init 
* Description    : Initialize file system and SD card feature.
* Input          : No input
* Return         : No return value
*******************************************************************************/
void FATFS_Init(void) 
{
	/* Link driver for the two partition */
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
	retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
	
	/* Mount logical disk for the two partition */
	if(f_mount(&SDFatFS1,"0:/", 1) != FR_OK)
  {
		/* Error management */
		PWR_Go_To_Shutdown_Mode();		/* turn off the device */
	}
	else
	{
		if(f_mount(&SDFatFS2,"1:/", 1) != FR_OK)
		{
			/* Error management */
			PWR_Go_To_Shutdown_Mode();		/* turn off the device */
		}
	}
	
	/* check if there is enough place for 1 hour of recording on the audio partition. */
	
	if (FATFS_SD_Enough_Space(500) == 1) 
	{	/* Not enough space */
		
		/* Deinitialize SD card */
		f_mount(NULL, SDPath, 0);
		FATFS_UnLinkDriver(SDPath);
		
		/* Start Led animation */
		IHM_LED_Start(LED_Continious_Mode,LED1_2);
		HAL_Delay(500);
		IHM_LED_Stop(LED1_2);
		HAL_Delay(500);
		IHM_LED_Start(LED_Continious_Mode,LED1_2);
		HAL_Delay(500);
		IHM_LED_Stop(LED1_2);
		HAL_Delay(500);
		IHM_LED_Start(LED_Continious_Mode,LED1_2);
		HAL_Delay(500);
		IHM_LED_Stop(LED1_2);
		HAL_Delay(500);
		IHM_LED_Start(LED_Continious_Mode,LED1_2);
		HAL_Delay(500);
		IHM_LED_Stop(LED1_2);
		HAL_Delay(500);
		IHM_LED_Start(LED_Continious_Mode,LED1_2);
		HAL_Delay(500);
		IHM_LED_Stop(LED1_2);
		HAL_Delay(500);
		/* Stop Led animation */
		
		PWR_Go_To_Shutdown_Mode();		/* turn off the device */
		
	}


	
	Restor_Context();		/* Restor context of working  */ 
}

/*******************************************************************************
* Function Name  : FATFS_Stop_System_SD_Space_PB 
* Description    : Callback to notify that the sd card is full
* Input          : No input
* Return         : No return value
*******************************************************************************/
__weak void FATFS_Stop_System_SD_Space_PB (void)
{
	/* Re-defined in main.c */
}



/*******************************************************************************
* Function Name  : FATFS_SD_Enough_Space 
* Description    : Check if there is enough space on the sd card.
* Input          : (uint32_t)The free space needed (In M Byte). 
* Return         : return 1 if there is not enough space, else 0.
*******************************************************************************/
uint8_t FATFS_SD_Enough_Space (uint32_t Free_Space)
{
	FATFS* pSDFatFS =&SDFatFS1 ;
	
	f_getfree(SDPath,&Clust_free, &pSDFatFS);		/* Get number of free cluster */
	
	if ( Clust_free < 62000)		/* No need to evalate space if to many cluster are free */  
	{
	
		cluster_size = SDFatFS1.csize;		/* Get number of sector in a cluster */
		Sector_size  = 512;		/* Set sector size */
	
		Size_free_SD = (cluster_size * Sector_size) * Clust_free;		/* Calculate free space */
	
		if( Size_free_SD < (Free_Space * 1000000) )		/* Compare the free space to the threshold */ 
		{	
			return 1; 
		}		
		else 
		{
			return 0;
		}
	}
		
	else 
	{
		return 0;
	}
	
}

/*******************************************************************************
* Function Name  : FATFS_Format_uSD
* Description    : Format the sd card partitions.
* Input          : No argument. 
* Return         : If sucess return TBD, else TBD.     														TBD !!!!!!!!!!
*******************************************************************************/
uint8_t FATFS_Format_uSD (void)
{
	uint8_t status = 0;
	retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
	
	//f_mkfs("",0,16384,NULL,8192);
	if(f_mount(&SDFatFS,"", 1) == FR_OK)
  {	
		Restor_Context();
		
		if (Audio_Rec_Number != 0)
		{
			while(Audio_Rec_Number != 0)
			{
				Audio_Rec_Number--;
				sprintf((char*)&WavFileName,"Rec%04d.txt",Audio_Rec_Number); 
				f_unlink((const char*)&WavFileName);
				HAL_Delay(500);
			}
			f_unlink("Context.txt");
		}
			f_mount(NULL, SDPath, 0);
			FATFS_UnLinkDriver(SDPath);
	}	


	return status;
	
}

/*******************************************************************************
* Function Name  : FATFS_DeInit 
* Description    : Save context of working, deinitialize the file system and disable HW access to the sd card.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
static void FATFS_DeInit(void)
{
	Save_Context();		/* Save index of files */
	f_mount(NULL, "0:/", 0);		/* unmount the audio partition */
	f_mount(NULL, "1:/", 0);		/* unmount the motion partition */
	FATFS_UnLinkDriver("0:/");		/* unlink the diskio for audio partition */
	FATFS_UnLinkDriver("1:/");		/* unlink the diskio for motion partition */
}

/*******************************************************************************
* Function Name  : FATFS_Error_Handel 
* Description    : Manages saving errors.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
static void FATFS_Error_Handel(void)
{
	#ifdef USE_LOG_FILE 
	FRESULT res;                                          
  uint32_t byteswritten=0;
	#endif
	
	Audio_Stop_Record();		/* Stop audio acquisition */
	Sensors_Stop();			/* Stop sensors acquisition */
	
	//IHM_Debug_Led(12); 		// test de proto
	
	f_mount(NULL, "0:/", 0);	/* unmount the audio partition */
	f_mount(NULL, "1:/", 0);	/* unmount the motion partition */
	FATFS_UnLinkDriver("0:/");	/* unlink the diskio for audio partition */
	FATFS_UnLinkDriver("1:/");	/* unlink the diskio for motion partition */																																																				// ajouter fonction pour reset sdmmc
	
	#ifdef USE_LOG_FILE 
	/* Link driver for audio and motion partitions*/
  	retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
	retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
	
	/* Mount logical disk for partitions */
	if(f_mount(&SDFatFS1,"0:/", 1) != FR_OK)		/* Mount audio partition*/
  	{
		/* Error management */
		PWR_Go_To_Shutdown_Mode();		/* Reset the device */
	}
	else
	{
		if(f_mount(&SDFatFS2,"1:/", 1) != FR_OK)		/* Mount motion partition*/
		{
			/* Error management */
			PWR_Go_To_Shutdown_Mode();		/* Reset the device */
		}
	}

	if(f_open(&SDFile,"1:/Context.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)		/* Open context file */
	{
		/* Error management */
		PWR_Reset_System();		/* Reset the device */
	}
	res = f_write(&SDFile, &Audio_Rec_Number, 4 , (void *)&byteswritten);		/* Write audio file index */
	
  if((byteswritten == 0) || (res != FR_OK))
  {
		/* Error management */
		PWR_Reset_System();		/* Reset the device */
	}	
	
	HAL_Delay(10);
	
	res = f_write(&SDFile, &Actim_Fil_Number, 4 , (void *)&byteswritten);		/* Write Motion file index */
	
  if((byteswritten == 0) || (res != FR_OK))
  {
		/* Error management */
		PWR_Reset_System();
	}	
			
	f_close(&SDFile);		/* Close context file */
	
	/* Unmount logical drive for the two partitions */
	f_mount(NULL, "0:/", 0);
	f_mount(NULL, "1:/", 0);
	
	/* Unlink the diskio driver for the two partitions */
	FATFS_UnLinkDriver("0:/");
	FATFS_UnLinkDriver("1:/");
	#endif
	PWR_Reset_System();	/* Reset the device */
}

/*

---------------------------Fonctions utiles ------------------------------------------------

*/ 


/*******************************************************************************
* Function Name  : Save_Context
* Description    : Save files indexes.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
void Save_Context (void)                                                           
{
	#ifdef USE_LOG_FILE 
	FRESULT res;                                          
  uint32_t byteswritten=0;

	
	if(f_open(&SDFile,"1:/Context.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 	/* Open context file */ 
	{
		/* Error management */
		FATFS_Error_Handel();
	}
	
	res = f_write(&SDFile, &Audio_Rec_Number, 4 , (void *)&byteswritten);		/* Write audio file index */
  if((byteswritten == 0) || (res != FR_OK))
  {
		/* Error management */
		FATFS_Error_Handel();
	}	
	
	HAL_Delay(10);
	
	res = f_write(&SDFile, &Actim_Fil_Number, 4 , (void *)&byteswritten);		/* Write motion file index */
  if((byteswritten == 0) || (res != FR_OK))
  {
		/* Error management */
		PWR_Reset_System();
	}
			
	if(f_close(&SDFile) != FR_OK) /* Close context file */
	{
		/* Error management */
		FATFS_Error_Handel();
	}
	#endif
}


/*******************************************************************************
* Function Name  : Restor_Context
* Description    : Recover indexes of audio and motion files.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
void Restor_Context (void)																																
{
	FRESULT res = FR_OK; 
	
	#ifdef USE_LOG_FILE
  uint32_t byteswritten=0; 
	#else 
	FILINFO fno;
	DIR dir;
	#endif
	
	#ifdef USE_LOG_FILE
	if(f_open(&SDFile,"1:/Context.txt",  FA_READ) != FR_OK)		/* Open context file in read only mode */ 
	{
		/* If the file does not exist, initialize the indexes. 
		   The file does not exist in the case of data were transferd to the baby dock, it means the begening of a new acquisition sequence*/
		Audio_Rec_Number = 0;	/* Initialize audio file index */
		Actim_Fil_Number = 0;	/* Initialize motion file index */
	}
	
	else
	{
		/* If the file exist, it means that we restart the device afeter a shutting down.
			 In this case we need to recover the indexes*/
		res = f_read(&SDFile, &Audio_Rec_Number, 4 , (void *)&byteswritten);		/* read audio file index from context file */
		if((byteswritten == 0) || (res != FR_OK))
		{
			/* Error managment */
			PWR_Reset_System();
		}
	
		res = f_read(&SDFile, &Actim_Fil_Number, 4 , (void *)&byteswritten);		/* read motion file index from context file */
		if((byteswritten == 0) || (res != FR_OK))
		{
			/* Error management */
			PWR_Reset_System();
		}
		
		f_close(&SDFile);		/* Close context file */
	}
	#else
	
	/* Find the index of audio file */
	res = f_opendir(&dir,"0:/" );

  if(res == FR_OK)
  {
    while(1)
    {
      res = f_readdir(&dir, &fno);
      if(res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if(fno.fname[0] == '.')
      {
        continue;
      }

			if((fno.fattrib & AM_DIR) == 0)
			{          
				Audio_Rec_Number++;
			}        
    }
  }
  f_closedir(&dir);
	
	/* Find the index of motion file */
	
	res = f_opendir(&dir,"1:/" );

  if(res == FR_OK)
  {
    while(1)
    {
      res = f_readdir(&dir, &fno);
      if(res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if( (fno.fname[0] == '.') || (fno.fname[0] == 'C') )
      {
        continue;
      }

			if((fno.fattrib & AM_DIR) == 0)
			{          
				Actim_Fil_Number++;
			}        
    }
  }
  f_closedir(&dir);
	#endif
}

/*******************************************************************************
* Function Name  : Audio_Svgrd_Pending
* Description    : Check if data are ready for saving.
* Input          : No input. 
* Return         : return 0 if no pending data else 1.
*******************************************************************************/
uint8_t Audio_Svgrd_Pending (void)
{
	uint8_t tmp; 
	
	if((HalfCryptSvgrdBuffCplt  == 0) && (CryptSvgrdBuffCplt == 0))		/* Check data ready flags */ 
	{
		tmp = 0;	/* No pending data */
	}
	else
	{
		tmp = 1;	/* pending data */
	}
	
	return tmp;	/* return status */
}

/*******************************************************************************
* Function Name  : FATFS_Force_Stop
* Description    : Set request to stop all saving applications.
* Input          : No input. 
* Return         : return 0 if no pending data else 1.
*******************************************************************************/
void FATFS_Force_Stop(void)
{
	ForceCloseFileRqst = 1;		/* Update request flag */
}

/*******************************************************************************
* Function Name  : Set_Tick_Start_Time
* Description    : Get the systick start time of the audio file.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
void Set_Tick_Start_Time (void)
{
	Time_start = HAL_GetTick();		/* get systick time */
}

/*******************************************************************************
* Function Name  : Set_Tick_End_Time
* Description    : Get the systick end time of the audio file.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
void Set_Tick_End_Time (void)
{
	Time_end = HAL_GetTick();		/* get systick time */
}


/*

---------------------------Fonctions de Process-----------------------------------------

*/ 

/*******************************************************************************
* Function Name  : Svgrd_Process
* Description    : Handle saving.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
void Svgrd_Process(void)
{
	Audio_Svgrd_Process();
	Swap_File_Process();
	Acti_Svgrd_Process();
	Force_close_Files_Process();
}

/*******************************************************************************
* Function Name  : Acti_Svgrd_Process
* Description    : Management of saving motion data.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
void Acti_Svgrd_Process(void)
{
	FRESULT res;                                          
  uint32_t byteswritten=0; 
	
	/* Check if there is a new file request and no file is already open */
	if ((OpenActiFileRqst == 1) && ( Actim_File_State == 0) )
	{/* Open new motion file */
				
		Time_Acti_start = HAL_GetTick();		/* Get systick start time for motion file */
		RTC_Get_time_date(&RTC_Buff[0]);		/* Get start real time and date for motion file */
		
		sprintf((char*)&ActFileName, "1:/Act%04d.txt", Actim_Fil_Number);  /* Set file name */
	
		if(f_open(&Acti_File,(const char*)&ActFileName, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 	/* Open new file */
		{			
			/* Error management */ 
		//	IHM_Debug_Led(5); // test de proto
			//FATFS_Error_Handel();
		}
		else
		{
			/* Allocate continious area for motion file */
			if (f_expand(&Acti_File,((ActiCounterBuffMax*1024)+72),0)	!= FR_OK)
			{
				/* Error management */ 
				//IHM_Debug_Led(6);
				//while(1);   										// to remove
			}	
			if(f_lseek(&Acti_File, 72) != FR_OK)		/* Setting cursor of motion file after header */
			{
				/* Error management */
				FATFS_Error_Handel();
			}
						
			// Reset Acquisition 
			//Acti_Init_Acquisition_Variables();   No need to reset for continious acquisition

			Acti_Init_Debug_Variables();		/* Reset debeug varables */
			Acti_CounterBuff = 0;		/* Reset counter saved buffer*/
			
			/* update flags */
			Actim_File_State = 1;
			OpenActiFileRqst = 0;

			Actim_Fil_Number++;		/* Increment motion file index */
			
			/* Memory copy : Save RTC buffer in dedicated buffers */
			Rtc_Acti_Time_Start[0] = RTC_Buff[0];
			Rtc_Acti_Time_Start[1] = RTC_Buff[1];
			Rtc_Acti_Time_Start[2] = RTC_Buff[2];
			Rtc_Acti_Time_Start[3] = RTC_Buff[3];
				
			Rtc_Acti_Date_Start[0] = RTC_Buff[4];
			Rtc_Acti_Date_Start[1] = RTC_Buff[5];
			Rtc_Acti_Date_Start[2] = RTC_Buff[6];
		}
	} 
	
	/* Check if the first buffer of motion data and the file are ready for writing operation */
	if ( (HalfActiSvgrdBuffCplt == 1) && (Actim_File_State == 1) )
	{
		/* Write the first half of the buffer */
		res = f_write(&Acti_File, pAccGyrBuff_Half ,Size_Acc_Gyr_Buff  , (void *)&byteswritten);   
    if((byteswritten == 0) || (res != FR_OK))		/* if writing operation failed */
    {
			/* Error management */
		//	IHM_Debug_Led(7); // test proto   
			FATFS_Error_Handel();
    }
		else 
		{	/* If writing succeed */
			Acti_CounterBuff++;		/* Increment number of buffer saved */
			HalfActiSvgrdBuffCplt = 0;		/* Update Flag */
		}	
	}		
	
	/* Check if the second buffer of motion data and the file are ready for writing operation */	
	if ( (ActiSvgrdBuffCplt == 1) && (Actim_File_State == 1) )
	{
		/* Write the second half of the buffer */		
		res = f_write(&Acti_File, pAccGyrBuff_Cplt ,Size_Acc_Gyr_Buff  , (void *)&byteswritten);
		
    if((byteswritten == 0) || (res != FR_OK))
    {
			/* Error managemenet */
		//	IHM_Debug_Led(8);   
			FATFS_Error_Handel();
    }
		else 
		{	/* If writing succeed */
			Acti_CounterBuff++;		/* Increment number of buffer saved */ 			
			ActiSvgrdBuffCplt = 0;		/* Update flag */
		}
	}	
	
	/* Check if it's needed to close file. As precaution we check if the file is really open and no data are pending */
	if ( (CloseActiFileRqst == 1) && (Actim_File_State == 1)
				&& (ActiSvgrdBuffCplt == 0) && (HalfActiSvgrdBuffCplt == 0))
	{

		Time_Acti_end = HAL_GetTick();		/* Get systick end time */
		RTC_Get_time_date(&RTC_Buff[0]);		/* Get real time clock end time */
		
		/* Memory copy : Save RTC buffer in dedicated buffers */
		Rtc_Acti_Time_End[0] = RTC_Buff[0];
		Rtc_Acti_Time_End[1] = RTC_Buff[1];
		Rtc_Acti_Time_End[2] = RTC_Buff[2];
		Rtc_Acti_Time_End[3] = RTC_Buff[3];
			
		Set_Acti_Cust_Header(&My_Buff_Acti_Cust_Header[0] );	/* Update header */

		if(f_lseek(&Acti_File, 0) == FR_OK)		/* Setting the file cursor at the beginning */
    {
			res = f_write(&Acti_File, &My_Buff_Acti_Cust_Header,72, (void *)&byteswritten);		/* Write the header */ 
      if((byteswritten == 0) || (res != FR_OK))
      {
				/* Error management */
				//FATFS_Error_Handel();
      }
		}

		if (f_close(&Acti_File) == FR_OK ) 	/* Close motion file */
		{		
			/* update flags */
			CloseActiFileRqst = 0;
			Actim_File_State = 0;
			OpenActiFileRqst = 1;		/* If closing succeed notify a open file request */
		}
		else 
		{
			/* Error management */
			//IHM_Debug_Led(9);
			//FATFS_Error_Handel();                                                                            // commented for test
		}	
	} 
	
	/* Check if it's time to change file */
	if ( (Acti_CounterBuff >= ActiCounterBuffMax) && (Actim_File_State == 1) )														// (... == ...) --> (... >= ...) for test
	{
		CloseActiFileRqst = 1;	/* Notify a close file request */
	}
	
}

/*******************************************************************************
* Function Name  : Audio_Svgrd_Process
* Description    : Management of saving audio data.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
void Audio_Svgrd_Process (void)
{
	FRESULT res;                                          
  uint32_t byteswritten=0; 
	
	/* To prevent data loss when opening a new file, we implement an algorithm of file swapping.
		 Before closing a file we open an other, then we start writing in the new file ans close the old one */
	
	/* Check if the first half of encrypted audio data is ready, 
		 to prevent errors we check if at least one file is open */
	if( (HalfCryptSvgrdBuffCplt == 1) && (( Audio_File_1_State == 1 ) || ( Audio_File_2_State == 1 ))  )
	{
		
		HalfCryptSvgrdBuffCplt=0;		/* Update flag */
		
		/* write encrypted audio data */
		res = f_write(pAudio_File, pCryptSvgrdHalfBuff, ((32*BuffSize)+16) , (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK))
    {
			/* Error management */ 
		//	IHM_Debug_Led(2); 							// for debug
			FATFS_Error_Handel();
    }
		else 
		{
			/* if writing operation succeed */			
			CounterBuff++;	/* Increment the number of buffer saved */
		}
	}

	/* Check if the second half of encrypted audio data is ready, 
		 to prevent errors we check if at least one file is open */
	if( (CryptSvgrdBuffCplt == 1) && (( Audio_File_1_State == 1 ) || ( Audio_File_2_State == 1 )) )
	{

		CryptSvgrdBuffCplt = 0;/* Update flag */
		
		/* write encrypted audio data */
		res = f_write(pAudio_File, pCryptSvgrdCpltBuff, ((32*BuffSize)+16) , (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK))
    {
			/* Error management */
			//IHM_Debug_Led(3);
			FATFS_Error_Handel();
    }		
		else 
		{
			/* If writing operation succeed */
			CounterBuff++;	/* Increment the number of saved buffer */
		}
	}
	
	/* Check if is there a opening file request and at least one is closed */
	if ( (OpenFileRqst == 1) && (( Audio_File_1_State == 0 ) || ( Audio_File_2_State == 0 )) )
	{	
		/* Check if at least one file is open */
		if( ( Audio_File_1_State != 0 ) || ( Audio_File_2_State != 0 ))
		{
			f_sync(pAudio_File);		/* As precaution, save data of the current file */
		}
	
		sprintf((char*)&WavFileName,"0:/Rec%04d.txt",Audio_Rec_Number);		/* Prepare the file name */
	
		/* Open a new file */
		if(f_open(pAudio_Next_File,(const char*)&WavFileName, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) 
		{
			OpenFileRqst = 0;		/* Update flag */
			Swap_file_state = Swap_file_state_Run; 	/* Update status of the swap file state machine */
			
					/* Allocate a continious memory area for the audio file */
			if (f_expand(pAudio_Next_File,((CounterBuffMax*8208)+84),0)	!= FR_OK)
			{
				/* Error management */
			}		
		}
		else
		{
			/* Error management */
		//	IHM_Debug_Led(0); 							// for debug
			//FATFS_Error_Handel();
		}
			
		
		/* Check if it's the first opned file after turning on */			
		if( ( Audio_File_1_State == 0 ) && ( Audio_File_2_State == 0 ))
		{
				/* We didn't update the status of the file, so if both of them are reset it means that it's the first 
				 acquisition after the wake up of the device.
				 Exceptionally, in this case, we make some initialization operations here. 
				 */
			
				/* The ARS peripheral must be initialize hear because the IV progress during the calibration step */
				CRYPT_Init(0);		/* Initialize the working of the peripheral in charge of encryption */
				Cryp_Init_process_variables();		/* Reset the encryption process flags */
			
				Audio_File_1_State = 1;		/* Update file status flag */
				pAudio_File = &Audio_File_1;		/* Set the pointer of the current file on the first file object */

			  Set_Tick_Start_Time();		/* Set systick start time */

				RTC_Get_time_date(&RTC_Buff[0]);		/* Set RTC start time */
				
			/* Memory copy */
				Rtc_Time_Start[0] = RTC_Buff[0];
				Rtc_Time_Start[1] = RTC_Buff[1];
				Rtc_Time_Start[2] = RTC_Buff[2];
				Rtc_Time_Start[3] = RTC_Buff[3];
				
				Rtc_Date_Start[0] = RTC_Buff[4];
				Rtc_Date_Start[1] = RTC_Buff[5];
				Rtc_Date_Start[2] = RTC_Buff[6];
			
				/* Reset audio acquisition */
				
				Audio_Init_Svgrd_Buffer();		/* Reset audio acquisition buffer  */
				Audio_Init_Debug_Variables();		/* Reset audio debug variables */
				Crypt_Init_Debug_Variables();		/* Reset encryption debug variable */ 
				/* Reset encrypted audio data ready flags */
				HalfCryptSvgrdBuffCplt = 0;
			  CryptSvgrdBuffCplt 		= 0;			
	
				OpenActiFileRqst			= 1;		/* Notify a request of opening motion file (it means start motion sub-application) */
		}
		else
		{
			if (OpenFileRqst == 0)
			{
				/* If it isn't the first acquisition after wake-up */
			
				/* Update the status of the opened file */
				if (pAudio_Next_File == &Audio_File_1)
				{
					Audio_File_1_State = 1;
				}
				else 
				{
					Audio_File_2_State = 1;
				}
			}
		}
		
		if(OpenFileRqst == 0)
		{
			Set_Audio_Cust_Header_Start(My_Buff_Cust_Header);
			if(f_write(pAudio_Next_File, &My_Buff_Cust_Header,84, (void *)&byteswritten) != FR_OK)	/* Setting cursor of audio file after header */
			{
				if (f_lseek(pAudio_Next_File, 84) != FR_OK)
				{
					/* Error management  */ 
					FATFS_Error_Handel();
				}		
			}
		
			Audio_Rec_Number++;		/* Increment audio file index */
		}

	}
	// close file
	
	/* Check if is there a file closing request */
	/* to prevent errors we check if both of audio files are open and there is no data pending */
	if ( (CloseFileRqst == 1) && ( Audio_File_1_State == 1 ) && ( Audio_File_2_State == 1 ) 
		&& ( CryptSvgrdBuffCplt == 0 ) && ( HalfCryptSvgrdBuffCplt == 0 ) )
	{
		byteswritten = 0;
	
		/* Update and write header */
		if(f_lseek(pAudio_Old_File, 0) == FR_OK)		/* Setting the file cursor at the beginning */
    {
			/* if succeed write the new header */
			res = f_write(pAudio_Old_File, &My_Buff_Cust_Header,84, (void *)&byteswritten);
      if((byteswritten == 0) || (res != FR_OK))
      {
				/* Error management */ 
				FATFS_Error_Handel();
      }
		}
	
		if (f_close(pAudio_Old_File)== FR_OK)		/* Close old audio file */
		{
			/* Update the status of the closed file */
			if (pAudio_Old_File == &Audio_File_1)
			{
				Audio_File_1_State = 0;
			}
			else
			{
				Audio_File_2_State = 0;
			}		
			CloseFileRqst = 0; /* update flag */
		}
		else 
		{
			/* Error management: Error in closing file */
			//IHM_Debug_Led(4);							// for debug
			//FATFS_Error_Handel();																																/*  Commented for test  */
		}

		
	}
}

/*******************************************************************************
* Function Name  : Force_close_Files_Process
* Description    : Management of stopping saving after user request.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
static void Force_close_Files_Process(void)
{
		FRESULT res;                                          
		uint32_t byteswritten=0; 
	
	/* Chech if is there a force close request */
	/* check also if is there data which are wating to be written and if only one audio file is open */
	if ( (ForceCloseFileRqst == 1) && ((Audio_File_1_State + Audio_File_2_State) == 1 ) 
		&& ( CryptSvgrdBuffCplt == 0 ) && ( HalfCryptSvgrdBuffCplt == 0 ) )
	{
		/* When we force close the systick end time is not the scheduled so we have to updated end time in the header*/
		// Audio time of end
		Time_end = HAL_GetTick ();		/* Get systick end time for audio file */
		RTC_Get_time_date(&RTC_Buff[0]);		/* Get RTC end time for audio file */
		
		/* Memory copy */
		Rtc_Time_End[0] = RTC_Buff[0];
		Rtc_Time_End[1] = RTC_Buff[1];
		Rtc_Time_End[2] = RTC_Buff[2];
		Rtc_Time_End[3] = RTC_Buff[3];

		Time_Acti_end = Time_end;		/* Get systick end time for motion file */
		
		/* Mem cpy: RTC end time for motion file*/
		Rtc_Acti_Time_End[0] = RTC_Buff[0];
		Rtc_Acti_Time_End[1] = RTC_Buff[1];
		Rtc_Acti_Time_End[2] = RTC_Buff[2];
		Rtc_Acti_Time_End[3] = RTC_Buff[3];
	
		Set_Acti_Cust_Header(&My_Buff_Acti_Cust_Header[0] );		/* update Header motion files */
		
		//write audio header 
		if(f_lseek(pAudio_File, 0) == FR_OK)	/* Settig the audio file cursor at beginning */
    {
			/* If succeed */
			Set_Audio_Cust_Header2(& My_Buff_Cust_Header[0]);	/* update Header of audio file */	
			
			res = f_write(pAudio_File, My_Buff_Cust_Header,84, (void *)&byteswritten);  /* write Audio file Header */  
      if((byteswritten == 0) || (res != FR_OK))
      {   
				/* Error management */
				FATFS_Error_Handel();
      }
		}
		
		if (f_close(pAudio_File)== FR_OK)		/* Close audio file */
		{
			/* Update audio file status */
			if (pAudio_File == &Audio_File_1)
			{
				Audio_File_1_State = 0;
			}
			else
			{
				Audio_File_2_State = 0;
			}
		}
		else 
		{
			/* Error management */
			//IHM_Debug_Led(4);						// for debug
			/* No need to handel error before turning off */
		}
		
		/* check if audio file closing succeeded and motion file is still open */
		if ( (ForceCloseFileRqst == 1) && ( Audio_File_1_State == 0 ) && ( Audio_File_2_State == 0 ) && (End_Init_Filter == 1) )
		{
			
			if(f_lseek(&Acti_File, 0) == FR_OK)	/* Setting the motion file cursor at the beginning */
			{
		
				res = f_write(&Acti_File, &My_Buff_Acti_Cust_Header,72, (void *)&byteswritten);	/* Write motion file header */      
				if((byteswritten == 0) || (res != FR_OK))
				{
					/* Error management: when writing motion header */
					FATFS_Error_Handel();
				}
			}
			else
			{
				/* Error management : when mooving file cursor */
				FATFS_Error_Handel();
			}
			
			
			if (f_close(&Acti_File)!=FR_OK)
			{
				/* Error management closing motion file */
				//IHM_Debug_Led(9);																			// for debug
				FATFS_Error_Handel();
			}
			Swap_file_state = Swap_file_state_Off;
		}
	}	
	if ((ForceCloseFileRqst == 1) && ( CryptSvgrdBuffCplt == 0 ) && ( HalfCryptSvgrdBuffCplt == 0 )&& ((Audio_File_1_State + Audio_File_2_State) == 2 ))
	{
		if ((CounterBuff < CounterBuffDelay)&&(CounterBuff != 0))
		{
			CloseFileRqst = 1;		
		}
		else
		{
			if (f_close(pAudio_Next_File)== FR_OK)		/* Close audio file */
			{
				/* Update audio file status */
				if (pAudio_Next_File == &Audio_File_1)
				{
					Audio_File_1_State = 0;
				}
				else
				{
					Audio_File_2_State = 0;
				}
			}
		}
	}

}

/*******************************************************************************
* Function Name  : Swap_File_Process
* Description    : State machine to manage file swapping process.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
void Swap_File_Process (void)
{
	/* Wait until the end of the caliration sequence of the dfsdm's digitals filters. */
	if (HAL_GetTick()> 1000 && End_Init_Filter == 0)
	{
		End_Init_Filter = 1;
		Swap_file_state = Swap_file_state_Init;
	}
	
	/* Notify a closing request of the old audio file after wating for a predefined number of write operation
		 in the new audio file*/
	if ( (Wait_Close_Rqst == 1) && (CounterBuff >= CounterBuffDelay) )  																							// (... == ...) --> (... >= ...) for test
	{
		Wait_Close_Rqst = 0;
		CloseFileRqst = 1; 		
	}
	
	/* State machine which manage the swapping of audio files */
	switch (Swap_file_state)
	{
		
		case Swap_file_state_Off:	
		
			/* We access this state after wake up or by pushing the button more than 3 secs */
		
			/* When end_init_filter is set, it means that we enter in swap file state off after working
				 by keeping On/Off button pushed more than 3 sec. */
		
			if (End_Init_Filter == 1 )
			{
				FATFS_DeInit();		/* Deinitialize sd card feature */
				Sd_DeInit_Callback();		/* Callback to notify that SD sub-application is deinitialize*/
				End_Init_Filter++;		/* Incremete it to prevent recursive execution */
			}
		break;
			
		case Swap_file_state_Init: 
			
			/* We access this state from Swap_file_state_Off (after wake up) when time of calib is end */
		
		if (End_Init_Filter == 1)
		{
			/* Initialize the application */
			
			/* All initialize have been correctly executed, calibration time is end 
				 so we initialize the next file to open and set an opening file request */
			
			pAudio_Next_File = &Audio_File_1; 
			OpenFileRqst = 1;
		}
		
		break;
		
		case Swap_file_state_Run:
			/* In this case we check if it's time to open a new file and if it's the time to swap file */
			/* The variable Swap_Timeout_State is useful to avoid many opning or swapping file */
		
			/* Check if it's time to open a new file */
		if( (CounterBuff >= (CounterBuffMax - CounterBuffDelay)) && (Swap_Timeout_State == 0) )																				// (... == ...) --> (... >= ...) for test
		{
			/* Check if there is no data waiting for encryption or writing to prevent fatfs overrun */
			if ( (Cryp_Pending() == 0) && (Audio_Svgrd_Pending() == 0) )
			{
				/* New file request */
				Swap_Timeout_State = 1;
				Swap_file_state = Swap_file_state_New_file_rqst; 
			}
		}

		/* check if it's time to swap file */
		if((CounterBuff >= CounterBuffMax) && (Swap_Timeout_State == 1) )																							// (... == ...) --> (... >= ...) for test
		{
			/* Before start the process of swapping file we have to make the initialisation of the periph AES 
			 because the IV increase. */
			
			/* To make the initialisation at the good moment we have to check before if there 
			 is no buffer waiting to be crypted or writen */
			if ( (Cryp_Pending() == 0) && (Audio_Svgrd_Pending() == 0) )
			{
				/* Swap file request */
				CRYPT_Init(0);
				Swap_Timeout_State = 2;
				Swap_file_state = Swap_file_state_Close_file_rqst;
			}
		}
		break;
		
		case Swap_file_state_New_file_rqst:
			/* called from run mode state X miliseconde before pre-set timeout to prépare next file*/
			
			/* Update the pointer of the file to open */
		if (pAudio_File == &Audio_File_1)
		{
			pAudio_Next_File = &Audio_File_2;
		}
		else 
		{
			pAudio_Next_File = &Audio_File_1;
		}
		
		/* open request for svgrd process */
		OpenFileRqst = 1;		
		
		
		break;
		
		case Swap_file_state_Close_file_rqst: 
			/* called from run mode state atpre-set timeout to swap files and close the old one */
		
			/* Update the pointer of the files */
		if (pAudio_Next_File == &Audio_File_1)
		{
			pAudio_File     = &Audio_File_1;
			pAudio_Old_File = &Audio_File_2;
		}
		else
		{
			pAudio_File			= &Audio_File_2;
			pAudio_Old_File = &Audio_File_1;
		}

		Wait_Close_Rqst = 1; /* enable flag to close old file after equivalent periode to CounterBuffDelay */		

		Swap_file_state = Swap_file_state_Run;		/* Go to run mode */
		
		/* Update end time */
		Set_Tick_End_Time();
		RTC_Get_time_date(&RTC_Buff[0]);
				
		Rtc_Time_End[0] = RTC_Buff[0];
		Rtc_Time_End[1] = RTC_Buff[1];
		Rtc_Time_End[2] = RTC_Buff[2];
		Rtc_Time_End[3] = RTC_Buff[3];

		/* Update audio header */
		Set_Audio_Cust_Header2(My_Buff_Cust_Header);
		
		/* Get start time for the new record */
		Set_Tick_Start_Time();
		RTC_Get_time_date(&RTC_Buff[0]);
				
		Rtc_Time_Start[0] = RTC_Buff[0];
		Rtc_Time_Start[1] = RTC_Buff[1];
		Rtc_Time_Start[2] = RTC_Buff[2];
		Rtc_Time_Start[3] = RTC_Buff[3];
		Rtc_Date_Start[0] = RTC_Buff[4];
		Rtc_Date_Start[1] = RTC_Buff[5];
		Rtc_Date_Start[2] = RTC_Buff[6];
		
		/* Reset debug variable */
		CounterBuff=0;
		Audio_Init_Debug_Variables();
	  Crypt_Init_Debug_Variables();
		
		/* Reset Swap_Timeout_State to wait for a new open file timeout */
		Swap_Timeout_State=0;
		
		break; 
		
		default:
			
		break;
		
	}
}

/*******************************************************************************
* Function Name  : Set_Audio_Cust_Header2
* Description    : fill information in audio header.
* Input          : Audio header buffer. 
* Return         : No return value.
*******************************************************************************/
static void Set_Audio_Cust_Header2(uint8_t* CustHeader )
{
	uint16_t tmp;
	uint32_t tmp32;
	
	// serial number
	CustHeader[0] = (uint8_t)(BL_Serial_Number & 0x00FF);
	CustHeader[1] = (uint8_t)((BL_Serial_Number >>8) & 0x00FF);
	
	// Start date
	
	tmp = 2000 + (uint16_t) Rtc_Date_Start[0];
	CustHeader[2] = (uint8_t) (tmp & 0x00FF) ;
	CustHeader[3] = (uint8_t) ((tmp >> 8) & 0x00FF);	
	CustHeader[4] = Rtc_Date_Start[1];
	CustHeader[5] = Rtc_Date_Start[2];

	// Time start 
	CustHeader[6] = Rtc_Time_Start[0];
	CustHeader[7] = Rtc_Time_Start[1];
	CustHeader[8] = Rtc_Time_Start[2];
	CustHeader[9] = Rtc_Time_Start[3];
	
	// time end

	CustHeader[10] = Rtc_Time_End[0];
	CustHeader[11] = Rtc_Time_End[1];
	CustHeader[12] = Rtc_Time_End[2];
	CustHeader[13] = Rtc_Time_End[3];
	
	// audio resolution
	CustHeader[14] = Res ;
	
	// audio freq
	tmp = SamplingFreq;
	CustHeader[15] = (uint8_t) ( tmp & 0x00FF )   ;
	CustHeader[16] = (uint8_t) (( tmp >> 8) & 0x00FF )  ;
	
	// nbr channel
	CustHeader[17] = (uint8_t) Nbr_Ch	 ;
	
	// sys tick start
	tmp32 = Time_start;
	CustHeader[18] = (uint8_t) (tmp32 & 0x000000FF) ;
	CustHeader[19] = (uint8_t) ((tmp32 >> 8) & 0x000000FF) ;
	CustHeader[20] = (uint8_t) ((tmp32 >> 16) & 0x000000FF) ;
	CustHeader[21] = (uint8_t) ((tmp32 >> 24) & 0x000000FF) ;
	
	// sys tick end 
	tmp32 = Time_end;
	CustHeader[22] = (uint8_t) (tmp32 & 0x000000FF) ;
	CustHeader[23] = (uint8_t) ((tmp32 >> 8) & 0x000000FF) ;
	CustHeader[24] = (uint8_t) ((tmp32 >> 16) & 0x000000FF) ;
	CustHeader[25] = (uint8_t) ((tmp32 >> 24) & 0x000000FF) ;
	
	Audio_Get_Debug_Variables( &CustHeader[26] );		/* Get debug variables from acquisition file */
	
	// nbr cryp buff
	tmp32 = Cryp_Get_Debug_Variables();		/* Get encryption counter from crypt file */
	CustHeader[66] = (uint8_t) (tmp32 & 0x000000FF);
	CustHeader[67] = (uint8_t) ((tmp32 >> 8) & 0x000000FF);
	CustHeader[68] = (uint8_t) ((tmp32 >> 16) & 0x000000FF);
	CustHeader[69] = (uint8_t) ((tmp32 >> 24) & 0x000000FF);

	// nbr saved buff
	tmp32 = CounterBuff;
	CustHeader[70] = (uint8_t) (tmp32 & 0x000000FF);
	CustHeader[71] = (uint8_t) ((tmp32 >> 8) & 0x000000FF);
	CustHeader[72] = (uint8_t) ((tmp32 >> 16) & 0x000000FF);
	CustHeader[73] = (uint8_t) ((tmp32 >> 24) & 0x000000FF);
	
	/* For futur implementations */
	for(tmp = 74; tmp < 84 ; tmp++)
	{
		CustHeader[tmp] = 0; 
	}
}

/*******************************************************************************
* Function Name  : Set_Audio_Cust_Header_Start
* Description    : fill information in audio header at the begginig.
* Input          : Audio header buffer. 
* Return         : No return value.
*******************************************************************************/
static void Set_Audio_Cust_Header_Start(uint8_t* CustHeader )
{
	uint16_t tmp;
	uint32_t tmp32;
	
	// serial number
	CustHeader[0] = (uint8_t)(BL_Serial_Number & 0x00FF);
	CustHeader[1] = (uint8_t)((BL_Serial_Number >>8) & 0x00FF);
	
	// Start date
	
	tmp = 2000 + (uint16_t) Rtc_Date_Start[0];
	CustHeader[2] = (uint8_t) (tmp & 0x00FF) ;
	CustHeader[3] = (uint8_t) ((tmp >> 8) & 0x00FF);	
	CustHeader[4] = Rtc_Date_Start[1];
	CustHeader[5] = Rtc_Date_Start[2];

	// Time start 
	CustHeader[6] = Rtc_Time_Start[0];
	CustHeader[7] = Rtc_Time_Start[1];
	CustHeader[8] = Rtc_Time_Start[2];
	CustHeader[9] = Rtc_Time_Start[3];
	
	// time end

	CustHeader[10] = 0;
	CustHeader[11] = 0;
	CustHeader[12] = 0;
	CustHeader[13] = 0;
	
	// audio resolution
	CustHeader[14] = Res ;
	
	// audio freq
	tmp = SamplingFreq;
	CustHeader[15] = (uint8_t) ( tmp & 0x00FF )   ;
	CustHeader[16] = (uint8_t) (( tmp >> 8) & 0x00FF )  ;
	
	// nbr channel
	CustHeader[17] = (uint8_t) Nbr_Ch	 ;
	
	// sys tick start
	tmp32 = Time_start;
	CustHeader[18] = (uint8_t) (tmp32 & 0x000000FF) ;
	CustHeader[19] = (uint8_t) ((tmp32 >> 8) & 0x000000FF) ;
	CustHeader[20] = (uint8_t) ((tmp32 >> 16) & 0x000000FF) ;
	CustHeader[21] = (uint8_t) ((tmp32 >> 24) & 0x000000FF) ;	
	
	/* For futur implementations */
	for(tmp = 22; tmp < 84 ; tmp++)
	{
		CustHeader[tmp] = 0; 
	}
}

/*******************************************************************************
* Function Name  : Set_Acti_Cust_Header
* Description    : fill information in audio header.
* Input          : Audio header buffer. 
* Return         : No return value.
*******************************************************************************/
static void Set_Acti_Cust_Header(uint8_t* CustHeader )
{
	// total size = 72
	
	
	uint16_t tmp;
	uint32_t tmp32;
	
	// serial number
	CustHeader[0] = (uint8_t)(BL_Serial_Number & 0x00FF);
	CustHeader[1] = (uint8_t)((BL_Serial_Number >>8) & 0x00FF);
	
	// Start date
	tmp = 2000 + (uint16_t) Rtc_Acti_Date_Start[0];
	CustHeader[2] = (uint8_t) (tmp & 0x00FF) ;
	CustHeader[3] = (uint8_t) ((tmp >> 8) & 0x00FF);	
	CustHeader[4] = Rtc_Acti_Date_Start[1];
	CustHeader[5] = Rtc_Acti_Date_Start[2];

	// Time start 
	CustHeader[6] = Rtc_Acti_Time_Start[0];
	CustHeader[7] = Rtc_Acti_Time_Start[1];
	CustHeader[8] = Rtc_Acti_Time_Start[2];
	CustHeader[9] = Rtc_Acti_Time_Start[3];
	
	// time end
	CustHeader[10] = Rtc_Acti_Time_End[0];
	CustHeader[11] = Rtc_Acti_Time_End[1];
	CustHeader[12] = Rtc_Acti_Time_End[2];
	CustHeader[13] = Rtc_Acti_Time_End[3];
	
	// Sensors info
	tmp = 52;
	
	CustHeader[14] = (uint8_t) ( tmp & 0x00FF ) ;
	CustHeader[15] = (uint8_t) (( tmp >> 8) & 0x00FF )  ;
	
	CustHeader[16] = CustHeader[14];
	CustHeader[17] = CustHeader[15];
	
	CustHeader[18] = 3; // High performance		/* 0 for low power, 1 for normal, 3 for High performance. */
	CustHeader[19] = 3; // High performance		/* 0 for low power, 1 for normal, 3 for High performance. */
	CustHeader[20] = 0; // +- 2G							/* 0: 2g, 1: 16g, 3: 4g, 4 : 8g. */
	CustHeader[21] = 1;	// 250 dps						/* 0: 125dsp 1: 250dps, 3: 500dps, 4 : 1000dps, 5: 2000 dps. */
	
	// sys tick start
	tmp32 = Time_Acti_start;
	CustHeader[22] = (uint8_t) (tmp32 & 0x000000FF) ;
	CustHeader[23] = (uint8_t) ((tmp32 >> 8) & 0x000000FF) ;
	CustHeader[24] = (uint8_t) ((tmp32 >> 16) & 0x000000FF) ;
	CustHeader[25] = (uint8_t) ((tmp32 >> 24) & 0x000000FF) ;
	
	// sys tick end 
	
	tmp32 = Time_Acti_end;
	CustHeader[26] = (uint8_t) (tmp32 & 0x000000FF) ;
	CustHeader[27] = (uint8_t) ((tmp32 >> 8) & 0x000000FF) ;
	CustHeader[28] = (uint8_t) ((tmp32 >> 16) & 0x000000FF) ;
	CustHeader[29] = (uint8_t) ((tmp32 >> 24) & 0x000000FF) ;
	
	Sensors_Acti_Get_Debug_Variables( &CustHeader[30] );
	
	// nbr saved buff
	tmp32 = Acti_CounterBuff;
	CustHeader[58] = (uint8_t) (tmp32 & 0x000000FF);
	CustHeader[59] = (uint8_t) ((tmp32 >> 8) & 0x000000FF);
	CustHeader[60] = (uint8_t) ((tmp32 >> 16) & 0x000000FF);
	CustHeader[61] = (uint8_t) ((tmp32 >> 24) & 0x000000FF);


	/* For futur implementations */
	for(tmp = 62; tmp < 72 ; tmp++)
	{
		CustHeader[tmp] = 0; 
	}

	
}

/*******************************************************************************
* Function Name  : Sd_DeInit_Callback
* Description    : Notify the deinitialization of sd card.
* Input          : No input. 
* Return         : No return value.
*******************************************************************************/
__weak void Sd_DeInit_Callback(void)
{
	/* Redefined in PWR.c */
}

/*

---------------------------Fonctions Non utilis?es -----------------------------------------

*/ 

FIL test_file;

void Write_dev_crypt_test_report(int16_t* pointeur1, int16_t* pointeur2)
{
	FRESULT res;                                          
  uint32_t byteswritten=0; 
	
		if(f_open(&test_file,"devcrypt.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
		{
     
				while(1);
		}
		else
		{
		  res = f_write(&test_file,(const char*) pointeur1, 8208, (void *)&byteswritten);
      
      if((byteswritten == 0) || (res != FR_OK))
      {
        while(1);
      }
			
			else
			{
				HAL_Delay(150);
				//byteswritten = 0;
				res = f_write(&test_file,(const char*) pointeur2, 8208, (void *)&byteswritten);
      
				if((byteswritten == 0) || (res != FR_OK))
				{
					while(1);
				}
				
				else
				{
					f_close(&test_file);
				}
				
			}
		
		
		}

}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */  
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

