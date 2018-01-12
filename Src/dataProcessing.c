#include "dataProcessing.h"

#define contactorOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)
#define contactorClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)

#define drianValveOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)
#define drianValveClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)

#define inletValveOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)
#define inletValveClose		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

#define signalRelayOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)
#define signalRelayClose	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)

#define readS2Pin1			HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)
#define readS2Pin2			HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)
#define readS2Pin3			HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)
#define readS2Pin4			HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)
#define readS2Pin5			HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)
#define readS2Pin6			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)

#define readS1Pin1			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
#define readS1Pin2			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)
#define readS1Pin3			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)
#define readS1Pin4			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)
#define readS1Pin5			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)
#define readS1Pin6			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)

uint8_t keyStatus[4];
uint16_t humiCurrent;
uint16_t humiOpening;
uint16_t humiCurrentUpperLimit;
uint16_t powerProportion;

uint16_t drainWaterTime;


uint16_t ctrlToDisplayTemp[255];
uint16_t ctrlToPLCTemp[255];

volatile uint16_t ADC_ConvertedValue[3];
uint32_t ADC_Average[3];

static void adcProcesdsing() {

	
	uint32_t a=0, b=0, c=0;

	for (uint8_t i = 0; i < 100; i++)
	{
		for (uint8_t j = 0; j < 3; j++) {
			ADC_Average[j] += ADC_ConvertedValue[j];
		}
	}

	humiOpening = ADC_Average[0] *10/ 4095;
	humiCurrent = ADC_Average[1] * 8 / 4095;//0-80A对应0-3.3V
	powerProportion = ADC_Average[2] * (1001 - 250) / 4095 / 100 + 250;

	for (uint8_t i = 0; i < 3; i++)
	{
		ADC_Average[i] = 0;
	}
	
}

static dialSwitch() {
	/******************************  加湿电流  *****************************/
	if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 0) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = 2 * 11;
	}

	if ((readS2Pin2 == 0) && (readS2Pin3 == 1) && (readS2Pin4 == 0) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = 3 * 11;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 0) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = 4 * 11;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 1) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = 8 * 11;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 1) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = 13 * 11;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 0) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = 15 * 11;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 0) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = 23 * 11;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 1) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = 32 * 11;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 1) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = 45 * 11;
	}

	if ((readS2Pin2 == 0) && (readS2Pin3 == 1) && (readS2Pin4 == 1) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = 65 * 11;
	}
	
/******************************************  额外排水时间  ****************************************/
	if ((readS1Pin1 == 1) && (readS1Pin2 == 1))
	{
		drainWaterTime = 80;
	}

	if ((readS1Pin1 == 0) && (readS1Pin2 == 1))
	{
		drainWaterTime = 30;
	}

	if ((readS1Pin1 == 1) && (readS1Pin2 == 0))
	{
		drainWaterTime = 10;
	}

	if ((readS1Pin1 == 0) && (readS1Pin2 == 0))
	{
		drainWaterTime = 0;
	}
	

}

static void relayCtrl() {

}

static void keyProcessing() {

	static uint8_t keyTemp[] = { 0,0,0,0 };

	if (ctrlToDisplayTemp[0] & (0x0001 << 0))
	{
		keyStatus[0] = 1;
	}
	else {
		keyStatus[0] = 0;
	}

	if (ctrlToDisplayTemp[0] & (0x0001 << 1))
	{
		keyStatus[1] = 1;
	}
	else {
		keyStatus[1] = 0;
	}

	if (ctrlToDisplayTemp[0] & (0x0001 << 2))
	{
		keyStatus[2] = 1;
	}
	else {
		keyStatus[2] = 0;
	}

	if (ctrlToDisplayTemp[0] & (0x0001 << 3))
	{
		keyStatus[3] = 1;
	}
	else {
		keyStatus[3] = 0;
	}

	if (keyStatus[0]!=keyTemp[0])
	{
		keyStatus[0] = keyTemp[0];
		//do something
	}

	if (keyStatus[1] != keyTemp[1])
	{
		keyStatus[1] = keyTemp[1];
		//do something
	}

	if (keyStatus[2] != keyTemp[2])
	{
		keyStatus[2] = keyTemp[2];
		//do something
	}

	if (keyStatus[3] != keyTemp[3])
	{
		keyStatus[3] = keyTemp[3];
		//do something
	}
}

void dataProcessing() {

	keyProcessing();
	adcProcesdsing();
	dialSwitch();
	relayCtrl();
	ctrlToDisplayTemp[5] = humiCurrent;
	ctrlToDisplayTemp[6] = humiOpening;
	ctrlToDisplayTemp[7] = powerProportion;

	ctrlToPLCTemp[0] = humiCurrent;
	humiOpening = ctrlToPLCTemp[5];
}