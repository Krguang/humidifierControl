#include "dataProcessing.h"

uint8_t keyStatus[4];
uint16_t humiCurrent;
uint16_t humiOpening;

uint16_t ctrlToDisplayTemp[255];
uint16_t ctrlToPLCTemp[255];

volatile uint16_t ADC_ConvertedValue[3];
uint32_t ADC_Average[3];

uint16_t microAdjust;

static void adcProcesdsing() {

	uint32_t a=0, b=0, c=0;

	for (uint8_t i = 0; i < 100; i++)
	{
		for (uint8_t j = 0; j < 3; j++) {
			ADC_Average[j] += ADC_ConvertedValue[j];
		}
	}

	humiOpening = ADC_Average[0] *10/ 4096;
	humiCurrent = ADC_Average[1] *6/5316;//5316=(3.06*1.4/3.3)*4095
	microAdjust = ADC_Average[2] *33/ 4096;

	for (uint8_t i = 0; i < 3; i++)
	{
		ADC_Average[i] = 0;
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

	ctrlToDisplayTemp[5] = humiCurrent;
	ctrlToDisplayTemp[6] = humiOpening;

	ctrlToPLCTemp[0] = humiCurrent;
	humiOpening = ctrlToPLCTemp[5];
}