/**
 * @file       ds1307.h
 * @version    1.0.0
 * @date       
 * @author     Truong Vo
 *
 * @brief      DS1307 library for real time clock
 *
 * @note	     None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "ds1307.h"

/* Private defines ---------------------------------------------------- */
#define DS1307_I2C_TIMEOUT 1000
#define DS1307_YEAR_BASE   2000

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */
/**
 * @brief  		Convert decimal number to bcd number
 *
 * @param[in]	dec_num		Decimal number
 *
 * @attention	None
 *
 * @return		BCD number
 */
static uint8_t dec_to_bcd(uint8_t dec_num);

/**
 * @brief  		Convert bcd number to decimal number
 *
 * @param[in]	dec_num		BCD number
 *
 * @attention	None
 *
 * @return		Decimal number
 */
static uint8_t bcd_to_dec(uint8_t bin_num);

/**
 * @brief  		Read a single DS1307 timekeeper register over I2C
 *
 * @param[in]	ds1307							Pointer to an initialized DS1307
 * @param[in]	timekeeper_register_address		Timekeeper Register address for each function
 * @param[in]	ret_val							Store data read
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK:		Return success and data will store in ret_val
 * 	- DS1307_ERROR:		I2C communication failure
 * 	- DS1307_BUSY:		I2C bus busy
 * 	- DS1307_TIMEOUT:	Timeout reception
 * 	- DS1307_ERR_PARA:	Invalid arguments
 */
static ds1307_state_t ds1307_read_i2c(ds1307_t *ds1307, uint8_t timekeeper_register_address, uint8_t *ret_val);

/**
 * @brief  		Write a single DS1307 timekeeper register over I2C
 *
 * @param[in]	ds1307							Pointer to an initialized DS1307
 * @param[in]	timekeeper_register_address		Timekeeper Register address for each function
 * @param[in]	value							Data need to write
 *
 * @attention	None
 *
 * @return
 * 	- DS1307_OK:		Return success and data will store in ret_val
 * 	- DS1307_ERROR:		I2C communication failure
 * 	- DS1307_BUSY:		I2C bus busy
 * 	- DS1307_TIMEOUT:	Timeout transmission
 * 	- DS1307_ERR_PARA:	Invalid arguments
 */
static ds1307_state_t ds1307_write_i2c(ds1307_t *ds1307, uint8_t timekeeper_register_address, uint8_t value);

/* Function definitions ----------------------------------------------- */

void ds1307_init(ds1307_t *ds1307, I2C_HandleTypeDef *ds1307_i2c)
{
  ds1307->ds1307_i2c = ds1307_i2c;
}

ds1307_state_t ds1307_set_clock_halt(ds1307_t *ds1307, uint8_t clock_halt)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }

  uint8_t clock_halt_raw = (clock_halt ? 0x80 : 0x00);

  uint8_t cur_sec;
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_SECOND_ADDRESS, &cur_sec);
  if (status == DS1307_OK)
  {
    return ds1307_write_i2c(ds1307, DS1307_SECOND_ADDRESS, clock_halt_raw | (cur_sec & 0x7F));
  }
  else
  {
    return status;
  }
}

ds1307_state_t ds1307_get_clock_halt(ds1307_t *ds1307, uint8_t *clock_halt)
{
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_SECOND_ADDRESS, clock_halt);
  if (status != DS1307_OK)
  {
    return status;
  }
  else
  {
    *clock_halt = (*clock_halt >> 7) & 0x01;
    return DS1307_OK;
  }
}

ds1307_state_t ds1307_set_dayofweek(ds1307_t *ds1307, uint8_t dayofweek)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }
  if ((dayofweek < 1) || (dayofweek > 7))
  {
    return DS1307_ERR_PARA;
  }

  return ds1307_write_i2c(ds1307, DS1307_DAY_ADDRESS, dec_to_bcd(dayofweek));
}

