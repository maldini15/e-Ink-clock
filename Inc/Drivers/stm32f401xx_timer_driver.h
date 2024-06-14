#ifndef DRIVERS_STM32F401XX_TIMER_DRIVER_H_
#define DRIVERS_STM32F401XX_TIMER_DRIVER_H_

#include "stm32f401xx.h"

typedef struct {
	uint8_t Interrupt_Number;
	uint8_t Interrupt_Port;
	uint8_t Interrupt_RisingEdge;
	uint8_t Interrupt_FallingEdge;
	uint8_t Interrupt_Pending;
	uint16_t Prescaler_Value;
} TIMER_Config_t;

typedef struct {
	TIMER_RegDef_t *pTIMERReg;
	TIMER_Config_t TIMER_Config;
} TIMER_Handle_t;

#define CEN_CR1POS	0
#define UDIS_CR1POS	1
#define URS_CR1POS	2
#define OPM_CR1POS	3
#define DIR_CR1POS	4
#define UG_EGRPOS	0
#define UIF_SRPOS	0

#define pTIMER2				((TIMER_RegDef_t*)TIMER2_BASEADDR)
#define pTIMER3				((TIMER_RegDef_t*)TIMER3_BASEADDR)
#define pTIMER4				((TIMER_RegDef_t*)TIMER4_BASEADDR)
#define pTIMER5				((TIMER_RegDef_t*)TIMER5_BASEADDR)

#define TIMER2_PERI_CLK_ENABLE() 	(pRCC->RCC_APB1ENR |= (1 << 0))
#define TIMER2_PERI_CLK_DISABLE() 	(pRCC->RCC_APB1ENR &= ~(1 << 0))
#define TIMER2_REG_RESET()			do{pRCC->RCC_APB1RSTR |= (1 << 0); pRCC->RCC_APB1RSTR &= ~(1 << 0);}while(0)

#define TIMER3_PERI_CLK_ENABLE() 	(pRCC->RCC_APB1ENR |= (1 << 1))
#define TIMER3_PERI_CLK_DISABLE() 	(pRCC->RCC_APB1ENR &= ~(1 << 1))
#define TIMER3_REG_RESET()			do{pRCC->RCC_APB1RSTR |= (1 << 1); pRCC->RCC_APB1RSTR &= ~(1 << 1);}while(0)

#define TIMER4_PERI_CLK_ENABLE() 	(pRCC->RCC_APB1ENR |= (1 << 2))
#define TIMER4_PERI_CLK_DISABLE() 	(pRCC->RCC_APB1ENR &= ~(1 << 2))
#define TIMER4_REG_RESET()			do{pRCC->RCC_APB1RSTR |= (1 << 2); pRCC->RCC_APB1RSTR &= ~(1 << 2);}while(0)

#define TIMER5_PERI_CLK_ENABLE() 	(pRCC->RCC_APB1ENR |= (1 << 3))
#define TIMER5_PERI_CLK_DISABLE() 	(pRCC->RCC_APB1ENR &= ~(1 << 3))
#define TIMER5_REG_RESET()			do{pRCC->RCC_APB1RSTR |= (1 << 3); pRCC->RCC_APB1RSTR &= ~(1 << 3);}while(0)

void TIMER_Init(const TIMER_Handle_t *pTIMERHandle);
void TIMER_DeInit(const TIMER_RegDef_t *pTIMERPort);
void TIMER_SetMeasurement(TIMER_RegDef_t *pTIMERx, const uint16_t msTick);
uint16_t TIMER_GetRemaining(const TIMER_RegDef_t *pTIMERx);
void TIMER_Delay(TIMER_RegDef_t *pTIMERx, const uint16_t msTick);

#endif /* DRIVERS_STM32F401XX_TIMER_DRIVER_H_ */
