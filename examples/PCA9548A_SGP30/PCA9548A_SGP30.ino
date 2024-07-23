#include "SGP30.h"
#include "TCA9548.h"

PCA9546 MP(0x70);

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
  Serial.println(__FILE__);
  Serial.print("TCA9548_LIB_VERSION: ");
  Serial.println(TCA9548_LIB_VERSION);
  Serial.print("SGP30_LIB_VERSION: ");
  Serial.println(SGP30_LIB_VERSION);
  Serial.println();

  Wire.begin();
  //  initialize multiplexer
  if (MP.begin() == false)
  {
    Serial.println("Multiplexer error");
  }
  channels = MP.channelCount();

  //  initialize the temperature sensors
  MP.selectChannel(0);
  if (SGP0.begin() == false)
  {
    Serial.println("SGP0 error");
  }
  MP.selectChannel(1);
  if (SGP1.begin() == false )
  {
    Serial.println("SGP1 error");
  }
  MP.selectChannel(2);
  if (SGP2.begin() == false )
  {
    Serial.println("SGP2 error");
  }
  MP.selectChannel(3);
  if (SGP3.begin() == false )
  {
    Serial.println("SGP3 error");
  }
  MP.selectChannel(4);
  if (SGP4.begin() == false )
  {
    Serial.println("SGP4 error");
  }
  MP.selectChannel(5);
  if (SGP5.begin() == false )
  {
    Serial.println("SGP5 error");
  }
  MP.selectChannel(6);
  if (SGP6.begin() == false )
  {
    Serial.println("SGP6 error");
  }
  MP.selectChannel(7);
  if (SGP7.begin() == false )
  {
    Serial.println("SGP7 error");
  }
}


void loop()
{
  if ((millis() - lastTime) > 5000)
  {
    lastTime = millis();
   
    MP.selectChannel(0);
    SGP0.read();
    Serial.print(SGP0.getCO2());
    Serial.print("\n");
    
    MP.selectChannel(1);
    SGP1.read();
    Serial.print(SGP1.getCO2());
    Serial.print("\b");
  
    MP.selectChannel(2);
    SGP2.read();
    Serial.print(SGP2.getCO2());
    Serial.print("\n");

    MP.selectChannel(3);
    SGP3.read();
    Serial.print(SGP3.getCO2());
    Serial.print("\n");

    MP.selectChannel(4);
    SGP4.read();
    Serial.print(SGP4.getCO2());
    Serial.print("\n");

    MP.selectChannel(5);
    SGP5.read();
    Serial.print(SGP5.getCO2());
    Serial.print("\n");

    MP.selectChannel(6);
    SGP6.read();
    Serial.print(SGP6.getCO2());
    Serial.print("\n");

    MP.selectChannel(7);
    SGP7.read();
    Serial.print(SGP7.getCO2());
    Serial.print("\n");

  }
}