

#include "GameSequences.h"
#include "game.h"
/** @brief CGameSequence
  *
  * @todo: document this function
  */
CGameSequence::CGameSequence( CGame* game) {

    mDone = false;
    mGame = game;


}

/** @brief ~CGameSequence
  *
  * @todo: document this function
  */
CGameSequence::~CGameSequence() {

}

/*
 * For parts that need initialisation after object creation.
 */
void CGameSequence::init() {}


/** @brief done
  *
  * @todo: document this function
  */
bool CGameSequence::done() {
    return( mDone );
}

// ------------------------------------------------------------------------


/** @brief CTimedGameSequence
  *
  * @todo: document this function
  */
CTimedGameSequence::CTimedGameSequence( CGame* game, int duration):
    CGameSequence( game ) {

    mDuration = new CDelay( duration );

}

/** @brief ~CTimedGameSequence
  *
  * @todo: document this function
  */
CTimedGameSequence::~CTimedGameSequence() {
    delete( mDuration );
}


void CTimedGameSequence::init() {
    mDuration->restart();
}


bool CTimedGameSequence::isOver() {

    mDone = mDuration->isOver();

    return( mDone );

}

// --------------------------------------------------------------------------




CGameOverSequence::CGameOverSequence( CGame* game ): CGameSequence( game )  {

    mGame->switchCameraController( new CRotateAroundCameraController( mGame->getCamera(), mGame->getShip() ) );
    mGame->switchShipController( new CEmptyShipController( mGame->getShip() ) );

}

bool CGameOverSequence::done() {
    return( true );
}


CWaitForInputSequence::CWaitForInputSequence( CGame* game ): CGameSequence( game ) {

    mGameMessageWindow = new CGameMessageWindow();
}

CWaitForInputSequence::~CWaitForInputSequence() {
    delete( mGameMessageWindow );
}

void CWaitForInputSequence::init() {
    mGameMessageWindow->setMessage( "Press [Thrust] or [Fire] to continue." );
}

bool CWaitForInputSequence::done() {

    if( CInputManager::getInstance()->isReleased( IMP_THRUST )
        || CInputManager::getInstance()->isReleased( IMR_FIRE ) ) {
        mGame->setStatus( GS_END );
        mDone = true;
     }

    return( mDone );

}

CPauseSequence::CPauseSequence( CGame* game ): CTimedGameSequence( game, 2000 ) { }

bool CPauseSequence::done() {
    return( isOver() );
}

// --------------------------------------------------------------------------


CLandingGameplaySequence::CLandingGameplaySequence( CGame* game ): CGameSequence( game ) {

    mGame->switchCameraController( new CBehindCameraController( mGame->getCamera(), mGame->getShip() ));
    mGame->switchShipController( new CPlayerControlledShipController(mGame->getShip() ));

}

bool CLandingGameplaySequence::done() {

    mGame->gameplay();

    switch( mGame->getStatus() ) {

        case GS_GAMEOVER:  /* CGame::GAMEOVER */
            mGame->addSequence( "gameover", new CGameOverSequence( mGame ) );
            mGame->addSequence( "pause", new CPauseSequence( mGame ) );
            mGame->addSequence( "waitforinput", new CWaitForInputSequence( mGame ), "pause" );
            mDone = true;
        break;

        case GS_GAMEWON: /* CGame::GAMEWON */
            mGame->addSequence( "gamewon", new CLandingWonSequence( mGame ) );
            mGame->addSequence( "pause", new CPauseSequence( mGame ) );
            mGame->addSequence( "waitforinput", new CWaitForInputSequence( mGame ), "pause" );
            mDone = true;
        break;

        case GS_REFUELING:
            mGame->addSequence( "refueling", new CRefuelingSequence( mGame ) );
            mDone = true;
        break;

    }

    return( mDone );
}



// --------------------------------------------------------------------------

#include "hoppersounds.h"

