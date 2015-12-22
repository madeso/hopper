

#ifndef _POWEREDBYWINDOW_
#define _POWEREDBYWINDOW_


#include "dynagui.h"
#include <string>
#include "delay.h"

class CPoweredByWindow {

public:
    CPoweredByWindow();
    ~CPoweredByWindow();

    void run();

private:

    DynaGUI* mGUI;

    DImage* mLogo;

    DWindow* mWindow;

    std::vector<std::string> mLogoNames;
    std::vector<std::string>::const_iterator mCurrentLogo;

    CDelay* mDelay;

    bool mFadeOutDone;

};


#endif

