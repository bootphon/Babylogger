/**********************
* Team : Coml
**********************/

#include "Acquisition_Audio.h"
#include "fatfs.h"

/* Private functions -----------------------------------------------------*/

/*Declaration variables --------------------------------------------------*/
int32_t                      RecBuff0[2*BuffSize];
int32_t                      RecBuff1[2*BuffSize];
int32_t                      RecBuff2[2*BuffSize];
int32_t                      RecBuff3[2*BuffSize];

uint8_t                     DmaRecFlt0HalfBuffCplt  = 0;
uint8_t                     DmaRecFlt1HalfBuffCplt  = 0;
uint8_t                     DmaRecFlt2HalfBuffCplt  = 0;
uint8_t                     DmaRecFlt3HalfBuffCplt  = 0;

uint8_t                     DmaRecFlt0BuffCplt      = 0;
uint8_t                     DmaRecFlt1BuffCplt      = 0;
uint8_t                     DmaRecFlt2BuffCplt      = 0;
uint8_t                     DmaRecFlt3BuffCplt      = 0;


int16_t                      SvgrdBuff[(4*8*BuffSize)+16];
int16_t*                     pSvgrdHalfBuff = &SvgrdBuff[0];
int16_t*                     pSvgrdCpltBuff = &SvgrdBuff[(16*BuffSize)+8];

uint8_t 		     WavHeader[44];
uint8_t*		     pWavHeader = &WavHeader[0];

uint8_t			     RTC_Buff[7] = {0};


/*Process variables --------------------------------------------------*/


	
/* Externs variables ---------------------------------------------------------*/




// debeug variables ---------------------------------------------------------/

uint32_t hx1 												 = 0;
uint32_t hx2 												 = 0;
uint32_t hx3 												 = 0;
uint32_t hx4 												 = 0;
uint32_t hy												 = 0;

uint32_t cx1												 = 0;
uint32_t cx2												 = 0;
uint32_t cx3												 = 0;
uint32_t cx4												 = 0;
uint32_t cy												 = 0;


// special variable 
uint32_t 										Buff_index; 


void Audio_Init(void)
{
	DFSDM_Init();
	Audio_Set_Header(SamplingFreq,Res,Nbr_Ch);
	DFSDM_Start_Record();
}

void Audio_Stop_Record(void)
{
	DFSDM_Stop_Record();
}


void DmaAudioRecHalfCallback (uint8_t Flt)
{
  uint16_t i = 0;
	
	if ( Flt == 0 )
	{
		DmaRecFlt0HalfBuffCplt = 1;
		hx1++;
	}
	
		if ( Flt == 1 )
	{
		DmaRecFlt1HalfBuffCplt = 1;
		hx2++;
	}
		if ( Flt == 2 )
	{
		DmaRecFlt2HalfBuffCplt = 1;
		hx3++;
	}
	
		if ( Flt == 3 )
	{
		DmaRecFlt3HalfBuffCplt = 1;
		hx4++;
	}
	
	if ( (DmaRecFlt0HalfBuffCplt == 1) && (DmaRecFlt1HalfBuffCplt == 1)
		 && (DmaRecFlt2HalfBuffCplt == 1) && (DmaRecFlt3HalfBuffCplt == 1))
	{
    
		DmaRecFlt0HalfBuffCplt = DmaRecFlt1HalfBuffCplt = DmaRecFlt2HalfBuffCplt = DmaRecFlt3HalfBuffCplt = 0;
				
		//RTC_Get_time((uint8_t*)&SvgrdBuff[Buff_index]);
		RTC_Get_time_date(&RTC_Buff[0]);
		SvgrdBuff[Buff_index]= ((0x0000 | (RTC_Buff[1]<<8)) | ((int16_t) RTC_Buff[0]));
		SvgrdBuff[Buff_index+1]= ((0x0000 | (RTC_Buff[3]<<8)) | ((int16_t) RTC_Buff[2]));
		Buff_index +=2;
		
		for(i = 0; i < BuffSize; i++)
    {
			SvgrdBuff[Buff_index]     = SaturaLH((RecBuff0[i] >> 8 ), -32768, 32767);  
			SvgrdBuff[Buff_index+1] 	= SaturaLH((RecBuff1[i] >> 8 ), -32768, 32767); 
			SvgrdBuff[Buff_index+2]   = SaturaLH((RecBuff2[i] >> 8 ), -32768, 32767);  
			SvgrdBuff[Buff_index+3] 	= SaturaLH((RecBuff3[i] >> 8 ), -32768, 32767); 			
			Buff_index +=4; 
		}
		
		
		
		if (Buff_index == ((16 * BuffSize)+8))
		{
			hy++;
			HalfSvgrBuffCpltCallback();
		}
		if (Buff_index == ((32 * BuffSize)+16))
		{
			cy++;
			SvgrBuffCpltCallback();
			Buff_index=0;
		}
	}
}


