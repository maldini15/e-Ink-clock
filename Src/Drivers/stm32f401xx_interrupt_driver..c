#include "Drivers/stm32f401xx_interrupt_driver.h"

void NVIC_EnableInterrupt(uint8_t interruptNumber) {

	uint32_t *pNVICIRQEn = (uint32_t*) NVIC_SETENABLE_BASEADDR;

	if (interruptNumber == 0)
		*pNVICIRQEn |= (1 << EXTI0_IRQ_N);
	else if (interruptNumber == 1)
		*pNVICIRQEn |= (1 << EXTI1_IRQ_N);
	else if (interruptNumber == 2)
		*pNVICIRQEn |= (1 << EXTI2_IRQ_N);
	else if (interruptNumber == 3)
		*pNVICIRQEn |= (1 << EXTI3_IRQ_N);
	else if (interruptNumber == 4)
		*pNVICIRQEn |= (1 << EXTI4_IRQ_N);
	else if (interruptNumber >= 5 && interruptNumber < 9)
		*pNVICIRQEn |= (1 << EXTI9_5_IRQ_N);
	else if (interruptNumber >= 9 && interruptNumber < 15)
		*(pNVICIRQEn + 0x01) |= (1 << (EXTI15_10_IRQ_N - 32));

}

void Interrupt_Clear_Pending(Interrupt_Handle_t *pInteruptHandle) {
	pInteruptHandle->pEXTIReg->EXTI_PR |= (1
			<< pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number);
}

void Interrupt_Enable(Interrupt_Handle_t *pInteruptHandle) {

	RCC_SYSCFG_CLK_ENABLE();

	uint8_t syscfg_register =
			(pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number / 4);

	uint8_t syscfg_register_position = (4
			* (pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number % 4));

	pInteruptHandle->pSYSCFGReg->SYSCFG_EXTICR[syscfg_register] |=
			(pInteruptHandle->Interrupt_Pin_Config.Interrupt_Port
					<< syscfg_register_position);

	pInteruptHandle->pEXTIReg->EXTI_IMR |= (1
			<< pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number);

	if (pInteruptHandle->Interrupt_Pin_Config.Interrupt_RisingEdge) {
		pInteruptHandle->pEXTIReg->EXTI_RTSR |= (1
				<< pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number);
	} else {
		pInteruptHandle->pEXTIReg->EXTI_RTSR &= ~(1
				<< pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number);
	}

	if (pInteruptHandle->Interrupt_Pin_Config.Interrupt_FallingEdge) {
		pInteruptHandle->pEXTIReg->EXTI_FTSR |= (1
				<< pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number);
	} else {
		pInteruptHandle->pEXTIReg->EXTI_FTSR &= ~(1
				<< pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number);
	}

	NVIC_EnableInterrupt(
			pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number);

}

void Interrupt_Disable(Interrupt_Handle_t *pInteruptHandle) {

	pInteruptHandle->pEXTIReg->EXTI_IMR &= ~(1
			<< pInteruptHandle->Interrupt_Pin_Config.Interrupt_Number);
}
