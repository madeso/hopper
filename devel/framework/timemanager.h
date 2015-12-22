

#ifndef _CTIMEMANAGER_H_
#define _CTIMEMANAGER_H_

#include "Ogre.h"
#include "singleton.h"

/// basic time management.

class CTimeManager:public SSingleton<CTimeManager> {

  public:
    CTimeManager();
    // ~CTimeManager();

    void update();

//    int getElapsed();
//    int getElapsed( int min, int max );
    int getFrametime();
    float getTimeFactor();
    int getCurrent();

    void pause();
    void unpause();

    // void reset();

  private:

    int mCurrentTime;
    int mLastTime;
    int mFrameTime;
    bool mPaused;

    Ogre::Timer* mTimer;

};



#endif


