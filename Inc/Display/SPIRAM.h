#ifndef __SPI_RAM_H_
#define __SPI_RAM_H_

#include <stdint.h>

#define SPIRAM_CS 1

/**
 * SRAM opcodes
 **/
#define CMD_WREN  0x06
#define CMD_WRDI  0x04
#define CMD_RDSR  0x05
#define CMD_WRSR  0x01
#define CMD_READ  0x03
#define CMD_WRITE 0x02

/**
 * SRAM modes
 **/
#define BYTE_MODE   0x00
#define PAGE_MODE   0x80
#define STREAM_MODE 0x40

void SPIRAM_Set_Mode(uint8_t mode);

uint8_t SPIRAM_RD_Byte(uint32_t Addr);
void SPIRAM_WR_Byte(uint32_t Addr, uint8_t Data);

void SPIRAM_RD_Page(uint32_t Addr, uint8_t *pBuf);
void SPIRAM_WR_Page(uint32_t Addr, uint8_t *pBuf);

void SPIRAM_RD_Stream(uint32_t Addr, uint8_t *pBuf, uint32_t Len);
void SPIRAM_WR_Stream(uint32_t Addr, uint8_t *pBuf, uint32_t Len);

#endif
