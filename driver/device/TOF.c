/**
  ******************************************************************************
  * @file    ./src/TOF.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Infrared TOF Module Driver.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "TOF.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if TOF_PORT_CON_USC
static KYLINK_CORE_HANDLE USC_PORT_HANDLE;
static kyLinkPackageDef *pRx = 0;
#else
static uint8_t _tof_data_update = 0;
static TOF_ORIGIN_DATA TOF_DATA = {0};
#endif /* TOF_PORT_CON_USC */

static uint8_t read_len;
static uint8_t read_buf[8];
/* Private function prototypes -----------------------------------------------*/
#if !TOF_PORT_CON_USC
static void _rc_data_decode(uint8_t data);
static uint16_t crc16(uint8_t *pBuffer, uint16_t len);
#endif /* !TOF_PORT_CON_USC */
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the UART to communicate with TOF module.
  * @param  None
  * @retval None
  */
void TOFDriverInit(void)
{
	uart1_init();
#if TOF_PORT_CON_USC
	kyLinkInit(&USC_PORT_HANDLE, NULL);
	pRx = GetRxPackage(&USC_PORT_HANDLE);
#endif /* TOF_PORT_CON_USC */
}

/**
  * @brief  Read a data from module.
  * @param  *d: pointer to a memory to store the TOF distance.
  * @retval data update flag.
  */
uint8_t GetNewTOFData(float *d)
{
	if((read_len = uart1_pullBytes(read_buf, 8)) > 0) {
		for(uint8_t idx = 0; idx < read_len; idx ++) {
#if TOF_PORT_CON_USC
			kylink_decode(&USC_PORT_HANDLE, read_buf[idx]);
#else
			_rc_data_decode(read_buf[idx]);
#endif /* TOF_PORT_CON_USC */
		}
#if TOF_PORT_CON_USC
		if(kyLinkCheckUpdate(&USC_PORT_HANDLE) == kyTRUE) {
			*d = pRx->FormatData.PacketData.TypeData.USC_DIST_CM;
			return 1;
		}
#else
		if(_tof_data_update) {
			_tof_data_update = 0;
			*d = (((uint16_t)TOF_DATA.HighByte << 8) | TOF_DATA.LowByte) / 10.0f;
			return 1;
		}
#endif /* TOF_PORT_CON_USC */
	}
	return 0;
}

#if !TOF_PORT_CON_USC
/**
  * @brief  com package decode function.
  * @param  data: byte read from module.
  * @retval None
  */
static uint8_t _data_step = 0;
static TOF_ORIGIN_DATA _tof_recv = {0};
static void _rc_data_decode(uint8_t data)
{
	switch(_data_step) {
		case 0: {
			if(data == 0xA5) {
				_tof_recv.Header1 = data;
				_data_step ++;
			}
		} break;
		case 1: {
			if(data == 0x5A) {
				_tof_recv.Header2 = data;
				_data_step ++;
			} else {
				_data_step = 0;
			}
		} break;
		case 2: {
			_tof_recv.HighByte = data;
			_data_step ++;
		} break;
		case 3: {
			_tof_recv.LowByte = data;
			_data_step ++;
		} break;
		case 4: {
			_tof_recv.LowCRC = data;
			_data_step ++;
		} break;
		case 5: {
			_tof_recv.HighCRC = data;
			if((((uint16_t)_tof_recv.HighCRC << 8) | _tof_recv.LowCRC) == crc16((uint8_t *)&_tof_recv, 4)) {
				TOF_DATA = _tof_recv;
				_tof_data_update = 1;
			}
			_data_step = 0;
		} break;
		default: _data_step = 0; break;
	}
}

/**
 * @brief  compute crc16.
 * @param  pBuffer: pointer to the buffer.
 * @param  len: buffer length.
 * @retval compute resualt.
 */
static uint16_t crc16(uint8_t *pBuffer, uint16_t len)
{
	uint8_t crcHi = 0xFF;
	uint8_t crcLo = 0xFF;
	uint16_t index, i;
	for(i = 0; i < len; i ++)
	{
		index = crcLo ^ *( pBuffer++ );
		crcLo = (uint8_t)( crcHi ^ (crc16Table[index] & 0xff));
		crcHi = crc16Table[index] >> 8;
	}
	return (uint16_t)(crcHi << 8 | crcLo);
}
#endif /* !TOF_PORT_CON_USC */

/******************************** END OF FILE *********************************/
