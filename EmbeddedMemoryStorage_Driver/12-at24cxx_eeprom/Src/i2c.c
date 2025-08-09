#include "i2c.h"
#include "stm32f4xx.h"

#define GPIOBEN		(1U<<1)
#define I2C1EN		(1U<<21)


#define I2C_100KHZ					80    // 0b 0101 0000
#define SD_MOD_MAX_RISE_TIME		17

#define		SR2_BUSY		(1U<<1)
#define     CR1_START		(1U<<8)
#define		SR1_SB			(1U<<0)
#define		SR1_ADDR		(1U<<1)
#define		SR1_TXE			(1U<<7)
#define		CR1_ACK			(1U<<10)
#define     CR1_STOP		(1U<<9)
#define		SR1_RXNE		(1U<<6)
#define	    SR1_BTF			(1U<<2)


#define _I2C_7BIT_ADD_WRITE(__ADDRESS__)                    ((uint8_t)((__ADDRESS__) & (uint8_t)(~I2C_OAR1_ADD0)))
#define _I2C_7BIT_ADD_READ(__ADDRESS__)                     ((uint8_t)((__ADDRESS__) | I2C_OAR1_ADD0))
#define _i2c_mem_addr_msb(__ADDRESS__)                      ((uint8_t)((uint16_t)(((uint16_t)((__ADDRESS__) & (uint16_t)0xFF00)) >> 8)))
#define _i2c_mem_addr_lsb(__ADDRESS__)                      ((uint8_t)((uint16_t)((__ADDRESS__) & (uint16_t)0x00FF)))
#define I2C_MEMADD_SIZE_8BIT            					0x00000001U

/*PB8.....SCL
 *PB9....SDA
 * */


static void i2c_request_mem_write(uint16_t saddr, uint16_t maddr, uint16_t mem_addr_sz);
static void i2c_request_mem_read(uint16_t saddr, uint16_t maddr, uint16_t mem_addr_sz);


void i2c1_init(void)
{
	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |=GPIOBEN;

	/*Set PB8 and PB9 mode to alternate function mode*/
	GPIOB->MODER &=~(1U<<16);
	GPIOB->MODER |=(1U<<17);

	GPIOB->MODER &=~(1U<<18);
	GPIOB->MODER |=(1U<<19);

	/*Set PB8 and PB9 output type to open drain*/
	GPIOB->OTYPER |=(1U<<8);
	GPIOB->OTYPER |=(1U<<9);

	/*Enable Pull-up resisters for PB8 and PB9*/
	GPIOB->PUPDR |=(1U<<16);
	GPIOB->PUPDR &=~(1U<<17);

	GPIOB->PUPDR |=(1U<<18);
	GPIOB->PUPDR &=~(1U<<19);

	/*Set PB8 and PB9 alternate function type to I2C1 (AF4)*/
	GPIOB->AFR[1] &=~(1U<<0);
	GPIOB->AFR[1] &=~(1U<<1);
	GPIOB->AFR[1] |=(1U<<2);
	GPIOB->AFR[1] &=~(1U<<3);

	GPIOB->AFR[1] &=~(1U<<4);
	GPIOB->AFR[1] &=~(1U<<5);
	GPIOB->AFR[1] |=(1U<<6);
	GPIOB->AFR[1] &=~(1U<<7);

	/*Enable clock access to I2C1*/
	RCC->APB1ENR |=I2C1EN;

	/*Enter reset mode*/
	I2C1->CR1 |=(1U<<15);

	/*Come out of reset mode*/
	I2C1->CR1 &=~(1U<<15);

	/*Set the peripheral clock*/
	I2C1->CR2 = (1U<<4); // 16Mhz

	/*Set I2C to standard mode, 100kH clock*/
	I2C1->CCR =	I2C_100KHZ;

	/*Set rise time*/
	I2C1->TRISE = SD_MOD_MAX_RISE_TIME;

	/*Enable I2C module*/
	I2C1->CR1 |=(1U<<0);

}