ds1307_state_t ds1307_set_date(ds1307_t *ds1307, uint8_t date)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }
  if ((date < 1) || (date > 31))
  {
    return DS1307_ERR_PARA;
  }

  return ds1307_write_i2c(ds1307, DS1307_DATE_ADDRESS, dec_to_bcd(date));
}

ds1307_state_t ds1307_set_month(ds1307_t *ds1307, uint8_t month)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }
  if ((month < 1) || (month > 12))
  {
    return DS1307_ERR_PARA;
  }

  return ds1307_write_i2c(ds1307, DS1307_MONTH_ADDRESS, dec_to_bcd(month));
}

ds1307_state_t ds1307_set_year(ds1307_t *ds1307, uint16_t year)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }
  if ((year < DS1307_YEAR_BASE) || (year > (DS1307_YEAR_BASE + 99)))
  {
    return DS1307_ERR_PARA;
  }

  return ds1307_write_i2c(ds1307, DS1307_YEAR_ADDRESS, dec_to_bcd((uint8_t) (year - DS1307_YEAR_BASE)));
}

// Bit6: 1: Mode 12, 0: Mode 24
// Bit5: Mode 12: (0 = AM, 1 = PM), Mode 24: (20 -> 23)
ds1307_state_t ds1307_set_hour_mode12(ds1307_t *ds1307, uint8_t hour12, uint8_t meridiem)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }
  if ((hour12 < 1) || (hour12 > 12))
  {
    return DS1307_ERR_PARA;
  }

  // Bit6: 1: Mode 12, 0: Mode 24
  // Bit5: Mode 12: (0 = AM, 1 = PM)
  uint8_t hour_raw = 0x40 | (meridiem ? 0x20 : 0x00);

  return ds1307_write_i2c(ds1307, DS1307_HOUR_ADDRESS, dec_to_bcd(hour12) | hour_raw);
}

ds1307_state_t ds1307_set_hour_mode24(ds1307_t *ds1307, uint8_t hour24)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }
  if (hour24 > 23)
  {
    return DS1307_ERR_PARA;
  }

  return ds1307_write_i2c(ds1307, DS1307_HOUR_ADDRESS, dec_to_bcd(hour24) & 0x3F);
}

ds1307_state_t ds1307_set_minute(ds1307_t *ds1307, uint8_t minute)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }
  if (minute > 59)
  {
    return DS1307_ERR_PARA;
  }

  return ds1307_write_i2c(ds1307, DS1307_MINUTE_ADDRESS, dec_to_bcd(minute));
}

ds1307_state_t ds1307_set_second(ds1307_t *ds1307, uint8_t second)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }
  if (second > 59)
  {
    return DS1307_ERR_PARA;
  }

  uint8_t        clock_halt;
  ds1307_state_t status = ds1307_get_clock_halt(ds1307, &clock_halt);
  if (status != DS1307_OK)
  {
    return status;
  }

  return ds1307_write_i2c(ds1307, DS1307_SECOND_ADDRESS, dec_to_bcd(second) | (clock_halt << 7));
}

ds1307_state_t ds1307_get_dayofweek(ds1307_t *ds1307, uint8_t *dayofweek)
{
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_DAY_ADDRESS, dayofweek);
  if (status != DS1307_OK)
  {
    return status;
  }

  *dayofweek = bcd_to_dec(*dayofweek);
  return DS1307_OK;
}

ds1307_state_t ds1307_get_date(ds1307_t *ds1307, uint8_t *date)
{
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_DATE_ADDRESS, date);
  if (status != DS1307_OK)
  {
    return status;
  }

  *date = bcd_to_dec(*date);
  return DS1307_OK;
}

ds1307_state_t ds1307_get_month(ds1307_t *ds1307, uint8_t *month)
{
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_MONTH_ADDRESS, month);
  if (status != DS1307_OK)
  {
    return status;
  }

  *month = bcd_to_dec(*month);
  return DS1307_OK;
}

