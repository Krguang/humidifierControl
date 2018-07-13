#include "key.h"
#include "usart.h"

#define const_key_time_short1 20 //短按的按键去抖动延时的时间
#define const_key_time_long1 2000 //长按的按键去抖动延时的时间

unsigned char ucKeySec = 0; //被触发的按键编号
unsigned int uiKeyTimeCnt1 = 0; //按键去抖动延时计数器
unsigned char ucKeyLock1 = 0; //按键触发后自锁的变量标志
unsigned char ucShortTouchFlag1 = 0; //短按的触发标志

void key_scan() {

	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 1)//IO 是高电平，说明两个按键没有全部被按下，这时要及时清零一些标志位
	{
		ucKeyLock1 = 0; //按键自锁标志清零
		uiKeyTimeCnt1 = 0;//按键去抖动延时计数器清零，此行非常巧妙，是我实战中摸索出来的。
		if (ucShortTouchFlag1 == 1) //短按触发标志
		{
			ucShortTouchFlag1 = 0;
			ucKeySec = 1; //触发一号键的短按
		}
	}
	else if (ucKeyLock1 == 0)//有按键按下，且是第一次被按下
	{
		uiKeyTimeCnt1++; //累加定时中断次数
		if (uiKeyTimeCnt1>const_key_time_short1)
		{
			ucShortTouchFlag1 = 1; //激活按键短按的有效标志
		}
		if (uiKeyTimeCnt1>const_key_time_long1)
		{
			ucShortTouchFlag1 = 0; //清除按键短按的有效标志
			uiKeyTimeCnt1 = 0;
			ucKeyLock1 = 1; //自锁按键置位,避免一直触发
			ucKeySec = 2; //触发 1 号键的长按
		}
	}
}

void key_service() //第三区 按键服务的应用程序
{
	switch (ucKeySec) //按键服务状态切换
	{
	case 1:// 1 号键的短按 对应朱兆祺学习板的 S1 键
		printf("短按\n");
		ucKeySec = 0; //响应按键服务处理程序后，按键编号清零，避免一致触发
		break;
	case 2:// 1 号键的长按 对应朱兆祺学习板的 S1 键
		printf("长按\n");
		ucKeySec = 0; //响应按键服务处理程序后，按键编号清零，避免一致触发
		break;

	}
}