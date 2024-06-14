#include "Drivers/stm32f401xx_timer_driver.h"
#include "Drivers/stm32f401xx_interrupt_driver.h"

static void TIMER_Clock_Control(const TIMER_RegDef_t *pTIMERx, const uint8_t enable);
static void TIMER_Reg_Reset(const TIMER_RegDef_t *pTIMERx);

static uint8_t count;

void TIMER_Init(const TIMER_Handle_t *pTIMERHandle) {

	TIMER_Clock_Control(pTIMERHandle->pTIMERReg, ENABLE);

	pTIMERHandle->pTIMERReg->TIM_SMCR &= ~(0X03U);

	uint32_t regCR1Temp = pTIMERHandle->pTIMERReg->TIM_CR1;
	regCR1Temp &= ~(ENABLE << UDIS_CR1POS);
	regCR1Temp |= (ENABLE << URS_CR1POS);
	regCR1Temp |= (ENABLE << OPM_CR1POS);
	regCR1Temp |= (ENABLE << DIR_CR1POS);

	pTIMERHandle->pTIMERReg->TIM_CR1 = regCR1Temp;

	pTIMERHandle->pTIMERReg->TIM_PSC =
			(uint32_t) pTIMERHandle->TIMER_Config.Prescaler_Value;

	//TODO enable interrupts
}

void TIMER_DeInit(const TIMER_RegDef_t *pTIMERPort) {

	TIMER_Reg_Reset(pTIMERPort);
	TIMER_Clock_Control(pTIMERPort, DISABLE);
}

static void TIMER_Reg_Reset(const TIMER_RegDef_t *pTIMERx) {
	if (pTIMERx == pTIMER2) {
		TIMER2_REG_RESET();
	} else if (pTIMERx == pTIMER3) {
		TIMER3_REG_RESET();
	} else if (pTIMERx == pTIMER4) {
		TIMER4_REG_RESET();
	} else if (pTIMERx == pTIMER5) {
		TIMER5_REG_RESET();
	}
}

static void TIMER_Clock_Control(const TIMER_RegDef_t *pTIMERx, const uint8_t enable) {

	if (enable) {
		if (pTIMERx == pTIMER2) {
			TIMER2_PERI_CLK_ENABLE();
		} else if (pTIMERx == pTIMER3) {
			TIMER3_PERI_CLK_ENABLE();
		} else if (pTIMERx == pTIMER4) {
			TIMER4_PERI_CLK_ENABLE();
		} else if (pTIMERx == pTIMER5) {
			TIMER5_PERI_CLK_ENABLE();
		}
	} else {
		if (pTIMERx == pTIMER2) {
			TIMER2_PERI_CLK_DISABLE();
		} else if (pTIMERx == pTIMER3) {
			TIMER3_PERI_CLK_DISABLE();
		} else if (pTIMERx == pTIMER4) {
			TIMER4_PERI_CLK_DISABLE();
		} else if (pTIMERx == pTIMER5) {
			TIMER5_PERI_CLK_DISABLE();
		}
	}
}

void TIMER_SetMeasurement(TIMER_RegDef_t *pTIMERx, const uint16_t msTick) {

	count = 0;

	pTIMERx->TIM_EGR |= (ENABLE << UG_EGRPOS);

	pTIMERx->TIM_CNT = (uint32_t) msTick;

	pTIMERx->TIM_SR &= ~(ENABLE << UIF_SRPOS);

	pTIMERx->TIM_CR1 |= (ENABLE << CEN_CR1POS);
}

uint16_t TIMER_GetRemaining(const TIMER_RegDef_t *pTIMERx) {

	uint16_t remanining = 0;

	if (!(pTIMERx->TIM_SR & (ENABLE << UIF_SRPOS))) {
		count++;
		remanining = (uint16_t) (pTIMERx->TIM_CNT);
		if (remanining == 0) {
			return 0;
		}
	} else {
		return 0;
	}
	return remanining;
}

void TIMER_Delay(TIMER_RegDef_t *pTIMERx, const uint16_t msTick) {

	TIMER_SetMeasurement(pTIMERx, msTick);
	while (TIMER_GetRemaining(pTIMERx))
		;
}

