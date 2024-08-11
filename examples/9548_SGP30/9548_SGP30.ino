

#include "SGP30.h"
#include <DFRobot_I2C_Multiplexer.h>

/*Create an I2C Multiplexer object, the address of I2C Multiplexer is 0x70*/
DFRobot_I2C_Multiplexer I2CMultiplexer(&Wire, 0x70);

uint8_t channels = 0;

SGP30 SGP0;    //  channel 0
SGP30 SGP1;   //  channel 1
SGP30 SGP2;   //  channel 2
SGP30 SGP3;    //  channel 3
SGP30 SGP4;   //  channel 4
SGP30 SGP5;   //  channel 5
SGP30 SGP6;    //  channel 6
SGP30 SGP7;   //  channel 7

uint32_t lastTime = 0;

void setup()
{
  Serial.begin(115200);
  I2CMultiplexer.begin();
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

}

void loop()
{
  if ((millis() - lastTime) > 5000)
  {
    lastTime = millis();
   
    I2CMultiplexer.selectPort(0);
    SGP0.measure(true);
    Serial.print(SGP0.getH2_raw());
    Serial.print("\t");
    Serial.print(SGP0.getEthanol_raw());
    Serial.print("\n");
    Serial.print(SGP0.getCO2());
    Serial.print("\n");
    
    I2CMultiplexer.selectPort(1);
    SGP1.measure(true);
    Serial.print(SGP1.getH2_raw());
    Serial.print("\t");
    Serial.print(SGP1.getEthanol_raw());
    Serial.print("\n");
    Serial.print(SGP1.getCO2());
    Serial.print("\n");
  
    I2CMultiplexer.selectPort(2);
    SGP1.measure(true);
    Serial.print(SGP2.getH2_raw());
    Serial.print("\t");
    Serial.print(SGP2.getEthanol_raw());
    Serial.print("\n");
    Serial.print(SGP2.getCO2());
    Serial.print("\n");

    I2CMultiplexer.selectPort(3);
    SGP1.measure(true);
    Serial.print(SGP3.getH2_raw());
    Serial.print("\t");
    Serial.print(SGP3.getEthanol_raw());
    Serial.print("\n");
    Serial.print(SGP3.getCO2());
    Serial.print("\n");

    I2CMultiplexer.selectPort(4);
    SGP1.measure(true);
    Serial.print(SGP4.getH2_raw());
    Serial.print("\t");
    Serial.print(SGP4.getEthanol_raw());
    Serial.print("\n");
    Serial.print(SGP4.getCO2());
    Serial.print("\n");

    I2CMultiplexer.selectPort(5);
    SGP1.measure(true);
    Serial.print(SGP5.getH2_raw());
    Serial.print("\t");
    Serial.print(SGP5.getEthanol_raw());
    Serial.print("\n");
    Serial.print(SGP5.getCO2());
    Serial.print("\n");

    I2CMultiplexer.selectPort(6);
    SGP1.measure(true);
    Serial.print(SGP6.getH2_raw());
    Serial.print("\t");
    Serial.print(SGP6.getEthanol_raw());
    Serial.print("\n");
    Serial.print(SGP6.getCO2());
    Serial.print("\n");

    I2CMultiplexer.selectPort(7);
    SGP1.measure(true);
    Serial.print(SGP7.getH2_raw());
    Serial.print("\t");
    Serial.print(SGP7.getEthanol_raw());
    Serial.print("\n");
    Serial.print(SGP7.getCO2());
    Serial.print("\n");
  }
}


//  -- END OF FILE --
