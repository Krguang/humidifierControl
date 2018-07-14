#include "humiCtrl.h"
#include "dataProcessing.h"
#include "cmsis_os.h"


#define contactorOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)		//�Ӵ�������
#define contactorClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)

#define drianValveOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)		//��ˮ
#define drianValveClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)

#define inletValveOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)		//��ˮ
#define inletValveClose		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

#define signalRelayOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)		//����źż̵���
#define signalRelayClose	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)

#define waterLevelWarnning	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)						//��ˮλ����
#define switchSignal		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)						//�����ź�

uint8_t keyStatus[4];

uint8_t startLowerLimitCountFlag;
uint16_t lowerLimitCount;

uint8_t manualDrainWaterFlag;
uint16_t manualDrainWaterCount;
uint8_t drainWaterFlag;
uint16_t drainWaterCount;
uint8_t overCurrentFlag;
uint16_t overCurrentCount;
uint8_t blinkFlag;
uint8_t alarmFlag;					//������־
uint8_t allowRunFlagDrainWater;		//���������źţ���ˮ���
uint8_t allowRunFlagProportion;		//���������źţ��������

uint16_t shutOffCurrentTopLimit;	//�ضϵ�������
uint16_t shutOffCurrentLowerLimit;	//�ضϵ�������
uint16_t startInletCurrent;			//��ʼ��ˮ����
uint16_t startDrainCurrent;			//��ʼ��ˮ����
uint16_t stopInletCurrent;			//ֹͣ��ˮ����


static void osDelaySecond(int s);
static void drainWater(int s);
static void cleanBucket();
static void ledSwitch(uint8_t color, uint8_t statu);
static void ledBlink(uint8_t color);
static void greenLedDark();
static void humiSuspend();
static void inletValveOpenWithLimit();
static void manualDrainWaterScan(int s);


