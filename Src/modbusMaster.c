#include "modbusMaster.h"
#include "usart.h"

uint8_t M2ASlaveAdd = 1;

uint8_t txBuf[50];
uint8_t txCount = 0;

uint16_t localData[50];


static uint16_t GetCRC16(uint8_t *arr_buff, uint8_t len) {  //CRCУ�����
	uint16_t crc = 0xFFFF;
	uint8_t i, j;
	for (j = 0; j < len; j++) {
		crc = crc ^*arr_buff++;
		for (i = 0; i < 8; i++) {
			if ((crc & 0x0001) > 0) {
				crc = crc >> 1;
				crc = crc ^ 0xa001;
			}
			else
				crc = crc >> 1;
		}
	}
	return (crc);
}

void sendDataMaster03() {
	uint16_t temp;
	txBuf[0] = M2ASlaveAdd;
	txBuf[1] = 0x03;
	txBuf[2] = 0x00;
	txBuf[3] = 0x00;
	txBuf[4] = 0x00;
	txBuf[5] = 0x04;//��4λ
	temp = GetCRC16(txBuf, 6);
	txBuf[6] = (uint8_t)(temp & 0xff);
	txBuf[7] = (uint8_t)(temp >> 8);
	txCount = 8;
	HAL_UART_Transmit(&huart2, txBuf, txCount, 0xffff);
}

void sendDataMaster16() {

	uint16_t temp;
	uint8_t i;

	txBuf[0] = M2ASlaveAdd;
	txBuf[1] = 0x10;
	txBuf[2] = 0x00;         //���ݵ���ʼ��ַ��
	txBuf[3] = 0x03;
	txBuf[4] = 0x00;         //���ݵĸ�����
	txBuf[5] = 0x0a;
	txBuf[6] = 0x14;         //���ݵ��ֽ�����
	for (i = 0; i<txBuf[5]; i++) {
		txBuf[7 + 2 * i] = (uint8_t)(localData[i+ txBuf[3]] >> 8);
		txBuf[8 + 2 * i] = (uint8_t)(localData[i+ txBuf[3]] & 0xff);
	}
	temp = GetCRC16(txBuf, 2 * txBuf[5] + 7);
	txBuf[7 + 2 * txBuf[5]] = (uint8_t)(temp & 0xff);
	txBuf[8 + 2 * txBuf[5]] = (uint8_t)((temp >> 8) & 0xff);
	txCount = 9 + 2 * txBuf[5];
	HAL_UART_Transmit(&huart2, txBuf, txCount, 0xffff);
}

static void ModbusDecode(uint8_t *MDbuf, uint8_t len) {

	uint16_t  crc;
	uint8_t crch, crcl;
	uint16_t temp;

	if (MDbuf[0] != M2ASlaveAdd) return;								//��ַ���ʱ���ٶԱ�֡���ݽ���У��
	if (MDbuf[1] != 0x03) return;									//���鹦����
	crc = GetCRC16(MDbuf, len - 2);								//����CRCУ��ֵ
	crch = crc >> 8;
	crcl = crc & 0xFF;
	if ((MDbuf[len - 1] != crch) || (MDbuf[len - 2] != crcl)) return;	//��CRCУ�鲻��ʱֱ���˳�
	for (uint8_t i = 0; i < MDbuf[2]/2; i++)
	{
		localData[i] = (uint16_t)(MDbuf[3 + 2*i] << 8) + MDbuf[4 + 2*i];
	}
}

void Usart1RxMonitor() {
	if (uart2_recv_end_flag)
	{
		ModbusDecode(Usart2ReceiveBuffer.BufferArray, Usart2ReceiveBuffer.BufferLen);
		Usart2ReceiveBuffer.BufferLen = 0;
		uart2_recv_end_flag = 0;
	}
}
