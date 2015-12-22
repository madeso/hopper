
#include "menu.h"


#include "logger.h"

#include "screen.h"
#include "hopperinputactions.h"


#include "levelinfo.h"

/**
Title Screen / Menu showing the landing AI in action.
*/

CMenu::CMenu():CGame(CLevelInfo::getRandomLevel( LANDING_GAME ) ) {

    LOG("\n\n");
    LOG("======================\n");
    LOG("= Menu               =\n");
    LOG("======================\n");

    makeUI();

    mShipController=new CLandingShipController( mShip, mLevel );

    mReturnState=GM_NOTHING;

    mMouseX=320;
    mMouseY=220;

    mCamDelay=new CDelay(6000);
    mCamMode=0;

    LOG("\n Init CMenu OK \n");

    // mFPSWin=new CFPSWindow();

    mCamController=new CBehindCameraController( mCam, mShip );

    mBypassInput=false;

    mOptionsUI = 0;
    mNewGameWindow = 0;

}


CMenu::~CMenu() {

    LOG("\n~CMenu()" );

//    delete(mDebugWin);

    delete(mCamController);



    LOG("\n~CMenu() : destroying windows...");

    mGUI->hideMousePointer();

    LOG("\n~CMenu() : destroying menu window...");
    mGUI->destroyWindow( mWindow );
    LOG("\n~CMenu() : destroying Logo window...");
    mGUI->destroyWindow( mLogo );

    LOG("\n~CMenu() : destroying 'Powered by' window...");
    delete( mPoweredByWindow );

    LOG("\n~CMenu() : done ");
}

int CMenu::run() {

    mFrametime=mTimeManager->getFrametime();

    if( mGameState != GS_PAUSED ) {
        physicsUpdate();
    }

    mPoweredByWindow->run();

    gameplay();
    mShipController->run();

    if( mHeartBeat->isOver() ) {
        mHeartBeat->restart();
//        updateEmitters();
        mAirDustParticles->setPosition( mCam->getPosition() + mShip->getLinearVelocity() );
        mAirDustParticles->emit( true );
        mParticleEmitterManager->update();
    }

    if( mOptionsUI && mOptionsUI->done() ) {
            delete(mOptionsUI);
            mOptionsUI=0;
            // debugOverlays();
            mWindow->show();
            mBypassInput=false;
    }

    if( mNewGameWindow  && mNewGameWindow->done() ) {

        mReturnState = mNewGameWindow->getSelection();
        if( mReturnState == GM_NOTHING ) {
            mWindow->show();
        }

        delete( mNewGameWindow );
        mNewGameWindow = 0;

    }


    mFPSWin->setFPS( (int)CScreen::getInstance()->getFPS() );

    if( !mBypassInput ) {

        if( mInputManager->isReleased( IMR_NEWGAME_LANDING )) {
            return( GM_NEWGAME_LANDING );
        }

        if( mInputManager->isReleased( IMR_NEWGAME_RACING )) {
            return( GM_NEWGAME_RACING );
        }

        if( mInputManager->isReleased( IMR_NEWGAME_TUTORIAL )) {
            return( GM_NEWGAME_TUTORIAL );
        }

        if( mInputManager->isReleased( IMR_NEWGAME_EDITOR )) {
            return( GM_EDITOR );
        }

        if( mInputManager->isReleased( IMR_NEWGAME_TEST )) {
            return( GM_NEWGAME_TEST );
        }

        if(mInputManager->isReleased( IMR_FREECAM )) {
            mFreeCam = !mFreeCam;
            delete(mCamController);

            if( mFreeCam )
                mCamController = new CEditorCameraController( mCam );
            else
                mCamController = new CBehindCameraController( mCam, static_cast<CActor* >(mShip) );

        }

        if( mInputManager->isReleased( IMR_ESCAPE )) {
            return( GM_QUIT );
        }

    }

    if( mReturnState != GM_NOTHING) return( mReturnState );

    mSoundManager->set3DListener( mCam->getPosition(),
                    mCam->getVelocity(),
                    mCam->getDirection(),
                    Ogre::Vector3( 0,1,0) );                        // up vector


    return(GM_GAMEPLAY);

}


