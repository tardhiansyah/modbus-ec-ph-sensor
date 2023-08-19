/**
 * @file main.cpp
 * @author tardhiansyah (tardhians@gmail.com)
 * @brief Example using the Modbus Water Sensor Library
 * @version 0.1.2
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <ModbusWaterSensor.h>

#define RX2_PIN     16
#define TX2_PIN     17
#define BAUDRATE_2  9600    // usually the sensor use baudrate 9600

// Communicating with sensor with Serial2 (HardwareSerial) of ESP32
EC_TDS sensor1(Serial2, RX2_PIN, TX2_PIN, BAUDRATE_2);
PH_Temperature sensor2(Serial2, RX2_PIN, TX2_PIN, BAUDRATE_2);

void setup()
{
  Serial.begin(115200); 

  sensor1.begin(0x02, 0x03, 0x00, 0x02);
  sensor2.begin(0x01, 0x03, 0x00, 0x02);
}

void loop()
{
  float tds = sensor1.readValue();
  float temperature = sensor2.readTemperature();
  float pH = sensor2.readPH();

  Serial.print("TDS: ");
  Serial.println(tds);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("pH: ");
  Serial.println(pH);

  delay(500);
}