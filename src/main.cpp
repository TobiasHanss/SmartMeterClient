#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "OLED.h"
#include "settings.h"
#include "eMShome.h"
#include "WebIf.h"
#include "output.h"

CSettings Settings("/settings.json",1024);
CSettings Config("/config.json",400);
CSettings Secure("/secure.json",100);

COutput Outputs;
WebIf   WebInterface(80);
eMShome SmartMeter(Config.get("eMShomeIP"),Config.get("eMShomePW"));

//************************************************************
//************************************************************
void Connect2LocalWifi(void)
{
  
  InitDisplay();
  DisplayPrint(10,10,"Connecting to %s\n",Config.get("SSID"));
  DisplayUpdate();

  WiFi.begin(Config.get("SSID"), Secure.get("PSK"));

  MDNS.begin(Config.get("DevName"));

  while(WiFi.status() != WL_CONNECTED) 
  { 
    delay(500);
    Serial.print(".");
  }

    // Check if connected to wifi
  if(WiFi.status() != WL_CONNECTED) 
  {
      Serial.println("No Wifi!");
      return;
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  DisplayClear();
  DisplayUpdate();
}

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  
  Connect2LocalWifi();
  WebInterface.init();
  SmartMeter.connect();

  
}

void loop() 
{
  Outputs.update();
  SmartMeter.update();
  WebInterface.update();
  //DisplayUpdate();
  delay(100);
}




