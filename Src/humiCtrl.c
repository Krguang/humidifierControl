#include "humiCtrl.h"
#include "dataProcessing.h"
#include "cmsis_os.h"

#define contactorOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)
#define contactorClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)

#define drianValveOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)
#define drianValveClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)

#define inletValveOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)
#define inletValveClose		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

#define signalRelayOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)
#define signalRelayClose	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)

uint8_t keyStatus[4];
uint16_t drainWaterShortTime;
uint16_t drainWaterLongTime;



static void keyProcessing();


void osDelaySecond(int s) {
	for (int i = 0; i < s; i++)
	{
		osDelay(1000);
	}
}

void humiCtrlInit() {

	drianValveClose;
	inletValveOpen;
	contactorOpen;
	
}

static void drainWater(int s) {
	contactorClose;
	drianValveOpen;
	osDelaySecond(s);
	drianValveClose;
	contactorOpen;
}


void humiCtrl() {

	if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4) == 1)
	{
		inletValveClose;
	}
	else {
		if (humiCurrent <= humiCurrentUpperLimit)
		{
			return;
		}
		if ((humiCurrent > humiCurrentUpperLimit) && (humiCurrent < (humiCurrentUpperLimit*1.4)))
		{
			drainWater(drainWaterShortTime);
		}
		if (humiCurrent >= humiCurrentUpperLimit*1.4)
		{
			drainWater(drainWaterLongTime);
		}
	}

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

	if (keyStatus[0] != keyTemp[0])
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