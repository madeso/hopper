

#include "PoweredByWindow.h"

#include "screen.h"


CPoweredByWindow::CPoweredByWindow() {

    mGUI = DynaGUI::getInstance();

    // mText = mGUI->createWindow( BetaGUI::MAX, BetaGUI::CENTERED );
    mWindow = mGUI->createWindow( DWindow::MAX, DWindow::CENTERED );

    mWindow->addText( "Powered by :", 175 );
    mWindow->newLine();
    mLogo = mWindow->addImage( 175,50, "Logo/Ogre" );

/*
    float screenwidth = CScreen::getInstance()->getRenderWindow()->getWidth();
    float screenheight = CScreen::getInstance()->getRenderWindow()->getHeight();

    float width = 175;
    float height = 50;

    float x = screenwidth - width - 20;
    float y = (screenheight - height) / 2 + 32;
*/


    mLogoNames.push_back("Logo/Ogre");
    mLogoNames.push_back("Logo/OgreOde");
    mLogoNames.push_back("Logo/ODE");
    mLogoNames.push_back("Logo/OpenAL");
    mLogoNames.push_back("Logo/SQlite");

    mCurrentLogo = mLogoNames.begin();

    //mLogo=mGUI->createImage(Ogre::Vector4(x,y,width, height), (*mCurrentLogo) );
    mLogo->setMaterial( (*mCurrentLogo) );

    mFadeOutDone = false;

    mDelay = new CDelay( 2000 );

}


void CPoweredByWindow::run() {

    if( mDelay->isOver() ) {

        if( mFadeOutDone ) {
            mCurrentLogo++;
            if( mCurrentLogo == mLogoNames.end() )
                mCurrentLogo = mLogoNames.begin() ;
            mLogo->setMaterial( (*mCurrentLogo) );
            mFadeOutDone = false;
            mDelay->restart();
        }
        else {
            // mLogo->setTransparency( ... );
            mFadeOutDone = true;
        }

    }

}



CPoweredByWindow::~CPoweredByWindow() {

    mGUI->destroyWindow( mWindow );

    delete( mDelay );
}


