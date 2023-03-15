/**
 * @file ModbusWaterSensor.cpp
 * @author tardhiansyah (tardhians@gmail.com)
 * @brief CPP file of Modbus Water Sensor Library
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "ModbusWaterSensor.h"

void ModbusSensor::begin(byte addressCode, byte functionCode, byte registerAddress, byte numRegister)
{
  _readCommand[0] = addressCode;
  _readCommand[1] = functionCode;
  _readCommand[2] = registerAddress >> 8;    // bit shift to 0x00
  _readCommand[3] = registerAddress;
  _readCommand[4] = numRegister >> 8;        // bit shift to 0x00
  _readCommand[5] = numRegister;
}

void ModbusSensor::requestData()
{
  // get CRC of the read command
  int crc = getCRC16(_readCommand, 6);
  _readCommand[6] = crc & 0xFF;       // least significant CRC
  _readCommand[7] = (crc >> 8) & 0xFF;         // most significant CRC

  // send the read command
  _modbus.write(_readCommand, sizeof(_readCommand));
  _modbus.flush();
}

byte ModbusSensor::receiveData()
{
  byte responseData[9];

  // Check if sensor responded to the read command
  if (!_modbus.available())
  {
    Serial.println(F("No response from sensor"));
    return NO_RESPONSE;
  }

  // read response data from sensor
  if (_modbus.available())
  {
    _modbus.readBytes(responseData, sizeof(responseData));
  }

  // get CRC of the response data
  int crc = getCRC16(responseData, 7);

  // verify integrity of the data
  if (responseData[7] != (crc & 0xFF) || responseData[8] != ((crc >> 8) & 0xFF))
  {
    Serial.println(F("response data from sensor is corrupted"));
    return INTEGRITY_FAILED;
  }

  // filter and copy only necessary respons
  _rawData[0] = responseData[3];
  _rawData[1] = responseData[4];
  _rawData[2] = responseData[5];
  _rawData[3] = responseData[6];

  return NO_ERROR;
}

void ModbusSensor::resetRawData()
{
  memset(_rawData, 0, sizeof(_rawData));
}

int ModbusSensor::getCRC16(const byte *data, byte size)
{
  // source code for CRC16 Calculation: https://github.com/LacobusVentura/MODBUS-CRC16

  uint16_t crc = 0xFFFF;
	for(byte i = 0; i < size; i++)
	{
		crc = crc ^ data[i];
		for(byte bit = 0; bit < 8; bit++)
		{
			if(crc & 0x01)
			{
				crc = (crc >> 1) ^ 0xA001;
			}
			else
			{
				crc = crc >> 1;
			}
		}
	}

	return crc;
}

float EC_TDS::readValue()
{
  // send command to request sensor data
  requestData();

  delay(DELAY_MODBUS);

  // receive data from sensor
  byte error = receiveData();
  float value = (error == NO_ERROR) ? (float) (_rawData[2] * 256 + _rawData[3]) / 10 : 0.0;

  // reset data from sensor 
  resetRawData();

  return value;
}

float EC_TDS::readCalibrationValue()
{
  // send command to request sensor data
  requestData();

  delay(DELAY_MODBUS);

  // receive data from sensor
  byte error = receiveData();
  float value = (error == NO_ERROR) ? (float) (_rawData[0] * 256 + _rawData[1]) / 10 : 0.0;

  // reset data from sensor 
  resetRawData();

  return value;
}

float PH_Temperature::readPH()
{
  // send command to request sensor data
  requestData();

  delay(DELAY_MODBUS);

  // receive data from sensor
  byte error = receiveData();
  float value = (error == NO_ERROR) ? (float) (_rawData[2] * 256 + _rawData[3]) / 10 : 0.0;

  // reset data from sensor 
  resetRawData();

  return value;
}

float PH_Temperature::readTemperature()
{
  // send command to request sensor data
  requestData();

  delay(DELAY_MODBUS);

  // receive data from sensor
  byte error = receiveData();
  float value = (error == NO_ERROR) ? (float) (_rawData[0] * 256 + _rawData[1]) / 10 : 0.0;

  // reset data from sensor 
  resetRawData();

  return value;
}