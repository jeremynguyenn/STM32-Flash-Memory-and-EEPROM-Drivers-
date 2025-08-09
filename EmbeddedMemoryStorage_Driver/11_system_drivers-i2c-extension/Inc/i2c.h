#ifndef __I2C_H__
#define __I2C_H__
#include <stdint.h>
void i2c1_init(void);
void i2c1_byte_read(char saddr,char maddr, char *data);
void i2c1_burst_read(char saddr, char maddr, char *data, int n);
void i2c1_write_byte(char saddr, char maddr, char data);
void i2c1_burst_write(char saddr,char maddr, char *data, int n);
void i2c_mem_read( uint16_t saddr, uint16_t maddr, uint16_t mem_addr_sz, uint8_t *p_data, uint16_t size);
void i2c_mem_write( uint16_t saddr, uint16_t maddr, uint16_t mem_addr_sz, uint8_t *p_data, uint16_t size);

#endif
