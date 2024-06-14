#include <Display/ER-EPD0583.h>
#include <Display/SPIRAM.h>
#include <stdlib.h>
#include "Drivers/stm32f401xx_gpio_driver.h"
#include "Drivers/stm32f401xx_spi_driver.h"
#include "Drivers/stm32f401xx_timer_driver.h"

void EPD_Reset(void) {

	GPIO_WriteToOutputPin(pGPIOA, EPD_RST, ENABLE);
	TIMER_Delay(pTIMER2, 200);
	GPIO_WriteToOutputPin(pGPIOA, EPD_RST, DISABLE);
	TIMER_Delay(pTIMER2, 200);
	GPIO_WriteToOutputPin(pGPIOA, EPD_RST, ENABLE);
	TIMER_Delay(pTIMER2, 200);
}

void EPD_SendCommand(uint8_t Reg) {

	GPIO_WriteToOutputPin(pGPIOA, EPD_DC, DISABLE);
	GPIO_WriteToOutputPin(pGPIOA, EPD_CS, DISABLE);

	SPI_SendData(pSPI1, &(uint8_t ) { Reg }, 1);

	GPIO_WriteToOutputPin(pGPIOA, EPD_CS, ENABLE);

}

void EPD_SendData(uint8_t Data) {
	GPIO_WriteToOutputPin(pGPIOA, EPD_DC, ENABLE);
	GPIO_WriteToOutputPin(pGPIOA, EPD_CS, DISABLE);
	SPI_SendData(pSPI1, &(uint8_t ) { Data }, 1);
	GPIO_WriteToOutputPin(pGPIOA, EPD_CS, ENABLE);
}

void EPD_WaitUntilIdle(void) {
	while (GPIO_ReadFromInputPin(pGPIOA, EPD_BUSY) == DISABLE) {
		TIMER_Delay(pTIMER2, 100);
	}
}

void EPD_TurnOnDisplay(void) {
	EPD_SendCommand(DISPLAY_REFRESH);
	TIMER_Delay(pTIMER2, 100);
	EPD_WaitUntilIdle();
}

uint8_t EPD_Init(void) {

	EPD_Reset();

	EPD_SendCommand(FITI);  // Enter FITI Command
	EPD_SendData(0x60);
	EPD_SendData(0xA5);

	EPD_SendCommand(FITI);  // Enter FITI Command
	EPD_SendData(0x89);
	EPD_SendData(0xA5);

	EPD_SendCommand(FITI);  // Sorting off
	EPD_SendData(0xA1);
	EPD_SendData(0x00);

	EPD_SendCommand(FITI);  //Vcom driving
	EPD_SendData(0x73);
	EPD_SendData(0x07);

	EPD_SendCommand(FITI);
	EPD_SendData(0x76);
	EPD_SendData(0x1F);

	EPD_SendCommand(FITI);   //boost constant on time
	EPD_SendData(0x7E);
	EPD_SendData(0x31);

	EPD_SendCommand(FITI);
	EPD_SendData(0xB8);
	EPD_SendData(0x80);

	EPD_SendCommand(FITI);   //vgl=> GND:08, HZ:00[default]
	EPD_SendData(0x92);
	EPD_SendData(0x00);

	EPD_SendCommand(FITI); //VCOM(2frme off)=> GND:01(0x88=06)[default], HZ:11
	EPD_SendData(0x87);
	EPD_SendData(0x01);

	EPD_SendCommand(FITI);   //r_vcom_init_sel
	EPD_SendData(0x88);
	EPD_SendData(0x06);

	EPD_SendCommand(FITI);
	EPD_SendData(0xA8);
	EPD_SendData(0x30);

	EPD_SendCommand(POWER_ON);
	EPD_WaitUntilIdle();

	EPD_SendCommand(PANEL_SETTING);
	EPD_SendData(0x5F);	 //BW  mode

	EPD_SendCommand(PLL_CONTROL);
	EPD_SendData(0x3C); //set 50hz

	EPD_SendCommand(RESOLUTION_SETTING);
	EPD_SendData(0x02);
	EPD_SendData(0x88);
	EPD_SendData(0x01);
	EPD_SendData(0xE0);

	return 0;
}

void EPD_Clear(void) {
	uint16_t Width, Height;
	Width = (EPD_WIDTH_RES % 8 == 0) ? (EPD_WIDTH_RES / 8) : (EPD_WIDTH_RES / 8 + 1);
	Height = EPD_HEIGHT_RES;

	EPD_SendCommand(DATA_START_TRANSMISSION_1);
	for (uint16_t j = 0; j < Height; j++) {
		for (uint16_t i = 0; i < Width; i++) {
			EPD_SendData(0xFF);
		}
	}
	EPD_SendCommand(DATA_START_TRANSMISSION_2);
	for (uint16_t j = 0; j < Height; j++) {
		for (uint16_t i = 0; i < Width; i++) {
			EPD_SendData(0xFF);
		}
	}
}

void EPD_Display(uint16_t Image_Offset) {
	uint32_t Width, Height;
	Width = (EPD_WIDTH_RES % 8 == 0) ? (EPD_WIDTH_RES / 8) : (EPD_WIDTH_RES / 8 + 1);
	Height = EPD_HEIGHT_RES;

	EPD_SendCommand(DATA_START_TRANSMISSION_1);
	for (uint32_t j = 0; j < Height; j++) {
		for (uint32_t i = 0; i < Width; i++) {
			EPD_SendData(SPIRAM_RD_Byte(i + j * Width));
		}
	}
	TIMER_Delay(pTIMER2, 2);

	EPD_SendCommand(DATA_START_TRANSMISSION_2);
	for (uint32_t j = 0; j < Height; j++) {
		for (uint32_t i = 0; i < Width; i++) {
			EPD_SendData(SPIRAM_RD_Byte(i + j * Width + Image_Offset));
		}
	}
	TIMER_Delay(pTIMER2, 2);
	EPD_TurnOnDisplay();
}

void EPD_Sleep(void) {
	EPD_SendCommand(POWER_OFF);
	EPD_WaitUntilIdle();
	EPD_SendCommand(DEEP_SLEEP);
	EPD_SendData(0XA5);
}