void i2c1_byte_read(char saddr,char maddr, char *data)
{
	volatile int tmp;

	/*Wait until is BUS  not busy*/
	while (I2C1->SR2 & (SR2_BUSY)){}

	/*Generate start*/
	 I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	  while (!(I2C1->SR1 & (SR1_SB))){}

	/*Transmit slave address + write*/
	   I2C1->DR = saddr << 1;

	/*Wait until address flag is set*/
	   while (!(I2C1->SR1 & (SR1_ADDR))){}

	/*Clear addr flag*/
	   tmp = I2C1->SR2;

	/*Send memory address*/
	   I2C1->DR = maddr;

	/*Wait until transmitter is empty*/
	  while (!(I2C1->SR1 & SR1_TXE)){}

	/*Generate restart*/
      I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
     while (!(I2C1->SR1 & SR1_SB)){}

	/*Transmit slave address + read*/
     I2C1->DR = saddr << 1 | 1;

	/*Wait until address flag is set*/
     while (!(I2C1->SR1 & (SR1_ADDR))){}

	/*Disable the Acknowledge*/
     I2C1->CR1 &= ~CR1_ACK;

	/*Clear addr flag*/
	   tmp = I2C1->SR2;

	/*Generate stop after data is received*/
	 I2C1->CR1 |= CR1_STOP;

	/*Wait until RXNE flag is set*/
	  while (!(I2C1->SR1 & SR1_RXNE)){}

	/*Read data from DR*/
	  *data++  =  I2C1->DR;

}

void i2c1_burst_read(char saddr, char maddr, char *data, int n)
{

	volatile int tmp;

	/*Wait until is BUS  not busy*/
	while (I2C1->SR2 & (SR2_BUSY)){}

	/*Generate start*/
	 I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	  while (!(I2C1->SR1 & (SR1_SB))){}

	/*Transmit slave address + write*/
	   I2C1->DR = saddr << 1;

	/*Wait until address flag is set*/
	   while (!(I2C1->SR1 & (SR1_ADDR))){}

	/*Clear addr flag*/
	   tmp = I2C1->SR2;

	/*Wait until transmitter is empty*/
    while (!(I2C1->SR1 & SR1_TXE)){}

	/*Send memory address*/
	   I2C1->DR = maddr;


	/*Generate restart*/
      I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
     while (!(I2C1->SR1 & SR1_SB)){}

	/*Transmit slave address + read*/
     I2C1->DR = saddr << 1 | 1;

	/*Wait until address flag is set*/
     while (!(I2C1->SR1 & (SR1_ADDR))){}

 	/*Clear addr flag*/
 	   tmp = I2C1->SR2;

	/*Disable the Acknowledge*/
     I2C1->CR1 |= CR1_ACK;

      while(n > 0)
      {
    	  if(n == 1)
    	  {
    			/*Disable the Acknowledge*/
    		     I2C1->CR1 &= ~CR1_ACK;


    			/*Generate stop after data is received*/
    			 I2C1->CR1 |= CR1_STOP;

    			/*Wait until RXNE flag is set*/
    			  while (!(I2C1->SR1 & SR1_RXNE)){}

    			/*Read data from DR*/
    			  *data++  =  I2C1->DR;
    			  break;
    	  }
    	  else
    	  {

  			/*Wait until RXNE flag is set*/
  			  while (!(I2C1->SR1 & SR1_RXNE)){}

  			/*Read data from DR*/
  			  *data++  =  I2C1->DR;

  			  n--;
    	  }
      }

}

void i2c1_write_byte(char saddr, char maddr, char data)
{
	volatile int tmp;

	/*Wait until is BUS  not busy*/
	while (I2C1->SR2 & (SR2_BUSY)){}

	/*Generate start*/
	 I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	  while (!(I2C1->SR1 & (SR1_SB))){}

	/*Transmit slave address + write*/
	   I2C1->DR = saddr << 1;

	/*Wait until address flag is set*/
	   while (!(I2C1->SR1 & (SR1_ADDR))){}

	/*Clear addr flag*/
	   tmp = I2C1->SR2;


	/*Wait until transmitter is empty*/
	   while (!(I2C1->SR1 & SR1_TXE)){}

	   /*Send memory address*/
	   I2C1->DR = maddr;

	  /*Send data*/
	  I2C1->DR = data;

	  /* Wait until transfer finished */
	  while (!(I2C1->SR1 & (SR1_BTF))){}

	  /* Generate stop */
	  I2C1->CR1 |= CR1_STOP;
}

