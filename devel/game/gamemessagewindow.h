
#ifndef _GAMEMESSAGEWINDOW_
#define _GAMEMESSAGEWINDOW_

#include "dynagui.h"
#include "delay.h"

/**
  * CGameMessageWindow : displays a short text for the provided amout of time and then hide itself
  *
  * if the display timeout is 0, the window never hides itself
  * timeout in ms, defaults to 2 secs.
  */

class CGameMessageWindow {

    public:
    CGameMessageWindow( int timeout = 2000 );
    ~CGameMessageWindow();

    void show();
    void hide();

    void update();

    void setMessage( std::string message );

    private:
    DynaGUI* mGUI;
    DWindow* mWindow;
    DText* mMessage;

    int mTimeOut;
    CDelay* mHideDelay;

};


#endif

