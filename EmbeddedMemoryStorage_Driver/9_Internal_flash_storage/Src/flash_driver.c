#include "string.h"
#include "flash_driver.h"
#include "timebase.h"

#define TEMP_BUFF_SZ		4



FLASH_ProcessTypeDef pFlash;
uint8_t temp_buff[TEMP_BUFF_SZ];

DEV_StatusTypeDef  flash_wait_for_last_operation(uint32_t timeout);
static void flush_caches(void);


DEV_StatusTypeDef flash_ex_erase(FLASH_EraseInitTypeDef *pt_erase_init, uint32_t *sect_err)
{
	DEV_StatusTypeDef  status =  DEV_ERROR;
	uint32_t index = 0U;

	/*wait for last operation to be completed*/
	status = flash_wait_for_last_operation(FLASH_TIMEOUT_VALUE);

	if( status  == DEV_OK)
	{
	   *sect_err =  0xFFFFFFFFU;

	   if(pt_erase_init->TypeErase ==  FLASH_TYPEERASE_MASSERASE)
	   {
		   /*Mass erase*/
		   flash_mass_erase((uint8_t)pt_erase_init->VoltageRange);

			/*wait for last operation to be completed*/
			status = flash_wait_for_last_operation(FLASH_TIMEOUT_VALUE);

			/*Disable MER bit*/
			FLASH->CR &=~(1U<<2);
	   }
	   else
	   {
		   /*Sector erase*/
		   for(index = pt_erase_init->Sector; index <  (pt_erase_init->NbSectors + pt_erase_init->Sector); index++)
		   {
			   flash_sector_erase(index, (uint8_t)pt_erase_init->VoltageRange);

				/*wait for last operation to be completed*/
				status = flash_wait_for_last_operation(FLASH_TIMEOUT_VALUE);

				/*Clear SNB bit and SER bit*/
				CLEAR_BIT(FLASH->CR, (FLASH_CR_SER | FLASH_CR_SNB));

				if(status !=DEV_OK)
				{
					*sect_err =  index;
					break;
				}
		   }
	   }

	   flush_caches();

	}

	return status;
}

void flash_sector_erase(uint32_t sector, uint8_t voltage_range)
{
	uint32_t tmp_psize = 0U;

	if(voltage_range ==  FLASH_VOLTAGE_RANGE_1)
	{
		tmp_psize = FLASH_PSIZE_BYTE;
	}
	else if(voltage_range ==  FLASH_VOLTAGE_RANGE_2)
	{
		tmp_psize = FLASH_PSIZE_HALF_WORD;
	}
	else if(voltage_range ==  FLASH_VOLTAGE_RANGE_3)
	{
		tmp_psize = FLASH_PSIZE_WORD;
	}
	else
	{
		tmp_psize = FLASH_PSIZE_DOUBLE_WORD;

	}

	FLASH->CR &=~FLASH_CR_PSIZE;
	FLASH->CR |= tmp_psize;

	/*Set SER bit and select sector*/
	FLASH->CR |=  FLASH_CR_SER |(sector << FLASH_CR_SNB_Pos);


}

void flash_mass_erase(uint8_t voltage_range)
{
	/*Clear PSIZE field*/
	FLASH->CR &=~FLASH_CR_PSIZE;

	/*Set MER bit*/
	FLASH->CR |= FLASH_CR_MER;
	/*Set Start bit and voltage*/
	FLASH->CR |=  FLASH_CR_STRT |((uint32_t)voltage_range <<8U);
}

DEV_StatusTypeDef  flash_wait_for_last_operation(uint32_t timeout)
{
   uint32_t tickstart = 0U;

   pFlash.ErrorCode = 0;

   tickstart = get_tick();

   while((FLASH->SR & FLASH_SR_BSY) != RESET)
   {
	   if(timeout != MAX_DELAY)
	   {
		   if((timeout  == 0) || ((get_tick() -  tickstart) > timeout))
		   {
			   return DEV_TIMEOUT;
		   }
	   }
   }

   /*Check end of operation flag*/
   if((FLASH->SR & (1U<<0)) != RESET)
   {
	   /*Clear flash end of operation pending bit*/
	   FLASH->SR = (1U<<0);
   }

   return DEV_OK;
}