void i2c1_burst_write(char saddr,char maddr, char *data, int n)
{
	volatile int tmp;

	/*Wait until is BUS  not busy*/
	while (I2C1->SR2 & (SR2_BUSY)){}

	/*Generate start*/
	 I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	  while (!(I2C1->SR1 & (SR1_SB))){}

	/*Transmit slave address + write*/
	   I2C1->DR = saddr << 1;

	/*Wait until address flag is set*/
	   while (!(I2C1->SR1 & (SR1_ADDR))){}

	/*Clear addr flag*/
	   tmp = I2C1->SR2;


	/*Wait until transmitter is empty*/
	   while (!(I2C1->SR1 & SR1_TXE)){}

	   /*Send memory address*/
	   I2C1->DR = maddr;

	   for(int i =0; i < n; i++)
	   {
			/*Wait until transmitter is empty*/
			   while (!(I2C1->SR1 & SR1_TXE)){}

			   /*Transmit data*/
			   I2C1->DR = *data++;
	   }
		  /* Wait until transfer finished */
		  while (!(I2C1->SR1 & (SR1_BTF))){}

		  /* Generate stop */
		  I2C1->CR1 |= CR1_STOP;
}

static void i2c_request_mem_write(uint16_t saddr, uint16_t maddr, uint16_t mem_addr_sz)
{

    __IO uint32_t tmpreg = 0x00U;

	  /* Generate Start */
	  SET_BIT(I2C1->CR1, I2C_CR1_START);

	  /* Wait until SB flag is set */
	  while (!(I2C1->SR1 & (SR1_SB))){}

	  I2C1->DR =  _I2C_7BIT_ADD_WRITE(saddr);

	  /* Wait until ADDR flag is set */
	  while (!(I2C1->SR1 & (SR1_ADDR))){}


	     /*Clear ADDR flag*/
	    tmpreg = I2C1->SR1;
	    tmpreg = I2C1->SR2;

	    /* Wait until TXE flag is set */
	      while (!(I2C1->SR1 & SR1_TXE)){}

	      /* If Memory address size is 8Bit */
	      if (mem_addr_sz == I2C_MEMADD_SIZE_8BIT)
	      {
	        /* Send Memory Address */
	        I2C1->DR = _i2c_mem_addr_lsb(maddr);
	      }

	      /* If Memory address size is 16Bit */
	      else
	      {
	        /* Send MSB of Memory Address */
	        I2C1->DR = _i2c_mem_addr_msb(maddr);

	        /* Wait until TXE flag is set */
	        while (!(I2C1->SR1 & SR1_TXE)){}

	        /* Send LSB of Memory Address */
	        I2C1->DR = _i2c_mem_addr_lsb(maddr);
	      }

}

