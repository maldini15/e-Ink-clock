#ifndef INC_STM32F401XX_GPIO_DRIVER_H_
#define INC_STM32F401XX_GPIO_DRIVER_H_

#include "stm32f401xx.h"

typedef struct {
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PortOpType;
	uint8_t GPIO_PinAltFunMode;
} GPIO_PinConfig_t;

typedef struct {
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_Pin_Config;
} GPIO_Handle_t;

#define GPIO_PIN_N_0	0
#define GPIO_PIN_N_1	1
#define GPIO_PIN_N_2	2
#define GPIO_PIN_N_3	3
#define GPIO_PIN_N_4	4
#define GPIO_PIN_N_5	5
#define GPIO_PIN_N_6	6
#define GPIO_PIN_N_7	7
#define GPIO_PIN_N_8	8
#define GPIO_PIN_N_9	9
#define GPIO_PIN_N_10	10
#define GPIO_PIN_N_11	11
#define GPIO_PIN_N_12	12
#define GPIO_PIN_N_13	13
#define GPIO_PIN_N_14	14
#define GPIO_PIN_N_15	15

#define GPIO_MODE_INPUT		0
#define GPIO_MODE_OUTPUT	1
#define GPIO_MODE_ALTERNATE 2
#define GPIO_MODE_ANALOG	3

//interrupts modes
#define GPIO_IT_FT			4
#define GPIO_IT_RT			5
#define GPIO_IT_RFT			6

//OUTPUT TYPES
#define GPIO_OUT_TYPE_PP	0
#define GPIO_OUT_TYPE_OD	1

//SPEED
#define GPIO_SPEED_LOW		0
#define GPIO_SPEED_MEDIUM	1
#define GPIO_SPEED_HIGH		2
#define GPIO_SPEED_VERYHIGH	3

//AUX RESISTANCE
#define GPIO_PULL_DEACT		0
#define GPIO_PULL_UP		1
#define GPIO_PULL_DOWN		2

#define GPIOA_BASEADDR		(AHB1_BASEADDR)
#define GPIOB_BASEADDR		((AHB1_BASEADDR) + (0x0400U))
#define GPIOC_BASEADDR		((AHB1_BASEADDR) + (0x0800U))
#define GPIOD_BASEADDR		((AHB1_BASEADDR) + (0x0C00U))
#define GPIOE_BASEADDR		((AHB1_BASEADDR) + (0x1000U))
#define GPIOH_BASEADDR		((AHB1_BASEADDR) + (0x1C00U))

#define pGPIOA				((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define pGPIOB				((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define pGPIOC				((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define pGPIOD				((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define pGPIOE				((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define pGPIOF				((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define pGPIOH				((GPIO_RegDef_t*)GPIOH_BASEADDR)

#define GPIOA_PERI_CLK_ENABLE() 	(pRCC->RCC_AHB1ENR |= (1 << 0))
#define GPIOA_PERI_CLK_DISABLE() 	(pRCC->RCC_AHB1ENR &= ~(1 << 0))
#define GPIOA_REG_RESET()			do{pRCC->RCC_AHB1RSTR |= (1 << 0); pRCC->RCC_AHB1RSTR &= ~(1 << 0);}while(0)

#define GPIOB_PERI_CLK_ENABLE() 	(pRCC->RCC_AHB1ENR |= (1 << 1))
#define GPIOB_PERI_CLK_DISABLE() 	(pRCC->RCC_AHB1ENR &= ~(1 << 1))
#define GPIOB_REG_RESET()			do{pRCC->RCC_AHB1RSTR |= (1 << 1); pRCC->RCC_AHB1RSTR &= ~(1 << 1);}while(0)

#define GPIOC_PERI_CLK_ENABLE() 	(pRCC->RCC_AHB1ENR |= (1 << 2))
#define GPIOC_PERI_CLK_DISABLE() 	(pRCC->RCC_AHB1ENR &= ~(1 << 2))
#define GPIOC_REG_RESET()			do{pRCC->RCC_AHB1RSTR |= (1 << 2); pRCC->RCC_AHB1RSTR &= ~(1 << 2);}while(0)

#define GPIOD_PERI_CLK_ENABLE() 	(pRCC->RCC_AHB1ENR |= (1 << 3))
#define GPIOD_PERI_CLK_DISABLE() 	(pRCC->RCC_AHB1ENR &= ~(1 << 3))
#define GPIOD_REG_RESET()			do{pRCC->RCC_AHB1RSTR |= (1 << 3); pRCC->RCC_AHB1RSTR &= ~(1 << 3);}while(0)

#define GPIOE_PERI_CLK_ENABLE() 	(pRCC->RCC_AHB1ENR |= (1 << 4))
#define GPIOE_PERI_CLK_DISABLE() 	(pRCC->RCC_AHB1ENR &= ~(1 << 4))
#define GPIOE_REG_RESET()			do{pRCC->RCC_AHB1RSTR |= (1 << 4); pRCC->RCC_AHB1RSTR &= ~(1 << 4);}while(0)

#define GPIOH_PERI_CLK_ENABLE() 	(pRCC->RCC_AHB1ENR |= (1 << 7))
#define GPIOH_PERI_CLK_DISABLE() 	(pRCC->RCC_AHB1ENR &= ~(1 << 7))
#define GPIOH_REG_RESET()			do{pRCC->RCC_AHB1RSTR |= (1 << 7); pRCC->RCC_AHB1RSTR &= ~(1 << 7);}while(0)

void GPIO_Init(const GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(const GPIO_RegDef_t *pGPIOPort);

uint8_t GPIO_ReadFromInputPin(const GPIO_RegDef_t *pGPIOx,
		const uint8_t pinNumber);
uint16_t GPIO_ReadFromInputPort(const GPIO_RegDef_t *pGPIOxconst);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, const uint8_t pinNumber,
		const uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, const uint16_t value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, const uint8_t pinNumber);

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t enable);
void GPIO_IRQHandling(uint8_t pinNumber);

#endif
