#include "Gyro/MPU-9250.h"
#include "Drivers/stm32f401xx_i2c_driver.h"

static uint8_t isSelfTestSuccessful();
static uint8_t readReg(uint8_t regAddr);
static void writeReg(uint8_t regAddr, uint8_t value);

static const uint16_t mpu6500StTb[] = { 0x0A3C, 0x0A56, 0x0A70, 0x0A8B,
		0x0A96, 0x0AB1, 0x0ADD, 0x0AF8, 0x0B15, 0x0B31, 0x0B46, 0x0B5B, 0x0B88,
		0x0B9D, 0x0BB3, 0x0BD1, 0x0C00, 0x0C1E, 0x0C35, 0x0C5D, 0x0C7C, 0x0C94,
		0x0CB1, 0x0CCD, 0x0CF6, 0x0D1F, 0x0D41, 0x0D5B, 0x0D85, 0x0DB8, 0x0DDB,
		0x0DEE, 0x0E12, 0x0E36, 0x0E5A, 0x0E8F, 0x0E94, 0x0ECA, 0x0EE7, 0x0F1E,
		0x0F3C, 0x0F63, 0x0F7B, 0x0FAB, 0x0FD3, 0x0FFD, 0x102C, 0x1056, 0x1080,
		0x10AA, 0x10D4, 0x1100, 0x112B, 0x1157, 0x1183, 0x11A8, 0x11E6, 0x120B,
		0x1239, 0x1268, 0x1297, 0x12C7, 0x12F7, 0x1327, 0x1369, 0x139A, 0x13CD,
		0x140F, 0x1442, 0x1475, 0x14A9, 0x14EE, 0x1533, 0x1579, 0x15BF, 0x1605,
		0x164D, 0x1694, 0x16DD, 0x1726, 0x177F, 0x17D9, 0x1834, 0x188F, 0x18EB,
		0x1948, 0x19A5, 0x1A04, 0x1A63, 0x1AC3, 0x1B25, 0x1B87, 0x1BEB, 0x1C4F,
		0x1CB4, 0x1D1A, 0x1D82, 0x1DEA, 0x1E53, 0x1EBD, 0x1F28, 0x1F93, 0x200F,
		0x2081, 0x20FE, 0x2174, 0x21EB, 0x2263, 0x22DC, 0x2356, 0x23D1, 0x244D,
		0x24CA, 0x2548, 0x25C7, 0x2647, 0x26C8, 0x274A, 0x27CD, 0x2851, 0x28D7,
		0x295D, 0x29E5, 0x2A6E, 0x2AF8, 0x2B83, 0x2C0F, 0x2C9C, 0x2D2B, 0x2DBA,
		0x2E4B, 0x2EDC, 0x2F6F, 0x3003, 0x3098, 0x313E, 0x31E5, 0x328D, 0x3336,
		0x33E0, 0x348B, 0x3537, 0x35E4, 0x3692, 0x3741, 0x37F1, 0x38A2, 0x3954,
		0x3A07, 0x3ABC, 0x3B71, 0x3C28, 0x3CE0, 0x3D99, 0x3E53, 0x3F0E, 0x3FCB,
		0x4089, 0x4148, 0x4208, 0x42C9, 0x438B, 0x444E, 0x4512, 0x45D7, 0x469D,
		0x4764, 0x482C, 0x48F5, 0x49BF, 0x4A8A, 0x4B56, 0x4C23, 0x4CF1, 0x4DC0,
		0x4E90, 0x4F61, 0x5033, 0x5106, 0x51DA, 0x52AF, 0x5385, 0x545C, 0x5534,
		0x560D, 0x56E7, 0x57C2, 0x589E, 0x597B, 0x5A59, 0x5B38, 0x5C18, 0x5CF9,
		0x5DDB, 0x5EBE, 0x5FA2, 0x6087, 0x616D, 0x6254, 0x633C, 0x6425, 0x650F,
		0x65FA, 0x66E6, 0x67D3, 0x68C1, 0x69B0, 0x6AA0, 0x6B91, 0x6C83, 0x6D76,
		0x6E6A, 0x6F5F, 0x7055, 0x714C, 0x7244, 0x733D, 0x7437, 0x7532, 0x762E,
		0x772B, 0x7829, 0x7928, 0x7A28, 0x7B29, 0x7C2B, 0x7D2E, 0x7E32, 0x7F37,
		0x804D, 0x8154, 0x826C, 0x8385, 0x84A0, 0x85BB, 0x86D8, 0x87F6, 0x8915,
		0x8A35, 0x8B56, 0x8C78, 0x8D9B, 0x8EBF, 0x8FE4, 0x910A, 0x9231, 0x9359,
		0x9482, 0x95AC, 0x96D7, 0x9803, 0x9930, 0x9A5E, 0x9B8D, 0x9CBD, 0x9DEE };