static void i2c_request_mem_read(uint16_t saddr, uint16_t maddr, uint16_t mem_addr_sz)
{
	  __IO uint32_t tmpreg = 0x00U;

	  /* Enable Acknowledge */
	  SET_BIT(I2C1->CR1, I2C_CR1_ACK);

	  /* Generate Start */
	  SET_BIT(I2C1->CR1, I2C_CR1_START);

	  /* Wait until SB flag is set */
	  while (!(I2C1->SR1 & (SR1_SB))){}

	  I2C1->DR =  _I2C_7BIT_ADD_WRITE(saddr);

	  /* Wait until ADDR flag is set */
	  while (!(I2C1->SR1 & (SR1_ADDR))){}


	     /*Clear ADDR flag*/
	    tmpreg = I2C1->SR1;
	    tmpreg = I2C1->SR2;

	    /* Wait until TXE flag is set */
	      while (!(I2C1->SR1 & SR1_TXE)){}


	      /* If Memory address size is 8Bit */
	      if (mem_addr_sz == I2C_MEMADD_SIZE_8BIT)
	      {
	        /* Send Memory Address */
	        I2C1->DR = _i2c_mem_addr_lsb(maddr);
	      }

	      /* If Memory address size is 16Bit */
	      else
	      {
	        /* Send MSB of Memory Address */
	        I2C1->DR = _i2c_mem_addr_msb(maddr);

	        /* Wait until TXE flag is set */
	        while (!(I2C1->SR1 & SR1_TXE)){}

	        /* Send LSB of Memory Address */
	        I2C1->DR = _i2c_mem_addr_lsb(maddr);
	      }
	        /* Wait until TXE flag is set */
	        while (!(I2C1->SR1 & SR1_TXE)){}

	        /* Generate Restart */
	        SET_BIT(I2C1->CR1, I2C_CR1_START);

	        /* Wait until SB flag is set */
	        while (!(I2C1->SR1 & (SR1_SB))){}

	        /* Send slave address */
	        I2C1->DR = _I2C_7BIT_ADD_READ(saddr);

	        /* Wait until ADDR flag is set */
	        while (!(I2C1->SR1 & (SR1_ADDR))){}
}

void i2c_mem_write( uint16_t saddr, uint16_t maddr, uint16_t mem_addr_sz, uint8_t *p_data, uint16_t size)
{

	/*Check busy flag*/
	if(I2C1->SR2 & (SR2_BUSY))
	{
    	delay(5);

    	if(I2C1->SR2 & (SR2_BUSY))
    	{
    		//Do something
    	}

	}

	uint32_t XferSize = size;
	uint32_t XferCount =  size;

    /* Disable Pos */
    CLEAR_BIT(I2C1->CR1, I2C_CR1_POS);


    /* Send Slave Address and Memory Address */
    i2c_request_mem_write( saddr, maddr, mem_addr_sz);

    while (XferSize > 0U)
    {
        /*Wait until transmitter is empty*/
          while(!(I2C1->SR1 & SR1_TXE)){}

          /* Write data to DR */
          I2C1->DR = *p_data;

          /* Increment Buffer pointer */
          p_data++;


          /* Update counter */
          XferSize--;
          XferCount--;

          if ((I2C1->SR1 & SR1_BTF) && (XferSize != 0U))
            {
              /* Write data to DR */
              I2C1->DR = *p_data;

              /* Increment Buffer pointer */
              p_data++;

              /* Update counter */
              XferSize--;
              XferCount--;

            }
    }

    /* Wait until BTF flag is set */
    while(!((I2C1->SR1 & SR1_BTF))){}

    /* Generate Stop */
    SET_BIT(I2C1->CR1, I2C_CR1_STOP);

}

