
#include "timemanager.h"

#include "screen.h"


CTimeManager::CTimeManager() {

    mTimer=CScreen::getInstance()->getRoot()->getTimer();

    mLastTime=mTimer->getMilliseconds();

    mPaused=false;
}


int CTimeManager::getCurrent() {
    // return( mTimer->getMilliseconds());
    return( mCurrentTime );
}


void CTimeManager::update() {

    //if( !mPaused ) {

        mCurrentTime=mTimer->getMilliseconds();
        mFrameTime=mCurrentTime-mLastTime;
        mLastTime=mCurrentTime;
    //}

}

/*
int CTimeManager::getElapsed() {
    return(mFrameTime);
}
*/

int CTimeManager::getFrametime() {

    if( mFrameTime<1 ) return(1);
    if( mFrameTime>500 ) return(500);
    return( mFrameTime);

}

float CTimeManager::getTimeFactor() {

    if( mFrameTime<1 ) mFrameTime=1;
    if( mFrameTime>500 ) mFrameTime=500;

    return( (float)mFrameTime/16.66666666666f );  // 60 FPS = 1
    // return( (float)mFrameTime/32.f );  // half speed
}


/*

int CTimeManager::getElapsed( int min, int max ) {

    int t=getElapsed();

    if(t>max) return(max);
    if(t<min) return(min);
    return(t);

}
*/

void CTimeManager::pause() {
    mPaused=true;
}

void CTimeManager::unpause() {
    mPaused=false;
}

