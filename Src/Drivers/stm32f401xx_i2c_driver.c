#include "Drivers/stm32f401xx_i2c_driver.h"
#include "Drivers/stm32f401xx_gpio_driver.h"

static void I2C_EnablePins(const I2C_RegDef_t *pI2Cx);
static void I2C_Clock_Control(const I2C_RegDef_t *pI2Cx, const uint8_t enable);
static uint8_t I2C_GetStatusReg1Flag(const I2C_RegDef_t *pI2Cx,
		const uint8_t flagNumber);
static uint8_t I2C_GetStatusReg2Flag(const I2C_RegDef_t *pI2Cx,
		const uint8_t flagNumber);

uint32_t getAPBClkFreqKhz() {

	const uint8_t apbPrescaler = (pRCC->RCC_CFGR >> PPRE1_RCC_CFGRPOS) & 0x07;
	const uint8_t ahbPrescaler = (pRCC->RCC_CFGR >> HPRE_RCC_CFGRPOS) & 0x0f;

	const uint32_t apbPrescalerValue =
			(apbPrescaler < 0x08) ? 1 : 1 << (apbPrescaler - 0x08);
	const uint32_t ahbPrescalerValue =
			(ahbPrescaler < 0x04) ? 1 : 1 << (ahbPrescaler - 0x04);

	return ((SYSCLKMHZ / ahbPrescalerValue) / apbPrescalerValue) * 1000;
}

void I2C_Init(const I2C_Handle_t *i2cHandle) {

	I2C_Clock_Control(i2cHandle->pI2Cx, ENABLE);

	uint32_t regCR2Temp = i2cHandle->pI2Cx->I2C_CR2;

	regCR2Temp &= ~(0x3F << FREQ_CR2POS);
	regCR2Temp |= ((getAPBClkFreqKhz() / 1000) << FREQ_CR2POS);

	uint32_t regCCRTemp = 0U;
	if (i2cHandle->I2C_Pin_Config.I2C_SCLSpeed == I2C_SCL_SPEED_SM_1KH) {
		regCCRTemp &= ~(ENABLE << FS_CCRPOS);
		regCCRTemp |= ((getAPBClkFreqKhz()
				/ (I2C_SM_CCR_CONST * I2C_SCL_SPEED_SM_1KH)) << CCR_CCRPOS);
	} else {
		regCCRTemp |= (ENABLE << FS_CCRPOS);

		if (i2cHandle->I2C_Pin_Config.I2C_FMDutyCycle == I2C_FM_DUTY_CYCLE_2) {
			regCCRTemp |= ((getAPBClkFreqKhz()
					/ (I2C_DUTY0_CCR_CONST
							* i2cHandle->I2C_Pin_Config.I2C_SCLSpeed))
					<< CCR_CCRPOS);
		} else {
			regCCRTemp |= ((getAPBClkFreqKhz()
					/ (I2C_DUTY1_CCR_CONST
							* i2cHandle->I2C_Pin_Config.I2C_SCLSpeed))
					<< CCR_CCRPOS);
		}
	}

	if (i2cHandle->I2C_Pin_Config.I2C_SCLSpeed == I2C_SCL_SPEED_SM_1KH) {
		i2cHandle->pI2Cx->I2C_TRISE = ((getAPBClkFreqKhz() * I2C_TRISE_MAX_SM)
				/ MHZ2HZ) + 1;
	} else {
		i2cHandle->pI2Cx->I2C_TRISE = ((getAPBClkFreqKhz() * I2C_TRISE_MAX_FM)
				/ MHZ2HZ) + 1;
	}

	if (i2cHandle->I2C_Pin_Config.I2C_FMDutyCycle == I2C_FM_DUTY_CYCLE_2) {
		regCCRTemp &= ~(ENABLE << DUTY_CCRPOS);
	} else {
		regCCRTemp |= (ENABLE << DUTY_CCRPOS);
	}

	uint32_t regCR1Temp = i2cHandle->pI2Cx->I2C_CR1;
	if (i2cHandle->I2C_Pin_Config.I2C_ACKControl == ENABLE) {
		regCR1Temp |= (ENABLE << ACK_CR1POS);
	} else {
		regCR1Temp &= ~(ENABLE << ACK_CR1POS);
	}

	uint32_t regOAR1Temp = i2cHandle->pI2Cx->I2C_OAR1;
	regOAR1Temp &= ~(1 << ADDMODE_OAR1POS);
	regOAR1Temp |= ((i2cHandle->I2C_Pin_Config.I2C_DeviceAddress & 0x7fU)
			<< ADD_OAR1POS);

	i2cHandle->pI2Cx->I2C_OAR1 = regOAR1Temp;
	i2cHandle->pI2Cx->I2C_CR2 = regCR2Temp;
	i2cHandle->pI2Cx->I2C_CR1 = regCR1Temp;
	i2cHandle->pI2Cx->I2C_CCR = regCCRTemp;

	I2C_EnablePins(i2cHandle->pI2Cx);

//TODO SOME BITS HAVE TO BE CONFIGURS AFTER ENABLE
	i2cHandle->pI2Cx->I2C_CR1 |= (ENABLE << PE_CR1POS);
}

void I2C_DeInit(const I2C_Handle_t *I2CHandle) {

	I2CHandle->pI2Cx->I2C_CR1 &= ~(ENABLE << PE_CR1POS);

	if ( pI2C1 == I2CHandle->pI2Cx)
		I2C1_REG_RESET();
	else if ( pI2C2 == I2CHandle->pI2Cx)
		I2C2_REG_RESET();
	else if ( pI2C3 == I2CHandle->pI2Cx)
		I2C3_REG_RESET();
}

