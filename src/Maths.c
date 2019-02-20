/**
  ******************************************************************************
  * @file    ./src/Maths.c 
  * @author  kyChu
  * @version V1.0.0
  * @date    17-April-2018
  * @brief   user math module.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Maths.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
 * apply deadband function.
 */
float apply_deadband(float value, float deadband)
{
	if(fabs(value) <= deadband) {value = 0;}

	if(value > deadband) {value -= deadband;}
	if(value < -deadband) {value += deadband;}

	return value;
}

/*
 * step change function.
 */
void step_change(float *in, float target, float step, float deadBand)
{
	if(fabsf(*in - target) <= deadBand) {*in = target; return;}

	if(*in > target) {*in -= fabsf(step); return;}
	if(*in < target) {*in += fabsf(step); return;}
}

/******************************** END OF FILE *********************************/
