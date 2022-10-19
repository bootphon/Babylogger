#include "stm32l4xx_hal.h"
  


void DFSDM_Start_Record (void);
void DFSDM_Stop_Record (void);	
void DFSDM_Init(void);

__weak void DmaAudioRecHalfCallback (uint8_t Flt);
__weak void DmaAudioRecCpltCallback (uint8_t Flt);

#define DFSDM_User_Oversampling						128
#define DFSDM_User_Clock_Divider					24
#define DFSDM_User_Filter_Order						DFSDM_FILTER_SINC3_ORDER	
#define DFSDM_User_RightBitShift					3

#define SamplingFreq											16000
#define Nbr_Ch														4
#define Res																16
#define BuffSize													256   // Size of the buffer of one channel which is transmited in block

#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

