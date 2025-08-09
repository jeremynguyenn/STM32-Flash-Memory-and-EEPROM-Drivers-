#ifndef __FLASH_DRIVER_H
#define __FLASH_DRIVER_H

#include "stm32f4xx.h"


#define FLASH_VOLTAGE_RANGE_1        0x00000000U  /*!< Device operating range: 1.8V to 2.1V                */
#define FLASH_VOLTAGE_RANGE_2        0x00000001U  /*!< Device operating range: 2.1V to 2.7V                */
#define FLASH_VOLTAGE_RANGE_3        0x00000002U  /*!< Device operating range: 2.7V to 3.6V                */
#define FLASH_VOLTAGE_RANGE_4        0x00000003U  /*!< Device operating range: 2.7V to 3.6V + External Vpp */

#define FLASH_PSIZE_BYTE           0x00000000U
#define FLASH_PSIZE_HALF_WORD      0x00000100U
#define FLASH_PSIZE_WORD           0x00000200U
#define FLASH_PSIZE_DOUBLE_WORD    0x00000300U

#define FLASH_TIMEOUT_VALUE 	   50000 /*50 seconds*/


#define FLASH_TYPEPROGRAM_BYTE        0x00000000U  /*!< Program byte (8-bit) at a specified address           */
#define FLASH_TYPEPROGRAM_HALFWORD    0x00000001U  /*!< Program a half-word (16-bit) at a specified address   */
#define FLASH_TYPEPROGRAM_WORD        0x00000002U  /*!< Program a word (32-bit) at a specified address        */
#define FLASH_TYPEPROGRAM_DOUBLEWORD  0x00000003U  /*!< Program a double word (64-bit) at a specified address */


#define FLASH_TYPEERASE_SECTORS         0x00000000U  /*!< Sectors erase only          */
#define FLASH_TYPEERASE_MASSERASE       0x00000001U  /*!< Flash Mass erase activation */

#define FLASH_KEY1               0x45670123U
#define FLASH_KEY2               0xCDEF89ABU


#define FLASH_SECTOR_0     0U /*!< Sector Number 0   */
#define FLASH_SECTOR_1     1U /*!< Sector Number 1   */
#define FLASH_SECTOR_2     2U /*!< Sector Number 2   */
#define FLASH_SECTOR_3     3U /*!< Sector Number 3   */
#define FLASH_SECTOR_4     4U /*!< Sector Number 4   */
#define FLASH_SECTOR_5     5U /*!< Sector Number 5   */
#define FLASH_SECTOR_6     6U /*!< Sector Number 6   */
#define FLASH_SECTOR_7     7U /*!< Sector Number 7   */


typedef enum
{
	DEV_OK  = 0x00U,
	DEV_ERROR = 0x01U,
	DEV_BUSY  = 0x02U,
	DEV_TIMEOUT = 0x03U
}DEV_StatusTypeDef;

typedef enum
{
	FLASH_PROC_NONE = 0U,
	FLASH_PROC_SECTERASE,
	FLASH_PROC_MASSERASE,
	FLASH_PROC_PROGRAM
}FLASH_ProcedureTypeDef;

typedef enum
{
	DEV_UNLOCKED = 0x00U,
	DEV_LOCKED   = 0x01U
}DEV_LockType;


typedef struct
{
	/*Internal variable to indicate currently running procedure*/
	__IO FLASH_ProcedureTypeDef  ProcedureOngoing;
	/*Internal variable to save number of sectors to erase*/
	__IO uint32_t NbSectorsToErase;
	__IO uint32_t Sector;
	__IO uint32_t Address;
	DEV_LockType Lock;
	__IO uint32_t ErrorCode;

}FLASH_ProcessTypeDef;


typedef struct
{
	uint32_t TypeErase;
	uint32_t Sector;
	uint32_t NbSectors;
	uint32_t VoltageRange;


}FLASH_EraseInitTypeDef;
DEV_StatusTypeDef flash_program(uint32_t prg_type,  uint32_t address, uint64_t data);

void flash_program_byte(uint32_t address, uint8_t data);

void flash_program_halfword(uint32_t address, uint16_t data);

void flash_program_word(uint32_t address, uint32_t data);

void flash_program_doubleword(uint32_t address, uint64_t data);
void flash_sector_erase(uint32_t sector, uint8_t voltage_range);
void flash_mass_erase(uint8_t voltage_range);
DEV_StatusTypeDef flash_unlock(void);
float flash_read_num(uint32_t start_sect_addr);
void flash_write_num(uint32_t start_sect_addr, float num);
void flash_read_data(uint32_t start_sect_addr, uint32_t *rx_buff, uint16_t numberofwords);
uint32_t flash_write_data(uint32_t start_sect_addr,uint32_t *data, uint16_t numberofwords);
void get_str(uint32_t *src_data, char *dest_buff);

#endif
