#include "WebIf.h"
#include "debug.h"
#include <SPIFFS.h> 
#include <ArduinoJson.h>
#include "eMShome.h"
#include "settings.h"
#include <WiFi.h>
#include "output.h"

extern eMShome SmartMeter;
extern CSettings Settings;
extern CSettings Config;
extern COutput Outputs;


WebIf::WebIf(int nPort)
{
    m_oServer = new WebServer(nPort);
}

void WebIf::listFiles()
{
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    Serial.println("FILE   \t\tSize");
    while(file){
        Serial.printf("%s \t%d Byte\n",file.name(),file.size());
        file = root.openNextFile();
    }
}

void WebIf::init()
{

    if (!SPIFFS.begin(true)) {
        fail("SPIFFS init failed");
    }

    //listFiles();
       
    m_oServer->on("/data.json", std::bind(&WebIf::onRequestData,this));

    m_oServer->on("/settings.post", HTTP_POST ,std::bind(&WebIf::onSettingsPost,this));

    m_oServer->on("/output.post", HTTP_POST ,std::bind(&WebIf::onOutputPost,this));

    m_oServer->on("/config.post", HTTP_POST ,std::bind(&WebIf::onConfigPost,this));

    m_oServer->on("/secure.json", std::bind(&WebIf::onSecure,this));

    m_oServer->serveStatic("/", SPIFFS, "/");

    m_oServer->begin();
}

void WebIf::onSecure(void)
{
    m_oServer->send(403, "text/plain", "Access denied"); //Send web page
}

void WebIf::onSettingsPost(void)
{

    Settings.set("L1_EN","off");
    Settings.set("L2_EN","off");
    Settings.set("L3_EN","off");
    Settings.set("ALL_EN","off");

    for (int i = 0 ; i < m_oServer->args();i++ )
    {
        String Value = m_oServer->arg(i);
        if (Value == "off")
            Settings.set(m_oServer->argName(i),"on"); 
        else
            Settings.set(m_oServer->argName(i),Value);       

        Serial.printf("%s -> %s\n",m_oServer->argName(i),m_oServer->arg(i));
    }
    Settings.save();
    m_oServer->send(200, "text/plain", ""); //Send web page
    

}

void WebIf::onOutputPost(void)
{

    Outputs.set(0,false);
    Outputs.set(1,false);
    Outputs.set(2,false);

    for (int i = 0 ; i < m_oServer->args();i++ )
    {
        if (m_oServer->argName(i) == "OUT1") {Outputs.set(0,true);}
        if (m_oServer->argName(i) == "OUT2") {Outputs.set(1,true);}
        if (m_oServer->argName(i) == "OUT3") {Outputs.set(2,true);}      
        //Serial.printf("%s -> %s\n",m_oServer->argName(i),m_oServer->arg(i));
    }

    m_oServer->send(200, "text/plain", ""); //Send web page
}

void WebIf::onConfigPost(void)
{
    for (int i = 0 ; i < m_oServer->args();i++ )
    {
        String Value = m_oServer->arg(i);
        if (Value == "off")
            Config.set(m_oServer->argName(i),"on"); 
        else
            Config.set(m_oServer->argName(i),Value);       

        Serial.printf("%s -> %s\n",m_oServer->argName(i),m_oServer->arg(i));
    }
    Config.save();
    m_oServer->send(200, "text/plain", ""); //Send web page
    ESP.restart();
}

void WebIf::onRequestData(void)
{
    StaticJsonDocument<300> oJSON;    
    static int i = 0 ;

    oJSON["Epoch"] = String(SmartMeter.getEpoch());
    oJSON["L1_W"] = String(SmartMeter.getActivePower_W(1));    
    oJSON["L2_W"] = String(SmartMeter.getActivePower_W(2)); 
    oJSON["L3_W"] = String(SmartMeter.getActivePower_W(3)); 
    oJSON["ALL_W"] = String(SmartMeter.getActivePower_W(0)); 
    oJSON["LOCAL_IP"] = WiFi.localIP();
    oJSON["MAC"]    = WiFi.macAddress();  
    oJSON["OUT1"] = Outputs.getOnOff(0);
    oJSON["OUT2"] = Outputs.getOnOff(1);
    oJSON["OUT3"] = Outputs.getOnOff(2);
    String buf;
    serializeJson(oJSON,buf);
    m_oServer->send(200, "application/json", buf);
}

void WebIf::update(void)
{
    m_oServer->handleClient();
}