DEV_StatusTypeDef flash_program(uint32_t prg_type,  uint32_t address, uint64_t data)
{
	DEV_StatusTypeDef status = DEV_ERROR;

	/*wait for last operation to be completed*/
	status = flash_wait_for_last_operation(FLASH_TIMEOUT_VALUE);

	if( status == DEV_OK)
	{
		if(prg_type == FLASH_TYPEPROGRAM_BYTE)
		{
			flash_program_byte(address,(uint8_t)data);
		}

		else if(prg_type == FLASH_TYPEPROGRAM_HALFWORD)
		{
			flash_program_halfword(address,(uint16_t)data);
		}
		else if(prg_type == FLASH_TYPEPROGRAM_WORD)
		{
			flash_program_word(address,(uint32_t)data);
		}
		else
		{
			flash_program_doubleword(address,data);

		}

		/*wait for last operation to be completed*/
		status = flash_wait_for_last_operation(FLASH_TIMEOUT_VALUE);

		/*Clear Program bit*/
		FLASH->CR &=~FLASH_CR_PG;
	}

	return status;
}
void flash_program_byte(uint32_t address, uint8_t data)
{
	/*Clear PSIZE field*/
	FLASH->CR &=~FLASH_CR_PSIZE;

	/*Set PSIZE for byte*/
	FLASH->CR |=FLASH_PSIZE_BYTE;

	/*Set PG bit*/
	FLASH->CR |= FLASH_CR_PG;

	/*Put data at the address*/
	*(__IO uint8_t *)address =  data;
}

void flash_program_halfword(uint32_t address, uint16_t data)
{
	/*Clear PSIZE field*/
	FLASH->CR &=~FLASH_CR_PSIZE;

	/*Set PSIZE for halfword*/
	FLASH->CR |=FLASH_PSIZE_HALF_WORD;

	/*Set PG bit*/
	FLASH->CR |= FLASH_CR_PG;

	/*Put data at the address*/
	*(__IO uint16_t *)address =  data;
}

void flash_program_word(uint32_t address, uint32_t data)
{
	/*Clear PSIZE field*/
	FLASH->CR &=~FLASH_CR_PSIZE;

	/*Set PSIZE for halfword*/
	FLASH->CR |=FLASH_PSIZE_WORD;

	/*Set PG bit*/
	FLASH->CR |= FLASH_CR_PG;

	/*Put data at the address*/
	*(__IO uint32_t *)address =  data;
}

void flash_program_doubleword(uint32_t address, uint64_t data)
{
	/*Clear PSIZE field*/
	FLASH->CR &=~FLASH_CR_PSIZE;

	/*Set PSIZE for halfword*/
	FLASH->CR |=FLASH_PSIZE_DOUBLE_WORD;

	/*Set PG bit*/
	FLASH->CR |= FLASH_CR_PG;

	/*Put  first word at the address*/
	*(__IO uint32_t *)address =  (uint32_t)data;

	/*Flush pipeline : ensure programming is performed steps.*/
	__ISB();

	/*Put  second word at the address*/
	*(__IO uint32_t *)(address+4) =  (uint32_t)(data>>32);
}

static void flush_caches(void)
{
	/*Flush instruction cache*/
	  if (READ_BIT(FLASH->ACR, FLASH_ACR_ICEN) != RESET)
	  {
		    /* Disable instruction cache  */
			  FLASH->ACR &= (~FLASH_ACR_ICEN);

			/* Reset instruction cache */
			  FLASH->ACR |= FLASH_ACR_ICRST;
			 FLASH->ACR &= ~FLASH_ACR_ICRST;


			 /* Enable instruction cache */
			 FLASH->ACR |= FLASH_ACR_ICEN;
	  }

	  /*Flush data cache*/

	  if (READ_BIT(FLASH->ACR, FLASH_ACR_DCEN) != RESET)
	  {
	    /* Disable data cache  */
		  FLASH->ACR &= (~FLASH_ACR_DCEN);

	    /* Reset data cache */
	    FLASH->ACR |= FLASH_ACR_DCRST;
	    FLASH->ACR &= ~FLASH_ACR_DCRST;

	    /* Enable data cache */
	    FLASH->ACR |= FLASH_ACR_DCEN;
	  }
}


DEV_StatusTypeDef flash_unlock(void)
{
	DEV_StatusTypeDef status = DEV_OK;

	if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
	{

		/*Authorize flash write access*/
		FLASH->KEYR  =  FLASH_KEY1;
		FLASH->KEYR  =  FLASH_KEY2;

		if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
		{
			status =  DEV_ERROR;
		}

	}

	return status;
}

