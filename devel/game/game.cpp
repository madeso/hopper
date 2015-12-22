

#include "game.h"
#include "math.h"
#include "logger.h"
#include "screen.h"
#include "gameconstants.h"
#include "hopperinputactions.h"
#include "hoppersounds.h"
#include "hoppercollisionmanager.h"
#include <vector>
// #include "materialtypes.h"
#include "objecttypes.h"
#include "resourcemanager.h"
#include "hopperparticles.h"
#include "projectilemanager.h"
#include "triggerboxmanager.h"



CGame::CGame(int aLevelNum) {

   LOG("\n\n===========================================");
   LOG("\nNew Game with level #%d",aLevelNum);
   LOG("\n===========================================\n");

    mTimeManager=CTimeManager::getInstance();

    mInputManager=CInputManager::getInstance();

    mSceneManager=CSceneManager::getInstance();

    mHeartBeat=new CDelay( 1000/60 );

//==================== Camera ============================

    mCam=CCamera::getInstance();
    //mCam=CTerrainCamera::getInstance();

    mCameraMode=CAM_BEHIND;
    mFreeCam=false;
    //mCameraMode=CAM_COCKPIT;

// ==================== Sound ===========

    // mSoundRefuel=0; // looping sound

// ================= UI =========================

    // these are instanciated by each game part
    mShipStatWindow=0;
    mShipOrientationWindow=0;
    mDirectionArrowWindow=0;


// ================== Physics ===========================
    LOG("\nSetting Physics parameters...");

    mPhysicsManager=CPhysicsManager::getInstance();
    //CCollisionManager::getInstance()->setCollisionHandler(this);
    CHopperCollisionManager::getInstance(); // it registers itself to the physics manager;

    LOG(" OK ");


//============ Particles ==============
    LOG("\nCreating the particleEmitterManager...");
    mParticleEmitterManager=CParticleEmitterManager::getInstance();
    CResourceManager::getInstance()->initParticleSystems();

    mAirDustParticles=mParticleEmitterManager->createEmitter( PART_AIRDUST );
    LOG(" done.");


// ========== Explosions ===============

    mExplosionManager = CExplosionManager::getInstance();


// ========== TriggerBoxes ==============

    CTriggerBoxManager::getInstance();

// ==================== game objects =====================


    mLevel=new CLevel( aLevelNum );
    mLevel->initTerrainPhysics();

    mShip=mLevel->getShip();

    LOG("\nCGame game objects got.");


    mFPSWin=new CFPSWindow();



// ================ Projectiles ================================

    CProjectileManager::getInstance();

// =========== gameplay init ====================================

    mSequenceManager = new CGameSequenceManager();
    mGameState=GS_GAMEPLAY;
    // mGameState=GS_PRESTART;

    // mShipExploded=false;
    // computeLandingPads();
    mTooHigh=false;

    mCamController=new CBehindCameraController( mCam, static_cast<CActor* >(mShip) );

    mShipController = 0; // new CEmptyShipController( mShip );

    LOG("\nCGame init done.\n");

//    mGameOverDelay=0;

}


CGame::~CGame() {

    LOG("\n ----------------------------------------------");
    LOG("\nCGame::~CGame()");

    //if( mGameOverDelay ) delete( mGameOverDelay );
    delete( mHeartBeat);

    if( mShipController ) delete( mShipController );
    delete( mCamController );

    LOG("\nkilling the projectile manager...");
    CProjectileManager::kill();
    LOG(" Ok.");

    delete( mSequenceManager);

    delete(mLevel);

    CTriggerBoxManager::kill();


    LOG("\nkilling the collision manager...");
    CHopperCollisionManager::kill();
    LOG("\nOk.");

    LOG("\nkilling the physics manager...");
    CPhysicsManager::kill();
    LOG("\nOk.");

    LOG("\n~CGame : deleting the FPS window...");
    delete(mFPSWin);
    LOG("Ok.");

    //LOG("\n~CGame : deleting the REFUEL sound...");
    //if( mSoundRefuel) delete(mSoundRefuel);
    //LOG("Ok.");

    LOG("\nkilling the explosion manager...");
    CExplosionManager::kill();
    LOG("\nOk.");

    LOG("\nkilling the particle manager...");
    mParticleEmitterManager->destroyEmitter( mAirDustParticles );
    CParticleEmitterManager::kill();
    LOG("\nOk.");

    LOG("\n~CGame : killing the scene manager...");
    CSceneManager::kill();
    LOG("Ok.");
}

// ==========================================================


int CGame::getStatus() const  {
    return( mGameState );
}

void CGame::setStatus( int aStatus ) {
    mGameState = aStatus;
}


int CGame::run() {

    mFrametime=mTimeManager->getFrametime();

    mFPSWin->setFPS( (int)CScreen::getInstance()->getFPS() );

    if(mInputManager->isReleased( IMR_SCREENSHOT))
        CScreen::getInstance()->shot();

    if( mGameState == GS_PAUSED ) {

        // mTimeManager->pause();

        if( mGamePausedWindow->done() ) {
            if( mGamePausedWindow->returnToMenu() )
                mGameState=GS_QUIT;
            else
                mGameState=GS_GAMEPLAY;

            delete(mGamePausedWindow);
            // mTimeManager->unpause();
        }

    }
    else {

        manageInputs();

		mSequenceManager->run();

        mShipController->run();

		physicsUpdate();

		mCamController->run();

        if( mHeartBeat->isOver() ) {

            mHeartBeat->restart();

            updateUI();

            mAirDustParticles->setPosition( mCam->getPosition() + mShip->getLinearVelocity() );
            mAirDustParticles->emit( true );
            mParticleEmitterManager->update();

       }



    } // !GS_PAUSED


    mSoundManager->set3DListener( mCam->getPosition(),
                    mCam->getVelocity(),
                    mCam->getDirection(),
                    Ogre::Vector3( 0,1,0) );                        // up vector



    // return states => communication with the game manager :


    if( mGameState == GS_END ) {
        if( mShip->getStatus() != CShip::STATUS_OK )
            return( GM_GAMEOVER );
        else
            return( GM_GAMEWON );
    }
    else {
        if( mGameState == GS_QUIT )
            return( GM_MENU );
    }

    if( mGameState == GS_LEVEL_CHANGE )
        return( GM_LEVEL_CHANGE);

    return( GM_GAMEPLAY );

}