static void I2C_EnablePins(const I2C_RegDef_t *pI2Cx) {

	if ( pI2C1 == pI2Cx) {

		GPIO_Handle_t pGPIOB_handler;

		pGPIOB_handler.pGPIOx = pGPIOB;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_8;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_ALTERNATE;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_VERYHIGH;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_PULL_UP;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinAltFunMode = 0x04;
		pGPIOB_handler.GPIO_Pin_Config.GPIO_PortOpType = GPIO_OUT_TYPE_PP;

		GPIO_Init(&pGPIOB_handler);

		pGPIOB_handler.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_N_9;
		GPIO_Init(&pGPIOB_handler);
	}
}

static uint8_t I2C_GetStatusReg1Flag(const I2C_RegDef_t *pI2Cx,
		const uint8_t flagNumber) {

	return ((pI2Cx->I2C_SR1 & (1 << flagNumber)) != DISABLE);
}

static uint8_t I2C_GetStatusReg2Flag(const I2C_RegDef_t *pI2Cx,
		const uint8_t flagNumber) {

	return ((pI2Cx->I2C_SR2 & (1 << flagNumber)) != DISABLE);
}

void I2C_SendData(I2C_RegDef_t *pI2Cx, const uint8_t slaveAddr,
		uint8_t *dataBuffer, uint8_t length, uint8_t repStart) {

	pI2Cx->I2C_CR1 |= (1 << START_CR1POS);

	//EV5
	while (!I2C_GetStatusReg1Flag(pI2Cx, SB_SR1POS))
		;

	pI2Cx->I2C_DR = slaveAddr << 1;

	while (I2C_GetStatusReg1Flag(pI2Cx, SB_SR1POS))
		;

	//EV6
	while (!I2C_GetStatusReg1Flag(pI2Cx, ADDR_SR1POS))
		;

	(void) I2C_GetStatusReg1Flag(pI2Cx, SB_SR1POS);
	(void) I2C_GetStatusReg2Flag(pI2Cx, 0);

	while (I2C_GetStatusReg1Flag(pI2Cx, ADDR_SR1POS))
		;

	while (length--) {
		//EV8
		while (!I2C_GetStatusReg1Flag(pI2Cx, TXE_SR1POS))
			;
		pI2Cx->I2C_DR = *dataBuffer;
		dataBuffer++;
	}

	//EV9
	while ((!I2C_GetStatusReg1Flag(pI2Cx, BTF_SR1POS))
			|| (!I2C_GetStatusReg1Flag(pI2Cx, TXE_SR1POS)))
		;

	if (repStart == 0) {
		pI2Cx->I2C_CR1 |= (1 << STOP_CR1POS);
	}

}

void I2C_ReadData(I2C_RegDef_t *pI2Cx, const uint8_t slaveAddr,
		uint8_t *dataBuffer, uint8_t length, uint8_t repStart) {

	pI2Cx->I2C_CR1 |= (1 << START_CR1POS);

	//EV5
	while (!I2C_GetStatusReg1Flag(pI2Cx, SB_SR1POS))
		;

	pI2Cx->I2C_DR = (slaveAddr << 1) + 1;

	while (I2C_GetStatusReg1Flag(pI2Cx, SB_SR1POS))
		;

	//EV6
	while (!I2C_GetStatusReg1Flag(pI2Cx, ADDR_SR1POS))
		;

	if (length == 1) {
		pI2Cx->I2C_CR1 &= ~(1 << ACK_CR1POS);
	}

	(void) I2C_GetStatusReg1Flag(pI2Cx, SB_SR1POS);
	(void) I2C_GetStatusReg2Flag(pI2Cx, 0);

	while (I2C_GetStatusReg1Flag(pI2Cx, ADDR_SR1POS))
		;

	while (length--) {

		*dataBuffer = pI2Cx->I2C_DR;
		dataBuffer++;

		pI2Cx->I2C_CR1 |= (1 << ACK_CR1POS);

		//EV7
		while (!I2C_GetStatusReg1Flag(pI2Cx, RXNE_SR1POS))
			;
	}

	pI2Cx->I2C_CR1 &= ~(1 << ACK_CR1POS);

	if (repStart == 0) {
		pI2Cx->I2C_CR1 |= (1 << STOP_CR1POS);
	}

//EV7_1  MAYBE REMOVE?
	while (!I2C_GetStatusReg1Flag(pI2Cx, RXNE_SR1POS))
		;

	*dataBuffer = pI2Cx->I2C_DR;
}

static void I2C_Clock_Control(const I2C_RegDef_t *pI2Cx, const uint8_t enable) {

	if (enable == ENABLE) {
		if (pI2C1 == pI2Cx)
			I2C1_PERI_CLK_ENABLE();
		else if (pI2C2 == pI2Cx)
			I2C2_PERI_CLK_ENABLE();
		else if (pI2C3 == pI2Cx)
			I2C3_PERI_CLK_ENABLE();
	} else {
		if (pI2C1 == pI2Cx)
			I2C1_PERI_CLK_DISABLE();
		else if (pI2C2 == pI2Cx)
			I2C2_PERI_CLK_DISABLE();
		else if (pI2C3 == pI2Cx)
			I2C3_PERI_CLK_DISABLE();
	}
}