void i2c_mem_read( uint16_t saddr, uint16_t maddr, uint16_t mem_addr_sz, uint8_t *p_data, uint16_t size)
{
	  __IO uint32_t tmpreg = 0x00U;

	    /* Wait until BUSY flag is reset */
		while (I2C1->SR2 & (SR2_BUSY)){}

		uint32_t XferSize = size;
		uint32_t XferCount =  size;

	    /* Disable Pos */
	    CLEAR_BIT(I2C1->CR1, I2C_CR1_POS);

	    /* Send Slave Address and Memory Address */
	    i2c_request_mem_read( saddr, maddr, mem_addr_sz);


	    if (XferSize == 0U)
	    {
	      /* Clear ADDR flag */
	      tmpreg = I2C1->SR1;
	      tmpreg = I2C1->SR2;
	      /* Generate Stop */
	      SET_BIT(I2C1->CR1, I2C_CR1_STOP);
	    }

	    else if (XferSize == 1U)
	    {
	      /* Disable Acknowledge */
	      CLEAR_BIT(I2C1->CR1, I2C_CR1_ACK);

	      /* Clear ADDR flag */
	      tmpreg = I2C1->SR1;
	      tmpreg = I2C1->SR2;
	      /* Generate Stop */
	      SET_BIT(I2C1->CR1, I2C_CR1_STOP);
	    }

	    else if (XferSize == 2U)
	    {
	      /* Disable Acknowledge */
	      CLEAR_BIT(I2C1->CR1, I2C_CR1_ACK);

	      /* Enable Pos */
	      SET_BIT(I2C1->CR1, I2C_CR1_POS);

	      /* Clear ADDR flag */
	      tmpreg = I2C1->SR1;
	      tmpreg = I2C1->SR2;
	    }
	    else
	    {
	      /* Clear ADDR flag */
	        tmpreg = I2C1->SR1;
	        tmpreg = I2C1->SR2;
	    }

	    while (XferSize > 0U)
	    {
	        if (XferSize <= 3U)
	         {
	            /* One byte */
	            if (XferSize == 1U)
	            {
	                /* Wait until RXNE flag is set */
	              	while (!(I2C1->SR1 & SR1_RXNE)){}

	                /* Read data from DR */
	                 *p_data = (uint8_t)I2C1->DR;

	                 /* Increment Buffer pointer */
	                 p_data++;

	                 /* Update counter */
	                 XferSize--;
	                 XferCount--;

	            }
	            /* Two bytes */
	             else if (XferSize == 2U)
	             {
	                 /* Wait until BTF flag is set */
	               	while (!(I2C1->SR1 & (SR1_BTF))){}

	                /* Generate Stop */
	                SET_BIT(I2C1->CR1, I2C_CR1_STOP);

	                /* Read data from DR */
		             *p_data = (uint8_t)I2C1->DR;

		                 /* Increment Buffer pointer */
		                 p_data++;

		                 /* Update counter */
		                 XferSize--;
		                 XferCount--;

					/* Read data from DR */
					 *p_data = (uint8_t)I2C1->DR;

						 /* Increment Buffer pointer */
						 p_data++;

						 /* Update counter */
						 XferSize--;
						 XferCount--;



	             }
	            /* 3 Last bytes */
	            else
	            {
	                /* Wait until BTF flag is set */
	              	while (!(I2C1->SR1 & (SR1_BTF))){}

	                /* Disable Acknowledge */
	                CLEAR_BIT(I2C1->CR1, I2C_CR1_ACK);

	                /* Read data from DR */
		             *p_data = (uint8_t)I2C1->DR;

					 /* Increment Buffer pointer */
					 p_data++;

					 /* Update counter */
					 XferSize--;
					 XferCount--;

					/* Wait until BTF flag is set */
						while (!(I2C1->SR1 & (SR1_BTF))){}

				          /* Generate Stop */
				          SET_BIT(I2C1->CR1, I2C_CR1_STOP);

				          /* Read data from DR */
				          *p_data = (uint8_t)I2C1->DR;

						 /* Increment Buffer pointer */
						 p_data++;

						 /* Update counter */
						 XferSize--;
						 XferCount--;

				          /* Read data from DR */
				          *p_data = (uint8_t)I2C1->DR;

						 /* Increment Buffer pointer */
						 p_data++;

						 /* Update counter */
						 XferSize--;
						 XferCount--;


	            }

	         }

	        else
	        {
	            /* Wait until RXNE flag is set */
	          	while (!(I2C1->SR1 & SR1_RXNE)){}

	            /* Read data from DR */
	            *p_data = (uint8_t)I2C1->DR;

				 /* Increment Buffer pointer */
					 p_data++;

					 /* Update counter */
					 XferSize--;
					 XferCount--;

					if(I2C1->SR1 & (SR1_BTF))
					{
					  /* Read data from DR */
					  *p_data = (uint8_t)I2C1->DR;

					  /* Increment Buffer pointer */
					  p_data++;

					  /* Update counter */
					  XferSize--;
					  XferCount--;
					}

	        }
	    }
}






