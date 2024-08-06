#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <SensirionI2cScd30.h>
#include <DFRobot_I2C_Multiplexer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*Create an I2C Multiplexer object, the address of I2C Multiplexer is 0x70*/
DFRobot_I2C_Multiplexer I2CMultiplexer(&Wire, 0x70);
//Create a SCD4x object
SensirionI2CScd4x scd4x;    
//Create a SCD4x object
SensirionI2cScd30 scd30;

// wifi connection
const char* ssid = "JH1200-1200G-C6A0";
const char* password = "12345678";
const char* mqttServer = "192.168.168.100";

// mqtt client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup()
{
  Serial.begin(115200);
    
  WiFi.mode(WIFI_STA);
  connectWifi();
  delay(1000);

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
  
  if (mqttClient.connected()) { 
     
    scd4x_publish(0);
    scd4x_publish(1);
    scd4x_publish(2);
    scd4x_publish(3);
    scd30_publish(4);
  
    mqttClient.loop();
  } else {                  
    connectMQTTServer();    
  }
  
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

void scd4x_publish(int bus)
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
        char payload[120]; 
        snprintf(payload, sizeof(payload), "{\"temperature\":%.2f, \"humidity\":%.2f, \"co2\":%d, \"bus\":%d}", temperature, humidity, co2, bus);
        Serial.printf(payload);
        Serial.printf("\n");
        String topicString = WiFi.macAddress();
        char publishTopic[topicString.length() + 1];  
        strcpy(publishTopic, topicString.c_str());
        mqttClient.publish(publishTopic, payload);
    }
}

void scd30_publish(int bus) 
{
    uint16_t error;
    char errorMessage[256];

    float co2 = 0.0;
    float temperature = 0.0;
    float humidity = 0.0;

    delay(2500);

    I2CMultiplexer.selectPort(bus);

    error = scd30.blockingReadMeasurementData(co2, temperature, humidity);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute blockingReadMeasurementData(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }

    char payload[120]; 
    snprintf(payload, sizeof(payload), "{\"temperature\":%.2f, \"humidity\":%.2f, \"co2\":%.2f, \"bus\":%d}", temperature, humidity, co2, bus);
    Serial.printf(payload);
    Serial.printf("\n");
    String topicString = WiFi.macAddress();
    char publishTopic[topicString.length() + 1];  
    strcpy(publishTopic, topicString.c_str());
    mqttClient.publish(publishTopic, payload);
}

void connectMQTTServer()
{
  String clientId = "esp8266-" + WiFi.macAddress();
 
  if (mqttClient.connect(clientId.c_str())) { 
    Serial.println("MQTT Server Connected.");
    Serial.println("Server Address: ");
    Serial.println(mqttServer);
    Serial.println("ClientId:");
    Serial.println(clientId);
  } else {
    Serial.print("MQTT Server Connect Failed. Client State:");
    Serial.println(mqttClient.state());
    delay(3000);
  }   
}

void connectWifi(){
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");  
  Serial.println(""); 
}

//  -- END OF FILE --

