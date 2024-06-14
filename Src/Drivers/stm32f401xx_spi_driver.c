#include "Drivers/stm32f401xx_spi_driver.h"
#include "Drivers/stm32f401xx_gpio_driver.h"

static void SPI_Clock_Control(const SPI_RegDef_t *pSPIx, const uint8_t enable);
static void SPI_EnableGPIOPins(const SPI_RegDef_t *pSPIx);
static void SPI_DisableGPIOPins(const SPI_RegDef_t *pSPIx);

void SPI_Init(const SPI_Handle_t *spiHandle) {

	SPI_Clock_Control(spiHandle->pSPI, ENABLE);

	uint32_t regCR1Temp = spiHandle->pSPI->SPI_CR1;

	regCR1Temp |= (spiHandle->SpiSettings.is_master << ISMASTER_CR1POS);
	regCR1Temp |= (spiHandle->SpiSettings.speed << BAUD_CR1POS);
	regCR1Temp |= (spiHandle->SpiSettings.crc_en << CRCENABLE_CR1POS);
	regCR1Temp |= (spiHandle->SpiSettings.dataframeformat << DFF_CR1POS);
	regCR1Temp |= (spiHandle->SpiSettings.frameformat << LSBFIRST_CR1POS);
	regCR1Temp |= (spiHandle->SpiSettings.cpha << CPHA_CR1POS);
	regCR1Temp |= (spiHandle->SpiSettings.cpol << CPOL_CR1POS);
	regCR1Temp |= (spiHandle->SpiSettings.swslaveselect << SSM_CR1POS);

	//TODO REMOVE ASAP
	regCR1Temp &= ~(spiHandle->SpiSettings.swslaveselect << SSI_CR1POS);

	if (spiHandle->SpiSettings.mode == SPI_MODE_FD) {
		regCR1Temp &= ~(ENABLE << BIDIMODE_CR1POS);
		regCR1Temp &= ~(ENABLE << RXONLY_CR1POS);
	} else if (spiHandle->SpiSettings.mode == SPI_MODE_HD) {
		regCR1Temp |= (ENABLE << BIDIMODE_CR1POS);
		regCR1Temp |= (ENABLE << BIDIOE_CR1POS);
	} else if (spiHandle->SpiSettings.mode == SPI_MODE_RXONLY) {
		regCR1Temp &= ~(ENABLE << BIDIMODE_CR1POS);
		regCR1Temp |= (ENABLE << RXONLY_CR1POS);
	}

	SPI_EnableGPIOPins(spiHandle->pSPI);

	spiHandle->pSPI->SPI_CR2 |= (1 << 4);
	spiHandle->pSPI->SPI_CR1 = regCR1Temp;

	spiHandle->pSPI->SPI_CR1 |= (ENABLE << SPIENABLE_CR1POS);

}

void SPI_DeInit(const SPI_Handle_t *spiHandle) {

	spiHandle->pSPI->SPI_CR1 &= ~(ENABLE << SPIENABLE_CR1POS);

	if ( pSPI1 == spiHandle->pSPI)
		SPI1_REG_RESET();
	else if ( pSPI2 == spiHandle->pSPI)
		SPI2_REG_RESET();
	else if ( pSPI3 == spiHandle->pSPI)
		SPI3_REG_RESET();
	else if ( pSPI4 == spiHandle->pSPI)
		SPI4_REG_RESET();
}

static void SPI_Clock_Control(const SPI_RegDef_t *pSPIx, const uint8_t enable) {

	if (enable == ENABLE) {
		if ( pSPI1 == pSPIx)
			SPI1_PERI_CLK_ENABLE();
		else if ( pSPI2 == pSPIx)
			SPI2_PERI_CLK_ENABLE();
		else if ( pSPI3 == pSPIx)
			SPI3_PERI_CLK_ENABLE();
		else if ( pSPI4 == pSPIx)
			SPI4_PERI_CLK_ENABLE();

	} else {
		if ( pSPI1 == pSPIx)
			SPI1_PERI_CLK_DISABLE();
		else if ( pSPI2 == pSPIx)
			SPI2_PERI_CLK_DISABLE();
		else if ( pSPI3 == pSPIx)
			SPI3_PERI_CLK_DISABLE();
		else if ( pSPI4 == pSPIx)
			SPI4_PERI_CLK_DISABLE();
	}
}

