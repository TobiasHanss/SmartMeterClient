#ifndef __WEBIF__
#define __WEBIF__
#include <WebServer.h>  // standard library

class WebIf
{
public:
    WebIf(int nPort);

    void init(void);
    void update(void);

private:
    void onRequestData(void);
    void onSettingsPost(void);
    void onOutputPost(void);
    void onConfigPost(void);
    void onSecure(void);
    void listFiles(void);    
    WebServer *m_oServer;

    bool m_Out1;
    bool m_Out2;
    bool m_Out3;
};
#endif
