/**
 * @file       ds1307.h
 * @version    1.0.0
 * @date       
 * @author     Truong Vo
 *
 * @brief      DS1307 library for real time clock
 *
 * @note	   None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DS1307_H
#define __DS1307_H

/* Includes ----------------------------------------------------------- */
#include "stm32f4xx.h"

/* Public defines ----------------------------------------------------- */
#define DS1307_ADDRESS			(0X68)

#define DS1307_SECOND_ADDRESS	(0x00)
#define DS1307_MINUTE_ADDRESS	(0x01)
#define DS1307_HOUR_ADDRESS		(0x02)
#define DS1307_DAY_ADDRESS		(0x03)
#define DS1307_DATE_ADDRESS		(0x04)
#define DS1307_MONTH_ADDRESS	(0x05)
#define DS1307_YEAR_ADDRESS		(0x06)
#define DS1307_CONTROL_ADDRESS	(0x07)

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief DS1307 state
 */
typedef enum
{
	DS1307_OK		= 0,
	DS1307_ERROR	= -1,
	DS1307_BUSY		= -2,
	DS1307_TIMEOUT	= -3,
	DS1307_ERR_PARA	= -4
}
ds1307_state_t;

/**
 * @brief <structure descriptiton>
 */
typedef struct
{
	I2C_HandleTypeDef *ds1307_i2c;

	uint8_t clock_halt;
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint16_t year;

	uint8_t meridiem;
}
ds1307_t;


/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief	Initialize a DS1307 driver
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	ds1307_i2c	Pointer to an HAL I2C handle used to communicate with DS1307
 *
 * @attention	None
 *
 * @return		None
 */
void ds1307_init(ds1307_t *ds1307, I2C_HandleTypeDef *ds1307_i2c);

/**
 * @brief  		Set clock halt in second register
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	clock_halt	0: start, 1: stop
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success and clock halt will set 0/1 in second register
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout transmission
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_set_clock_halt(ds1307_t *ds1307, uint8_t clock_halt);

/**
 * @brief  		Get clock halt in second register
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	clock_halt	Store the clock halt value, 0: start, 1: stop
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success and data will store in clock_halt
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout reception
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_get_clock_halt(ds1307_t *ds1307, uint8_t *clock_halt);

/**
 * @brief  		Set day of week
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	dayofweek	1-7, Monday-Sunday
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout transmission
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_set_dayofweek(ds1307_t *ds1307, uint8_t dayofweek);

/**
 * @brief  		Set day of month
 *
 * @param[in]	ds1307	Pointer to an initialized DS1307
 * @param[in]	date	From 1 to 31
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout transmission
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_set_date(ds1307_t *ds1307, uint8_t date);

/**
 * @brief  		Set month
 *
 * @param[in]	ds1307	Pointer to an initialized DS1307
 * @param[in]	date	From 1 to 12
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout transmission
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_set_month(ds1307_t *ds1307, uint8_t month);

/**
 * @brief  		Set year
 *
 * @param[in]	ds1307	Pointer to an initialized DS1307
 * @param[in]	year	From 2000 to 2099
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout transmission
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_set_year(ds1307_t *ds1307, uint16_t year);

/**
 * @brief  		Get day of week
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	dayofweek	Store the dayofweek value, value range is 1-7 (monday-sunday)
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout reception
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_get_dayofweek(ds1307_t *ds1307, uint8_t *dayofweek);

/**
 * @brief  		Get day of month
 *
 * @param[in]	ds1307	Pointer to an initialized DS1307
 * @param[in]	date	Store the date value, from 1 to 31
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout reception
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_get_date(ds1307_t *ds1307, uint8_t *date);

/**
 * @brief  		Get month
 *
 * @param[in]	ds1307	Pointer to an initialized DS1307
 * @param[in]	month	Store the month value, from 1 to 12
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout reception
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_get_month(ds1307_t *ds1307, uint8_t *month);

/**
 * @brief  		Get year
 *
 * @param[in]	ds1307	Pointer to an initialized DS1307
 * @param[in]	year	Store the year value, from 2000 to 2099
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout reception
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_get_year(ds1307_t *ds1307, uint16_t *year);

/**
 * @brief  		Set hour in mode 12h
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	hour12		From 1 to 12
 * @param[in]	meridiem	0: AM, 1: PM
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout transmission
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_set_hour_mode12(ds1307_t *ds1307, uint8_t hour12, uint8_t meridiem);

/**
 * @brief  		Set hour in mode 24h
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	hour24		From 00 to 23
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout transmission
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_set_hour_mode24(ds1307_t *ds1307, uint8_t hour24);

/**
 * @brief  		Set minute
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	minute		From 00 to 59
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout transmission
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_set_minute(ds1307_t *ds1307, uint8_t minute);

/**
 * @brief  		Set second
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	second		From 00 to 59
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout transmission
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_set_second(ds1307_t *ds1307, uint8_t second); // OK

/**
 * @brief  		Get hour in mode 12h
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	hour12		Store hour mode 12h, from 1 to 12
 * @param[in]	meridiem	Store meridiem, 0: AM, 1: PM
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout reception
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_get_hour_mode12(ds1307_t *ds1307, uint8_t *hour12, uint8_t *meridiem);

/**
 * @brief  		Get hour in mode 24h
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	hour24		Store hour mode 24h, from 00 to 23
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout reception
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_get_hour_mode24(ds1307_t *ds1307, uint8_t *hour24);

/**
 * @brief  		Get minute
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	minute		Store minute, from 0 to 59
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout reception
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_get_minute(ds1307_t *ds1307, uint8_t *minute);

/**
 * @brief  		Get second
 *
 * @param[in]	ds1307		Pointer to an initialized DS1307
 * @param[in]	second		Store second, from 0 to 59
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK: Return success
 * 	- DS1307_ERROR: I2C communication failure
 * 	- DS1307_BUSY: I2C bus busy
 * 	- DS1307_TIMEOUT: Timeout reception
 * 	- DS1307_ERR_PARA: Invalid arguments
 */
ds1307_state_t ds1307_get_second(ds1307_t *ds1307, uint8_t *second);

#endif

/* End of file -------------------------------------------------------- */