void humiCtrl() {

	//printf("humiCurrent = %d \n", humiCurrent);
	//printf("humiCurrentUpperLimit = %d \n", humiCurrentUpperLimit);
	//printf("shutOffCurrentTopLimit = %d \n", shutOffCurrentTopLimit);
	//printf("shutOffCurrentLowerLimit = %d \n", shutOffCurrentLowerLimit);
	//printf("startInletCurrent = %d \n", startInletCurrent);
	//printf("startDrainCurrent = %d \n", startDrainCurrent);
	//printf("stopInletCurrent = %d \n", stopInletCurrent);

	if (1 == waterLevelWarnning)
	{
		inletValveClose;
	}
	
	if (humiMode == PROPORTIONMODE)
	{
		shutOffCurrentLowerLimit = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 0.3;
		startInletCurrent = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 0.9;
		stopInletCurrent = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 1.1;
		startDrainCurrent = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 1.2;

		if (humiOpening < 250)				//�����ڱ���ģʽʱ�������ź�С��25%��������
		{
			allowRunFlagProportion = 0;
		}
		else
		{
			allowRunFlagProportion = 1;
		}
	}
	else if (humiMode == SWITCHMODE)
	{
		shutOffCurrentLowerLimit = humiCurrentUpperLimit * powerProportion / 1000 * 0.3;
		startInletCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 0.9;
		stopInletCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 1.1;
		startDrainCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 1.2;
	}
	

	//�����������ĸ�������1.�����źűպϡ�2.����ˮ״̬��3.�Ǳ�����4.����ģʽʱ������ֵ����25%
	if ((1 == allowRunFlagDrainWater)&&(0 == alarmFlag)&&(1 == allowRunFlagProportion))
	{

		if (1 == switchSignal)
		{
			signalRelayClose;

			if (humiCurrent >= shutOffCurrentTopLimit)			//�����ضϵ������ػ�
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
			else if (humiCurrent > startDrainCurrent)			//������ˮ��������ˮ
			{
				osDelaySecond(1);
				if (humiCurrent > startDrainCurrent) {
					drainWater(autoDrainWaterTime);				//�˴���ˮ��Ϊ����ʽ����Ϊ��ˮʱ�Ӵ�����Ͽ����޵����������������״̬
				}

				ledSwitch(1, 1);
				ledSwitch(0, 0);
			}

			else if (humiCurrent < startInletCurrent)			//�������㣬��ˮ
			{

				drianValveClose;
				inletValveOpenWithLimit();
				contactorOpen;
				if (humiCurrent <= shutOffCurrentLowerLimit)
				{
					startLowerLimitCountFlag = 1;
					if (lowerLimitCount > 30)	//����Ϊ30�룬ʵ��Ϊ30*60��
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
			else if ((humiCurrent >= startInletCurrent)&&(humiCurrent <= startDrainCurrent))//����������������Χ��
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
	else if (1 == alarmFlag)
	{
		ledSwitch(1, 1);
		ledSwitch(0, 0);
		signalRelayOpen;
	}

	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0)
	{
		osDelay(20);
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0)
		{
			osDelay(20);
			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0){}

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

	
	

	//switch (ucKeySec) //��������״̬�л�
	//{
	//case 1:// 1 �ż��Ķ̰�
	//	allowRunFlagDrainWater ^= 1;
	//	ucKeySec = 0; //��Ӧ�������������󣬰���������㣬����һ�´���
	//	break;
	//case 2:// 1 �ż��ĳ���
	//	humiCtrlInit();
	//	ucKeySec = 0; //��Ӧ�������������󣬰���������㣬����һ�´���
	//	break;
	//}

	
	manualDrainWaterScan(1800);	//ʵ��ֵΪcleanDrainWaterTime��������10s��
}


static void inletValveOpenWithLimit() {

	if (0 == waterLevelWarnning)
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

//��ʪ���ݳ�ʼ��
void humiCtrlInit() {

	startLowerLimitCountFlag = 0;
	lowerLimitCount = 0;
	alarmFlag = 0;
	allowRunFlagDrainWater = 1;
	allowRunFlagProportion = 1;
	signalRelayClose;
	shutOffCurrentTopLimit = humiCurrentUpperLimit*1.414;

}

	


static void manualDrainWaterScan(int s) {

	if (0 == allowRunFlagDrainWater)
	{
		contactorClose;
		inletValveClose;
		drianValveOpen;
		manualDrainWaterFlag = 1;
	}
	//printf("manualDrainWaterCount = %d \n", manualDrainWaterCount);
	if (manualDrainWaterCount > s)
	{
		manualDrainWaterFlag = 0;
		manualDrainWaterCount = 0;
		drianValveClose;
		contactorOpen;
		allowRunFlagDrainWater = 1;
	}
}


//��ˮ S ��
static void drainWater(int s) {
	contactorClose;
	inletValveClose;
	drianValveOpen;
	osDelaySecond(s);
	drianValveClose;
	contactorOpen;
}

//ϴͰ
static void cleanBucket() {

	while (0 == waterLevelWarnning) {

		drianValveClose;
		inletValveOpen;
		contactorOpen;
	}
	drainWater(cleanDrainWaterTime);
}


/**
* @brief ָʾ�ƿ���
* @param color��0����ɫ��1����ɫ
* @param statu: 0:�رա� 1����
*/
static void ledSwitch(uint8_t color, uint8_t statu) {
	//�ر��̵�ʱ��Ҫ�ر�pwm�����
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
* @brief ָʾ����˸
* @param color��0����ɫ��1����ɫ
*/
static void ledBlink(uint8_t color) {
	//�ر��̵�ʱ��Ҫ�ر�pwm�����
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


//�̵ư���ר���ڴ���
static void greenLedDark() {


}


//����
static void humiSuspend() {

	contactorClose;
	inletValveClose;
	drianValveClose;
	greenLedDark();
	ledSwitch(1, 0);
}
