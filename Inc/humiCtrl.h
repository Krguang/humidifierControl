#pragma once
#include "stm32f1xx_hal.h"
#include "main.h"

extern uint8_t nonstopWorkFlag;			//连续工作标志
extern uint32_t nonstopWorkCount;			//连续工作计数
extern uint8_t ledBlinkFlagTemp;
extern uint8_t extraDrainWaterFlag;
extern uint16_t extraDrainWaterCount;
extern uint8_t manualDrainWaterFlag;
extern uint16_t manualDrainWaterCount;
extern uint8_t drainWaterFlag;
extern uint16_t drainWaterCount;
extern uint8_t overCurrentFlag;
extern uint16_t overCurrentCount;
extern uint8_t blinkFlag;
extern uint8_t startLowerLimitCountFlag;
extern uint16_t lowerLimitCount;
void humiCtrlInit();
void humiCtrl();