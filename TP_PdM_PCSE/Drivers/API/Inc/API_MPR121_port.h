/**
 * @file API_MPR121_port.h
 * @brief Hardware access layer for the MPR121 driver.
 *
 * Created on: Apr 13, 2026
 * Author: joaquin
 */

#ifndef API_INC_API_MPR121_PORT_H_
#define API_INC_API_MPR121_PORT_H_

#include "typedefs.h"

/**
 * @brief Initializes the MPR121 low-level interface.
 *
 * @return true Initialization completed successfully.
 * @return false Initialization failed.
 */
bool_t API_MPR121_port_init();

/**
 * @brief Reads the current touch status from the MPR121.
 *
 * @param pdata Pointer to the variable where the key mask is stored.
 * @return true Read completed successfully.
 * @return false Read failed.
 */
bool_t API_MPR121_port_readKeys(uint16_t *pdata);

#endif /* API_INC_API_MPR121_PORT_H_ */
