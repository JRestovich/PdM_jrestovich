/*
 * API_delay.c
 *
 *  Created on: Mar 19, 2026
 *      Author: joaquin
 */

#include "API_delay.h"

void delayInit(delay_t* delay, tick_t duration)
{
  if(delay == NULL) {
    return;
  }

  delay->duration = duration;
  delay->startTime = 0;
  delay->running = false;
}

bool_t delayRead(delay_t *delay)
{
  /// verify that pointer to delay is valid
  if(delay == NULL) {
    return false;
  }

  if(delay->running) {
    if(HAL_GetTick() - delay->startTime >= delay->duration) {
      delay->running = false;
      return true;
    }
  } else {
    delay->startTime = HAL_GetTick();
    delay->running = true;
  }
  return false;
}

void delayWrite(delay_t* delay, tick_t duration)
{
  /// verify that pointer to delay is valid
  if(delay == NULL) {
    return;
  }
  delay->duration = duration;
}

bool_t delayIsRunning(delay_t * delay)
{
	/// verify that pointer to delay is valid
	  if(delay == NULL) {
		  return false;
	  }
	  return delay->running;

}
