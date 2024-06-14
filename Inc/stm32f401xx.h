#ifndef INC_STM32F401XX_H_
#define INC_STM32F401XX_H_

#include <stdint.h>

#define ENABLE		1
#define DISABLE 	0
#define SET 		ENABLE
#define RESET 		DISABLE

#define SYSCLKMHZ	16

#define NVIC_SETENABLE_BASEADDR			0xE000E100U
#define NVIC_CLEARENABLE_BASEADDR		0xE000E180U
#define NVIC_SETPENDING_BASEADDR		0xE000E200U
#define NVIC_CLEARPPENDING				0xE000E280U
#define NVIC_ACTIVEBIT_BASEADDR			0xE000E300U
#define NVIC_INTERRUPTPRIORITY			0xE000E400U
#define NVIC_CONTROLLERTYPE_BASEADDR	0xE000E004U

typedef struct {
	volatile uint32_t RCC_CR;
	volatile uint32_t RCC_PLLCFGR;
	volatile uint32_t RCC_CFGR;
	volatile uint32_t RCC_CIR;
	volatile uint32_t RCC_AHB1RSTR;
	volatile uint32_t RCC_AHB2RSTR;
	volatile uint32_t RESERVED[2];
	volatile uint32_t RCC_APB1RSTR;
	volatile uint32_t RCC_APB2RSTR;
	volatile uint32_t RESERVED1[2];
	volatile uint32_t RCC_AHB1ENR;
	volatile uint32_t RCC_AHB2ENR;
	volatile uint32_t RESERVED2[2];
	volatile uint32_t RCC_APB1ENR;
	volatile uint32_t RCC_APB2ENR;
	volatile uint32_t RESERVED3[2];
	volatile uint32_t RCC_AHB1LPENR;
	volatile uint32_t RCC_AHB2LPENR;
	volatile uint32_t RESERVED4[2];
	volatile uint32_t RCC_APB1LPENR;
	volatile uint32_t RCC_APB2LPENR;
	volatile uint32_t RESERVED5[2];
	volatile uint32_t RCC_BDCR;
	volatile uint32_t RCC_CSR;
	volatile uint32_t RESERVED6[2];
	volatile uint32_t RCC_SSCGR;
	volatile uint32_t RCC_PLLI2SCFGR;
	volatile uint32_t RESERVED7;
	volatile uint32_t RCC_DCKCFGR;
} RCC_RegDef_t;

typedef struct {
	volatile uint32_t SPI_CR1;
	volatile uint32_t SPI_CR2;
	volatile uint32_t SPI_SR;
	volatile uint32_t SPI_DR;
	volatile uint32_t SPI_CRCPR;
	volatile uint32_t SPI_RXCRCR;
	volatile uint32_t SPI_TXCRCR;
	volatile uint32_t SPI_I2SCFGR;
	volatile uint32_t SPI_I2SPR;
} SPI_RegDef_t;

typedef struct {
	volatile uint32_t I2C_CR1;
	volatile uint32_t I2C_CR2;
	volatile uint32_t I2C_OAR1;
	volatile uint32_t I2C_OAR2;
	volatile uint32_t I2C_DR;
	volatile uint32_t I2C_SR1;
	volatile uint32_t I2C_SR2;
	volatile uint32_t I2C_CCR;
	volatile uint32_t I2C_TRISE;
	volatile uint32_t I2C_FLTR;
} I2C_RegDef_t;

typedef struct {
	volatile uint32_t TIM_CR1;
	volatile uint32_t TIM_CR2;
	volatile uint32_t TIM_SMCR;
	volatile uint32_t TIM_DIER;
	volatile uint32_t TIM_SR;
	volatile uint32_t TIM_EGR;
	volatile uint32_t TIM_CCMR1;
	volatile uint32_t TIMCCMR2;
	volatile uint32_t TIM_CCER;
	volatile uint32_t TIM_CNT;
	volatile uint32_t TIM_PSC;
	volatile uint32_t TIM_ARR;
	volatile uint32_t RESERVED1;
	volatile uint32_t TIM_CCR1;
	volatile uint32_t TIM_CCR2;
	volatile uint32_t TIM_CCR3;
	volatile uint32_t TIM_CCR4;
	volatile uint32_t RESERVED2;
	volatile uint32_t TIM_DCR;
} TIMER_RegDef_t;

