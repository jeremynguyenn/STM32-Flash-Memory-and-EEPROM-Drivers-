#ifndef __I2C_H__
#define __I2C_H__

void i2c1_init(void);
void i2c1_byte_read(char saddr,char maddr, char *data);
void i2c1_burst_read(char saddr, char maddr, char *data, int n);
void i2c1_write_byte(char saddr, char maddr, char data);
void i2c1_burst_write(char saddr,char maddr, char *data, int n);

#endif
