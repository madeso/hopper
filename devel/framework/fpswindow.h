
#ifndef _FPSWINDOW_
#define _FPSWINDOW_


#include "DynaGUI.h"

class CFPSWindow {

public:
    CFPSWindow();
    ~CFPSWindow();

    void setFPS( int FPS );

private:

    DText* mText;
    DWindow* mWindow;

    DynaGUI* mGUI;

};


#endif

