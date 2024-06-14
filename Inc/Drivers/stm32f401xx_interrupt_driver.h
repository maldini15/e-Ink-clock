#ifndef INC_STM32F401XX_INTERRUPT_DRIVER_H_
#define INC_STM32F401XX_INTERRUPT_DRIVER_H_

#include "stm32f401xx.h"


typedef struct{
	uint8_t Interrupt_Number;
	uint8_t Interrupt_Port;
	uint8_t Interrupt_RisingEdge;
	uint8_t Interrupt_FallingEdge;
	uint8_t Interrupt_Pending;
}Interrupt_PinConfig_t;


typedef struct{
	SYSCFG_RegDef_t* pSYSCFGReg;
	EXTI_RegDef_t* pEXTIReg;
	Interrupt_PinConfig_t Interrupt_Pin_Config;
}Interrupt_Handle_t;

#define PA 0
#define PB 1
#define PC 2
#define PD 3
#define PE 4
#define PH 7

#define EXTI0_IRQ_N			6
#define EXTI1_IRQ_N			7
#define EXTI2_IRQ_N			8
#define EXTI3_IRQ_N			9
#define EXTI4_IRQ_N			10
#define EXTI9_5_IRQ_N		23
#define EXTI15_10_IRQ_N		40


void NVIC_EnableInterrupt(uint8_t portNumber);
void Interrupt_Enable(Interrupt_Handle_t *pInteruptHandle);
void Interrupt_Disable(Interrupt_Handle_t *pInteruptHandle);
void Interrupt_Clear_Pending(Interrupt_Handle_t *pInteruptHandle);

#endif
