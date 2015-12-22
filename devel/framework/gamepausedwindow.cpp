
#include "gamepausedwindow.h"


#include "inputmanager.h"
#include "hopperinputactions.h"


/** @brief CGamePausedWindow
  *
  */
CGamePausedWindow::CGamePausedWindow():CBaseWindow("Game Paused") {


    mOptionsButton=mWindow->addButton( "Options", 200, this );
    mWindow->newLine();
    mMenuButton=mWindow->addButton( "Return to menu", 200, this );
    mWindow->newLine();
    mWindow->newLine();
    mOKButton=mWindow->addButton( "Back to game", 200, this );

    mStatus=WAITING;
    mReturnToMenu=false;

    mGUI->showMousePointer();
}

CGamePausedWindow::~CGamePausedWindow() {
    mGUI->hideMousePointer();
}


/** @brief done
  *
  */
bool CGamePausedWindow::done()
{
    mGUI->injectMouse();

    switch(mStatus) {

        case DO_OPTIONS:
            if( mOptionsWindow->done() ) {
                delete( mOptionsWindow );
                mWindow->show();
                mStatus=WAITING;
            }
        break;

        case BACK_TO_MENU:
            mReturnToMenu=true;
            mStatus=FINISHED;
        break;

        default:
        case WAITING:
            if( CInputManager::getInstance()->isReleased( IMR_ESCAPE ) ) {
                mStatus=FINISHED;
            }
        break;

        case FINISHED:
            mDone=true;
        break;

    }


    return( mDone );
}



bool CGamePausedWindow::returnToMenu() {
    return( mReturnToMenu );
}



void CGamePausedWindow::onButtonPress( DButton* b) {

    if( b==mOptionsButton ) {
        mOptionsWindow=new COptionsWindow();
        mStatus=DO_OPTIONS;
        mWindow->hide();
    }

    if( b==mMenuButton ) {
        mStatus=BACK_TO_MENU;
    }

    if( b==mOKButton ) {
        mStatus=FINISHED;
    }
}