CRefuelingSequence::CRefuelingSequence( CGame* game ): CGameSequence( game ) {
    mGame->switchCameraController( new CRotateAroundCameraController( mGame->getCamera(), mGame->getShip(), 3 ));
    mGame->switchShipController( new CEmptyShipController(mGame->getShip() ));

    mSoundRefuel=new CSound3D( SOUND_REFUEL, true );
    mSoundRefuel->setPosition( mGame->getShip()->getPosition() );
    mSoundRefuel->play();
}

CRefuelingSequence::~CRefuelingSequence() {
    delete( mSoundRefuel );
}


void CRefuelingSequence::init() {

}


bool CRefuelingSequence::done() {

    CShip* ship = mGame->getShip();

    if( ship->getFuel() < 100 ) {
        ship->refuel( CTimeManager::getInstance()->getTimeFactor()/10.f );
        return( false );
    }
    else {

        CSoundManager::getInstance()->play3D( SOUND_REFUEL_OK, ship->getPosition(), 1 );
        mSoundRefuel->stop();
        mGame->addSequence( "Landing Gameplay", new CLandingGameplaySequence( mGame )  );
        mGame->setStatus( GS_GAMEPLAY );
        return( true );

    }
}

// --------------------------------------------------------------------------

CLandingWonSequence::CLandingWonSequence( CGame* game ): CGameSequence( game ) {

    mGame->switchCameraController( new CRotateAroundCameraController( mGame->getCamera(), mGame->getShip() ));
    mGame->switchShipController( new CEmptyShipController( mGame->getShip() ));

}


bool CLandingWonSequence::done() {

    return( true );
}


// ========================================================================

#include "AIShipControllers.h"


CRacingPreStartSequence::CRacingPreStartSequence( CGame* game ): CGameSequence( game ) {

    CLevel* level = mGame->getLevel();

    CShip* ship = mGame->getShip();

    mGame->switchShipController( new CAutoHoverShipController( mGame->getShip() ));

    mMessageWindow = new CGameMessageWindow();
    mMessageWindow->setMessage("Press Thrust to continue" );

    // build list of rings
    std::vector< CRing* > rings;

    for ( CLevel::GameObjectList::const_iterator i = level->mGameObjectList.begin(); i != level->mGameObjectList.end(); ++i ) {
        if( (*i).actor ) {
            if ( typeid( *(( *i ).actor ) ) == typeid( CRing ) ) {
                rings.push_back( static_cast<CRing*>((*i).actor) );
            }
        }
    }


    // init of the camera path controller

    CTrackCameraController* camController = new CTrackCameraController( mGame->getCamera() );
    mGame->switchCameraController( camController );


    camController->addKey( ship->getPosition() + Ogre::Vector3( 10, 10, 0 ), ship->getPosition() );
    camController->addKey( ship->getPosition() + Ogre::Vector3( 0, 10, 10 ), ship->getPosition() );

    camController->addKey( ship->getPosition() + Ogre::Vector3( 0, 5, 0 ), (*rings.begin())->getPosition() );

    for( std::vector<CRing*>::const_iterator i = rings.begin(); i != rings.end(); ++i ) {

        if( (i+1) != rings.end() ) {
            camController->addKey( (*i)->getPosition(), (*(i+1))->getPosition() );
        } else {
            camController->addKey( (*i)->getPosition(), ship->getPosition() );
        }


    }

    // copy of the first two positions
    camController->addKey( ship->getPosition() + Ogre::Vector3( 10, 10, 0 ), ship->getPosition() );
    camController->addKey( ship->getPosition() + Ogre::Vector3( 0, 10, 10 ), ship->getPosition() );

}


// --------------------------------------------------------------------------

CRacingPreStartSequence::~CRacingPreStartSequence() {
    delete( mMessageWindow );
}




bool CRacingPreStartSequence::done() {


    if( CInputManager::getInstance()->isReleased( IMP_THRUST )
        || CInputManager::getInstance()->isReleased( IMR_FIRE ) ) {
        mDone = true;
        mGame->addSequence( "racing sequence", new CRacingStartSequence(mGame ) );
     }

    return( mDone );

}

// --------------------------------------------------------------------------

