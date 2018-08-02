#include "humiCtrl.h"
#include "dataProcessing.h"
#include "cmsis_os.h"


#define contactorOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)		//接触器开关
#define contactorClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)

#define drainValveOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)		//排水
#define drainValveClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)

#define inletValveOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)		//进水
#define inletValveClose		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

#define signalRelayOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)		//输出信号继电器
#define signalRelayClose	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)

#define waterLevelWarning	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)						//高水位报警
#define switchSignal		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)						//开关信号

uint8_t nonstopWorkFlag;			//连续工作标志
uint32_t nonstopWorkCount;			//连续工作计数

uint8_t ledBlinkFlagTemp4;			//红绿灯交错闪烁标志
uint8_t ledBlinkFlagTemp8;

uint8_t startLowerLimitCountFlag;	//低电流计数标志
uint16_t lowerLimitCount;			//低电流计数

uint8_t extraDrainWaterFlag;		//外部排水标志
uint16_t extraDrainWaterCount;		//外部排水计数

uint8_t manualDrainWaterFlag;		//手动排水标志
uint16_t manualDrainWaterCount;		//手动排水计数

uint8_t drainWaterFlag;				//排水标志
uint16_t drainWaterCount;			//排水计数

uint8_t overCurrentFlag;			//超电流标志
uint16_t overCurrentCount;			//超电流计数

uint8_t blinkFlag;					//led闪烁标志
uint8_t alarmFlag;					//报警标志

uint8_t waterValveFailureFlag;	//水阀故障标记	无故障:1;有故障:1
uint8_t inletFlag;					//进水标志		进水开：1；进水关：0
uint16_t inletTimeCount;			//进水时间计数

uint8_t allowRunFlagDrainWater;		//允许运行信号，排水相关
uint8_t allowRunFlagProportion;		//允许运行信号，比例相关
uint8_t proportionLessThan5Flag;	//比例信号小于5%标志
uint8_t proportionGreaterThan25Flag;//比例信号大于25%标志


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
static void inletValveOpenWithLimit();
static void manualDrainWaterScan(int s);



/*
		设置电流90%			设置电流110%			基准电流120%			基准电流140%
---------------------------------------------------------------------------------------
			|		排水			|					|					|
	进水		|		进水			|		停止进水		|		排水			|	15s后停止工作
			|		不动作		|					|					|

*/

