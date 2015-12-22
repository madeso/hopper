

#include "gameplaytest.h"
#include "logger.h"


CGameplayTest::CGameplayTest( int aLevelNum ):CGame( aLevelNum ) {

    LOG("\n=== Gameplay Test init ===");

    // mShip->showBoundingBox(true);
    // mDirectionArrowWindow=new CDirectionArrowWindow();
    mShipStatWindow=new CShipStatWindow();
    mShipOrientationWindow=new CShipOrientationWindow();

    mShipController=new CPlayerControlledShipController( mShip );
    //mShipController=new CAutoHoverShipController( mShip );

    mTunnels = new CTunnels();

   // mCSGTunnels = new CSGTunnels( mShip->getPosition() - Ogre::Vector3 ( 50,50,50) );

}


CGameplayTest::~CGameplayTest() {

    delete( mShipController );

    //delete( mCSGTunnels );
    delete( mTunnels );

    delete( mShipOrientationWindow );
    delete( mShipStatWindow );

}


void CGameplayTest::gameplay() {

    mShipController->run();

    coreGameplay();

    mTunnels->update( mShip->getPosition() );


    // game over condition

    if( mShip->getStatus() == CShip::STATUS_DEAD ) {
        mGameState=GS_GAMEOVER;
    }

}