static uint8_t isSelfTestSuccessful() {

	uint8_t selfTest[6];
	int32_t gAvg[3] = { 0 }, gSTAvg[3] = { 0 };
	int32_t factoryTrim[6];
	float gDiff[3];
	uint8_t FS = 0;

	// Save old configuration
	const uint8_t saveRegSMPLRT_DIV = readReg(REG_SMPLRT_DIV);
	const uint8_t saveRegCONFIG = readReg(REG_CONFIG);
	const uint8_t saveRegGYROCONFIG = readReg(REG_GYRO_CONFIG);

	// Write test configuration
	writeReg(REG_SMPLRT_DIV, 0x00);// Set gyro sample rate to 1 kHz
	writeReg(REG_CONFIG, 0x02);// Set gyro sample rate to 1 kHz and DLPF to 92 Hz
	writeReg(REG_GYRO_CONFIG, 1 << GYRO_CONFIG_FS_POS);// Set full scale range for the gyro to 250 dps

	for (uint8_t i = 0; i < 200; i++) {
		uint8_t rawData[6] = { 0, 0, 0, 0, 0, 0 };
		// get average current values of gyro
		readRegs(MPU6500I2CADDR, REG_GYRO_XOUT_H, 6, &rawData[0]); // Read the six raw data registers sequentially into data array
		gAvg[0] += (int16_t) ((int16_t) rawData[0] << 8) | rawData[1]; // Turn the MSB and LSB into a signed 16-bit value
		gAvg[1] += (int16_t) ((int16_t) rawData[2] << 8) | rawData[3];
		gAvg[2] += (int16_t) ((int16_t) rawData[4] << 8) | rawData[5];
	}

	for (uint8_t i = 0; i < 3; i++) { // Get average of 200 values and store as average current readings
		gAvg[i] /= 200;
	}

	// Configure the accelerometer for self-test
	writeReg(MPU6500I2CADDR, REG_GYRO_CONFIG, 0xE0);// Enable self test on all three axes and set gyro range to +/- 250 degrees/s
	nrf_delay_ms(25); // Delay a while to let the device stabilize

	for (uint8_t i = 0; i < 200; i++) {
		readRegs(MPU6500I2CADDR, REG_GYRO_XOUT_H, 6, &rawData[0]); // Read the six raw data registers sequentially into data array
		gSTAvg[0] += (int16_t) (((int16_t) rawData[0] << 8) | rawData[1]); // Turn the MSB and LSB into a signed 16-bit value
		gSTAvg[1] += (int16_t) (((int16_t) rawData[2] << 8) | rawData[3]);
		gSTAvg[2] += (int16_t) (((int16_t) rawData[4] << 8) | rawData[5]);
	}

	// Get average of 200 values and store as average self-test readings
	for (uint8_t i = 0; i < 3; i++) {
		gSTAvg[i] /= 200;
	}

	// Configure the gyro and accelerometer for normal operation
	writeReg(REG_GYRO_CONFIG, 0x00);
	nrf_delay_ms(25); // Delay a while to let the device stabilize

	// Retrieve gyro factory Self-Test Code from USR_Reg
	const uint8_t selfTestXGyro = readReg(REG_ST_X_GYRO); // X-axis gyro self-test results
	const uint8_t selfTestYGyro = readReg(REG_ST_Y_GYRO); // Y-axis gyro self-test results
	const uint8_t selfTestZGyro = readReg(REG_ST_Z_GYRO); // Z-axis gyro self-test results

	for (uint8_t i = 0; i < 6; i++) {
		if (selfTest[i] != 0) {
			factoryTrim[i] = mpu6500StTb[selfTest[i] - 1];
		} else {
			factoryTrim[i] = 0;
		}
	}

	// Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
	// To get percent, must multiply by 100
	for (uint8_t i = 0; i < 3; i++) {
		gDiff[i] = 100.0f
				* ((float) ((gSTAvg[i] - gAvg[i]) - factoryTrim[i + 3]))
				/ factoryTrim[i + 3]; // Report percent differences
	}

	// Restore old configuration
	writeReg(REG_SMPLRT_DIV, saveRegSMPLRT_DIV);
	writeReg(REG_CONFIG, saveRegCONFIG);
	writeReg(REG_GYRO_CONFIG, saveRegGYROCONFIG);

	// Check result
	return ((gDiff[0] < MPU6500_ST_GYRO_LOW || gDiff[0] > MPU6500_ST_GYRO_HIGH)
			|| (gDiff[1] < MPU6500_ST_GYRO_LOW
					|| gDiff[1] > MPU6500_ST_GYRO_HIGH)
			|| (gDiff[2] < MPU6500_ST_GYRO_LOW
					|| gDiff[2] > MPU6500_ST_GYRO_HIGH));
}

void initGyro() {

	isSelfTestSuccessful();
}

void deInit() {
}

void setSleepMode() {
}

uint16_t getRoll() {
}