/// called by CGameplay* classes : common gameplay
void CGame::coreGameplay() {


    CProjectileManager::getInstance()->update();

    mExplosionManager->update();

    CTriggerBoxManager::getInstance()->update();

    // cloud layer

/*
    if( mShip->getPosition().y > mLevel->getCloudLayerHeight() ) {

//        mShip->setCollisionMode(true); // no thrusting

        if(!mTooHigh) {
            // mSoundManager->play(SOUND_FREEZING);
            mShip->freeze();
            //mSoundFreezing->play();
            mTooHigh=true;
        }

    }
    else {
        mTooHigh=false;
        if( mShip->isFrozen() ) {
            mShip->unfreeze();
        }
    }
    */


    // Gameplay objects : Fans, turrets, mines
        Ogre::Vector3 shipPos;
        CFan* fp;
        CLevel::GameObjectList::iterator i;

        shipPos=mShip->getPosition();

        for(i=mLevel->mGameObjectList.begin(); i!=mLevel->mGameObjectList.end(); ++i ) {

            if( (*i).actor ) {


                if( typeid( *((*i).actor) ) == typeid( CTurret ) ) {
                    static_cast<CTurret*>((*i).actor)->update(mShip);
                }

                if( typeid( *((*i).actor) ) == typeid( CMine ) ) {
                    CMine* mine = static_cast<CMine*>((*i).actor);
                    if( mine->isDead() ) {
                        LOG("\nGame::coreGameplay() : deleting mine" );
                        delete( mine );
                        (*i).actor = 0;
                    }
                }

                if( (*i).type == O_ENTRY ) {
                    if( (*i).actor->getPosition().distance( mShip->getPosition() ) < 10 ) {
                        mGameState = GS_LEVEL_CHANGE;
                    }
                }

            }

        }



/*
    mDebugWindow.setText( 0, Ogre::StringConverter::toString(
        mShip->mPhysBody->getPointWorldVelocity( Ogre::Vector3(0,0,0)).z ));
*/


}


void CGame::manageInputs() {

    if(mInputManager->isReleased( IMR_DEBUG_ODE) ) {
        mPhysicsManager->getPhysicsWorld()->setShowDebugGeometries(!mPhysicsManager->getPhysicsWorld()->getShowDebugGeometries());
    }

    if(mInputManager->isReleased( IMR_DEBUG_BB )) {
        CSceneManager::getInstance()->showBoundingBoxes( !CSceneManager::getInstance()->getShowBoundingBoxes() );
    }

    if(mInputManager->isReleased( IMR_FREECAM )) {
        mFreeCam = !mFreeCam;
        delete(mCamController);
        delete(mShipController);

        if( mFreeCam ) {
            mCamController = new CEditorCameraController( mCam );
            mShipController=new CEmptyShipController( mShip );
            mShip->setAffectedByGravity(false);
        }
        else {
            mCamController = new CBehindCameraController( mCam, static_cast<CActor* >(mShip) );
            mShipController = new CPlayerControlledShipController( mShip );
            mShip->setAffectedByGravity(true);
        }

    }


    if(mInputManager->isReleased( IMR_ESCAPE ) || mInputManager->isReleased( IMR_PAUSE) ) {
        mGameState=GS_PAUSED;
        mGamePausedWindow=new CGamePausedWindow();
    }


    if( mInputManager->isKeyPressed( OIS::KC_K ) && mInputManager->isKeyPressed( OIS::KC_LCONTROL ) ) {
        mShip->setLife(0);
    }

}


void CGame::addSequence( Ogre::String name, CGameSequence* sequence, Ogre::String dependency ) {
    mSequenceManager->addSequence( name, sequence, dependency );
}


void CGame::updateUI() {

    if( mShipOrientationWindow ) {
        mShipOrientationWindow->setOrientation( mShip->getPitchRollOrientation() );
    }

    if( mShipStatWindow ) {
        mShipStatWindow->setHull( (int)mShip->getHullCondition() );
        mShipStatWindow->setFuel( (int)mShip->getFuel() );
    }
}



CCamera* CGame::getCamera() {
    return( mCam );
}

CShip* CGame::getShip() {
    return( mShip );
}

CLevel* CGame::getLevel() {
    return( mLevel );
}

void CGame::switchCameraController( CCameraController* cameraController ) {

    if( mCamController ) delete( mCamController );
    mCamController = cameraController;

}


void CGame::switchShipController( CShipController* shipController ) {
    if( mShipController ) delete( mShipController );
    mShipController = shipController;
}




void CGame::physicsUpdate() {

    float tf=mTimeManager->getTimeFactor();

    mShip->addForces( tf );

    mPhysicsManager->update( tf );


}

