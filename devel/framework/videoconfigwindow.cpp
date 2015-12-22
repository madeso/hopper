

#include "videoconfigwindow.h"
#include "screen.h"
#include "logger.h"

#include "configfile.h"


/** @brief CVideoConfigWindow
  *
  * @todo: document this function
  */
CVideoConfigWindow::CVideoConfigWindow() {

    mGUI=DynaGUI::getInstance();
    mWindow=mGUI->createWindow( DWindow::CENTERED, DWindow::CENTERED, "Video Options");
    mWindow->newLine();


    mScreenWidth=CScreen::getInstance()->getRenderWindow()->getWidth();
    mScreenHeight=CScreen::getInstance()->getRenderWindow()->getHeight();
    mCurrentResolution=
        Ogre::StringConverter::toString( mScreenWidth )
        + " x "
        + Ogre::StringConverter::toString(mScreenHeight);
    mResolutionIndex=0;
    mPossibleResolutions = CScreen::getInstance()->getRoot()->getRenderSystem()->getConfigOptions()["Video Mode"].possibleValues;

    Ogre::StringVector::const_iterator i;
    LOG("\n Possible video modes :");
    LOG("\n -----------------------------");
    for( i=mPossibleResolutions.begin(); i!=mPossibleResolutions.end(); ++i ) {
        LOG( "\n| %s", (*i).c_str() );
    }
    LOG("\n -----------------------------");

    mWindow->newLine();
    mToggleFullscreenButton=mWindow->addButton( "Toggle Fullscreen", 160, this );

    mWindow->newLine();
    mWindow->newLine();
    mWindow->addText( "Resolution :", 100 );
    mWindow->newLine();

    mPreviousResolutionButton=mWindow->addButton( "<", 30, this );
    mResolutionText=mWindow->addText( mCurrentResolution, 250 );
    mNextResolutionButton=mWindow->addButton( ">", 30, this );
    mSwitchResolution=mWindow->addButton( "Switch", 60, this );

    mWindow->newLine();
    mWindow->newLine();
    mWindow->addSpace( 150 );
    mOKButton=mWindow->addButton( "OK", 60, this );

    mDone=false;
}


CVideoConfigWindow::~CVideoConfigWindow() {
    mGUI->destroyWindow(mWindow);
}


bool CVideoConfigWindow::done() {
    return( mDone);
}

void CVideoConfigWindow::onButtonPress( DButton* b) {



    if ( b==mToggleFullscreenButton ) {
        Ogre::RenderWindow* rwin=CScreen::getInstance()->getRenderWindow();
        rwin->setFullscreen( !rwin->isFullScreen(), rwin->getWidth(), rwin->getHeight() );
    }


    if ( b==mPreviousResolutionButton ) {
        if( mResolutionIndex>0 ) mResolutionIndex--;
        else mResolutionIndex = mPossibleResolutions.size()-1;

        mCurrentResolution=mPossibleResolutions[mResolutionIndex];
        mResolutionText->setText( mCurrentResolution );
    }

    if ( b==mNextResolutionButton ) {
        if( mResolutionIndex<(mPossibleResolutions.size()-1) ) mResolutionIndex++;
        else mResolutionIndex = 0;
        mCurrentResolution=mPossibleResolutions[mResolutionIndex];
        mResolutionText->setText ( mCurrentResolution );
    }

    if( b==mSwitchResolution ) {
        Ogre::RenderWindow* rwin=CScreen::getInstance()->getRenderWindow();
        int width = Ogre::StringConverter::parseUnsignedInt( Ogre::StringUtil::split( mCurrentResolution, "x", 1 ).at( 0 ) );
        int height = Ogre::StringConverter::parseUnsignedInt( Ogre::StringUtil::split( mCurrentResolution, "x", 1 ).at( 1 ) ) ;

        LOG("\n CVideoConfigWindow:  new resolution : %d, %d", width, height );


        if( rwin->isFullScreen() ) {
            rwin->setFullscreen( false, rwin->getWidth(), rwin->getHeight() );
            rwin->resize( width, height );
            rwin->setFullscreen( true, width, height );
        }
        else {
            rwin->resize( width, height );
        }

        // rwin->setFullscreen( rwin->isFullScreen(), width, height );
        DynaGUI::getInstance()->resolutionChanged();
        CInputManager::getInstance()->windowResized();  // for mouse input
        CScreen::getInstance()->repositionWindow();
        /*
        CScreen::getInstance()->getRoot()->getRenderSystem()->setConfigOption ("Video Mode", mCurrentResolution);
        CScreen::getInstance()->getRoot()->getRenderSystem()->reinitialise(); // obviously this can't be done here !
        */
    }

    if( b==mOKButton ) {
        mDone=true;

        CConfigFile conf("Hopper.conf");

        if( conf.load() ) {
            conf.setValue( "ScreenWidth", CScreen::getInstance()->getWidth() );
            conf.setValue( "ScreenHeight", CScreen::getInstance()->getHeight() );
            conf.setValue( "Fullscreen", CScreen::getInstance()->getRenderWindow()->isFullScreen() );
            conf.save();
        }
    }

}

