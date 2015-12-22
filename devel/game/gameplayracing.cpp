
/// handle the racing mode game logic

#include "gameplayracing.h"

#include "gameconstants.h"
#include "hoppersounds.h"
#include "logger.h"
//#include "AutoHoverShipController.h"
#include "AIShipControllers.h"




CGameplayRacing::CGameplayRacing( int aLevelNum ): CGame( aLevelNum ) {

    mNextRing = 0;

    mDirectionArrowWindow = new CDirectionArrowWindow();
    mShipStatWindow = new CShipStatWindow();
    mShipOrientationWindow = new CShipOrientationWindow();
    mGameMessageWindow = new CGameMessageWindow();

    mWrongRingDelay = new CDelay( 2000 );


    mNumRings=mLevel->getNumberOf(O_RING);


    // cache rings
    // Note: Rings are chosen in the order they were created in the editor...
    CLevel::GameObjectList::iterator i;
    for ( i = mLevel->mGameObjectList.begin(); i != mLevel->mGameObjectList.end(); ++i ) {
        if( (*i).actor ) {
            if ( typeid( *(( *i ).actor ) ) == typeid( CRing ) ) {
                mRings.push_back( static_cast<CRing*>((*i).actor) );
            }
        }
    }

    computeRings();

    mSequenceManager->addSequence( "prestart", new CRacingPreStartSequence( this ) );
    // mSequenceManager->addSequence( "racing sequence", new CRacingStartSequence(this), "prestart" );  // will be destroyed by the manager


}

CGameplayRacing::~CGameplayRacing() {

    // delete( mShipController );

    delete( mDirectionArrowWindow );
    delete( mShipStatWindow );
    delete( mShipOrientationWindow );

    delete( mGameMessageWindow );

    delete( mWrongRingDelay );

}


void CGameplayRacing::prestart() {
    delete( mShipController );
    mShipController = new CAutoHoverShipController( mShip );

    mCountDown = new CCountDown();

    mGameState = GS_STARTING;

}



void CGameplayRacing::gameplay() {

    coreGameplay();

    ringTest();

}



void CGameplayRacing::ringTest() {

    // rings test

    CRing* ring;

    if ( mNextRing != 0 ) {

        std::vector<CRing*>::const_iterator i;
        for( i = mRings.begin(); i != mRings.end(); ++i ) {

            ring = (*i);

            if( !ring->isActive() ) {
                float d = ( mShip->getPosition() - ring->getPosition() ).length();

                if ( d < 7 ) {

                    if ( ring == mNextRing ) {
                        ring->activate();
                        CSoundManager::getInstance()->play3D( SOUND_LANDED, ring->getPosition(), 0.5 );
                        computeRings();

                    } else {

                        if ( mWrongRingDelay->isOver() ) {
                                mWrongRingDelay->restart();
                                CSoundManager::getInstance()->play3D( SOUND_WRONGRING, ring->getPosition(), 0.5 );
                                mGameMessageWindow->setMessage( "Wrong Ring !" );
                        }

                    }

                }

            }

        }

        mGameMessageWindow->update();
        mDirectionArrowWindow->lookAt( mNextRing->getPosition() );

    }

    // game over condition

    if( mShip->getStatus() == CShip::STATUS_DEAD )
        mGameState = GS_GAMEOVER;

}




/// Compute the next ring postion
void CGameplayRacing::computeRings() {

//    Ogre::String s="";
    int numok = 0;


    // Note: Rings are chosen in the order they were created in the editor...
    std::vector<CRing*>::const_iterator i;
    for( i = mRings.begin(); i != mRings.end(); ++i ) {
        if( (*i)->isActive()) {
            numok++;
        }
        else {
            mNextRing = (*i);
            break;
        }
    }


    if ( numok == mNumRings ) {
        mGameState = GS_GAMEWON;
    }


}

