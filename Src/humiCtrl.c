#include "humiCtrl.h"
#include "dataProcessing.h"
#include "cmsis_os.h"


#define contactorOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)		//接触器开关
#define contactorClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)

#define drianValveOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)		//排水
#define drianValveClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)

#define inletValveOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)		//进水
#define inletValveClose		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

#define signalRelayOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)		//输出信号继电器
#define signalRelayClose	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)

#define waterLevelWarnning	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)						//高水位报警
#define switchSignal		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)						//开关信号

uint8_t keyStatus[4];

uint8_t startLowerLimitCountFlag;
uint16_t lowerLimitCount;

uint8_t blinkFlag;
uint8_t alarmFlag;					//报警标志
uint8_t allowRunFlagDrainWater;		//允许运行信号，排水相关

uint16_t shutOffCurrentTopLimit;	//关断电流上限
uint16_t shutOffCurrentLowerLimit;	//关断电流下限
uint16_t startInletCurrent;			//开始进水电流
uint16_t startDrainCurrent;			//开始排水电流
uint16_t stopInletCurrent;			//停止进水电流


static void osDelaySecond(int s);
static void drainWater(int s);
static void cleanBucket();
static void ledSwitch(uint8_t color, uint8_t statu);
static void ledBlink(uint8_t color);
static void greenLedDark();
static void humiSuspend();


void humiCtrl() {


	if (humiMode == PROPORTIONMODE)
	{
		shutOffCurrentLowerLimit = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 0.3;
		startInletCurrent = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 0.9;
		stopInletCurrent = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 1.1;
		startDrainCurrent = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 1.2;
	}
	else if (humiMode == SWITCHMODE)
	{
		shutOffCurrentLowerLimit = humiCurrentUpperLimit * powerProportion / 1000 * 0.3;
		startInletCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 0.9;
		stopInletCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 1.1;
		startDrainCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 1.2;
	}
	

	//运行需满足四个条件：1.开关信号闭合。2.非排水状态。3.非报警。
	if ((1 == allowRunFlagDrainWater)&&(0 == alarmFlag))
	{

		if (1 == switchSignal)
		{
			if (humiCurrent >= shutOffCurrentTopLimit)			//超过关断电流，关机
			{
				osDelaySecond(15);
				if (humiCurrent >= shutOffCurrentTopLimit)
				{
					humiSuspend();
					signalRelayOpen;
					alarmFlag = 1;
					ledSwitch(1, 1);
					ledSwitch(0, 0);
				}
			}
			else if (humiCurrent >= startDrainCurrent)			//超过排水电流，排水
			{
				osDelaySecond(1);
				if (humiCurrent >= startDrainCurrent) {
					drainWater(autoDrainWaterTime);
				}
			}
			else if (humiCurrent <= startInletCurrent)
			{

				drianValveClose;
				inletValveOpen;
				contactorOpen;
				if (humiCurrent <= shutOffCurrentLowerLimit)
				{
					startLowerLimitCountFlag = 1;
					if (lowerLimitCount > 30)	//测试为30秒，实际为30*60秒
					{
						startLowerLimitCountFlag = 0;
						lowerLimitCount = 0;
						alarmFlag = 1;
						humiSuspend();
						signalRelayOpen;
					}
				}
				ledBlink(1);
				ledSwitch(0, 0);
			}
			else if ((humiCurrent <= startInletCurrent)&&(humiCurrent >= shutOffCurrentLowerLimit))//电流在正常工作范围内
			{
				drianValveClose;
				inletValveClose;
				contactorOpen;
				ledSwitch(1, 0);
				ledSwitch(0, 1);
			}
		}
		else
		{
			humiSuspend();
		}
	}
}


static void osDelaySecond(int s) {
	for (int i = 0; i < s; i++)
	{
		osDelay(1000);
	}
}

//加湿数据初始化
void humiCtrlInit() {

	startLowerLimitCountFlag = 0;
	lowerLimitCount = 0;
	alarmFlag = 0;
	allowRunFlagDrainWater = 1;
	signalRelayClose;

	shutOffCurrentTopLimit = humiCurrentUpperLimit*1.414;
}


//排水 S 秒
static void drainWater(int s) {
	contactorClose;
	inletValveClose;
	drianValveOpen;
	osDelaySecond(s);
	drianValveClose;
	contactorOpen;
}

//洗桶
static void cleanBucket() {

	while (0 == waterLevelWarnning) {

		drianValveClose;
		inletValveOpen;
		contactorOpen;
	}
	drainWater(cleanDrainWaterTime);
}


/**
* @brief 指示灯开关
* @param color：0：绿色。1：红色
* @param statu: 0:关闭。 1：打开
*/
static void ledSwitch(uint8_t color, uint8_t statu) {
	//关闭绿灯时，要关闭pwm输出。
	if (0 == color)
	{
		if (1 == statu)
		{
			HAL_GPIO_WritePin(green_led_GPIO_Port, green_led_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(green_led_GPIO_Port, green_led_Pin, GPIO_PIN_RESET);
		}
	}
	else if (1 == color)
	{
		if (1 == statu)
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
		}
	}
}

/**
* @brief 指示灯闪烁
* @param color：0：绿色。1：红色
*/
static void ledBlink(uint8_t color) {
	//关闭绿灯时，要关闭pwm输出。
	if (0 == color)
	{
		if (1 == blinkFlag)
		{
			HAL_GPIO_WritePin(green_led_GPIO_Port, green_led_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(green_led_GPIO_Port, green_led_Pin, GPIO_PIN_RESET);
		}
	}
	else if (1 == color)
	{
		if (1 == blinkFlag)
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
		}
	}
}


//绿灯暗，专用于待机
static void greenLedDark() {


}


//待机
static void humiSuspend() {

	contactorClose;
	inletValveClose;
	drianValveClose;
	greenLedDark();
	ledSwitch(1, 0);
}
