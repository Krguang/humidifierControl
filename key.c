#include "key.h"
#include "usart.h"

#define const_key_time_short1 20 //�̰��İ���ȥ������ʱ��ʱ��
#define const_key_time_long1 2000 //�����İ���ȥ������ʱ��ʱ��

unsigned char ucKeySec = 0; //�������İ������
unsigned int uiKeyTimeCnt1 = 0; //����ȥ������ʱ������
unsigned char ucKeyLock1 = 0; //���������������ı�����־
unsigned char ucShortTouchFlag1 = 0; //�̰��Ĵ�����־

void key_scan() {

	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 1)//IO �Ǹߵ�ƽ��˵����������û��ȫ�������£���ʱҪ��ʱ����һЩ��־λ
	{
		ucKeyLock1 = 0; //����������־����
		uiKeyTimeCnt1 = 0;//����ȥ������ʱ���������㣬���зǳ��������ʵս�����������ġ�
		if (ucShortTouchFlag1 == 1) //�̰�������־
		{
			ucShortTouchFlag1 = 0;
			ucKeySec = 1; //����һ�ż��Ķ̰�
		}
	}
	else if (ucKeyLock1 == 0)//�а������£����ǵ�һ�α�����
	{
		uiKeyTimeCnt1++; //�ۼӶ�ʱ�жϴ���
		if (uiKeyTimeCnt1>const_key_time_short1)
		{
			ucShortTouchFlag1 = 1; //������̰�����Ч��־
		}
		if (uiKeyTimeCnt1>const_key_time_long1)
		{
			ucShortTouchFlag1 = 0; //��������̰�����Ч��־
			uiKeyTimeCnt1 = 0;
			ucKeyLock1 = 1; //����������λ,����һֱ����
			ucKeySec = 2; //���� 1 �ż��ĳ���
		}
	}
}

void key_service() //������ ���������Ӧ�ó���
{
	switch (ucKeySec) //��������״̬�л�
	{
	case 1:// 1 �ż��Ķ̰� ��Ӧ������ѧϰ��� S1 ��
		printf("�̰�\n");
		ucKeySec = 0; //��Ӧ�������������󣬰���������㣬����һ�´���
		break;
	case 2:// 1 �ż��ĳ��� ��Ӧ������ѧϰ��� S1 ��
		printf("����\n");
		ucKeySec = 0; //��Ӧ�������������󣬰���������㣬����һ�´���
		break;

	}
}