uint32_t flash_get_error(void)
{
	return pFlash.ErrorCode;
}

static uint32_t get_sector(uint32_t address)
{
	uint32_t sector = 0;
	if((address < 0x08003FFF) && (address >= 0x08000000))
	 {
	     sector = FLASH_SECTOR_0;
	 }
   else if((address < 0x08007FFF) && (address >= 0x08004000))
   {
	 sector = FLASH_SECTOR_1;
   }
   else if((address < 0x0800BFFF) && (address >= 0x08008000))
   {
	 sector = FLASH_SECTOR_2;
   }
   else if((address < 0x0800FFFF) && (address >= 0x0800C000))
   {
	 sector = FLASH_SECTOR_3;
   }
   else if((address < 0x0801FFFF) && (address >= 0x08010000))
   {
	 sector = FLASH_SECTOR_4;
   }
   else if((address < 0x0803FFFF) && (address >= 0x08020000))
   {
	 sector = FLASH_SECTOR_5;
   }
   else if((address < 0x0805FFFF) && (address >= 0x08040000))
   {
	 sector = FLASH_SECTOR_6;
   }
   else if((address < 0x0807FFFF) && (address >= 0x08060000))
   {
	 sector = FLASH_SECTOR_7;
   }
   else
   {
     //Do something
   }

	return sector;

}


uint32_t flash_write_data(uint32_t start_sect_addr,uint32_t *data, uint16_t numberofwords)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t sect_err;
	uint16_t write_count  = 0;

	/*Unlock flash*/
	flash_unlock();

	/*Get Number of sectors to erase starting from first sector*/
	uint32_t start_sector =  get_sector(start_sect_addr);
	uint32_t end_sect_addr =  start_sect_addr + numberofwords * 4;
	uint32_t end_sector =  get_sector(end_sect_addr);

	/*Initialize EraseInit Struct*/
	EraseInitStruct.TypeErase =  FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector       =  start_sector;
	EraseInitStruct.NbSectors    =  (end_sector - start_sector) + 1;

	if(flash_ex_erase(&EraseInitStruct, &sect_err) != DEV_OK )
	{
		return flash_get_error();
	}

	/*Program flash word-by-word*/
	while(write_count < numberofwords )
	{
		if(flash_program(FLASH_TYPEPROGRAM_WORD, start_sect_addr,data[write_count]) == DEV_OK)
		{
			start_sect_addr += 4;
			write_count++;

		}
		else
		{
			return flash_get_error();

		}
	}

	return 0;
}



























void flash_read_data(uint32_t start_sect_addr, uint32_t *rx_buff, uint16_t numberofwords)
{

	while(1)
	{
		*rx_buff  =  *(__IO uint32_t *)start_sect_addr;
		start_sect_addr +=4;
		rx_buff++;
		if(!(numberofwords--))
		{
			break;
		}


	}

}

void float_to_bytes(uint8_t * rsult_buff, float value)
{
	union{
		float float_value;
		uint8_t bytes_value[4];
	}data_t;

	data_t.float_value = value;

	for( int i = 0; i < 4; i++)
	{
		rsult_buff[i] = data_t.bytes_value[i];
	}

}

float bytes_to_float(uint8_t * value)
{
	float result;
	union{
		float float_value;
		uint8_t bytes_value[4];
	}data_t;

	for(int i = 0; i < 4; i++)
	{
		data_t.bytes_value[i] =  value[i];
	}

	result =  data_t.float_value;

	return result;
}

 void get_str(uint32_t *src_data, char *dest_buff)
 {

	 int numberofbytes =  ((strlen((char *)src_data)/4) + ((strlen((char *)src_data) %4) != 0))*4;

	for( int i = 0 ; i < numberofbytes; i++ )
	{
		int word_idx =  i / sizeof(uint32_t);
		int byte_idx =  i % sizeof(uint32_t);

		dest_buff[i] =  (src_data[word_idx] >> (8 * byte_idx)) & 0xff;
	}
 }


 void flash_write_num(uint32_t start_sect_addr, float num)
 {
	 float_to_bytes(temp_buff, num);
	 flash_write_data(start_sect_addr,(uint32_t *)temp_buff, 1);
 }

 float flash_read_num(uint32_t start_sect_addr)
 {
	 uint8_t buff[4];
	 float value;
	 flash_read_data(start_sect_addr,(uint32_t *)buff,1);
	 value =  bytes_to_float(buff);

	 return value;
 }