CRacingStartSequence::CRacingStartSequence( CGame* game  ): CTimedGameSequence( game, 5000 ) {
    mGame->switchShipController( new CAutoHoverShipController( mGame->getShip() ));
    mGame->switchCameraController( new CRotateAroundCameraController( mGame->getCamera(), mGame->getShip() ));

    mCountDown = new CCountDown();

    mDoGo = true;
}



void CRacingStartSequence::init() {



}




CRacingStartSequence::~CRacingStartSequence() {
    delete( mCountDown );
}



bool CRacingStartSequence::done() {

    if( mDoGo && mCountDown->done() ) {
        mGame->addSequence( "gameplay", new CRacingGameplaySequence( mGame ) );
        mDoGo = false;
    }

    return( isOver() );
}

// --------------------------------------------------------------------------

CRacingGameplaySequence::CRacingGameplaySequence( CGame* game ): CGameSequence( game ) {

    mGame->switchShipController( new CPlayerControlledShipController( mGame->getShip() ) );
    mGame->switchCameraController( new CBehindCameraController( mGame->getCamera(), mGame->getShip() ) );

}




bool CRacingGameplaySequence::done() {

    mGame->gameplay();

    switch( mGame->getStatus() ) {

        case GS_GAMEOVER:  /* CGame::GAMEOVER */
            mGame->addSequence( "gameover", new CGameOverSequence( mGame ) );
            mGame->addSequence( "pause", new CPauseSequence( mGame ) );
            mGame->addSequence( "waitforinput", new CWaitForInputSequence( mGame ), "pause" );
            mDone = true;
        break;

        case GS_GAMEWON: /* CGame::GAMEWON */
            mGame->addSequence( "gamewon", new CRacingWonSequence( mGame ) );
            mGame->addSequence( "pause", new CPauseSequence( mGame ) );
            mGame->addSequence( "waitforinput", new CWaitForInputSequence( mGame ), "pause" );
            mDone = true;
        break;

    }


    return( mDone );
}

// --------------------------------------------------------------------------


CRacingWonSequence::CRacingWonSequence( CGame* game ): CGameSequence( game ) {

    mGame->switchShipController( new CAutoHoverShipController( mGame->getShip() ));
    mGame->switchCameraController( new CRotateAroundCameraController( mGame->getCamera(), mGame->getShip() ));

}

bool CRacingWonSequence::done() {
    return( true );
}



// =================================================================================
// =================================================================================



/*
Tutorial sequences :

    Start => camera animation ?  message ? image showing yaw/pitch/roll ?

    Thrust message => Interface box explaining that only thrusting is activated
    ( Thrust sequence => camera animation showing boxes ? )
    Thrust gameplay

    Orientation message => message "no thrust"
    Orientation sequence => gameplay

    Roll message => message "only roll"
    ( Roll sequence ? => showing rings ? )
    Roll gameplay

    Pitch message => "only pitch"
    ( Pitch sequence ? => camera movement showing rings ? )
    Pitch gameplay

    End => message "well done"  ( + camera movement ? )

*/


CTutorialCompleteSequence::CTutorialCompleteSequence( CGame* game ): CTimedGameSequence( game, 5000 ) {

    mGame->switchCameraController( new CRotateAroundCameraController( mGame->getCamera(), mGame->getShip() ));
    mGame->switchShipController( new CEmptyShipController( mGame->getShip() ) );

}

bool CTutorialCompleteSequence::done() {

    return( isOver() );

}

CTutorialGameplaySequence::CTutorialGameplaySequence( CGame* game ): CGameSequence( game ) {

    //mGame->switchCameraController( new CBehindCameraController( mGame->getCamera(), mGame->getShip() ) );
    //mGame->switchShipController( new CPlayerControlledShipController( mGame->getShip() ) );

}


bool CTutorialGameplaySequence::done() {
    mGame->gameplay();


    if( mGame->getStatus() == GS_END ) {
        mDone=true;
        mGame->addSequence( "tutorial_complete", new CTutorialCompleteSequence( mGame ) );
    }

    return( mDone );
}



