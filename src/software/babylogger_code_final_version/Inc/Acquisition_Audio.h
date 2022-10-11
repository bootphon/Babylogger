#include "RTC.h" 
#include "DFSDM.h"



void Audio_Init(void);
void Audio_Stop_Record(void);
void Audio_Set_Header(uint32_t Freq, uint8_t Resolution ,uint8_t NbrChannel);
void Audio_Update_Header(uint32_t CntrBuff);
void Audio_Init_Debug_Variables(void);
void Audio_Init_Svgrd_Buffer(void);
void Audio_Get_Debug_Variables(uint8_t* pDebeugVar);

__weak void HalfSvgrBuffCpltCallback(void);
__weak void SvgrBuffCpltCallback(void);

#define Audio_length 3616
//#define Audio_length 64


