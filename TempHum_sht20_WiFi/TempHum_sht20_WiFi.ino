/*
  This code will deploy data to your Antares project device with the following structure:
  (Note that nesting the JSON object can only be done up to 2 levels using this library)
  {
    "temperature": random-int,
    "humidity": random-int,
    "wind_speed": random-float,
    "rain_level": random-float,
    "location" : {
      "latitude": "static-string",
      "longitude": "static-string"
    }
  }
  For more information please visit https://antares.id/id/docs.html
*/

#include <AntaresESP32HTTP.h>
#include <WiFiManager.h>
#include <Wire.h>
#include "DFRobot_SHT20.h"
#include "src/evomo.c"
DFRobot_SHT20    sht20;

AntaresESP32HTTP antares(ACCESSKEY);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);    // make ESP32 access point
  WiFiManager wm;         // config to wifi connection
  bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect(device_ssid,device_password); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
  antares.setDebug(true);
  sht20.initSHT20();                                  // Init SHT20 Sensor
  delay(100);
  sht20.checkSHT20();                                 // Check SHT20 Sensor
}

float humd,temp;
void sensorSHT(){
  humd = sht20.readHumidity();                  // Read Humidity
  temp = sht20.readTemperature();               // Read Temperature
}

void loop() {
  sensorSHT();
  // Add variable data to storage buffer
  antares.add("T", temp);
  antares.add("H", humd);

  // Send from buffer to Antares
  antares.send(projectName, deviceName);
  delay(kirim);
}
