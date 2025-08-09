#ifndef __AT24CXX_EEPROM
#define __AT24CXX_EEPROM


void erase_all_pages(void);
float eeprom_read_number (uint16_t page, uint16_t offset);
void eeprom_write_number(uint16_t page, uint16_t offset, float data);
void  eeprom_read(uint16_t page, uint16_t offset, uint8_t *data, uint16_t data_size);
void  eeprom_write(uint16_t page, uint16_t offset, uint8_t *data, uint16_t data_size);


#endif
