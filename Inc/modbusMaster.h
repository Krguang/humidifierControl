#pragma once
#include "stm32f1xx_hal.h"
#include "main.h"

extern unsigned short localData[];

void Usart2RxMonitor();
void sendDataMaster03();
void sendDataMaster16();


