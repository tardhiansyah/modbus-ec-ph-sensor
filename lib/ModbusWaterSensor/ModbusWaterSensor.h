/**
 * @file ModbusWaterSensor.h
 * @author tardhiansyah (tardhians@gmail.com)
 * @brief Header file of Modbus Water Sensor Library
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef MODBUS_WATER_QUALITY_SENSOR
#define MODBUS_WATER_QUALITY_SENSOR

#include <Arduino.h>

enum ErrorType
{
  NO_ERROR,
  INTEGRITY_FAILED,
  NO_RESPONSE
};

class ModbusSensor
{
  public:
    ModbusSensor(HardwareSerial &modbus) : _modbus(modbus) {}
    ~ModbusSensor() {}
    void begin(byte addressCode, byte functionCode, byte registerAddress, byte numRegister);
  
  protected:
    byte _rawData[4];
    
    void requestData();
    byte receiveData();
    void resetRawData();
  
  private:
    HardwareSerial &_modbus;
    byte _readCommand[8];

    unsigned int getCRC16(const byte *data, unsigned int size);
};

class EC_TDS : public ModbusSensor
{
  public:
    EC_TDS(HardwareSerial& modbus) : ModbusSensor(modbus) {}
    ~EC_TDS() {}

    float readValue();
    float readCalibrationValue();
};

class PH_Temperature : public ModbusSensor
{
  public:
    PH_Temperature(HardwareSerial& modbus) : ModbusSensor(modbus) {}
    ~PH_Temperature() {}

    float readTemperature();
    float readPH();
};

#endif