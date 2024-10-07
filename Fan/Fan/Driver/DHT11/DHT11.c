#include "DHT11.h"

void DHT11_init()
{
	DHT11_outputMode();
	DHT11_sendStartSignal();
}

void DHT11_outputMode()
{
	Gpio_initPin(&DHT11_DDR,DHT11_DATA_PIN,OUTPUT);
	//DHT11_DDR |= (1<<DHT11_DATA_PIN);
}

void DHT11_inputMode()
{
	Gpio_initPin(&DHT11_DDR,DHT11_DATA_PIN,INPUT);
	//DHT11_DDR &= ~(1<<DHT11_DATA_PIN);
}

void DHT11_sendLowSignal()
{
	Gpio_writePin(&DHT11_PORT, DHT11_DATA_PIN, GPIO_PIN_RESET);
	//DHT11_PORT &= ~(1<<DHT11_DATA_PIN);
}

void DHT11_sendHighSignal()
{
	Gpio_writePin(&DHT11_PORT, DHT11_DATA_PIN, GPIO_PIN_SET);
	//DHT11_PORT |= (1<<DHT11_DATA_PIN);
}

void DHT11_sendStartSignal()
{
	DHT11_outputMode();
	DHT11_sendLowSignal();
	_delay_ms(18);
	DHT11_sendHighSignal();
	_delay_us(40);
}

uint8_t DHT11_readPin()
{
	return ((DHT11_PIN) & (1<<DHT11_DATA_PIN)) ? 1 : 0;
}

uint8_t DHT11_waitResponse()
{
	uint8_t timeCounter = 0;
	
	DHT11_inputMode();
	
	// Low
	while(!DHT11_readPin())
	{
		_delay_us(1);
		timeCounter++;
		
		if(timeCounter == 255) return DHT11_ERROR;
	}
	// High
	while(DHT11_readPin())
	{
		_delay_us(1);
		timeCounter++;
		
		if(timeCounter == 255) return DHT11_ERROR;
	}
	
	return DHT11_OK;
}

uint8_t DHT11_readSensorData(uint8_t *arry)
{
	uint8_t timeCounter = 0;
	
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<8; j++)
		{
			timeCounter = 0;
			// Low
			while(!DHT11_readPin())
			{
				_delay_us(1);
				timeCounter++;
				
				if(timeCounter == 255) return DHT11_ERROR;
			}
			// High
			while(DHT11_readPin())
			{
				_delay_us(1);
				timeCounter++;
				
				if(timeCounter == 255) return DHT11_ERROR;
			}
			
			if(timeCounter < 50)
			{
				arry[i] &= ~(1<<(7-j));
			}
			else
			{
				arry[i] |= (1<<(7-j));
			}
		}
	}
	return DHT11_OK;
}

DHT11_DataType DHT11_readData()
{
	DHT11_DataType dht11;
	
	uint8_t sensorData[5] = {0,};
	
	DHT11_sendStartSignal();
	
	if(DHT11_waitResponse() == DHT11_ERROR)
	{
		dht11.checkSumError = DHT11_ERROR;
		return dht11;
	}
	if(DHT11_readSensorData(sensorData) == DHT11_ERROR)
	{
		dht11.checkSumError = DHT11_ERROR;
		return dht11;
	}
	
	dht11.humidInt = sensorData[0];
	dht11.humidDec = sensorData[1];
	dht11.tempInt = sensorData[2];
	dht11.tempDec = sensorData[3];
	dht11.checkSumError = sensorData[4];
	
	return dht11;
}