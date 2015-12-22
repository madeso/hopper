

#include "CountDown.h"
#include "soundmanager.h"
#include "hoppersounds.h"

#include "screen.h"

CCountDown::CCountDown()  {

    mDelay = new CDelay( 1000 );

    mCount = 3;

    // mGameMessageWindow = new CGameMessageWindow();
    int w = CScreen::getInstance()->getRenderWindow()->getWidth();
    int h = CScreen::getInstance()->getRenderWindow()->getHeight();

    //mWindow = CGUIManager::getInstance()->createImage( Ogre::Vector4( (w-128)/2, 0+h/5 , 128, 128 ), "countdown.void" );
    mWindow = DynaGUI::getInstance()->createImageWindow( Ogre::Vector4( (w-128)/2, 0+h/5 , 128, 128 ), "countdown.void" );
}

CCountDown::~CCountDown() {
    delete( mDelay );

    // delete( mGameMessageWindow );

    DynaGUI::getInstance()->destroyWindow( mWindow );

}


bool CCountDown::done() {

    if( mDelay->isOver() ) {

        switch( mCount ) {

            case 3:
                CSoundManager::getInstance()->play2D( SOUND_THREE, 1.f );
                // mGameMessageWindow->setMessage( "* 3 *" );
                mWindow->setMaterial( "countdown.three" );
                mDelay->restart();
            break;

            case 2:
                CSoundManager::getInstance()->play2D( SOUND_TWO, 1.f );
                // mGameMessageWindow->setMessage( "* 2 *" );
                mWindow->setMaterial( "countdown.two" );
                mDelay->restart();
            break;

            case 1:
                CSoundManager::getInstance()->play2D( SOUND_ONE, 1.f );
                // mGameMessageWindow->setMessage( "* 1 *" );
                mWindow->setMaterial( "countdown.one" );
                mDelay->restart();
            break;

            case 0:
                CSoundManager::getInstance()->play2D( SOUND_GO, 1.f );
                // mGameMessageWindow->setMessage( " GO ! " );
                mWindow->setMaterial( "countdown.go" );
                mDelay->restart();
                return( true );
            break;

        }

        mCount-- ;

    }

    return( false );
}