void DmaAudioRecCpltCallback (uint8_t Flt)
{
	uint16_t i = 0;

	
	if ( Flt == 0 )
	{
		DmaRecFlt0BuffCplt = 1;
		cx1++;
	}
	
	if ( Flt == 1 )
	{
		DmaRecFlt1BuffCplt = 1;
		cx2++;
	}
	
	if ( Flt == 2 )
	{
		DmaRecFlt2BuffCplt = 1;
		cx3++;
	}
	
		if ( Flt == 3 )
	{
		DmaRecFlt3BuffCplt = 1;
		cx4++;
	}
	
	if ( (DmaRecFlt0BuffCplt == 1) && (DmaRecFlt1BuffCplt == 1)
		&& (DmaRecFlt2BuffCplt == 1) && (DmaRecFlt3BuffCplt == 1))
	{		

		DmaRecFlt0BuffCplt = DmaRecFlt1BuffCplt = DmaRecFlt2BuffCplt = DmaRecFlt3BuffCplt = 0;

	        //RTC_Get_time((uint8_t*)&SvgrdBuff[Buff_index]);
		RTC_Get_time_date(&RTC_Buff[0]);
		SvgrdBuff[Buff_index]  = ((0x0000 | (RTC_Buff[1]<<8)) | ((int16_t) RTC_Buff[0]));
		SvgrdBuff[Buff_index+1]= ((0x0000 | (RTC_Buff[3]<<8)) | ((int16_t) RTC_Buff[2]));
		Buff_index +=2;
		
		for(i = BuffSize; i < (2*BuffSize); i++)
		{
			SvgrdBuff[Buff_index]     = SaturaLH((RecBuff0[i] >> 8 ), -32768, 32767);  
			SvgrdBuff[Buff_index+1] 	= SaturaLH((RecBuff1[i] >> 8 ), -32768, 32767);  
			SvgrdBuff[Buff_index+2]   = SaturaLH((RecBuff2[i] >> 8 ), -32768, 32767);  
			SvgrdBuff[Buff_index+3] 	= SaturaLH((RecBuff3[i] >> 8 ), -32768, 32767);
			Buff_index += 4;
		}
		 
		
		if (Buff_index == ((16 * BuffSize)+8))
		{
			hy++;
			HalfSvgrBuffCpltCallback();
		}
		if (Buff_index == ((32 * BuffSize)+16))
		{
			cy++;
			SvgrBuffCpltCallback();
			Buff_index=0;
		}
	
	}
	
}





