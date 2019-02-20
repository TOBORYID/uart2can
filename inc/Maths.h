/**
  ******************************************************************************
  * @file    ./inc/Maths.h 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   Header for Maths.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MATHS_H
#define __MATHS_H

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LIMIT_MAX(x, y)                (((x) > (y)) ? (y) : (x))
#define LIMIT_MIN(x, y)                (((x) < (y)) ? (y) : (x))
#define LIMIT_RANGE(x, max, min)       (LIMIT_MAX(LIMIT_MIN((x), (min)), (max)))

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
float apply_deadband(float value, float deadband);
void step_change(float *in, float target, float step, float deadBand);

#endif /* __MATHS_H */

/******************************** END OF FILE *********************************/
