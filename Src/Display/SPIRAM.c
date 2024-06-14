#include "Display/SPIRAM.h"
#include "Drivers/stm32f401xx_gpio_driver.h"
#include "Drivers/stm32f401xx_spi_driver.h"

void SPIRAM_Set_Mode(uint8_t mode) {
	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, DISABLE);

	SPI_SendData(pSPI1, &(uint8_t ) { CMD_WRSR }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { mode }, 1);

	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, ENABLE);
}

uint8_t SPIRAM_RD_Byte(uint32_t Addr) {
	uint8_t RD_Byte;

	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, DISABLE);
	SPI_SendData(pSPI1, &(uint8_t ) { CMD_READ }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 16 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 8 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr }, 1);

	SPI_ReadData(pSPI1, &RD_Byte, 1);
	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, ENABLE);

	return RD_Byte;
}

void SPIRAM_WR_Byte(uint32_t Addr, const uint8_t Data) {
	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, DISABLE);

	SPI_SendData(pSPI1, &(uint8_t ) { CMD_WRITE }, 1);

	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 16 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 8 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr }, 1);

	SPI_SendData(pSPI1, &(uint8_t ) { Data }, 1);
	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, ENABLE);
}

void SPIRAM_RD_Page(uint32_t Addr, uint8_t *pBuf) {
	uint16_t i;

	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, DISABLE);
	SPI_SendData(pSPI1, &(uint8_t ) { CMD_READ }, 1);

	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 16 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 8 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr }, 1);

	for (i = 0; i < 32; i++) {
		SPI_ReadData(pSPI1, pBuf, 1);
		pBuf++;
	}
	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, ENABLE);
}

void SPIRAM_WR_Page(uint32_t Addr, uint8_t *pBuf) {
	uint16_t i;

	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, DISABLE);
	SPI_SendData(pSPI1, &(uint8_t ) { CMD_WRITE }, 1);

	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 16 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 8 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr }, 1);

	for (i = 0; i < 32; i++) {
		SPI_SendData(pSPI1, &*pBuf, 1);
		pBuf++;
	}
	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, ENABLE);
}

void SPIRAM_RD_Stream(uint32_t Addr, uint8_t *pBuf, uint32_t Len) {
	uint16_t i;

	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, DISABLE);
	SPI_SendData(pSPI1, &(uint8_t ) { CMD_READ }, 1);

	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 16 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 8 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr }, 1);

	for (i = 0; i < Len; i++) {
		SPI_ReadData(pSPI1, pBuf, 1);
		pBuf++;
	}
	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, ENABLE);
}

void SPIRAM_WR_Stream(uint32_t Addr, uint8_t *pBuf, uint32_t Len) {
	uint16_t i;

	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, DISABLE);
	SPI_SendData(pSPI1, &(uint8_t ) { CMD_WRITE }, 1);

	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 16 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr >> 8 }, 1);
	SPI_SendData(pSPI1, &(uint8_t ) { Addr }, 1);

	for (i = 0; i < Len; i++) {
		SPI_SendData(pSPI1, &*pBuf, 1);
		pBuf++;
	}
	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, ENABLE);
}
