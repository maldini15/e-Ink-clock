#ifndef INC_STM32F401XX_RTC_DRIVER_H_
#define INC_STM32F401XX_RTC_DRIVER_H_

#include "stm32f401xx.h"

typedef struct {
	uint8_t RTC_HourFormat;
	uint8_t RTC_ClkSource;
	uint32_t RTC_ClkFreqHz;
} RTC_PinConfig_t;

typedef struct {
	RTC_RegDef_t *pRTCx;
	RTC_PinConfig_t RTC_Config_t;
} RTC_Handle_t;

typedef struct time {
	uint8_t hour;
	uint8_t minutes;
	uint8_t seconds;
} Time;

typedef struct date {
	uint8_t day;
	uint8_t month;
	uint8_t year;
} Date;

#define RTC_CLK_LSE		1
#define RTC_CLK_LSI		2

#define RTC_AMPM	1
#define RTC_24H		0

#define PM_RTC_TRPOS	22
#define HT_RTC_TRPOS	20
#define HU_RTC_TRPOS	16
#define MNT_RTC_TRPOS	12
#define MNU_RTC_TRPOS	8
#define ST_RTC_TRPOS	4
#define SU_RTC_TRPOS	0

#define YT_RTC_DRPOS	20
#define YU_RTC_DRPOS	16
#define MT_RTC_DRPOS	12
#define MU_RTC_DRPOS	8
#define DT_RTC_DRPOS	4
#define DU_RTC_DRPOS	0
#define WDU_RTC_DRPOS	13

#define PREDIC_S_RTC_PRERPOS	0
#define PREDIC_S_RTC_PRERSIZE	14
#define PREDIC_A_RTC_PRERPOS	16
#define PREDIC_A_RTC_PRERSIZE	6

#define INIT_RTC_ISRPOS		7
#define INITF_RTC_ISRPOS	6

#define DBP_RTC_CRPOS		8
#define FMT_RTC_CRPOS		6

#define COMMAND1_RTC_WPR	0xCAU
#define COMMAND2_RTC_WPR	0x53U

#define pRTC			(RTC_RegDef_t*) RTC_BASEADDR;

#define RTC_PERI_CLK_ENABLE() 	(pRCC->RCC_BDCR |= (1 << RTCEN_RCC_BDCRPOS))
#define RTC_PERI_CLK_DISABLE() 	(pRCC->RCC_BDCR &= ~(1 << RTCEN_RCC_BDCRPOS))
#define RTC_REG_RESET()			do{pRCC->RCC_BDCR |= (1 << BDRST_RCC_BDCRPOS); pRCC->RCC_APB1RSTR &= ~(1 << BDRST_RCC_BDCRPOS);}while(0)

void RTC_Init(const RTC_Handle_t *RtcHandle);
void RTC_DeInit(RTC_RegDef_t *pRTCx);

Time RTC_GetTime(const RTC_RegDef_t *pRTCx);
Date RTC_GetDate(const RTC_RegDef_t *pRTCx);
void RTC_SetTime(RTC_RegDef_t *pRTCx, const Time time);
void RTC_SetDate(RTC_RegDef_t *pRTCx, const Date date);

#endif
