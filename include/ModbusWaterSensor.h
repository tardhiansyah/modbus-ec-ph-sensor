/**
 * @file ModbusWaterSensor.h
 * @author tardhiansyah (tardhians@gmail.com)
 * @brief Header file of Modbus Water Sensor Library
 * @version 0.1.1
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
    ModbusSensor(HardwareSerial &modbus, const byte RXpin, const byte TXpin, const unsigned int baudrate) : _modbus(modbus)
    {
      _RXpin = RXpin;
      _TXpin = TXpin;
      _baudrate = baudrate;

      _modbus.begin(_baudrate, SERIAL_8N1, _RXpin, _TXpin);
    }
    ~ModbusSensor() {}

    void begin(byte addressCode, byte functionCode, byte registerAddress, byte numRegister);
  
  protected:
    const unsigned int DELAY_MODBUS = 40;
    byte _rawData[4];
    
    void requestData();
    byte receiveData();
    void resetRawData();
  
  private:
    HardwareSerial &_modbus;
    byte _readCommand[8];
    byte _RXpin;
    byte _TXpin;
    unsigned int _baudrate;

    int getCRC16(const byte *data, byte size);
};

class EC_TDS : public ModbusSensor
{
  public:
    EC_TDS(HardwareSerial &modbus, const byte RXpin, const byte TXpin, const unsigned int baudrate) : ModbusSensor(modbus, RXpin, TXpin, baudrate) {}
    ~EC_TDS() {}

    float readValue();
    float readCalibrationValue();
};

class PH_Temperature : public ModbusSensor
{
  public:
    PH_Temperature(HardwareSerial &modbus, const byte RXpin, const byte TXpin, const unsigned int baudrate) : ModbusSensor(modbus, RXpin, TXpin, baudrate) {}
    ~PH_Temperature() {}

    float readTemperature();
    float readPH();
};

#endif