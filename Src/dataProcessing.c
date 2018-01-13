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
uint16_t humiCurrentUpperLimit;
uint64_t humiVoltage;
uint16_t powerProportion;

uint16_t drainWaterTime;


uint16_t ctrlToDisplayTemp[255];
uint16_t ctrlToPLCTemp[255];

volatile uint16_t ADC_ConvertedValue[3];
uint32_t ADC_Average[3];

struct 
{
	uint8_t switchMode;
	uint8_t ProportionMode;
	uint8_t communicationMode;

}HumiMode;



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


static void dialSwitch() {

	/***********************************  控制模式选择  **********************************/
	if ((readS1Pin5 == 1) && (readS1Pin6 == 1))
	{
		HumiMode.ProportionMode = 1;
		HumiMode.switchMode = 0;
		HumiMode.communicationMode = 0;
	}

	if ((readS1Pin5 == 0) && (readS1Pin6 == 0))
	{
		HumiMode.ProportionMode = 0;
		HumiMode.switchMode = 1;
		HumiMode.communicationMode = 0;
	}

	if ((readS1Pin5 == 0) && (readS1Pin6 == 1))
	{
		HumiMode.ProportionMode = 0;
		HumiMode.switchMode = 0;
		HumiMode.communicationMode = 1;
	}


	/***********************************  加湿电压选择  *************************************/
	if ((readS1Pin1 == 1) && (readS1Pin2 == 1))
	{
		humiVoltage = 380;
	}

	if ((readS1Pin1 == 0) && (readS1Pin2 == 0))
	{
		humiVoltage = 220;
	}

	if ((readS1Pin1 == 0) && (readS1Pin2 == 1))
	{
		humiVoltage = 440;
	}

	/***********************************  加湿电流选择  *************************************/
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


void dataProcessing() {

	adcProcesdsing();
	dialSwitch();

	ctrlToDisplayTemp[5] = humiCurrent;
	ctrlToDisplayTemp[6] = humiOpening;
	ctrlToDisplayTemp[7] = powerProportion;

	ctrlToPLCTemp[0] = humiCurrent;
	humiOpening = ctrlToPLCTemp[5];
}