void humiCtrl() {

	inletFlag = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);	//读取进水阀状态

	if (inletTimeCount > 30*60)			//进水时间计时，超过30分钟，判断为进水阀或出水阀损坏			
	{
		waterValveFailureFlag = 0;
	}

	//printf("inletFlag = %d \n", inletFlag);
	//printf("inletTimeCount = %d \n", inletTimeCount);
	//printf("waterValveFailureFlag = %d \n", waterValveFailureFlag);
	
	if (1 == waterLevelWarning)			//高水位报警
	{
		inletValveClose;
		ledSwitch(1, 1);
		ledSwitch(0, 1);
	}
	
	if (1 == switchSetFlag)				//拨码开关正常初始化
	{
		if (humiMode == PROPORTIONMODE)
		{
			shutOffCurrentLowerLimit = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 0.3;
			startInletCurrent = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 0.9;
			stopInletCurrent = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 1.1;

			if (humiOpening < 50)						//当比例信号低于5%是记录标志位
			{
				proportionLessThan5Flag = 1;
				proportionGreaterThan25Flag = 0;
				allowRunFlagProportion = 0;
			}

			if (humiOpening > 250)						//当比例信号大于25%时记录标志位
			{
				proportionLessThan5Flag = 0;
				proportionGreaterThan25Flag = 1;
				allowRunFlagProportion = 1;
			}

			if ((humiOpening >= 50)&&(humiOpening <= 250))	//当比例信号处于5%和25%之间时，根据上面的标志位来判断开关
			{
				if (1 == proportionLessThan5Flag)
				{
					allowRunFlagProportion = 0;
				}

				if (1 == proportionGreaterThan25Flag)
				{
					allowRunFlagProportion = 1;
				}
			}
		}
		else if (humiMode == SWITCHMODE)
		{
			shutOffCurrentLowerLimit = humiCurrentUpperLimit * powerProportion / 1000 * 0.3;
			startInletCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 0.9;
			stopInletCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 1.1;
		}
/*

		printf("当前电流 = %d \n", humiCurrent);
		printf("能量开度 = %d \n", powerProportion);
		printf("比例开度 = %d \n", humiOpening);
		printf("当前设定电流 = %d \n", humiCurrentUpperLimit);
		printf("关闭低电流 = %d \n", shutOffCurrentLowerLimit);
		printf("关闭高电流 = %d \n", shutOffCurrentTopLimit);
		printf("开始进水电流 = %d \n", startInletCurrent);
		printf("停止进水电流 = %d \n", stopInletCurrent);
		printf("开始排水电流 = %d \n\n", startDrainCurrent);
*/

		//运行需满足四个条件：1.开关信号闭合。2.非排水状态。3.非报警。4.比例模式时，比例值大于25%
		if ((1 == allowRunFlagDrainWater) && (0 == alarmFlag) && (1 == allowRunFlagProportion)&&(1 == waterValveFailureFlag))
		{

			if (1 == switchSignal)
			{
				signalRelayClose;

				if (humiCurrent >= shutOffCurrentTopLimit)			//超过关断电流，关机
				{
					overCurrentFlag = 1;
					if (overCurrentCount > 15)
					{
						overCurrentFlag = 0;
						overCurrentCount = 0;
						//osDelaySecond(15);
						if (humiCurrent >= shutOffCurrentTopLimit)
						{
							humiSuspend();
							alarmFlag = 1;
							ledSwitch(1, 1);
							ledSwitch(0, 0);
						}
					}

					ledSwitch(1, 1);
					ledSwitch(0, 0);

				}
				else if (humiCurrent >= startDrainCurrent)			//超过排水电流，排水
				{
					osDelaySecond(1);
					if (humiCurrent > startDrainCurrent) {
						drainWater(autoDrainWaterTime);				//此处排水该为阻塞式，因为排水时接触器会断开，无电流，会误进入其他状态
					}

					ledSwitch(1, 1);
					ledSwitch(0, 0);
				}

				else if (humiCurrent >= stopInletCurrent)			//停止进水
				{
					inletValveClose;
				}

				else if ((humiCurrent >= startInletCurrent) && (humiCurrent <= startDrainCurrent))//电流在正常工作范围内
				{
					//drainValveClose;
					//inletValveClose;
					contactorOpen;
					ledSwitch(1, 0);
					ledSwitch(0, 1);
					extraDrainWaterFlag = 1;							//开始额外排水计时
					if (extraDrainWaterCount > 20 * 60)					//正常运行20分钟后开始自动排水
					{
						extraDrainWaterCount = 0;
						extraDrainWaterFlag = 0;

						if (extraDrainWaterTime > 0)
						{
							contactorClose;
							inletValveClose;
							drainValveOpen;
							osDelaySecond(extraDrainWaterTime);
							drainValveClose;
							contactorOpen;
						}
					}

					//连续工作600小时后绿灯闪烁,报警继电器吸合
					if (1 == nonstopWorkFlag)
					{
						if (nonstopWorkCount > 600 * 3600) {

							ledBlink(0);
							signalRelayOpen;
						}
					}

				}

				else if (humiCurrent < startInletCurrent)			//电流不足，进水
				{

					drainValveClose;
					inletValveOpenWithLimit();
					contactorOpen;
					if (humiCurrent <= shutOffCurrentLowerLimit)
					{
						startLowerLimitCountFlag = 1;
						if (lowerLimitCount > 30)	//低电流关机 测试为30秒，实际为30*60秒
						{
							startLowerLimitCountFlag = 0;
							lowerLimitCount = 0;
							alarmFlag = 1;
							humiSuspend();
							ledBlink(1);
							ledSwitch(0, 0);
						}
					}
					ledBlink(1);
					ledSwitch(0, 0);
				}
				
				nonstopWorkFlag = 1;
			}
			else
			{
				humiSuspend();
				nonstopWorkFlag = 0;
			}
		}
		else if (1 == alarmFlag)		//有报警信号
		{
			ledSwitch(1, 1);
			ledSwitch(0, 0);
			signalRelayOpen;
			nonstopWorkFlag = 0;
		}
		else if (0 == allowRunFlagProportion)	//外部比例信号过低，停止加湿
		{
			humiSuspend();
		}
		else if(0 == waterValveFailureFlag)		//水阀损坏
		{
			humiSuspend();
			signalRelayOpen;
			ledBlink(1);
			ledSwitch(0, 0);
		}

		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0)
		{
			osDelay(50);
			if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0)
			{
				osDelay(50);
				while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) {}

				if (1 == allowRunFlagDrainWater)
				{
					allowRunFlagDrainWater = 0;
				}
				else {
					allowRunFlagDrainWater = 1;
					humiCtrlInit();
				}
			}
		}

		if (0 == allowRunFlagDrainWater)		//手动排水时，红绿，红绿交错闪烁
		{
			switch (ledBlinkFlagTemp4)
			{
			case 0: ledSwitch(1, 1);
				ledSwitch(0, 0);
				break;
			case 1:	ledSwitch(1, 0);
				ledSwitch(0, 1);
				break;
			case 2:	ledSwitch(1, 1);
				ledSwitch(0, 0);
				break;
			case 3:	ledSwitch(1, 0);
				ledSwitch(0, 1);
				break;
			default:
				break;
			}
		}

		manualDrainWaterScan(1800);	//手动排水的扫描函数。实际值为30分钟，测试用10s。
	}
	else
	{
		switch (ledBlinkFlagTemp8)
		{
		case 0: ledSwitch(1, 1);
			ledSwitch(0, 0);
			break;
		case 1:	ledSwitch(1, 0);
			ledSwitch(0, 0);
			break;
		case 2: ledSwitch(1, 1);
			ledSwitch(0, 0);
			break;
		case 3:	ledSwitch(1, 0);
			ledSwitch(0, 0);
			break;
		case 4: ledSwitch(0, 0);
			ledSwitch(0, 1);
			break;
		case 5:	ledSwitch(0, 0);
			ledSwitch(0, 0);
			break;
		case 6:	ledSwitch(0, 0);
			ledSwitch(0, 1);
			break;
		case 7:	ledSwitch(0, 0);
			ledSwitch(0, 0);
			break;
		default:
			break;
		}
	}
}