void Audio_Set_Header(uint32_t Freq, uint8_t Resolution ,uint8_t NbrChannel)
{
  uint32_t ByteRate = ( Freq * Resolution ) / 2;
	
  /* Write chunkID, must be 'RIFF'  ------------------------------------------*/
  WavHeader[0] = 'R';
  WavHeader[1] = 'I';
  WavHeader[2] = 'F';
  WavHeader[3] = 'F';
  
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the 
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  WavHeader[4] = 0x00;
  WavHeader[5] = 0x4C;
  WavHeader[6] = 0x1D;
  WavHeader[7] = 0x00;
  /* Write the file format, must be 'WAVE' -----------------------------------*/
  WavHeader[8]  = 'W';
  WavHeader[9]  = 'A';
  WavHeader[10] = 'V';
  WavHeader[11] = 'E';
  
  /* Write the format chunk, must be'fmt ' -----------------------------------*/
  WavHeader[12]  = 'f';
  WavHeader[13]  = 'm';
  WavHeader[14]  = 't';
  WavHeader[15]  = ' ';
  
  /* Write the length of the 'fmt' data, must be 0x10 ------------------------*/
  WavHeader[16]  = 0x10;
  WavHeader[17]  = 0x00;
  WavHeader[18]  = 0x00;
  WavHeader[19]  = 0x00;
  
  /* Write the audio format, must be 0x01 (PCM) ------------------------------*/
  WavHeader[20]  = 0x01;
  WavHeader[21]  = 0x00;
  
  /* Write the number of channels, ie. 0x01 (Mono) ---------------------------*/
  WavHeader[22]  = 0x04;
  WavHeader[23]  = 0x00;
  
  /* Write the Sample Rate in Hz ---------------------------------------------*/
  /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
  WavHeader[24]  = (uint8_t)((Freq & 0xFF));
  WavHeader[25]  = (uint8_t)((Freq >> 8) & 0xFF);
  WavHeader[26]  = (uint8_t)((Freq >> 16) & 0xFF);
  WavHeader[27]  = (uint8_t)((Freq >> 24) & 0xFF);
  
  /* Write the Byte Rate -----------------------------------------------------*/
  WavHeader[28]  = (uint8_t)((ByteRate & 0xFF));
  WavHeader[29]  = (uint8_t)((ByteRate >> 8) & 0xFF);
  WavHeader[30]  = (uint8_t)((ByteRate >> 16) & 0xFF);
  WavHeader[31]  = (uint8_t)((ByteRate >> 24) & 0xFF);
  
  /* Write the block alignment -----------------------------------------------*/
  WavHeader[32]  = 0x08;
  WavHeader[33]  = 0x00;
  
  /* Write the number of bits per sample -------------------------------------*/
  WavHeader[34]  = 0x10;
  WavHeader[35]  = 0x00;
  
  /* Write the Data chunk, must be 'data' ------------------------------------*/
  WavHeader[36]  = 'd';
  WavHeader[37]  = 'a';
  WavHeader[38]  = 't';
  WavHeader[39]  = 'a';
  
  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  WavHeader[40]  = 0x00;
  WavHeader[41]  = 0x4C;
  WavHeader[42]  = 0x1D;
  WavHeader[43]  = 0x00;
  
}
void Audio_Update_Header (uint32_t CntrBuff)
{
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the 
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  
  uint32_t tmp = (CntrBuff * 4 * (16/8) * 4* BuffSize)+36;
	
  WavHeader[4] = (uint8_t)(tmp);
  WavHeader[5] = (uint8_t)(tmp >> 8);
  WavHeader[6] = (uint8_t)(tmp >> 16);
  WavHeader[7] = (uint8_t)(tmp >> 24);
  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  tmp -=36;
  WavHeader[40] = (uint8_t)(tmp); 
  WavHeader[41] = (uint8_t)(tmp >> 8);
  WavHeader[42] = (uint8_t)(tmp >> 16);
  WavHeader[43] = (uint8_t)(tmp >> 24); 
  
}


void Audio_Init_Debug_Variables(void)	
{
	hx1=hx2=hx3=hx4=hy=cx1=cx2=cx3=cx4=cy=0;
}

void Audio_Init_Svgrd_Buffer(void)
{
	Buff_index = 0; 
} 

