#ifndef __OUTPUT__
#define __OUTPUT__

#include "Arduino.h"

#define NO_OF_OUTPUTS 3

class COutput
{
public:
    COutput(void);
    
    void setOnOff(uint8_t No, char * State){ m_Out[No] = ((State[0] == 'o') & (State[1] == 'n')) ? true : false; }
    void set(uint8_t No, bool State) { m_Out[No] = State; }

    String getOnOff(uint8_t No) { if(m_Out[No])return "on"; return "off";}
    bool get(uint8_t No) {return m_Out[No];}

    void update(void);


private:
    void checkOutputs(void);

    bool m_Out[NO_OF_OUTPUTS];
    uint32_t DelayCounter[NO_OF_OUTPUTS];
    unsigned long NextUpdateTime;
};

#endif