static void inletValveOpenWithLimit() {

	if (0 == waterLevelWarning)
	{
		inletValveOpen;
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

	extraDrainWaterFlag = 0;
	startLowerLimitCountFlag = 0;
	lowerLimitCount = 0;
	alarmFlag = 0;
	allowRunFlagDrainWater = 1;
	allowRunFlagProportion = 1;
	waterValveFailureFlag = 1;
	signalRelayClose;
	shutOffCurrentTopLimit = humiCurrentUpperLimit*1.4;
	startDrainCurrent = humiCurrentUpperLimit * 1.2;
}

	

//手动排水
static void manualDrainWaterScan(int s) {

	if (0 == allowRunFlagDrainWater)
	{
		contactorClose;
		inletValveClose;
		drainValveOpen;
		manualDrainWaterFlag = 1;
	}
	//printf("manualDrainWaterCount = %d \n", manualDrainWaterCount);
	if (manualDrainWaterCount > s)
	{
		manualDrainWaterFlag = 0;
		manualDrainWaterCount = 0;
		drainValveClose;
		contactorOpen;
		allowRunFlagDrainWater = 1;
	}
}


//排水 S 秒
static void drainWater(int s) {
	contactorClose;
	inletValveClose;
	drainValveOpen;
	osDelaySecond(s);
	drainValveClose;
	contactorOpen;
}

//洗桶
static void cleanBucket() {

	while (0 == waterLevelWarning) {

		drainValveClose;
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
	drainValveClose;
	greenLedDark();
	ledSwitch(1, 0);
}
