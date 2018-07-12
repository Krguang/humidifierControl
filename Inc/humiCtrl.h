#pragma once
#include "stm32f1xx_hal.h"
#include "main.h"

extern uint8_t blinkFlag;
extern uint8_t startLowerLimitCountFlag;
extern uint16_t lowerLimitCount;
void humiCtrlInit();
void humiCtrl();