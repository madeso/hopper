

#ifndef _COUNTDOWN_
#define _COUNTDOWN_

#include "delay.h"

#include "gamemessagewindow.h"


class CCountDown {

public:
    CCountDown();
    ~CCountDown();

    bool done();

private:
    CDelay* mDelay;

    int mCount;

    CGameMessageWindow* mGameMessageWindow;
    DWindow* mWindow;

};




#endif