ds1307_state_t ds1307_get_year(ds1307_t *ds1307, uint16_t *year)
{
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_YEAR_ADDRESS, (uint8_t *) year);
  if (status != DS1307_OK)
  {
    return status;
  }

  *year = bcd_to_dec(*year);
  *year += 2000;

  return DS1307_OK;
}

ds1307_state_t ds1307_get_hour_mode12(ds1307_t *ds1307, uint8_t *hour12, uint8_t *meridiem)
{
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_HOUR_ADDRESS, hour12);
  if (status != DS1307_OK)
  {
    return status;
  }

  *meridiem = (*hour12 >> 5) & 0x01;


//  *hour12 &= 0x1F;
  *hour12 = bcd_to_dec(*hour12 & 0x1F);
  return DS1307_OK;
}

ds1307_state_t ds1307_get_hour_mode24(ds1307_t *ds1307, uint8_t *hour24)
{
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_HOUR_ADDRESS, hour24);
  if (status != DS1307_OK)
  {
    return status;
  }

  *hour24 = bcd_to_dec(*hour24);
  return DS1307_OK;
}

ds1307_state_t ds1307_get_minute(ds1307_t *ds1307, uint8_t *minute)
{
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_MINUTE_ADDRESS, minute);
  if (status != DS1307_OK)
  {
    return status;
  }

  *minute = bcd_to_dec(*minute);
  return DS1307_OK;
}

ds1307_state_t ds1307_get_second(ds1307_t *ds1307, uint8_t *second)
{
  ds1307_state_t status = ds1307_read_i2c(ds1307, DS1307_SECOND_ADDRESS, second);
  if (status != DS1307_OK)
  {
    return status;
  }

  *second = bcd_to_dec(*second);
  return DS1307_OK;
}

/* Private definitions ----------------------------------------------- */
static uint8_t dec_to_bcd(uint8_t dec_num)
{
  return (dec_num % 10 + ((dec_num / 10) << 4));
}

static uint8_t bcd_to_dec(uint8_t bin_num)
{
  return (((bin_num & 0xF0) >> 4) * 10) + (bin_num & 0x0F);
}

static ds1307_state_t ds1307_read_i2c(ds1307_t *ds1307, uint8_t timekeeper_register_address, uint8_t *ret_val)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL) || (ret_val == NULL))
  {
    return DS1307_ERR_PARA;
  }

  HAL_StatusTypeDef status =
    HAL_I2C_Mem_Read(ds1307->ds1307_i2c, DS1307_ADDRESS << 1, timekeeper_register_address,
                     I2C_MEMADD_SIZE_8BIT, ret_val, 1, DS1307_I2C_TIMEOUT);

  if (status == HAL_OK)
  {
    return DS1307_OK;
  }
  else if (status == HAL_ERROR)
  {
    return DS1307_ERROR;
  }
  else if (status == HAL_BUSY)
  {
    return DS1307_BUSY;
  }
  else
  {
    return DS1307_TIMEOUT;
  }
}

static ds1307_state_t ds1307_write_i2c(ds1307_t *ds1307, uint8_t timekeeper_register_address, uint8_t value)
{
  if ((ds1307 == NULL) || (ds1307->ds1307_i2c == NULL))
  {
    return DS1307_ERR_PARA;
  }

  HAL_StatusTypeDef status =
    HAL_I2C_Mem_Write(ds1307->ds1307_i2c, DS1307_ADDRESS << 1, timekeeper_register_address,
                      I2C_MEMADD_SIZE_8BIT, &value, 1, DS1307_I2C_TIMEOUT);

  if (status == HAL_OK)
  {
    return DS1307_OK;
  }
  else if (status == HAL_ERROR)
  {
    return DS1307_ERROR;
  }
  else if (status == HAL_BUSY)
  {
    return DS1307_BUSY;
  }
  else
  {
    return DS1307_TIMEOUT;
  }
}

/* End of file -------------------------------------------------------- */
