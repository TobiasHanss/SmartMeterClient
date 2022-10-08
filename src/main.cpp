#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
//#include "OLED.h"
#include "settings.h"
#include "eMShome.h"
#include "WebIf.h"
#include "output.h"
#include <ArduinoOTA.h>
#include <WiFiAP.h>

#define LED 2

CSettings Settings("/settings.json",1024);
CSettings Config("/config.json",400);
CSettings Secure("/secure.json",100);

COutput Outputs;
WebIf   WebInterface(80);
eMShome SmartMeter(Config.get("eMShomeIP"),Config.get("eMShomePW"));

bool bInSetupMode = false;

//************************************************************
//************************************************************
void SetupOTA(void)
{
    ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";
        SPIFFS.end();
        Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}


//************************************************************
//************************************************************
void SetupAP(void)
{
  String sSSID = Config.get("DevName");
  sSSID.replace(" ","");
  char cSSID[12];
  sSSID.toCharArray(cSSID,sizeof(cSSID));
  Serial.printf("AP SSID:%s",cSSID);
  WiFi.softAP(cSSID);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}


//************************************************************
//************************************************************
void Connect2LocalWifi(void)
{
  WiFi.begin(Config.get("SSID"), Secure.get("PSK"));

  String sDevName = Config.get("DevName");
  sDevName.replace(" ","");
  char DevName[12];
  sDevName.toCharArray(DevName,sizeof(DevName));
  MDNS.begin(DevName);

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
}

//************************************************************
//************************************************************
void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  pinMode(LED, OUTPUT);
  digitalWrite(LED,HIGH);

  if (Secure.get("PSK"))
  {
    Connect2LocalWifi();
    SetupOTA();
    SmartMeter.connect();
    WebInterface.init(bInSetupMode);
  }
  else
  {
    bInSetupMode = true;
    SetupAP();
    WebInterface.init(bInSetupMode);
  }
}



//************************************************************
//************************************************************
int counter = 0;
void loop() 
{
  digitalWrite(LED,(bool)((++counter>>4)&0x1));

  if (!bInSetupMode)
  {
    Outputs.update();
    SmartMeter.update();
    ArduinoOTA.handle();
  }
  WebInterface.update();

  //DisplayUpdate();
  delay(50);
}




