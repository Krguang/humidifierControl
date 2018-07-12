#include "dataProcessing.h"

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


uint16_t humiCurrent;
uint16_t humiOpening;
uint16_t humiOpeningFromPLC;
uint16_t humiCurrentUpperLimit;
uint16_t humiVoltage = 380;
uint16_t powerProportion;

uint16_t extraDrainWaterTime;
uint16_t autoDrainWaterTime;
uint16_t cleanDrainWaterTime;

uint16_t ctrlToDisplayTemp[255];
uint16_t ctrlToPLCTemp[255];

volatile uint16_t ADC_ConvertedValue[3];
uint32_t ADC_Average[3];

//uint8_t humiMode;



static void adcProcesdsing() {

	
	uint32_t a=0, b=0, c=0;

	for (uint8_t i = 0; i < 100; i++)
	{
		for (uint8_t j = 0; j < 3; j++) {
			ADC_Average[j] += ADC_ConvertedValue[j];
		}
	}

	humiOpening = ADC_Average[0] *10/ 4096;
	//humiCurrent = ADC_Average[1] * 8 / 4095;//0-80A对应0-3.3V
	//humiCurrent = ADC_Average[1] * 6 / 5316;//5316=(3.06*1.4/3.3)*4095
	humiCurrent = ADC_Average[1] *6/4096 - 1;//选用39R电阻。0-60A-->0-60ma-->0-0.06*39*1.414V-->0-3.3V-->0-4095
										//0-600 对应 0-4095.
	powerProportion = ADC_Average[2] * (1001 - 250) / 4096 / 100 + 250;

	if (humiOpening < 0)
	{
		humiOpening = 0;
	}

	if (humiCurrent < 0)
	{
		humiCurrent = 0;
	}

	if (powerProportion < 0)
	{
		powerProportion = 0;
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		ADC_Average[i] = 0;
	}
	//printf("humiOpening = %d\n", humiOpening);
}

/*************************************由功率计算出额定电流************************************/
static uint16_t getIFromP(uint16_t p) {

	if (380 == humiVoltage || 400 == humiVoltage)
	{
		if (p < 17500)
		{
			return p*1.732 / 3 / humiVoltage * 10;
		}
		else
		{
			return p*1.732 / 3 / humiVoltage * 10 / 2;		//当功率大于等于17500时，供电线由3根变为6根，单根电流减半
		}
	}

	if (220 == humiVoltage)
	{
		return p / humiVoltage * 10;
	}

}

void dialSwitchInit() {

	//printf("start dialSwitchInit\n");

	/***********************************  控制模式选择  **********************************/

	/*
	if ((readS1Pin5 == 1) && (readS1Pin6 == 1))
	{
		humiMode = PROPORTIONMODE;
	}

	if ((readS1Pin5 == 0) && (readS1Pin6 == 0))
	{
		humiMode = SWITCHMODE;
	}

	if ((readS1Pin5 == 0) && (readS1Pin6 == 1))
	{
		humiMode = COMMUNICATION;
	}
	*/

	/***********************************  加湿电压选择  *************************************/
	if ((readS1Pin3 == 0) && (readS1Pin4 == 0)&& (readS1Pin5 == 0) && (readS1Pin6 == 0))//0000>>120
	{
		humiVoltage = 120;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 0) && (readS1Pin5 == 0) && (readS1Pin6 == 0))//1000>>200
	{
		humiVoltage = 200;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//0100>>208
	{
		humiVoltage = 208;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//1100>>220
	{
		humiVoltage = 220;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//0010>>230
	{
		humiVoltage = 230;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//1010>>240
	{
		humiVoltage = 240;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//0110>>276
	{
		humiVoltage = 276;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//1110>>346
	{
		humiVoltage = 346;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//0001>>380
	{
		humiVoltage = 380;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//1001>>400
	{
		humiVoltage = 400;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//0101>>415
	{
		humiVoltage = 415;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//1101>>440
	{
		humiVoltage = 440;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//0011>>460
	{
		humiVoltage = 460;
	}
	if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//1011>>500
	{
		humiVoltage = 500;
	}

	/***********************************  加湿电流选择  *************************************/
	/*
	*	humiCurrentUpperLimit为实际额定电流*10，方便后面数据处理
	*/
	if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 0) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = getIFromP(1500);
		autoDrainWaterTime = 2;
		cleanDrainWaterTime = 3 * 60;
	}

	if ((readS2Pin2 == 0) && (readS2Pin3 == 1) && (readS2Pin4 == 0) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = getIFromP(2300);
		autoDrainWaterTime = 2;
		cleanDrainWaterTime = 3 * 60;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 0) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = getIFromP(3000);
		autoDrainWaterTime = 8;
		cleanDrainWaterTime = 3 * 60;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 1) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = getIFromP(6100);
		autoDrainWaterTime = 8;
		cleanDrainWaterTime = 3 * 60;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 1) && (readS2Pin5 == 0))
	{
		humiCurrentUpperLimit = getIFromP(9800);
		autoDrainWaterTime = 10;
		cleanDrainWaterTime = 6 * 60;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 0) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = getIFromP(11400);
		autoDrainWaterTime = 10;
		cleanDrainWaterTime = 6 * 60;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 0) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = getIFromP(17500);
		autoDrainWaterTime = 10;
		cleanDrainWaterTime = 6 * 60;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 1) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = getIFromP(24300);
		autoDrainWaterTime = 15;
		cleanDrainWaterTime = 10 * 60;
	}

	if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 1) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = getIFromP(34200);
		autoDrainWaterTime = 15;
		cleanDrainWaterTime = 10 * 60;
	}

	if ((readS2Pin2 == 0) && (readS2Pin3 == 1) && (readS2Pin4 == 1) && (readS2Pin5 == 1))
	{
		humiCurrentUpperLimit = getIFromP(49400);
		autoDrainWaterTime = 20;
		cleanDrainWaterTime = 10 * 60;
	}
	
	//printf("humiCurrentUpperLimit = %d \n", humiCurrentUpperLimit);

/******************************************  额外排水时间  ****************************************/
	if ((readS1Pin1 == 1) && (readS1Pin2 == 1))
	{
		extraDrainWaterTime = 80;
	}

	if ((readS1Pin1 == 0) && (readS1Pin2 == 1))
	{
		extraDrainWaterTime = 30;
	}

	if ((readS1Pin1 == 1) && (readS1Pin2 == 0))
	{
		extraDrainWaterTime = 10;
	}

	if ((readS1Pin1 == 0) && (readS1Pin2 == 0))
	{
		extraDrainWaterTime = 0;
	}
}


void dataProcessing() {

	adcProcesdsing();

	ctrlToDisplayTemp[5] = humiCurrent;
	ctrlToDisplayTemp[6] = humiOpening;
	ctrlToDisplayTemp[7] = powerProportion;

	ctrlToPLCTemp[0] = humiCurrent;
	humiOpeningFromPLC = ctrlToPLCTemp[5];
}