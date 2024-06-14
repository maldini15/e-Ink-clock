#ifndef __GUI_PAINT_H
#define __GUI_PAINT_H

#include <Display/ER-EPD0583.h>
#include "Display/SPIRAM.h"
#include "Image/Fonts/fonts.h"

#define IMAGE_ROTATE_0            0
#define IMAGE_ROTATE_90           1
#define IMAGE_ROTATE_180          2
#define IMAGE_ROTATE_270          3

#define IMAGE_COLOR_POSITIVE 0X01
#define IMAGE_COLOR_INVERTED 0X02

#define IMAGE_BW 0
#define IMAGE_BWR 1

typedef struct {
	uint16_t Image_Name; //max = 128K / (Image_Width/8 * Image_Height)
	uint16_t Image_Offset;
	uint16_t Image_Width;
	uint16_t Image_Height;
	uint16_t Image_Rotate;
	uint16_t Image_Color;
	uint16_t Memory_Width;
	uint16_t Memory_Height;
} PAINT_IMAGE;
extern PAINT_IMAGE Paint_Image;

#define WHITE          0xFF
#define BLACK          0x00
#define RED            BLACK

#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE

typedef enum {
	DOT_PIXEL_1X1 = 1,
	DOT_PIXEL_2X2,
	DOT_PIXEL_3X3,
	DOT_PIXEL_4X4,
	DOT_PIXEL_5X5,
	DOT_PIXEL_6X6,
	DOT_PIXEL_7X7,
	DOT_PIXEL_8X8,
} DOT_PIXEL;
#define DOT_PIXEL_DFT  DOT_PIXEL_1X1

typedef enum {
	DOT_FILL_AROUND = 1,	// dot pixel 1 x 1
	DOT_FILL_RIGHTUP,		// dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT  DOT_FILL_AROUND

typedef enum {
	LINE_STYLE_SOLID = 0, LINE_STYLE_DOTTED,
} LINE_STYLE;

typedef enum {
	DRAW_FILL_EMPTY = 0, DRAW_FILL_FULL,
} DRAW_FILL;

typedef struct {
	uint16_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
} PAINT_TIME;
extern PAINT_TIME sPaint_time;

void Paint_Swop(uint16_t Point1, uint16_t Point2);
void Paint_DrawPixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color);
void Paint_SetPixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color);

//init and Clear
void Paint_NewImage(uint16_t Image_Name, uint16_t Width, uint16_t Height,
		uint16_t Rotate, uint16_t Color);

void Paint_Clear(uint16_t Color);
void Paint_ClearWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend,
		uint16_t Yend, uint16_t Color);

//Drawing
void Paint_DrawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color,
		DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay);
void Paint_DrawLine(uint16_t Xstart, uint16_t Ystart, uint16_t Xend,
		uint16_t Yend, uint16_t Color, LINE_STYLE Line_Style,
		DOT_PIXEL Dot_Pixel);
void Paint_DrawRectangle(uint16_t Xstart, uint16_t Ystart, uint16_t Xend,
		uint16_t Yend, uint16_t Color, DRAW_FILL Filled, DOT_PIXEL Dot_Pixel);
void Paint_DrawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius,
		uint16_t Color, DRAW_FILL Draw_Fill, DOT_PIXEL Dot_Pixel);

//Display string
void Paint_DrawChar(uint16_t Xstart, uint16_t Ystart, const char Acsii_Char,
		sFONT *Font, uint16_t Color_Background, uint16_t Color_Foreground);
void Paint_DrawString_EN(uint16_t Xstart, uint16_t Ystart, const char *pString,
		sFONT *Font, uint16_t Color_Background, uint16_t Color_Foreground);
void Paint_DrawNum(uint16_t Xpoint, uint16_t Ypoint, int32_t Nummber,
		sFONT *Font, uint16_t Color_Background, uint16_t Color_Foreground);
void Paint_DrawTime(uint16_t Xstart, uint16_t Ystart, PAINT_TIME *pTime,
		sFONT *Font, uint16_t Color_Background, uint16_t Color_Foreground);

//pic
void Paint_DrawBitMap(const unsigned char *image_buffer);

#endif
