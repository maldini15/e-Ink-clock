#ifndef INC_STM32F401XX_I2C_DRIVER_H_
#define INC_STM32F401XX_I2C_DRIVER_H_

#include "stm32f401xx.h"

typedef struct {
	uint32_t I2C_SCLSpeed;
	uint8_t I2C_DeviceAddress;
	uint8_t I2C_ACKControl;
	uint8_t I2C_FMDutyCycle;
} I2C_PinConfig_t;

typedef struct {
	I2C_RegDef_t *pI2Cx;
	I2C_PinConfig_t I2C_Pin_Config;
} I2C_Handle_t;

#define I2C_SCL_SPEED_SM_1KH	100000
#define I2C_SCL_SPEED_FM_2KH	200000
#define I2C_SCL_SPEED_FM_4KH	400000

#define I2C_TRISE_MAX_SM		1000
#define I2C_TRISE_MAX_FM		300

#define I2C_ACK_ENABLE	1
#define I2C_ACK_DISABLE	0

#define I2C_FM_DUTY_CYCLE_2		0
#define I2C_FM_DUTY_CYCLE_16_9	1

#define I2C_ACK_ENABLE	1
#define I2C_ACK_DISABLE	0

#define I2C_SM_CCR_CONST	2
#define I2C_DUTY0_CCR_CONST	3
#define I2C_DUTY1_CCR_CONST	25

#define PE_CR1POS		0
#define ACK_CR1POS		10
#define FS_CCRPOS		15
#define DUTY_CCRPOS 	14
#define CCR_CCRPOS		0
#define ADD_OAR1POS		1
#define ADDMODE_OAR1POS	15
#define FREQ_CR2POS		0
#define START_CR1POS	8
#define STOP_CR1POS		9
#define SB_SR1POS		0
#define ADDR_SR1POS		1
#define BTF_SR1POS		2
#define STOPF_SR1POS	4
#define TXE_SR1POS		7
#define RXNE_SR1POS		6
#define TIMEOUT_SR1POS	14
#define OVR_SR1POS		11
#define AF_SR1POS		10
#define ARLO_SR1POS		9
#define BERR_SR1POS		8

#define pI2C1				((I2C_RegDef_t*)I2C1_BASEADDR)
#define pI2C2				((I2C_RegDef_t*)I2C2_BASEADDR)
#define pI2C3				((I2C_RegDef_t*)I2C3_BASEADDR)

#define I2C1_PERI_CLK_ENABLE()		(pRCC->RCC_APB1ENR |= (1 << 21))
#define I2C1_PERI_CLK_DISABLE()		(pRCC->RCC_APB1ENR &= ~(1 << 21))
#define I2C1_REG_RESET()			do{pRCC->RCC_APB1RSTR |= (1 << 21); pRCC->RCC_AHB1RSTR &= ~(1 << 21);}while(0)

#define I2C2_PERI_CLK_ENABLE()		(pRCC->RCC_APB1ENR |= (1 << 22))
#define I2C2_PERI_CLK_DISABLE()		(pRCC->RCC_APB1ENR &= ~(1 << 22))
#define I2C2_REG_RESET()			do{pRCC->RCC_APB1RSTR |= (1 << 22); pRCC->RCC_AHB1RSTR &= ~(1 << 22);}while(0)

#define I2C3_PERI_CLK_ENABLE()		(pRCC->RCC_APB1ENR |= (1 << 23))
#define I2C3_PERI_CLK_DISABLE() 	(pRCC->RCC_APB1ENR &= ~(1 << 23))
#define I2C3_REG_RESET()			do{pRCC->RCC_APB1RSTR |= (1 << 23); pRCC->RCC_AHB1RSTR &= ~(1 << 23);}while(0)

void I2C_Init(const I2C_Handle_t *i2cHandle);

void I2C_DeInit(const I2C_Handle_t *i2cHandle);

void I2C_ReadData(I2C_RegDef_t *pI2Cx, const uint8_t slaveAddr,
		uint8_t *dataBuffer, uint8_t length, uint8_t repStart);

void I2C_SendData(I2C_RegDef_t *pI2Cx, const uint8_t slaveAddr,
		uint8_t *dataBuffer, uint8_t length, uint8_t repStart);

#endif
