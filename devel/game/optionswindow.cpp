
#include "optionswindow.h"

#include "logger.h"



COptionsWindow::COptionsWindow() {


    mGUI=DynaGUI::getInstance();
    mWindow=mGUI->createWindow( DWindow::CENTERED, DWindow::CENTERED, "Options");
    mWindow->newLine();

    mWindow->newLine();
    mInputButton=mWindow->addButton( "Input configuration", 160, this );

    mWindow->newLine();
    mVideoButton=mWindow->addButton( "Video options", 160, this );

    mWindow->newLine();
    mSoundButton=mWindow->addButton( "Sound options", 160, this );


    mWindow->newLine();
    mGameplayButton=mWindow->addButton( "Gameplay options", 160, this );

    mWindow->newLine();
    mWindow->newLine();
    mWindow->addSpace( 100 );
    mOK=mWindow->addButton( "Leave", 60, this );


    //mMouseSensitivityBar->setPercent( mMouseSensitivity*10 );

    mStatus=RUNNING;

    mVideoWindow=0;
    mSoundWindow=0;
    mInputWindow=0;
//    mGameplayWindow=0;
}


COptionsWindow::~COptionsWindow() {

    mGUI->destroyWindow(mWindow);

}


bool COptionsWindow::done() {

    switch( mStatus ) {
        case VIDEO:
            if( mVideoWindow->done() ) {
                delete(mVideoWindow);
                mWindow->show();
                mStatus=RUNNING;
            }
        break;

        case SOUND:
            if( mSoundWindow->done() ) {
                delete( mSoundWindow );
                mWindow->show();
                mStatus=RUNNING;
            }
        break;

        case INPUT:
            if( mInputWindow->done() ) {
                delete(mInputWindow);
                mWindow->show();
                mStatus=RUNNING;
            }
    }


    if( mStatus==DONE ) return(true);
    return(false);
}


void COptionsWindow::onButtonPress( DButton* b ) {

    if( b==mOK ) {
        mStatus=DONE;
    }

    if( b==mVideoButton ) {
        mWindow->hide();
        mVideoWindow=new CVideoConfigWindow();
        mStatus=VIDEO;
    }

    if( b==mSoundButton ) {
        mWindow->hide();
        mSoundWindow=new CSoundConfigWindow();
        mStatus=SOUND;
    }

    if( b==mInputButton ) {
        mWindow->hide();
        LOG("\n *** new input window !! ***");
        mInputWindow=new CInputConfigWindow();
        mStatus=INPUT;

    }

}


