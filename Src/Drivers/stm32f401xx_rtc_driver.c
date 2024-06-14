#include "Drivers/stm32f401xx_rtc_driver.h"

static uint8_t RTC_GetStatusFlag(const RTC_RegDef_t *pRTCx,
		const uint8_t flagNumber);

void RTC_Init(const RTC_Handle_t *RtcHandle) {

	RtcHandle->pRTCx->RTC_CR |= (ENABLE << DBP_RTC_CRPOS);
	RTC_PERI_CLK_ENABLE();

	uint32_t regBDSRTemp = pRCC->RCC_BDCR;
	uint32_t regPRERTemp = RtcHandle->pRTCx->RTC_PRER;

	regPRERTemp &= ~(PREDIC_A_RTC_PRERSIZE << PREDIC_A_RTC_PRERPOS);
	regPRERTemp &= ~(PREDIC_S_RTC_PRERSIZE << PREDIC_S_RTC_PRERPOS);
	regPRERTemp |= (256U << PREDIC_S_RTC_PRERPOS);

	if (RtcHandle->RTC_Config_t.RTC_ClkSource == RTC_CLK_LSE) {
		pRCC->RCC_CSR &= ~(ENABLE << LSION_RCC_CSRPOS);
		regBDSRTemp &= ~(ENABLE << LSEON_RCC_BDCRPOS);
		regPRERTemp |= ((265U / RtcHandle->RTC_Config_t.RTC_ClkFreqHz)
				<< PREDIC_A_RTC_PRERPOS);
	} else {
		pRCC->RCC_CSR |= (ENABLE << LSION_RCC_CSRPOS);
		regBDSRTemp |= (ENABLE << LSEON_RCC_BDCRPOS);
		regPRERTemp |= (128U << PREDIC_A_RTC_PRERPOS);
	}

	regBDSRTemp &= ~(0x3 << RTCSEL_RCC_BDCRPOS);
	regBDSRTemp |=
			(RtcHandle->RTC_Config_t.RTC_ClkSource << RTCSEL_RCC_BDCRPOS);

	if (RtcHandle->RTC_Config_t.RTC_HourFormat == RTC_AMPM) {
		RtcHandle->pRTCx->RTC_CR |= (ENABLE << FMT_RTC_CRPOS);
	} else {
		RtcHandle->pRTCx->RTC_CR &= ~(ENABLE << FMT_RTC_CRPOS);
	}

	pRCC->RCC_BDCR = regBDSRTemp;

	pRCC->RCC_BDCR |= (ENABLE << RTCEN_RCC_BDCRPOS);

	RtcHandle->pRTCx->RTC_CR &= ~(ENABLE << DBP_RTC_CRPOS);

}

void RTC_DeInit(RTC_RegDef_t *pRTCx) {

	pRTCx->RTC_CR |= (ENABLE << DBP_RTC_CRPOS);

	pRCC->RCC_BDCR &= ~(ENABLE << RTCEN_RCC_BDCRPOS);

	RTC_PERI_CLK_DISABLE();

	pRTCx->RTC_CR &= ~(ENABLE << DBP_RTC_CRPOS);
}

static uint8_t RTC_GetStatusFlag(const RTC_RegDef_t *pRTCx,
		const uint8_t flagNumber) {

	return ((pRTCx->RTC_ISR & (1 << flagNumber)) != DISABLE);
}

Time RTC_GetTime(const RTC_RegDef_t *pRTCx) {

	Time time;

	const uint32_t regTR = pRTCx->RTC_TR;

	time.hour = (((regTR >> HT_RTC_TRPOS) & 0x1) * 10)
			+ ((regTR >> HU_RTC_TRPOS) & 0x7);

	time.minutes = (((regTR >> MNT_RTC_TRPOS) & 0x3) * 10)
			+ ((regTR >> MNU_RTC_TRPOS) & 0x7);

	time.seconds = (((regTR >> ST_RTC_TRPOS) & 0x3) * 10)
			+ ((regTR >> SU_RTC_TRPOS) & 0x7);

	return time;
}

Date RTC_GetDate(const RTC_RegDef_t *pRTCx) {

	Date date;

	const uint32_t regDR = pRTCx->RTC_DR;

	date.day = (((regDR >> DT_RTC_DRPOS) & 0x7) * 10)
			+ ((regDR >> DU_RTC_DRPOS) & 0x7);

	date.month = (((regDR >> DT_RTC_DRPOS) & 0x1) * 10)
			+ ((regDR >> DU_RTC_DRPOS) & 0xf);

	date.year = (((regDR >> DT_RTC_DRPOS) & 0x1) * 10)
			+ ((regDR >> DU_RTC_DRPOS) & 0xf);

	return date;
}

void RTC_SetTime(RTC_RegDef_t *pRTCx, const Time time) {

	pRTCx->RTC_CR |= (ENABLE << DBP_RTC_CRPOS);

	pRTCx->RTC_WPR = COMMAND1_RTC_WPR;
	pRTCx->RTC_WPR = COMMAND2_RTC_WPR;

	pRTCx->RTC_ISR |= (1 << INIT_RTC_ISRPOS);

	while (!RTC_GetStatusFlag(pRTCx, INITF_RTC_ISRPOS))
		;

	uint32_t regTRTemp = pRTCx->RTC_TR;

	regTRTemp &= ~0x3FFFFFU;
	regTRTemp |= ((time.hour % 10) << HU_RTC_TRPOS);
	regTRTemp |= ((((time.hour / 10) % 10) & 0x3) << HT_RTC_TRPOS);

	regTRTemp |= ((time.minutes % 10) << MNU_RTC_TRPOS);
	regTRTemp |= ((((time.minutes / 10) % 10) & 0x3) << MNT_RTC_TRPOS);

	regTRTemp |= ((time.seconds % 10) << SU_RTC_TRPOS);
	regTRTemp |= ((((time.seconds / 10) % 10) & 0x3) << ST_RTC_TRPOS);

	pRTCx->RTC_TR = regTRTemp;

	pRTCx->RTC_WPR = 0;

	pRTCx->RTC_CR &= ~(ENABLE << DBP_RTC_CRPOS);
}

void RTC_SetDate(RTC_RegDef_t *pRTCx, const Date date) {
	pRTCx->RTC_CR |= (ENABLE << DBP_RTC_CRPOS);

	pRTCx->RTC_WPR = COMMAND1_RTC_WPR;
	pRTCx->RTC_WPR = COMMAND2_RTC_WPR;

	pRTCx->RTC_ISR |= (1 << INIT_RTC_ISRPOS);

	while (!RTC_GetStatusFlag(pRTCx, INITF_RTC_ISRPOS))
		;

	uint32_t regDRTemp = pRTCx->RTC_DR;

	regDRTemp &= ~0xFFFFFFU;
	regDRTemp |= ((date.year % 10) << YU_RTC_DRPOS);
	regDRTemp |= (((date.year / 10) % 10) << YT_RTC_DRPOS);

	regDRTemp |= ((date.month % 0x7) << MU_RTC_DRPOS);

	regDRTemp |= ((date.day % 10) << DT_RTC_DRPOS);
	regDRTemp |= (((date.day / 10) % 10) << DU_RTC_DRPOS);

	pRTCx->RTC_DR = regDRTemp;

	pRTCx->RTC_WPR = 0;

	pRTCx->RTC_CR &= ~(ENABLE << DBP_RTC_CRPOS);
}
