#include "Drivers/stm32f401xx_interrupt_driver.h"
#include "Drivers/stm32f401xx_gpio_driver.h"
#include "Drivers/stm32f401xx_spi_driver.h"
#include "Drivers/stm32f401xx_timer_driver.h"
#include "Drivers/stm32f401xx_i2c_driver.h"
#include "Drivers/stm32f401xx_rtc_driver.h"

#include "Image/GUI_Paint.h"
#include "cmsis_os.h"

void GPIOInit(void) {

	//GPIO_A INIT PIN E_DC	 E_RST	 E_BUSY
	GPIO_Handle_t pGPIOA_handler;
	pGPIOA_handler.pGPIOx = pGPIOA;
	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = EPD_RST;
	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_OUTPUT;
	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_PULL_DEACT;
	pGPIOA_handler.GPIO_Pin_Config.GPIO_PortOpType = GPIO_OUT_TYPE_PP;
	GPIO_Init(&pGPIOA_handler);

	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = EPD_DC;
	GPIO_Init(&pGPIOA_handler);

	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = EPD_CS;
	GPIO_Init(&pGPIOA_handler);

	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = SD_CS;
	GPIO_Init(&pGPIOA_handler);

	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = SPIRAM_CS;
	GPIO_Init(&pGPIOA_handler);

	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_INPUT;
	pGPIOA_handler.GPIO_Pin_Config.GPIO_PinNumber = EPD_BUSY;
	GPIO_Init(&pGPIOA_handler);
}

void SPIInit(void) {

	SPI_Handle_t pSPIHandler;
	pSPIHandler.pSPI = pSPI1;
	pSPIHandler.SpiSettings.is_master = SPI_IS_MASTER;
	pSPIHandler.SpiSettings.speed = SPI_SPEED8;
	pSPIHandler.SpiSettings.mode = SPI_MODE_FD;
	pSPIHandler.SpiSettings.crc_en = SPI_CRC_DISABLE;
	pSPIHandler.SpiSettings.frameformat = MSBFIRST;
	pSPIHandler.SpiSettings.dataframeformat = DFF_8B;
	pSPIHandler.SpiSettings.swslaveselect = SPI_SWSLAVEENABLED;
	pSPIHandler.SpiSettings.cpha = FIRSTCLKFIRSTCAPTURE;
	pSPIHandler.SpiSettings.cpol = CK0WHENIDLE;
	SPI_Init(&pSPIHandler);
}

void I2CInit(void) {

	I2C_Handle_t pI2CHandler;
	pI2CHandler.pI2Cx = pI2C1;

	I2C_Init(&pI2CHandler);
}

void TimerInit(void) {
	//Disable TIM2 during breakpoint
	*((uint32_t*) DBGMCU_APB1_FZ_ADDR) |= (1 << 0);	//Timer
	TIMER_Handle_t pTimer2Handler;
	pTimer2Handler.pTIMERReg = pTIMER2;
	pTimer2Handler.TIMER_Config.Prescaler_Value = 0x3e7f;
	TIMER_Init(&pTimer2Handler);
	TIMER_SetMeasurement((pTimer2Handler.pTIMERReg), 0x4e20U);
}

void SysClkInit(void) {

}



osThreadId_t defaultTaskHandle;
/* Definitions for defaultTask */
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

int main(void) {

	SysClkInit();
	I2CInit();
	GPIOInit();
	SPIInit();
	TimerInit();

	GPIO_WriteToOutputPin(pGPIOA, EPD_RST, ENABLE);
	TIMER_Delay(pTIMER2, 200);
	GPIO_WriteToOutputPin(pGPIOA, EPD_RST, DISABLE);
	TIMER_Delay(pTIMER2, 200);
	GPIO_WriteToOutputPin(pGPIOA, EPD_RST, ENABLE);
	TIMER_Delay(pTIMER2, 200);

	GPIO_WriteToOutputPin(pGPIOA, SD_CS, ENABLE);
	GPIO_WriteToOutputPin(pGPIOA, SPIRAM_CS, ENABLE);

	EPD_Init();
	EPD_Clear();

	//1.Create a new image cache named IMAGE_BW and fill it with white
	//Draw black picture
	Paint_NewImage(IMAGE_BW, EPD_WIDTH_RES, EPD_HEIGHT_RES, IMAGE_ROTATE_0,
	IMAGE_COLOR_INVERTED);
	Paint_Clear(WHITE);

	//Draw smth
	Paint_DrawString_EN(10, 20, "Hello World", &Font24, BLACK, WHITE);

	uint16_t imageOffset = IMAGE_BW * EPD_WIDTH_RES * EPD_HEIGHT_RES;
	//show
	EPD_Display(imageOffset);

	/* Init scheduler */
	osKernelInitialize();

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL,
			&defaultTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */

	for (;;)
		;

}

/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */

void NMI_Handler(void) {
	while (1) {
	}
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void) {
	while (1) {
	}
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void) {
	while (1) {
	}
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void) {
	while (1) {
	}
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void) {
	while (1) {
	}
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void) {
}

/**
 * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
 */
void TIM1_UP_TIM10_IRQHandler(void) {
	//HAL_TIM_IRQHandler(&htim1);
}

//global shared variable between main code and ISR
uint8_t volatile g_button_pressed = 0;
Interrupt_Handle_t interrupt_handler;

//INTERRUPT
/*interrupt_handler.Interrupt_Pin_Config.Interrupt_FallingEdge = ENABLE;
 interrupt_handler.Interrupt_Pin_Config.Interrupt_RisingEdge = DISABLE;
 interrupt_handler.Interrupt_Pin_Config.Interrupt_Number = 13;
 interrupt_handler.Interrupt_Pin_Config.Interrupt_Port = PC;

 interrupt_handler.pEXTIReg = pEXTI;
 interrupt_handler.pSYSCFGReg = pSYSCFG;

 Interrupt_Enable(&interrupt_handler);

 }

 void EXTI15_10_IRQHandler(void) {
 //Make this flag SET . if button pressed
 g_button_pressed = 1;
 Interrupt_Clear_Pending(&interrupt_handler);

 }*/
