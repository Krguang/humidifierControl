#include "humiCtrl.h"
#include "dataProcessing.h"
#include "cmsis_os.h"


#define contactorOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)		//�Ӵ�������
#define contactorClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)

#define drainValveOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)		//��ˮ
#define drainValveClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)

#define inletValveOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)		//��ˮ
#define inletValveClose		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

#define signalRelayOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)		//����źż̵���
#define signalRelayClose	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)

#define waterLevelWarning	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)						//��ˮλ����
#define switchSignal		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)						//�����ź�

uint8_t nonstopWorkFlag;			//����������־
uint32_t nonstopWorkCount;			//������������

uint8_t ledBlinkFlagTemp4;			//���̵ƽ�����˸��־
uint8_t ledBlinkFlagTemp8;

uint8_t startLowerLimitCountFlag;	//�͵���������־
uint16_t lowerLimitCount;			//�͵�������

uint8_t extraDrainWaterFlag;		//�ⲿ��ˮ��־
uint16_t extraDrainWaterCount;		//�ⲿ��ˮ����

uint8_t manualDrainWaterFlag;		//�ֶ���ˮ��־
uint16_t manualDrainWaterCount;		//�ֶ���ˮ����

uint8_t drainWaterFlag;				//��ˮ��־
uint16_t drainWaterCount;			//��ˮ����

uint8_t overCurrentFlag;			//��������־
uint16_t overCurrentCount;			//����������

uint8_t blinkFlag;					//led��˸��־
uint8_t alarmFlag;					//������־

uint8_t waterValveFailureFlag;	//ˮ�����ϱ��	�޹���:1;�й���:1
uint8_t inletFlag;					//��ˮ��־		��ˮ����1����ˮ�أ�0
uint16_t inletTimeCount;			//��ˮʱ�����

uint8_t allowRunFlagDrainWater;		//���������źţ���ˮ���
uint8_t allowRunFlagProportion;		//���������źţ��������
uint8_t proportionLessThan5Flag;	//�����ź�С��5%��־
uint8_t proportionGreaterThan25Flag;//�����źŴ���25%��־


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



/*
		���õ���90%			���õ���110%			��׼����120%			��׼����140%
---------------------------------------------------------------------------------------
			|		��ˮ			|					|					|
	��ˮ		|		��ˮ			|		ֹͣ��ˮ		|		��ˮ			|	15s��ֹͣ����
			|		������		|					|					|

*/

