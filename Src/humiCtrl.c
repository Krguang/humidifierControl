#include "humiCtrl.h"
#include "dataProcessing.h"
#include "cmsis_os.h"

#define contactorOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)		//接触器开关
#define contactorClose		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

#define drianValveOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)		//排水
#define drianValveClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)

#define inletValveOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)		//进水
#define inletValveClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)

#define signalRelayOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)		//输出信号继电器
#define signalRelayClose	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)

#define waterLevelWarnning	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)
#define sitchSignal			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)

uint8_t keyStatus[4];
uint16_t drainWaterShortTime;
uint16_t drainWaterLongTime;
uint8_t forcedToCloseInletValve;
uint16_t finalCurrentLimit;


static void keyProcessing();


void osDelaySecond(int s) {
	for (int i = 0; i < s; i++)
	{
		osDelay(1000);
	}
}


void humiCtrlInit() {

	drainWaterShortTime = humiCurrentUpperLimit / 11;
	drainWaterLongTime = humiCurrentUpperLimit;

	contactorClose;
	drianValveClose;
	inletValveOpen;
	osDelaySecond(drainWaterShortTime);
	inletValveClose;
	drianValveOpen;
	osDelaySecond(drainWaterShortTime);
	drianValveClose;
	inletValveOpen;
	contactorOpen;
}


static void inletValveOpenWithCondition(){
	if (forcedToCloseInletValve != 1)
	{
		inletValveOpen;
	}
}

static void drainValveOpenWithCondition() {
	drianValveOpen;
	forcedToCloseInletValve = 1;
}

static void drainValveCloseWithCondition() {
	drianValveClose;
	forcedToCloseInletValve = 0;
}



static void drainWater(int s) {
	contactorClose;
	inletValveClose;
	drainValveOpenWithCondition();
	osDelaySecond(s);
	drainValveCloseWithCondition();
	contactorOpen;
}

void humiCtrl() {

	if (humiMode == PROPORTIONMODE)
	{
		finalCurrentLimit = humiCurrentUpperLimit*humiOpening / 1000;
	}
	else if (humiMode == SWITCHMODE)
	{
		finalCurrentLimit = humiCurrentUpperLimit*powerProportion / 1000;
	}
	else if (humiMode == COMMUNICATION)
	{
		finalCurrentLimit = humiCurrentUpperLimit*humiOpeningFromPLC / 1000;
	}
	
	printf("humiCurrentUpperLimit = %d\n", humiCurrentUpperLimit);
	printf("finalCurrentLimit = %d\n", finalCurrentLimit);

	if (sitchSignal == 1)
	{
		if (waterLevelWarnning == 1)		//水位报警
		{
			inletValveClose;
			forcedToCloseInletValve = 1;
		}
		else {
			forcedToCloseInletValve = 0;
		}

		if (finalCurrentLimit >= 10)					//传入比例信号为0，关闭三个继电器
		{
			if (humiCurrent <= finalCurrentLimit)
			{
				inletValveOpenWithCondition();
				contactorOpen;
			}
			if ((humiCurrent > finalCurrentLimit) && (humiCurrent < (finalCurrentLimit*1.4)))
			{
				drainWater(drainWaterShortTime);
			}
			if (humiCurrent >= finalCurrentLimit*1.4)
			{
				drainWater(drainWaterLongTime);
			}
		}
		else
		{
			contactorClose;
			inletValveClose;
			drianValveClose;
		}
	}
	else
	{
		contactorClose;
		inletValveClose;
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