void CMenu::gameplay() {

    mGUI->injectMouse();


    // mShipController->run();

    coreGameplay();

    if( !mFreeCam ) {

        if( mCamDelay->isOver() ) {
            mCamDelay->restart();
            mCamMode=rand()%4;

            delete(mCamController);

            switch( mCamMode ) {
                default:
                case 0:
                    mCamController=new CBehindCameraController( mCam, mShip );
                    break;
                case 1:
                    mCamController=new CRotateAroundCameraController( mCam, mShip, 3 );
                    break;
                case 2:
                    mCamController=new CFollowCameraController( mCam, mShip, Ogre::Vector3( rand()%30+5, rand()%10, rand()%30+5 ) );
                    break;
                case 3:
                    mCamController=new CLookAtCameraController( mCam, mShip, static_cast<CLandingShipController*>(mShipController)->getGoalPosition() + Ogre::Vector3( 0,3,0 ) );
                    break;

            }

        }
    }

    mCamController->run();


}




void CMenu::onButtonPress( DButton* b ) {

/*
    if( b==mButtonLanding ) {
        mReturnState=GM_NEWGAME_LANDING;
    }

    if( b==mButtonRacing ) {
        mReturnState=GM_NEWGAME_RACING;
    }

    if( b==mButtonTutorial ) {
        mReturnState=GM_NEWGAME_TUTORIAL;
    }
*/


    if( b==mButtonNewGame ) {
        mWindow->hide();
        mNewGameWindow = new CNewGameWindow();
    }

    if( b==mButtonEditor ) {
        mReturnState=GM_EDITOR;
    }

    if( b==mButtonQuit ) {
        mReturnState=GM_QUIT;
    }

    if( b==mButtonOptions ) {
        if( !mOptionsUI ) {
            mWindow->hide();
            mBypassInput=true; // fudge to avoid keypressed collision while redefining keys;
            mOptionsUI=new COptionsWindow();

            //debugOverlays();
        }
    }
}



void CMenu::makeUI() {


    mScreenWidth=CScreen::getInstance()->getWidth();
    mScreenHeight=CScreen::getInstance()->getHeight();


    mGUI=DynaGUI::getInstance();
    mLogo=mGUI->createImageWindow( Ogre::Vector4((mScreenWidth-460)/2, 0 ,460,160 ), "Hoper/Logo" );

    mWindow=mGUI->createWindow( DWindow::CENTERED, DWindow::MAX );

    mButtonNewGame = mWindow->addButton( "New Game", 100, this );
    mWindow->addSpace( 40 );

    mButtonOptions=mWindow->addButton( "Options",100, this );
    mButtonEditor=mWindow->addButton( "Editor",100, this );
    mWindow->addSpace( 20 );
    mButtonQuit=mWindow->addButton( "Quit",100, this );


    mGUI->showMousePointer();
    // debugOverlays();

    mPoweredByWindow = new CPoweredByWindow();

    /*
    mDebugWin=new CDebugWindow(5);

    mDebugWin->setText(0, "coucou" );
    mDebugWin->setText(1, "c'est cool" );
    mDebugWin->setText(2, "yeehaa" );
    mDebugWin->setText(3, "ouaip" );
    mDebugWin->setText(4, "gnii" );
    mDebugWin->setText(5, "oops" );
    */

}


void CMenu::debugOverlays() {

        Ogre::OverlayManager::OverlayMapIterator i(Ogre::OverlayManager::getSingletonPtr()->getOverlayIterator());
        Ogre::Overlay* o;
        Ogre::OverlayContainer* oe;
        Ogre::OverlayContainer* oe2;


        LOG("\n----------------------------------------------------");

        while( i.hasMoreElements() ) {
            o=i.peekNextValue();

            LOG("\n+%s", o->getName().c_str() );

            Ogre::Overlay::Overlay2DElementsIterator oeit(o->get2DElementsIterator() );

            while( oeit.hasMoreElements() ) {
                oe=oeit.getNext();
                LOG("\n\t+%s", oe->getName().c_str() );


                Ogre::OverlayContainer::ChildContainerIterator oeit2( oe->getChildContainerIterator()  );
                while( oeit2.hasMoreElements() ) {
                    oe2=oeit2.getNext();
                    LOG("\n\t\t+%s", oe2->getName().c_str() );
                }

                // oeit.moveNext();
            }

            i.moveNext();
        }

        LOG("\n----------------------------------------------------\n");

}
