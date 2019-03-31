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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static CanTxMsg TxMessage = {0};

static uint8_t tof_rate_div = 0;
static uint8_t usc_rate_div = 0;
static uint8_t DataUpdateFlag = 0;

static _FLOAT_UNION _tof_data = {0}, _ult_data = {0};
static uint32_t LastTime = 0, CurrentTime = 0;

/* Private function prototypes -----------------------------------------------*/
static void CAN_MsgUpdate(void);
static void CAN_MsgPrepare(void);
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

	TOFDriverInit();
	UltDriverInit();

	CAN_MsgPrepare();

	for(int i = 0; i < DEVICE_CAN_ADDR + 1; i ++) {
		LED_B_ON();
		_delay_ms(50);
		LED_B_OFF();
		_delay_ms(450);
	}

	CurrentTime = _Get_Micros();
	LastTime = CurrentTime;

	for(;;) {
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
			LED_R_TOG();
			LED_B_TOG();
			LastTime = CurrentTime;
		}

		if(DataUpdateFlag) {
			DataUpdateFlag = 0;

			CAN_MsgUpdate();
			CAN_TransmitData(&TxMessage);
		}
	}
}

/**
  * @brief  Transmit Structure preparation.
  * @param  pTxMessage: pointer to the Tx Message.
  * @retval None
  */
static void CAN_MsgPrepare(void)
{
	TxMessage.StdId = 0x5A0 | DEVICE_CAN_ADDR;
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
