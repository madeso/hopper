

#ifndef _CTIMER_H_
#define _CTIMER_H_


#include "Ogre.h"


/// Simple delay class.

#include "timemanager.h"

class CDelay {

    public:

    CDelay( int aDelay );
    ~CDelay();

    bool isOver();

    void restart();


    private:
    Ogre::Timer* mTimer;
    //CTimeManager* mTimer;
    int mDelay;
    // int mOldTime;
    int mTestTime;

};



#endif



