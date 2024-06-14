#ifndef _EPD4IN2_H
#define _EPD4IN2_H

#include <stdint.h>

#define EPD_WIDTH_RES	648
#define EPD_HEIGHT_RES	480

#define PANEL_SETTING					0x00
#define POWER_SETTING					0x01
#define POWER_OFF						0x02
#define POWER_OFF_SEQUENCE_SETTING		0x03
#define POWER_ON						0x04
#define POWER_ON_MEASURE				0x05
#define BOOSTER_SOFT_START				0x06
#define DEEP_SLEEP						0x07
#define DATA_START_TRANSMISSION_1		0x10
#define DATA_STOP						0x11
#define DISPLAY_REFRESH					0x12
#define DATA_START_TRANSMISSION_2		0x13
#define LUT_FOR_VCOM					0x20
#define LUT_WHITE_TO_WHITE				0x21
#define LUT_BLACK_TO_WHITE				0x22
#define LUT_WHITE_TO_BLACK				0x23
#define LUT_BLACK_TO_BLACK				0x24
#define PLL_CONTROL						0x30
#define TEMPERATURE_SENSOR_COMMAND		0x40
#define TEMPERATURE_SENSOR_SELECTION	0x41
#define TEMPERATURE_SENSOR_WRITE		0x42
#define TEMPERATURE_SENSOR_READ			0x43
#define VCOM_AND_DATA_INTERVAL_SETTING	0x50
#define LOW_POWER_DETECTION				0x51
#define TCON_SETTING					0x60
#define RESOLUTION_SETTING				0x61
#define GSST_SETTING					0x65
#define GET_STATUS						0x71
#define AUTO_MEASUREMENT_VCOM			0x80
#define READ_VCOM_VALUE					0x81
#define VCM_DC_SETTING					0x82
#define PARTIAL_WINDOW					0x90
#define PARTIAL_IN						0x91
#define PARTIAL_OUT						0x92
#define PROGRAM_MODE					0xA0
#define ACTIVE_PROGRAMMING				0xA1
#define READ_OTP						0xA2
#define POWER_SAVING					0xE3
#define FITI							0xF8

#define SD_CS 0
#define EPD_CS 9
#define EPD_DC 2
#define EPD_RST 3
#define EPD_BUSY 10

void EPD_Reset(void);
void EPD_SendCommand(uint8_t Reg);
void EPD_SendData(uint8_t Data);
void EPD_WaitUntilIdle(void);

void EPD_TurnOnDisplay(void);
uint8_t EPD_Init(void);
void EPD_Clear(void);
void EPD_Display(uint16_t Image_Offset);
void EPD_Sleep(void);

#endif 
