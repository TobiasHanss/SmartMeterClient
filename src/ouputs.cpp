#include "output.h"

#include "eMShome.h"
#include "settings.h"

extern eMShome SmartMeter;
extern CSettings Settings;

COutput::COutput(void)
{
    for (uint8_t i = 0; i < NO_OF_OUTPUTS; i++)
    { 
        m_Out[i] = false;
        DelayCounter[i] = 0;
    }  
    NextUpdateTime = millis() +1000;
}


void COutput::update(void)
{
    unsigned long Time = millis();
    if (NextUpdateTime < Time)
    {  
        NextUpdateTime = Time + 1000;
        checkOutputs();
    }
}


void COutput::checkOutputs(void)
{
    int32_t OnAt;
    int32_t OffAt;
    int32_t Power;

    bool Enabled;

    char nLx_EN[6] = {"L1_EN"};
    char nLx_ON[6] = {"L1_ON"};
    char nLx_OFF[7] = {"L1_OFF"};   

    int32_t OnDelay  = Settings.getInt("ON_DELAY");
    int32_t OffDelay  = Settings.getInt("OFF_DELAY");;

    for (uint8_t i = 0; i < NO_OF_OUTPUTS; i++)
    {
        Enabled = Settings.getStr(nLx_EN) == "on" ? true : false;
        nLx_EN[1] += 1; //Increment Lx_EN
        OnAt =  Settings.getInt(nLx_ON);
        nLx_ON[1] += 1; //Increment Lx_ON
        OffAt = Settings.getInt(nLx_OFF);
        nLx_OFF[1] += 1; //Increment Lx_OFFS
        
        Power = SmartMeter.getActivePower_W(i+1);

        if (Enabled)
        {
            //Serial.printf("OutNo:%d State:%d OnAt:%d OffAt:%d Power:%d PowerGoodTime:%d\n",i,m_Out[i],OnAt,OffAt,Power,PowerGoodTime[i]);
            if (m_Out[i])
            {
                if (Power >= OffAt)
                {
                    DelayCounter[i] ++;
                }
                else
                {
                    DelayCounter[i] = 0;
                }

                if (DelayCounter[i] >= OffDelay) 
                {
                    m_Out[i]= false;
                }
            }
            else
            {
                if (Power <= OnAt)
                {
                    DelayCounter[i] ++;
                }
                else
                {
                    DelayCounter[i] = 0;
                }

                if (DelayCounter[i] >= OnDelay) 
                {
                    m_Out[i]= true;
                }
            }
        }
    }
}