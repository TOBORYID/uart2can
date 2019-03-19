/**
  ******************************************************************************
  * @file    ./main.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#if CAN_MASTER_MODE
#include <stdio.h>
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if CAN_MASTER_MODE
static CanRxMsg *pRxMessage = 0;
#else
static uint8_t HW_ID = 0x0;
static CanTxMsg TxMessage = {0};

static uint8_t tof_rate_div = 0;
static uint8_t usc_rate_div = 0;
static uint8_t DataUpdateFlag = 0;
#endif

static _FLOAT_UNION _tof_data = {0}, _ult_data = {0};
static uint32_t LastTime = 0, CurrentTime = 0;

/* Private function prototypes -----------------------------------------------*/
#if CAN_MASTER_MODE
static void printID(uint32_t id);
static void printDist(float d);
#else
static void CAN_MsgUpdate(void);
static void CAN_MsgPrepare(void);
#endif
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void StartThread(void const * arg)
{
	LED_Init(); LED_R_ON(); LED_B_OFF();

	CAN_If_Init();

#if CAN_MASTER_MODE
	uart2_init();
	pRxMessage = GetCAN_RxMsg();
#else
	TOFDriverInit();
	UltDriverInit();

	CAN_MsgPrepare();
#endif

	CurrentTime = _Get_Micros();
	LastTime = CurrentTime;

	for(;;) {
#if CAN_MASTER_MODE
		CurrentTime = _Get_Micros();
		if(GetMsgUpdateFlag()) {

			uart2_TxBytesDMA((uint8_t *)"id(", 3); uart2_flush();
			printID(pRxMessage->StdId);
			uart2_TxBytesDMA((uint8_t *)"): ", 3); uart2_flush();
			_tof_data.uData[0] = pRxMessage->Data[0];
			_tof_data.uData[1] = pRxMessage->Data[1];
			_tof_data.uData[2] = pRxMessage->Data[2];
			_tof_data.uData[3] = pRxMessage->Data[3];
			_ult_data.uData[0] = pRxMessage->Data[4];
			_ult_data.uData[1] = pRxMessage->Data[5];
			_ult_data.uData[2] = pRxMessage->Data[6];
			_ult_data.uData[3] = pRxMessage->Data[7];
			uart2_TxBytesDMA((uint8_t *)"tof: ", 5); uart2_flush();
			printDist(_tof_data.fData);
			uart2_TxBytesDMA((uint8_t *)"cm, usc: ", 9); uart2_flush();
			printDist(_ult_data.fData);
			uart2_TxBytesDMA((uint8_t *)"cm                 \r", 20); uart2_flush();

			LED_B_TOG();
			LED_R_ON();
			LastTime = CurrentTime;
		}
		if((CurrentTime - LastTime) > 500000) {
			LED_R_TOG();
			uart2_TxBytesDMA((uint8_t *)"\e[0;31mLOST.\n\e[0m", 17); uart2_flush();
			LastTime = CurrentTime;
		}
#else
		CurrentTime = _Get_Micros();
		if(GetNewTOFData(&(_tof_data.fData))) {
			DataUpdateFlag = 1;
			/* indicate status if updated. */
			tof_rate_div ++;
			if(tof_rate_div & 0x04) LED_R_TOG();
		}
		if(GetNewUltData(&(_ult_data.fData))) {
			DataUpdateFlag = 1;
			/* indicate status if updated. */
			usc_rate_div ++;
			if(usc_rate_div & 0x04) LED_B_TOG();
		}
		if(DataUpdateFlag) {
			LastTime = CurrentTime;
		} else if((CurrentTime - LastTime) > 60000) {
			DataUpdateFlag = 1;
			LED_R_ON();
			LED_B_ON();
			LastTime = CurrentTime;
		}

		if(DataUpdateFlag) {
			DataUpdateFlag = 0;

			CAN_MsgUpdate();
			CAN_TransmitData(&TxMessage);
		}
#endif
	}
}

#if CAN_MASTER_MODE
int __io_putchar(int ch)
{
	LOG_PORT_PUT_CHAR(ch);
	return ch;
}

static void printID(uint32_t id)
{
	int c = 0;
	if(id < 0x5A0) {
		uart2_TxBytes((uint8_t *)"??", 2);
	} else {
		id -= 0x5A0;
		c = id;
		if(id >= 100) {
			LOG_PORT_PUT_CHAR('?');
			c %= 100;
		}
		if(id >= 10) {
			LOG_PORT_PUT_CHAR('0' + c / 10);
			c %= 10;
		}
		LOG_PORT_PUT_CHAR('0' + c);
	}
}

static void printDist(float d)
{
	int c = d;
	if(d >= 1000) {
		LOG_PORT_PUT_CHAR('?');
		c %= 1000;
	}
	if(d >= 100) {
		LOG_PORT_PUT_CHAR('0' + c / 100);
		c = c % 100;
	}
	if(d >= 10) {
		LOG_PORT_PUT_CHAR('0' + c / 10);
		c = c % 10;
	}
	LOG_PORT_PUT_CHAR('0' + c);

}
#endif

#if !CAN_MASTER_MODE
/**
  * @brief  Transmit Structure preparation.
  * @param  pTxMessage: pointer to the Tx Message.
  * @retval None
  */
static void CAN_MsgPrepare(void)
{
	TxMessage.StdId = 0x5A0 | HW_ID;
	TxMessage.ExtId = 0x00;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 8;

	TxMessage.Data[0] = 0;
	TxMessage.Data[1] = 0;
	TxMessage.Data[2] = 0;
	TxMessage.Data[3] = 0;
	TxMessage.Data[4] = 0;
	TxMessage.Data[5] = 0;
	TxMessage.Data[6] = 0;
	TxMessage.Data[7] = 0;
}

/**
  * @brief  Update CAN Message to send.
  * @param  None
  * @retval None
  */
static void CAN_MsgUpdate(void)
{
	TxMessage.Data[0] = _tof_data.uData[0];
	TxMessage.Data[1] = _tof_data.uData[1];
	TxMessage.Data[2] = _tof_data.uData[2];
	TxMessage.Data[3] = _tof_data.uData[3];
	TxMessage.Data[4] = _ult_data.uData[0];
	TxMessage.Data[5] = _ult_data.uData[1];
	TxMessage.Data[6] = _ult_data.uData[2];
	TxMessage.Data[7] = _ult_data.uData[3];
}
#endif

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************************** END OF FILE *********************************/
