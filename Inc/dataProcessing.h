#pragma once
#include "stm32f1xx_hal.h"
#include "main.h"

extern uint16_t humiCurrent;
extern uint16_t humiOpening;
extern uint16_t ctrlToDisplayTemp[];
extern uint16_t ctrlToPLCTemp[];

extern volatile uint16_t ADC_ConvertedValue[];
extern uint32_t ADC_Average[];
extern uint16_t weiTiao;

void dataProcessing(void);