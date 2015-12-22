
/// handle the landing game logic

#include "gameplaylanding.h"

#include "gameconstants.h"
#include "logger.h"
#include "hoppersounds.h"
#include "GameSequences.h"



CGameplayLanding::CGameplayLanding(int aLevelNum):CGame(aLevelNum) {

    LOG("\n=== Gameplay Landing init ===");

    LOG("\nCGameplayLanding::CGameplayLanding : Creating game windows...");
    // mShip->showBoundingBox(true);
    mDirectionArrowWindow=new CDirectionArrowWindow();
    mShipStatWindow=new CShipStatWindow();
    mShipOrientationWindow=new CShipOrientationWindow();
    LOG("OK.");

    LOG("\nCGameplayLanding::CGameplayLanding : creating the ship controller...");
    // mShipController=new CPlayerControlledShipController( mShip );
    LOG("OK.");

    mNumLandingPads=mLevel->getNumberOf(O_LANDINGPAD);

    LOG("\nCGameplayLanding::CGameplayLanding() : done.");

    mSequenceManager->addSequence( "Landing gameplay", new CLandingGameplaySequence( this ) );

}

CGameplayLanding::~CGameplayLanding() {

    // delete( mShipController );

    delete(mDirectionArrowWindow);
    delete(mShipOrientationWindow);
    delete(mShipStatWindow);

    //delete(mSoundOutOfFuel);

}

void CGameplayLanding::gameplay() {


//    coreShipHandling();
    // mShipController->run();

    coreGameplay();


    landingTest();

    // game over condition

    if( mShip->getStatus() == CShip::STATUS_DEAD ) {
        mGameState=GS_GAMEOVER;
    }


    computeLandingPads();
    mDirectionArrowWindow->lookAt( mNextLandingPadPosition );

}


void CGameplayLanding::landingTest() {

    // =============== landing test

    // --- if the ship is idle, see if it has landed on a landingpad.

    if(mShip->isIdle() && mShip->isUp() ) {


    Ogre::Vector3 testpos( mShip->getTestPointPosition() );

        CLandingPad* lp;
        CLevel::GameObjectList::iterator i;

        for(i=mLevel->mGameObjectList.begin(); i!=mLevel->mGameObjectList.end(); ++i ) {

            if( (*i).actor ) {

               if( typeid( *((*i).actor) ) == typeid( CLandingPad ) ) {

                    lp=static_cast<CLandingPad*>((*i).actor);
                    if( ! lp->hasShipLanded() ) {

                        if(lp->mAABB.intersects( testpos ) ) {

                            lp->setShipLanded();

                            mGameState=GS_REFUELING;
                            // mSoundRefuel=new CSound3D( SOUND_REFUEL, true );
                            // mSoundRefuel->setPosition( lp->getPosition() );
                            // mSoundRefuel->play();

                        }
                    }
                }
            }

        }

   }



}


void CGameplayLanding::computeLandingPads() {

    float dist=0, mindist;
    Ogre::Vector3 pos;

    Ogre::String s="";
    mindist=100000.f;

    int numok=0;

    CLevel::GameObjectList::const_iterator i;
    for( i=mLevel->mGameObjectList.begin(); i!=mLevel->mGameObjectList.end(); ++i ) {
        if( (*i).type==O_LANDINGPAD ) {
            if( static_cast<CLandingPad*>((*i).actor)->hasShipLanded() ) {
                numok++;
            }
            else {
                // find the nearest unlanded landing pad
                pos=(*i).actor->getPosition() - mShip->getPosition();
                dist= (float)pos.length();
                if( dist < mindist ) {
                    mindist=dist;
                    mNextLandingPadPosition= (*i).actor->getPosition();
                }
            }
        }
    }

    // game won condition
    if(numok == mNumLandingPads ) {
        mGameState=GS_GAMEWON;
        // delete( mShipController );
        // mShipController = new CEmptyShipController( mShip );
    }

    // s= Ogre::StringConverter::toString(numok);


}