typedef struct {
	volatile uint32_t RTC_TR;
	volatile uint32_t RTC_DR;
	volatile uint32_t RTC_CR;
	volatile uint32_t RTC_ISR;
	volatile uint32_t RTC_PRER;
	volatile uint32_t RTC_WUTR;
	volatile uint32_t RTC_CALIBR;
	volatile uint32_t RTC_ALRMAR;
	volatile uint32_t RTC_ALRMBR;
	volatile uint32_t RTC_WPR;
	volatile uint32_t RTC_SSR;
	volatile uint32_t RESERVED1;
	volatile uint32_t RTC_TSTR;
	volatile uint32_t RESERVED2;
	volatile uint32_t RTC_TSSSR;
	volatile uint32_t RTC_CALR;
	volatile uint32_t RTC_TAFCR;
	volatile uint32_t RTC_ALRMASSR;
	volatile uint32_t RTC_ALRMBSSR;
	volatile uint32_t RESERVED3;
	volatile uint32_t RTC_BKP0R;
} RTC_RegDef_t;

typedef struct {
	volatile uint32_t GPIOx_MODER;
	volatile uint32_t GPIOx_OTYPER;
	volatile uint32_t GPIOx_OSPEEDR;
	volatile uint32_t GPIOx_PUPDR;
	volatile uint32_t GPIOx_IDR;
	volatile uint32_t GPIOx_ODR;
	volatile uint32_t GPIOx_BSRR;
	volatile uint32_t GPIOx_LCKR;
	volatile uint32_t GPIOx_AFR[2];
} GPIO_RegDef_t;

typedef struct {
	volatile uint32_t EXTI_IMR;
	volatile uint32_t EXTI_EMR;
	volatile uint32_t EXTI_RTSR;
	volatile uint32_t EXTI_FTSR;
	volatile uint32_t EXTI_SWIER;
	volatile uint32_t EXTI_PR;
} EXTI_RegDef_t;

typedef struct {
	volatile uint32_t SYSCFG_MEMRMP;
	volatile uint32_t SYSCFG_PMC;
	volatile uint32_t SYSCFG_EXTICR[4];
	volatile uint32_t SYSCFG_CMPCR;
} SYSCFG_RegDef_t;

#define MHZ2HZ	1000000000

#define FLASH_BASEADDR		0x08000000U
#define SRAM_BASEADDR		0x20000000U
#define APB2_BASEADDR		0x40010000U
#define AHB1_BASEADDR		0x40020000U
#define APB1_BASEADDR		0x40000000U
#define AHB2_BASEADDR		0x50000000U
#define ROM_BASEADDR		0x1FFF0000U
#define RCC_BASEADDR		0x40023800U
#define RTC_BASEADDR		0x40002800U
#define EXTI_BASEADDR		0x40013C00U
#define SYSCFG_BASEADDR		0x40013800U
#define pNVICIRQ_BASEADDR	0xE000E100U
#define I2C1_BASEADDR		0x40005400U
#define I2C2_BASEADDR		0x40005800U
#define I2C3_BASEADDR		0x40005C00U
#define SPI1_BASEADDR		0x40013000U
#define SPI2_BASEADDR		0x40003800U
#define SPI3_BASEADDR		0x40003C00U
#define SPI4_BASEADDR		0x40013400U
#define TIMER2_BASEADDR		0x40000000U
#define TIMER3_BASEADDR		(TIMER2_BASEADDR + (0x0400U))
#define TIMER4_BASEADDR		(TIMER3_BASEADDR + (0x0400U))
#define TIMER5_BASEADDR		(TIMER4_BASEADDR + (0x0400U))

#define HPRE_RCC_CFGRPOS		4
#define PPRE1_RCC_CFGRPOS		10
#define RTCEN_RCC_BDCRPOS		15
#define RTCSEL_RCC_BDCRPOS		8
#define RTCSEL_RCC_BDCRSIZE 	2
#define BDRST_RCC_BDCRPOS		16
#define LSION_RCC_CSRPOS		0
#define LSEON_RCC_BDCRPOS		0
#define SPI2RST_RCC_APB1RSTRPOS 14
#define SPI3RST_RCC_APB1RSTRPOS 15
#define SPI1RST_RCC_APB2RSTRPOS 12
#define SPI4RST_RCC_APB2RSTRPOS 13
#define SPI2EN_RCC_APB1ENRPOS	14
#define SPI3EN_RCC_APB1ENRPOS	15
#define SPI1EN_RCC_APB2ENRPOS	12
#define SPI4EN_RCC_APB2ENRPOS	13

#define DBGMCU_APB1_FZ_ADDR 0xE0042008U

#define pSYSCFG				((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)
#define pEXTI				((EXTI_RegDef_t*)EXTI_BASEADDR)

#define pRCC				((RCC_RegDef_t*)RCC_BASEADDR)

#define RCC_SYSCFG_CLK_ENABLE()		(pRCC->RCC_APB2ENR |= (1 << 14))
#define RCC_SYSCFG_CLK_DISABLE()	(pRCC->RCC_APB2ENR &= ~(1 << 14))

#endif
