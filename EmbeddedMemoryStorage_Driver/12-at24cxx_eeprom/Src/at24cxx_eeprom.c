#include <stdint.h>
#include <math.h>
#include "at24cxx_eeprom.h"
#include "i2c.h"
#include "timebase.h"
#include "string.h"



/*PB8.....SCL
 *PB9....SDA
 *GND....GND
 *3.3v..VCC*/

#define NUM_OF_PAGES 	256
#define PAGE_SIZE		64
#define DEVICE_ADDR 	0xA0

static uint16_t get_rw_byte_size(uint16_t size, uint16_t offset);

static void float_to_bytes(uint8_t * rsult_buff, float value);
static float bytes_to_float(uint8_t * value);
uint8_t temp_bytes[4];


void  eeprom_write(uint16_t page, uint16_t offset, uint8_t *data, uint16_t data_size)
{

	uint16_t mem_address;
	uint16_t _bytes_towrite;
    uint16_t page_address_bits;
    uint16_t end_page;
    uint16_t remaining_data_size;
    uint16_t data_pos = 0;

    /*Calculate end page*/

    remaining_data_size = data_size;
    page_address_bits =  log(PAGE_SIZE)/log(2);
    end_page =  page + ((data_size + offset -1)/PAGE_SIZE);

    while( page <=  end_page)
    {
    	/*Calculate address of mem location*/
    	mem_address =  (page << page_address_bits) |offset;

    	/*Calculate remaining bytes to write*/
    	_bytes_towrite = get_rw_byte_size(remaining_data_size,offset);

    	/*Write the data to the eeprom*/
    	 i2c_mem_write( DEVICE_ADDR, mem_address,2, &data[data_pos],_bytes_towrite);

    	 page++;
    	 offset = 0;
    	 remaining_data_size  =  _bytes_towrite;
    	 data_pos += _bytes_towrite;


    	 /*Delay 5ms*/
    	 delay(5);

    }


}


void  eeprom_read(uint16_t page, uint16_t offset, uint8_t *data, uint16_t data_size)
{
	uint16_t mem_address;
	uint16_t _bytes_toread;
    uint16_t page_address_bits;
    uint16_t end_page;
    uint16_t start_page;
    uint16_t numofpages;
    uint16_t data_pos = 0;

    /*Calculate the ending page */
    end_page =  page +  ((data_size + offset) / PAGE_SIZE);

    start_page =  page;

    /*Calculate num of pages to read*/
    numofpages =  (end_page - start_page) + 1;

    /*Calculate number of bits used for page addressing*/
    page_address_bits =  log(PAGE_SIZE)/log(2);


    for( int i = 0; i < numofpages; i++)
    {
    	/*Calculate mem address to read from*/
    	mem_address =  start_page << page_address_bits | offset;

    	_bytes_toread = get_rw_byte_size(data_size,offset);
    	i2c_mem_read(DEVICE_ADDR, mem_address,2,&data[data_pos],_bytes_toread);

    	start_page += 1;
    	offset = 0;
    	data_size  =  data_size - _bytes_toread;
    	data_pos   = data_pos + _bytes_toread;

    }


}

/*Get number of bytes to read or write*/
static uint16_t get_rw_byte_size(uint16_t size, uint16_t offset)
{
	if((size +  offset ) < PAGE_SIZE )
	{

		/*If there is enough space in the current page,
		 * return number of byte to read/write*/
		return size;
	}
	else
	{
		/*If there is not enough space in the current page,
		 * return number of bytes that can be written or read*/
		return (PAGE_SIZE - offset);
	}
}

void eeprom_page_erase(uint16_t page)
{
	// create a buffer to store the reset values
	uint8_t data[PAGE_SIZE];

	// calculate the memory address based on the page number
	uint16_t page_address_bits = log(PAGE_SIZE)/log(2);
	uint16_t mem_addr = page << page_address_bits;

	memset(data,0xff,PAGE_SIZE);

	// write the data to the EEPROM
	i2c_mem_write(DEVICE_ADDR, mem_addr, 2, data, PAGE_SIZE);

	/*Delay 5ms*/
	delay(5);


}

void eeprom_write_number(uint16_t page, uint16_t offset, float data)
{
  float_to_bytes(temp_bytes,data);

  eeprom_write(page, offset,temp_bytes,4);
}

float eeprom_read_number (uint16_t page, uint16_t offset)
{

	uint8_t buff[4];

	eeprom_read(page,offset,buff, 4);

	return (bytes_to_float(buff));

}



void erase_all_pages(void)
{
	for( int i = 0;  i < NUM_OF_PAGES; i++)
	{
		eeprom_page_erase(i);
	}
}
static void float_to_bytes(uint8_t * rsult_buff, float value)
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

static float bytes_to_float(uint8_t * value)
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
