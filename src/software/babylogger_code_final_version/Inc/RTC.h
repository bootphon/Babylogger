#include "stm32l4xx_hal.h"
//#include "main.h"

#include "stdint.h"



#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */




void RTC_Get_time_date (uint8_t * buff);
void RTC_Set_time(uint8_t* Time_Buff);
void RTC_Set_date(uint8_t* Date_Buff);

void RTC_Set_Time_str(uint8_t* time_str);
void RTC_Init(void);
void RTC_CalendarConfig(void);
void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate);



