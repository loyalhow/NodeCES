#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <SensirionI2cScd30.h>
#include <DFRobot_I2C_Multiplexer.h>

/*Create an I2C Multiplexer object, the address of I2C Multiplexer is 0x70*/
DFRobot_I2C_Multiplexer NCA9548_1(&Wire, 0x70);
DFRobot_I2C_Multiplexer NCA9548_2(&Wire, 0x71);
//Create a SCD4x object
SensirionI2CScd4x scd4x;    
//Create a SCD4x object
SensirionI2cScd30 scd30;


void setup()
{
  Serial.begin(115200);

  NCA9548_1.begin();
  NCA9548_2.begin();
  Wire.begin();
  delay(1000);

  Serial.println("Scan ready!");

  /*Print I2C device of each port*/
  for(uint8_t port = 0; port < 8; port++)
  {
    Serial.print("Port:");
    Serial.print(port);
    uint8_t *dev = NCA9548_1.scan(port);
    while (*dev)
    {
      Serial.print("  i2c addr ");
      Serial.print(*dev);
      dev++;
    }
    Serial.println();
  }

    /*Print I2C device of each port*/
  for(uint8_t port = 0; port < 8; port++)
  {
    Serial.print("Port:");
    Serial.print(port);
    uint8_t *dev = NCA9548_2.scan(port);
    while (*dev)
    {
      Serial.print("  i2c addr ");
      Serial.print(*dev);
      dev++;
    }
    Serial.println();
  }

  scd4x_init(NCA9548_1,0);
  scd4x_init(NCA9548_1,1);
  scd4x_init(NCA9548_1,2);
  scd4x_init(NCA9548_2,0);
  //scd30_init(NCA9548_2,1);

}

void loop()
{
    scd4x_read(NCA9548_1, "0x70", 0);
    scd4x_read(NCA9548_1, "0x70", 1);
    scd4x_read(NCA9548_1, "0x70", 2);
    scd4x_read(NCA9548_2, "0x71", 0);
  
}

void scd4x_init(DFRobot_I2C_Multiplexer multiplexer,int bus)
{

  multiplexer.selectPort(bus);
  scd4x.begin(Wire);
  scd4x.startPeriodicMeasurement();
}

void scd4x_read(DFRobot_I2C_Multiplexer multiplexer, String addr, int bus)
{
    uint16_t error;
    char errorMessage[256];

    

    uint16_t co2;
    float temperature;
    float humidity;
    
    delay(2500);

    multiplexer.selectPort(bus);

    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else if (co2 == 0) {
        Serial.println("Invalid sample detected, skipping.");
    } else {
        Serial.print(addr+","+String(bus)+","+String(temperature)+","+String(humidity)+","+String(co2)+"\n");

    }
    delay(2500);

}


//  -- END OF FILE --