void humiCtrl() {

	inletFlag = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);	//��ȡ��ˮ��״̬

	if (inletTimeCount > 30*60)			//��ˮʱ���ʱ������30���ӣ��ж�Ϊ��ˮ�����ˮ����			
	{
		waterValveFailureFlag = 0;
	}

	//printf("inletFlag = %d \n", inletFlag);
	//printf("inletTimeCount = %d \n", inletTimeCount);
	//printf("waterValveFailureFlag = %d \n", waterValveFailureFlag);
	
	if (1 == waterLevelWarning)			//��ˮλ����
	{
		inletValveClose;
		ledSwitch(1, 1);
		ledSwitch(0, 1);
	}
	
	if (1 == switchSetFlag)				//���뿪��������ʼ��
	{
		if (humiMode == PROPORTIONMODE)
		{
			shutOffCurrentLowerLimit = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 0.3;
			startInletCurrent = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 0.9;
			stopInletCurrent = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 1.1;

			if (humiOpening < 50)						//�������źŵ���5%�Ǽ�¼��־λ
			{
				proportionLessThan5Flag = 1;
				proportionGreaterThan25Flag = 0;
				allowRunFlagProportion = 0;
			}

			if (humiOpening > 250)						//�������źŴ���25%ʱ��¼��־λ
			{
				proportionLessThan5Flag = 0;
				proportionGreaterThan25Flag = 1;
				allowRunFlagProportion = 1;
			}

			if ((humiOpening >= 50)&&(humiOpening <= 250))	//�������źŴ���5%��25%֮��ʱ����������ı�־λ���жϿ���
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

		printf("��ǰ���� = %d \n", humiCurrent);
		printf("�������� = %d \n", powerProportion);
		printf("�������� = %d \n", humiOpening);
		printf("��ǰ�趨���� = %d \n", humiCurrentUpperLimit);
		printf("�رյ͵��� = %d \n", shutOffCurrentLowerLimit);
		printf("�رոߵ��� = %d \n", shutOffCurrentTopLimit);
		printf("��ʼ��ˮ���� = %d \n", startInletCurrent);
		printf("ֹͣ��ˮ���� = %d \n", stopInletCurrent);
		printf("��ʼ��ˮ���� = %d \n\n", startDrainCurrent);
*/

		//�����������ĸ�������1.�����źűպϡ�2.����ˮ״̬��3.�Ǳ�����4.����ģʽʱ������ֵ����25%
		if ((1 == allowRunFlagDrainWater) && (0 == alarmFlag) && (1 == allowRunFlagProportion)&&(1 == waterValveFailureFlag))
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
				else if (humiCurrent >= startDrainCurrent)			//������ˮ��������ˮ
				{
					osDelaySecond(1);
					if (humiCurrent > startDrainCurrent) {
						drainWater(autoDrainWaterTime);				//�˴���ˮ��Ϊ����ʽ����Ϊ��ˮʱ�Ӵ�����Ͽ����޵����������������״̬
					}

					ledSwitch(1, 1);
					ledSwitch(0, 0);
				}

				else if (humiCurrent >= stopInletCurrent)			//ֹͣ��ˮ
				{
					inletValveClose;
				}

				else if ((humiCurrent >= startInletCurrent) && (humiCurrent <= startDrainCurrent))//����������������Χ��
				{
					//drainValveClose;
					//inletValveClose;
					contactorOpen;
					ledSwitch(1, 0);
					ledSwitch(0, 1);
					extraDrainWaterFlag = 1;							//��ʼ������ˮ��ʱ
					if (extraDrainWaterCount > 20 * 60)					//��������20���Ӻ�ʼ�Զ���ˮ
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

					//��������600Сʱ���̵���˸,�����̵�������
					if (1 == nonstopWorkFlag)
					{
						if (nonstopWorkCount > 600 * 3600) {

							ledBlink(0);
							signalRelayOpen;
						}
					}

				}

				else if (humiCurrent < startInletCurrent)			//�������㣬��ˮ
				{

					drainValveClose;
					inletValveOpenWithLimit();
					contactorOpen;
					if (humiCurrent <= shutOffCurrentLowerLimit)
					{
						startLowerLimitCountFlag = 1;
						if (lowerLimitCount > 30)	//�͵����ػ� ����Ϊ30�룬ʵ��Ϊ30*60��
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
		else if (1 == alarmFlag)		//�б����ź�
		{
			ledSwitch(1, 1);
			ledSwitch(0, 0);
			signalRelayOpen;
			nonstopWorkFlag = 0;
		}
		else if (0 == allowRunFlagProportion)	//�ⲿ�����źŹ��ͣ�ֹͣ��ʪ
		{
			humiSuspend();
		}
		else if(0 == waterValveFailureFlag)		//ˮ����
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

		if (0 == allowRunFlagDrainWater)		//�ֶ���ˮʱ�����̣����̽�����˸
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

		manualDrainWaterScan(1800);	//�ֶ���ˮ��ɨ�躯����ʵ��ֵΪ30���ӣ�������10s��
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

//��ʪ���ݳ�ʼ��
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

	

//�ֶ���ˮ
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


//��ˮ S ��
static void drainWater(int s) {
	contactorClose;
	inletValveClose;
	drainValveOpen;
	osDelaySecond(s);
	drainValveClose;
	contactorOpen;
}

//ϴͰ
static void cleanBucket() {

	while (0 == waterLevelWarning) {

		drainValveClose;
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
	drainValveClose;
	greenLedDark();
	ledSwitch(1, 0);
}
