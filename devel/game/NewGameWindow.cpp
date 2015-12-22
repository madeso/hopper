

#include "NewGameWindow.h"


CNewGameWindow::CNewGameWindow() {

    mWindow = DynaGUI::getInstance()->createWindow( DWindow::CENTERED, DWindow::CENTERED );

    mTutorialButton = mWindow->addButton( "Tutorial", 100, this );
    mWindow->newLine();
    mLandingButton = mWindow->addButton( "Landing",100, this );
    mWindow->newLine();
    mRacingButton = mWindow->addButton( "Racing", 100, this );
    mWindow->newLine();
    mWindow->newLine();
    //CGUIManager::getInstance()->addSpace( mWindow, 50 );
    mCancelButton = mWindow->addButton( "Cancel", 100, this );


    mDone = false;


}

CNewGameWindow::~CNewGameWindow() {

    DynaGUI::getInstance()->destroyWindow( mWindow );

}

bool CNewGameWindow::done() {
    return( mDone );
}


int CNewGameWindow::getSelection() {
    return( mSelection );
}


void CNewGameWindow::onButtonPress( DButton* b ) {

    if( b == mTutorialButton ) {
        mSelection = GM_NEWGAME_TUTORIAL;
        mDone = true;
    }

    if( b == mLandingButton ) {
        mSelection = GM_NEWGAME_LANDING;
        mDone = true;
    }

    if( b == mRacingButton ) {
        mSelection = GM_NEWGAME_RACING;
        mDone = true;
    }

    if( b == mCancelButton ) {
        mSelection = GM_NOTHING;
        mDone = true;
    }

}
