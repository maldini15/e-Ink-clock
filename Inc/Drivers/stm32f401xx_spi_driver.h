#ifndef INC_STM32F401XX_SPI_DRIVER_H_
#define INC_STM32F401XX_SPI_DRIVER_H_

#include "stm32f401xx.h"

typedef struct {
	uint8_t speed;
	uint8_t is_master;
	uint8_t mode;
	uint8_t dataframeformat;
	uint8_t frameformat;
	uint8_t swslaveselect;
	uint8_t crc_en;
	uint8_t cpha;
	uint8_t cpol;
} SpiSettings_t;

typedef struct {
	SpiSettings_t SpiSettings;
	SPI_RegDef_t *pSPI;
} SPI_Handle_t;

#define SPI_CRC_DISABLE 0
#define SPI_CRC_ENABLE 1

#define SPI_IS_SLAVE 0
#define SPI_IS_MASTER 1

#define SPI_MODE_FD 0
#define SPI_MODE_HD 1
#define SPI_MODE_RXONLY 1

#define SPI_SPEED2 0
#define SPI_SPEED4 1
#define SPI_SPEED8 2
#define SPI_SPEED16 3
#define SPI_SPEED32 4
#define SPI_SPEED64 5
#define SPI_SPEED128 6
#define SPI_SPEED256 7

#define SPI_SWSLAVEDISABLED 0
#define SPI_SWSLAVEENABLED 1

#define DFF_8B 0
#define DFF_16B 1

#define MSBFIRST 0
#define LSBFIRST 1

#define CK0WHENIDLE 0
#define CK1WHENIDLE 1

#define FIRSTCLKFIRSTCAPTURE 0
#define FIRSTCLKSECCAPTURE 1

#define CPHA_CR1POS 0
#define CPOL_CR1POS 1
#define ISMASTER_CR1POS 2
#define BAUD_CR1POS 3
#define SPIENABLE_CR1POS 6
#define LSBFIRST_CR1POS 7
#define SSM_CR1POS 9
#define SSI_CR1POS 8
#define RXONLY_CR1POS 10
#define DFF_CR1POS 11
#define CRCENABLE_CR1POS 13
#define BIDIOE_CR1POS 14
#define BIDIMODE_CR1POS 15

#define SPI_TXE_FLAG_NUM 1
#define SPI_RXNE_FLAG_NUM 0
#define SPI_BUSY_FLAG_NUM 7

#define pSPI1				(SPI_RegDef_t*) SPI1_BASEADDR
#define pSPI2				(SPI_RegDef_t*) SPI2_BASEADDR
#define pSPI3				(SPI_RegDef_t*) SPI3_BASEADDR
#define pSPI4				(SPI_RegDef_t*) SPI4_BASEADDR

#define SPI1_PERI_CLK_ENABLE()		(pRCC->RCC_APB2ENR |= (ENABLE << SPI1EN_RCC_APB2ENRPOS))
#define SPI1_PERI_CLK_DISABLE() 	(pRCC->RCC_APB2ENR &= ~(ENABLE << SPI1EN_RCC_APB2ENRPOS))
#define SPI1_REG_RESET()			do{pRCC->RCC_APB2RSTR |= (ENABLE << SPI2RST_RCC_APB1RSTRPOS); pRCC->RCC_APB2RSTR &= ~(ENABLE << SPI2RST_RCC_APB1RSTRPOS);}while(0)

#define SPI2_PERI_CLK_ENABLE() 		(pRCC->RCC_APB1ENR |= (ENABLE << SPI2EN_RCC_APB1ENRPOS))
#define SPI2_PERI_CLK_DISABLE() 	(pRCC->RCC_APB1ENR &= ~(ENABLE << SPI2EN_RCC_APB1ENRPOS))
#define SPI2_REG_RESET()			do{pRCC->RCC_APB1RSTR |= (ENABLE << SPI2RST_RCC_APB1RSTRPOS); pRCC->RCC_APB1RSTR &= ~(ENABLE << SPI2RST_RCC_APB1RSTRPOS);}while(0)

#define SPI3_PERI_CLK_ENABLE() 		(pRCC->RCC_APB1ENR |= (ENABLE << SPI3EN_RCC_APB1ENRPOS))
#define SPI3_PERI_CLK_DISABLE() 	(pRCC->RCC_APB1ENR &= ~(ENABLE << SPI3EN_RCC_APB1ENRPOS))
#define SPI3_REG_RESET()			do{pRCC->RCC_APB1RSTR |= (ENABLE << SPI3RST_RCC_APB1RSTRPOS); pRCC->RCC_APB1RSTR &= ~(ENABLE << SPI3RST_RCC_APB1RSTRPOS);}while(0)

#define SPI4_PERI_CLK_ENABLE() 		(pRCC->RCC_APB2ENR |= (ENABLE << SPI4EN_RCC_APB2ENRPOS))
#define SPI4_PERI_CLK_DISABLE() 	(pRCC->RCC_APB2ENR &= ~(ENABLE << SPI4EN_RCC_APB2ENRPOS))
#define SPI4_REG_RESET()			do{pRCC->RCC_APB2RSTR |= (ENABLE << SPI4RST_RCC_APB2RSTRPOS); pRCC->RCC_APB2RSTR &= ~(ENABLE << SPI4RST_RCC_APB2RSTRPOS);}while(0)

void SPI_Init(const SPI_Handle_t *spiHandle);

void SPI_DeInit(const SPI_Handle_t *spiHandle);

uint8_t SPI_GetStatusFlag(const SPI_RegDef_t *pSPI, const uint8_t flagNumber);

void SPI_ReadData(const SPI_RegDef_t *pSPIx, uint8_t *dataBuffer,
		const uint8_t length);

void SPI_SendData(SPI_RegDef_t *pSPIx, const uint8_t *dataBuffer,
		uint8_t length);

#endif
