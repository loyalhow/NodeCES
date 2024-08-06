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

// wifi connection
const char* ssid = ""; //wifi id
const char* password = ""; //password
const char* mqttServer = "192.168.2.13";

// mqtt client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup()
{
  Serial.begin(115200);

  I2CMultiplexer.begin();
  Wire.begin();
  delay(1000);

  Serial.println("Scan ready!");

  /*Print I2C device of each port*/
  for(uint8_t port = 0; port < 8; port++)
  {
    Serial.print("Port:");
    Serial.print(port);
    uint8_t *dev = I2CMultiplexer.scan(port);
    while (*dev)
    {
      Serial.print("  i2c addr ");
      Serial.print(*dev);
      dev++;
    }
    Serial.println();
  }

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
        Serial.print("scd4x_bus:");
        Serial.print(bus);
        Serial.print("\t");
        Serial.print("Co2:");
        Serial.print(co2);
        Serial.print("\t");
        Serial.print("Temperature:");
        Serial.print(temperature);
        Serial.print("\t");
        Serial.print("Humidity:");
        Serial.println(humidity);
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

    Serial.print("scd30_bus:");
    Serial.print(bus);
    Serial.print("\t");
    Serial.print("co2: ");
    Serial.print(co2);
    Serial.print("\t");
    Serial.print("temperature: ");
    Serial.print(temperature);
    Serial.print("\t");
    Serial.print("humidity: ");
    Serial.print(humidity);
    Serial.println();

}

//  -- END OF FILE --

