

#include "delay.h"



CDelay::CDelay(int aDelay ) {

    mTimer=new Ogre::Timer();
    // mTimer=CTimeManager::getInstance();

    mDelay=aDelay;

    restart();

}

CDelay::~CDelay() {


    delete( mTimer );

}


bool CDelay::isOver() {

    // return( mTestTime < mTimer->getCurrent() );
    return( mTestTime < mTimer->getMilliseconds() );

}


void CDelay::restart() {

    // mTestTime = mTimer->getCurrent() + mDelay;
    mTestTime = mTimer->getMilliseconds() + mDelay;

}