static void SPI_EnableGPIOPins(const SPI_RegDef_t *pSPIx) {

	if ( pSPI1 == pSPIx) {
		GPIO_Handle_t pGPIOA_handler;

		pGPIOA_handler.pGPIOx = pGPIOA;
		pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_5;
		pGPIOA_handler.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_ALTERNATE;
		pGPIOA_handler.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_VERYHIGH;
		pGPIOA_handler.GPIO_Pin_Config.GPIO_PinPuPdControl =
		GPIO_PULL_DEACT;
		pGPIOA_handler.GPIO_Pin_Config.GPIO_PinAltFunMode = 0x05;
		pGPIOA_handler.GPIO_Pin_Config.GPIO_PortOpType = GPIO_OUT_TYPE_PP;

		GPIO_Init(&pGPIOA_handler);

		//pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_4;
		//GPIO_Init(&pGPIOA_handler);

		pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_6;
		GPIO_Init(&pGPIOA_handler);

		pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_7;
		GPIO_Init(&pGPIOA_handler);

	} else if ( pSPI2 == pSPIx) {
		GPIO_Handle_t pGPIOB_handler;

		pGPIOB_handler.pGPIOx = pGPIOB;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_ALTERNATE;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinPuPdControl =
		GPIO_PULL_DEACT;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PortOpType = GPIO_OUT_TYPE_PP;

		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_15;
		GPIO_Init(&pGPIOB_handler);

		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_14;
		GPIO_Init(&pGPIOB_handler);

		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_13;
		GPIO_Init(&pGPIOB_handler);

		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_12;
		GPIO_Init(&pGPIOB_handler);

	} else if ( pSPI3 == pSPIx)
		SPI3_PERI_CLK_ENABLE();
	else if ( pSPI4 == pSPIx)
		SPI4_PERI_CLK_ENABLE();
}

//TODO shjould not disable peripheral
static void SPI_DisableGPIOPins(const SPI_RegDef_t *pSPIx) {

	if ( pSPI1 == pSPIx)
		GPIO_DeInit(pGPIOA);
	else if ( pSPI2 == pSPIx)
		GPIO_DeInit(pGPIOB);
	else if ( pSPI3 == pSPIx)
		SPI4_PERI_CLK_ENABLE();
	else if ( pSPI4 == pSPIx)
		SPI4_PERI_CLK_ENABLE();
}

uint8_t SPI_GetStatusFlag(const SPI_RegDef_t *pSPI, const uint8_t flagNumber) {

	return ((pSPI->SPI_SR & (1 << flagNumber)) != DISABLE);

}

void SPI_SendData(SPI_RegDef_t *pSPI, const uint8_t *dataBuffer, uint8_t length) {

	while (length > 0) {
		while (!SPI_GetStatusFlag(pSPI, SPI_TXE_FLAG_NUM))
			;
		if ((pSPI->SPI_CR1 & (ENABLE << DFF_CR1POS)) == DFF_8B) {
			pSPI->SPI_DR = *dataBuffer;
			dataBuffer++;
			length--;
		} else {
			pSPI->SPI_DR = *((uint16_t*) dataBuffer);
			dataBuffer += 2;
			length -= 2;
		}
	}
}

void SPI_ReadData(const SPI_RegDef_t *pSPI, uint8_t *dataBuffer, uint8_t length) {

	while (length > 0) {
		while (!SPI_GetStatusFlag(pSPI, SPI_RXNE_FLAG_NUM))
			;
		if ((pSPI->SPI_CR1 & (ENABLE << DFF_CR1POS)) == DFF_8B) {
			*dataBuffer = (uint8_t) pSPI->SPI_DR;
			dataBuffer++;
			length--;
		} else {
			*dataBuffer = (uint16_t) pSPI->SPI_DR;
			dataBuffer += 2;
			length -= 2;
		}
	}
}
