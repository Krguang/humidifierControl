#pragma once
#include "stm32f1xx_hal.h"
#include "main.h"

#define PROPORTIONMODE	0
#define SWITCHMODE		1
#define COMMUNICATION	2

extern uint16_t humiCurrent;
extern uint16_t humiOpening;
extern uint16_t humiOpeningFromPLC;
extern uint16_t powerProportion;
extern uint16_t humiCurrentUpperLimit;
extern uint16_t ctrlToDisplayTemp[];
extern uint16_t ctrlToPLCTemp[];
extern uint8_t humiMode;

extern volatile uint16_t ADC_ConvertedValue[];
extern uint32_t ADC_Average[];


void dialSwitchInit();
void dataProcessing(void);