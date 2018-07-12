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
#define signalRelayClose	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)

#define waterLevelWarnning	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)						//��ˮλ����
#define switchSignal		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)						//�����ź�

uint8_t keyStatus[4];

uint8_t startLowerLimitCountFlag;
uint16_t lowerLimitCount;

uint8_t alarmFlag;					//������־
uint8_t allowRunFlagDrainWater;		//���������źţ���ˮ���

uint16_t shutOffCurrentTopLimit;	//�ضϵ�������
uint16_t shutOffCurrentLowerLimit;	//�ضϵ�������
uint16_t startInletCurrent;			//��ʼ��ˮ����
uint16_t startDrainCurrent;			//��ʼ��ˮ����
uint16_t stopInletCurrent;			//ֹͣ��ˮ����


static void keyProcessing();


void osDelaySecond(int s) {
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
	  signalRelayClose;

	  shutOffCurrentTopLimit = humiCurrentUpperLimit*1.414;
	  
	//  printf("shutOffCurrentTopLimit = %d \n", shutOffCurrentTopLimit);
	  //printf("humiOpening = %d \n", humiOpening);
	  //printf("powerProportion = %d \n", powerProportion);
	  //printf("shutOffCurrentLowerLimit = %d \n", shutOffCurrentLowerLimit);
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
static void ledSwitch(uint8_t color,uint8_t statu) {
	//�ر��̵�ʱ��Ҫ�ر�pwm�����

}

/**
* @brief ָʾ����˸
* @param color��0����ɫ��1����ɫ
*/
static void ledBlink(uint8_t color) {
	//�ر��̵�ʱ��Ҫ�ر�pwm�����

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


void humiCtrl() {

	shutOffCurrentLowerLimit = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 0.3;
	startInletCurrent = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 0.9;
	stopInletCurrent = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 1.1;
	startDrainCurrent = humiCurrentUpperLimit*humiOpening / 1000 * powerProportion / 1000 * 1.2;


	printf("humiCurrent = %d \n", humiCurrent);

	//��������������������1.�����źűպϡ�2.����ˮ״̬��3.�Ǳ���
	if ((1 == allowRunFlagDrainWater)&&(0 == alarmFlag))
	{

		if (1 == switchSignal)
		{
			if (humiCurrent >= shutOffCurrentTopLimit)			//�����ضϵ������ػ�
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
			else if (humiCurrent >= startDrainCurrent)			//������ˮ��������ˮ
			{
				osDelaySecond(1);
				if (humiCurrent >= startDrainCurrent) {
					drainWater(autoDrainWaterTime);
				}
			}
			else if ((humiCurrent <= startInletCurrent)&&(humiCurrent >= shutOffCurrentLowerLimit))//����������������Χ��
			{
				drianValveClose;
				inletValveClose;
				contactorOpen;
				ledSwitch(1, 0);
				ledSwitch(0, 1);
			}
			else if (humiCurrent <= shutOffCurrentLowerLimit)	//С�ڹضϵ�������ʼ��ʱ��30���Ӻ�ػ�������
			{
				startLowerLimitCountFlag = 1;
				if (lowerLimitCount > 30)	//����Ϊ30�룬ʵ��Ϊ30����
				{
					startLowerLimitCountFlag = 0;
					lowerLimitCount = 0;
					alarmFlag = 1;
					humiSuspend();
					signalRelayOpen;
				}
				else
				{
					drianValveClose;
					inletValveClose;
					contactorOpen;
				}
				ledBlink(1);
				ledSwitch(0, 0);
			}
		}
		else
		{
			humiSuspend();
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

