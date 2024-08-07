#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <SensirionI2cScd30.h>
#include <DFRobot_I2C_Multiplexer.h>

/*Create an I2C Multiplexer object, the address of I2C Multiplexer is 0x70*/
DFRobot_I2C_Multiplexer I2CMultiplexer(&Wire, 0x70);
//Create a SCD4x object
SensirionI2CScd4x scd4x;    
//Create a SCD4x object
SensirionI2cScd30 scd30;


void setup()
{
  Serial.begin(115200);

  I2CMultiplexer.begin();
  Wire.begin();
  delay(1000);

  Serial.println("Scan ready!");

  scd4x_init(0);
  scd4x_init(1);
  scd4x_init(2);
  scd4x_init(3);
  scd30_init(4);

}

void loop()
{
    scd4x_read(0);
    scd4x_read(1);
    scd4x_read(2);
    scd4x_read(3);
    scd30_read(4);
  
}

void scd4x_init(int bus)
{
  I2CMultiplexer.selectPort(bus);
  scd4x.begin(Wire);
  scd4x.startPeriodicMeasurement();
}

void scd30_init(int bus)
{
  I2CMultiplexer.selectPort(bus);
  scd30.begin(Wire, 0x61);
  scd30.startPeriodicMeasurement(0);
}

void scd4x_read(int bus)
{
    uint16_t error;
    char errorMessage[256];

    uint16_t co2;
    float temperature;
    float humidity;
    
    delay(2500);

    I2CMultiplexer.selectPort(bus);

    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else if (co2 == 0) {
        Serial.println("Invalid sample detected, skipping.");
    } else {
        Serial.print(String(bus)+","+String(temperature)+","+String(humidity)+","+String(co2)+"\n");
    }

}

void scd30_read(int bus) 
{
    uint16_t error;
    char errorMessage[256];

    float co2 = 0.0;
    float temperature = 0.0;
    float humidity = 0.0;

    delay(2500);

    I2CMultiplexer.selectPort(bus);

    scd30.blockingReadMeasurementData(co2, temperature, humidity);

    Serial.print(String(bus)+","+String(temperature)+","+String(humidity)+","+String(co2)+"\n");

}

//  -- END OF FILE --

