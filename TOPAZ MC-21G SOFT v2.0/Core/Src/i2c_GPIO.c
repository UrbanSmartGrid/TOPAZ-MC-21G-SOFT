#include "def.h"


#define	I2C_CLR_SDA		SFP_SDA_L
#define	I2C_SET_SDA		SFP_SDA_H

#define	I2C_CLR_SCL		SFP_SCL_L
#define	I2C_SET_SCL		SFP_SCL_H

#define	I2C_DELAY		i2c_delay(I2C_DELAY_200KHz);


/******************************************************************************/
#define	I2C_DELAY_200KHz	10
#define	I2C_DELAY_100KHz	20
inline void i2c_delay(uint16_t del)
{
	while(del--)
	{
		asm("NOP");
	}
}


/******************************************************************************/
void i2c_start_condition(void)
{
	I2C_SET_SCL
    I2C_SET_SDA
    I2C_DELAY
    I2C_CLR_SDA
    I2C_DELAY
    I2C_CLR_SCL
    I2C_DELAY
}


/******************************************************************************/
void i2c_stop_condition(void)
{
    I2C_CLR_SDA
    I2C_DELAY
    I2C_SET_SCL
    I2C_DELAY
    I2C_SET_SDA
    I2C_DELAY
}


/******************************************************************************/
// Writing a bit
inline void i2c_write_bit(uint8_t bit)
{
    if(bit > 0)
		I2C_SET_SDA
    else			
		I2C_CLR_SDA

    I2C_DELAY
    I2C_SET_SCL
    I2C_DELAY
    I2C_CLR_SCL
}




/******************************************************************************/
// Reading a bit
inline uint8_t i2c_read_bit(void)
{
    uint8_t bit;
    
	
    I2C_SET_SDA
    I2C_DELAY
    I2C_SET_SCL
    I2C_DELAY
    
    if(SFP_SDA_IN_L) 
		bit = 0;
    else 
		bit = 1;
    
    I2C_CLR_SCL
    
    return bit;
}


/******************************************************************************/
// Writing a byte
bool i2c_write_byte(uint8_t byte, bool start, bool stop)
{
    uint8_t ack = 0;

	
    if(start) 
		i2c_start_condition();

    for(uint8_t i = 0; i < 8; i++)
    {
        i2c_write_bit(byte & 0x80);		// write the most-significant bit
        byte <<= 1;
    }
    
	ack = (i2c_read_bit()==true)?false:true;	// inverting ack

    if(stop) 
		i2c_stop_condition();
    
	
    return ack;
}


/******************************************************************************/
// Reading a byte
uint8_t i2c_read_byte(bool ack, bool stop)
{
    uint8_t byte = 0;
	

    for(uint8_t i = 0; i < 8; i++)
    {
        byte <<= 1;
        byte |= i2c_read_bit();
    }

    if(ack) 
		i2c_write_bit(0);
    else 
		i2c_write_bit(1);

    if(stop) 
		i2c_stop_condition();

	
    return byte;
}


/******************************************************************************/
// Sending a byte
bool i2c_send_byte( int8_t address, uint8_t data)
{
    if(i2c_write_byte(address << 1, true, false))   // start, send address, write
    {
        // send data, stop
		if(i2c_write_byte(data, false, true))  
			return true;
    }
    
	
    i2c_stop_condition();   // make sure to impose a stop if NAK'd
	
	
    return false;
}


/******************************************************************************/
// Receiving a byte
uint8_t i2c_receive_byte(uint8_t address)
{
    if(i2c_write_byte((address << 1) | 0x01, true, false ))   // start, send address, read
    {
        return i2c_read_byte(false, true);
    }
	

    return 0;   // return zero if NAK'd
}


/******************************************************************************/
// Sending a byte of data
bool i2c_send_byte_data(uint8_t address, uint8_t reg, uint8_t data)
{
	if(i2c_write_byte(address << 1, true, false))	// start, send address, write
    {
        if(i2c_write_byte(reg, false, false))		// send desired register
        {
            if(i2c_write_byte(data, false, true))	// send data, stop
				return true;
        }
    }

    i2c_stop_condition();
	
	
    return false;
}


/******************************************************************************/
// Receiving a byte of data
uint8_t i2c_receive_byte_data(uint8_t address, uint8_t reg)
{
    if(i2c_write_byte(address << 1, true, false))	// start, send address, write
    {
        if(i2c_write_byte(reg, false, false))		// send desired register
        {
            if(i2c_write_byte((address << 1) | 0x01, true, false))	// start again, send address, read
            {
                return i2c_read_byte(false, true);	// read data
            }
        }
    }

    i2c_stop_condition();
	
	
    return 0;   // return zero if NACKed
}




