#include "Drivers/stm32f401xx_gpio_driver.h"

static void GPIO_Clock_Control(const GPIO_RegDef_t *pGPIOx,
		const uint8_t enable);

uint8_t GPIO_ReadFromInputPin(const GPIO_RegDef_t *pGPIOx,
		const uint8_t pinNumber) {

	return (uint8_t) ((pGPIOx->GPIOx_IDR >> pinNumber) & 0x01);
}

uint16_t GPIO_ReadFromInputPort(const GPIO_RegDef_t *pGPIOx) {

	return (uint16_t) (pGPIOx->GPIOx_IDR);
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, const uint8_t pinNumber,
		const uint8_t value) {

	if (value)
		pGPIOx->GPIOx_ODR |= (value & 0x01) << pinNumber;
	else
		pGPIOx->GPIOx_ODR &= 0x00 << pinNumber;
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, const uint16_t value) {

	pGPIOx->GPIOx_ODR = value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, const uint8_t pinNumber) {

	pGPIOx->GPIOx_ODR ^= 0x0001 << pinNumber;
}

static void GPIO_Clock_Control(const GPIO_RegDef_t *pGPIOx,
		const uint8_t enable) {

	if (enable == ENABLE) {
		if (pGPIOx == pGPIOA) {
			GPIOA_PERI_CLK_ENABLE();
		}
		if (pGPIOx == pGPIOB) {
			GPIOB_PERI_CLK_ENABLE();
		}
		if (pGPIOx == pGPIOC) {
			GPIOC_PERI_CLK_ENABLE();
		}
		if (pGPIOx == pGPIOD) {
			GPIOD_PERI_CLK_ENABLE();
		}
		if (pGPIOx == pGPIOE) {
			GPIOE_PERI_CLK_ENABLE();
		}
		if (pGPIOx == pGPIOH) {
			GPIOH_PERI_CLK_ENABLE();
		}
	} else {
		if (pGPIOx == pGPIOA) {
			GPIOA_PERI_CLK_DISABLE();
		}
		if (pGPIOx == pGPIOB) {
			GPIOB_PERI_CLK_DISABLE();
		}
		if (pGPIOx == pGPIOC) {
			GPIOC_PERI_CLK_DISABLE();
		}
		if (pGPIOx == pGPIOD) {
			GPIOD_PERI_CLK_DISABLE();
		}
		if (pGPIOx == pGPIOE) {
			GPIOE_PERI_CLK_DISABLE();
		}
		if (pGPIOx == pGPIOH) {
			GPIOH_PERI_CLK_DISABLE();
		}
	}
}

void GPIO_DeInit(const GPIO_RegDef_t *pGPIOPort) {
	if (pGPIOPort == pGPIOA) {
		GPIOA_REG_RESET();
	}
	if (pGPIOPort == pGPIOB) {
		GPIOB_REG_RESET();
	}
	if (pGPIOPort == pGPIOC) {
		GPIOC_REG_RESET();
	}
	if (pGPIOPort == pGPIOD) {
		GPIOD_REG_RESET();
	}
	if (pGPIOPort == pGPIOE) {
		GPIOE_REG_RESET();
	}
	if (pGPIOPort == pGPIOH) {
		GPIOH_REG_RESET();
	}
}

void GPIO_Init(const GPIO_Handle_t *pGPIOHandle) {

	uint32_t tempReg = 0;

	GPIO_Clock_Control(pGPIOHandle->pGPIOx, ENABLE);

	if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode <= GPIO_MODE_ANALOG) {
		tempReg = pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode
				<< (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->GPIOx_MODER &= ~(tempReg);

		pGPIOHandle->pGPIOx->GPIOx_MODER |= tempReg;
		tempReg = 0;
	} else {
		return;
	}

	if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinSpeed <= GPIO_SPEED_VERYHIGH) {
		tempReg = pGPIOHandle->GPIO_Pin_Config.GPIO_PinSpeed
				<< (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->GPIOx_OSPEEDR &= ~(tempReg);
		pGPIOHandle->pGPIOx->GPIOx_OSPEEDR |= tempReg;
		tempReg = 0;
	}

	if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinPuPdControl <= GPIO_PULL_DOWN) {
		tempReg = pGPIOHandle->GPIO_Pin_Config.GPIO_PinPuPdControl
				<< (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->GPIOx_PUPDR &= ~(tempReg);
		pGPIOHandle->pGPIOx->GPIOx_PUPDR |= tempReg;
		tempReg = 0;
	}

	if ( GPIO_MODE_ALTERNATE == pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode) {
		uint32_t high_or_low_reg = pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber
				/ 8;
		uint32_t bit_on_reg = pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber % 8;

		tempReg = pGPIOHandle->GPIO_Pin_Config.GPIO_PinAltFunMode
				<< (4 * bit_on_reg);
		pGPIOHandle->pGPIOx->GPIOx_AFR[high_or_low_reg] &= ~(tempReg);
		pGPIOHandle->pGPIOx->GPIOx_AFR[high_or_low_reg] |= tempReg;

		tempReg = 0;
	}
}