void Audio_Get_Debug_Variables(uint8_t* pDebeugVar)
{
	
	pDebeugVar[0]  = (uint8_t) hx1;
	pDebeugVar[1]  = (uint8_t) ((hx1 >> 8)  & 0x000000FF);
	pDebeugVar[2]  = (uint8_t) ((hx1 >> 16) & 0x000000FF);
	pDebeugVar[3]  = (uint8_t) ((hx1 >> 24) & 0x000000FF);
	
	pDebeugVar[4]  = (uint8_t)hx2;
	pDebeugVar[5]  = (uint8_t) ((hx2 >> 8)  & 0x000000FF);
	pDebeugVar[6]  = (uint8_t) ((hx2 >> 16) & 0x000000FF);
	pDebeugVar[7]  = (uint8_t) ((hx2 >> 24) & 0x000000FF);
	
	pDebeugVar[8]  = (uint8_t)hx3;
	pDebeugVar[9]  = (uint8_t) ((hx3 >> 8)  & 0x000000FF);
	pDebeugVar[10] = (uint8_t) ((hx3 >> 16) & 0x000000FF);
	pDebeugVar[11] = (uint8_t) ((hx3 >> 24) & 0x000000FF);
	
	pDebeugVar[12] = (uint8_t)hx4;
	pDebeugVar[13] = (uint8_t) ((hx4 >> 8)  & 0x000000FF);
	pDebeugVar[14] = (uint8_t) ((hx4 >> 16) & 0x000000FF);
	pDebeugVar[15] = (uint8_t) ((hx4 >> 24) & 0x000000FF);
	
	
	pDebeugVar[16] = (uint8_t)cx1;
	pDebeugVar[17] = (uint8_t) ((cx1 >> 8)  & 0x000000FF);
	pDebeugVar[18] = (uint8_t) ((cx1 >> 16) & 0x000000FF);
	pDebeugVar[19] = (uint8_t) ((cx1 >> 24) & 0x000000FF);
	
	
	pDebeugVar[20] = (uint8_t)cx2;
	pDebeugVar[21] = (uint8_t) ((cx2 >> 8)  & 0x000000FF);
	pDebeugVar[22] = (uint8_t) ((cx2 >> 16) & 0x000000FF);
	pDebeugVar[23] = (uint8_t) ((cx2 >> 24) & 0x000000FF);
	
	pDebeugVar[24] = (uint8_t)cx3;
	pDebeugVar[25] = (uint8_t) ((cx3 >> 8)  & 0x000000FF);
	pDebeugVar[26] = (uint8_t) ((cx3 >> 16) & 0x000000FF);
	pDebeugVar[27] = (uint8_t) ((cx3 >> 24) & 0x000000FF);
	
	pDebeugVar[28] = (uint8_t)cx4;
	pDebeugVar[29] = (uint8_t) ((cx4 >> 8)  & 0x000000FF);
	pDebeugVar[30] = (uint8_t) ((cx4 >> 16) & 0x000000FF);
	pDebeugVar[31] = (uint8_t) ((cx4 >> 24) & 0x000000FF);	
	
	pDebeugVar[32] = (uint8_t)hy;
	pDebeugVar[33] = (uint8_t) ((hy >> 8)  & 0x000000FF);
	pDebeugVar[34] = (uint8_t) ((hy >> 16) & 0x000000FF);
	pDebeugVar[35] = (uint8_t) ((hy >> 24) & 0x000000FF);
	
	pDebeugVar[36] = (uint8_t)cy;
	pDebeugVar[37] = (uint8_t) ((cy >> 8)  & 0x000000FF);
	pDebeugVar[38] = (uint8_t) ((cy >> 16) & 0x000000FF);
	pDebeugVar[39] = (uint8_t) ((cy >> 24) & 0x000000FF);
	
}
__weak void HalfSvgrBuffCpltCallback(void)
{
	
}
__weak void SvgrBuffCpltCallback(void)
{
	
}

