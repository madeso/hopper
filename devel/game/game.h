
#ifndef _CGAME_H_
#define _CGAME_H_

#include "gamepart.h"


#include "physicsmanager.h"


#include "level.h"


#include "inputmanager.h"
#include "scenemanager.h"
#include "timemanager.h"
#include "particleemittermanager.h"
#include "explosionmanager.h"


#include "camera.h"
// #include "TerrainCamera.h"
#include "CameraControllers.h"

#include "PlayerControlledShipControllers.h"

#include "directionarrowwindow.h"
#include "shiporientationwindow.h"
#include "shipstatwindow.h"
#include "fpswindow.h"

#include "sound.h"
#include "soundmanager.h"


#include "delay.h"

#include "debugwindow.h"

#include "gamepausedwindow.h"
#include "GameSequenceManager.h"


// --------- old game states ----------
#define GS_GAMEPLAY   1
#define GS_REFUELING   2
#define GS_GAMEOVER   3
#define GS_GAMEWON 4
#define GS_PAUSED 5
#define GS_QUIT 6
#define GS_PRESTART 7
#define GS_STARTING 8
#define GS_END 9
#define GS_LEVEL_CHANGE 10
#define GS_PREEND 11
#define GS_ENDING 12
// ------------------------------------


#define CAM_BEHIND 1
#define CAM_COCKPIT 2



/// Manages the gameplay states & the core/common gameplay.

class CGame: public IGamePart {

    friend class CGameplayLanding;
    friend class CGameplayRacing;
    friend class CGameplayTutorial;
    friend class CMenu;
    friend class CGameplayTest;
    friend class CGameSequence;

public:
    CGame(int aLevelNum);
    ~CGame();
    virtual  int run();   // game part implementation

    CCamera* getCamera();
    CShip* getShip();

    CLevel* getLevel();

    void switchCameraController( CCameraController* cameraController );
    void switchShipController( CShipController* shipController );

    virtual void gameplay()=0;

    int getStatus() const;
    void setStatus( int aStatus );
    void CGame::addSequence( Ogre::String name, CGameSequence* sequence, Ogre::String dependency = Ogre::StringUtil::BLANK );



protected:

    void coreGameplay();
/**
  *  Game states call order :
  *  PRESTART -> initialisation of the starting sequence -> prestart()
  *  STARTING -> starting sequence -> starting()
  *  GAMEPLAY -> the gameplay -> gameplay()
  *  PREGAMOVER -> initialisation of the gameover sequence -> pregameover();
  *  GAMEOVER or GAMEWON -> gameover or gamewon sequence -> gameover()
  *  PREEND -> initialisation of the ending sequence -> preend();
  *  ENDING -> ending sequence ( default : waiting for user input ) -> ending();
  *
  *  PAUSED -> any time.
  */
    enum Status { PAUSED, PRESTART, STARTING, GAMEPLAY, GAMEOVER, GAMEWON, PREEND, ENDING };

private:

    // ------------- System stuffs
    CInputManager *mInputManager;

    CSceneManager *mSceneManager;

    CPhysicsManager* mPhysicsManager;

    CSoundManager* mSoundManager;


    CTimeManager* mTimeManager;
    CDelay* mHeartBeat;
    int mFrametime;


    void physicsUpdate();


    CGameSequenceManager* mSequenceManager;



    void CGame::manageInputs();


    // ------------ UI

    CShipOrientationWindow* mShipOrientationWindow;
    CDirectionArrowWindow* mDirectionArrowWindow;
    CShipStatWindow* mShipStatWindow;

    CGamePausedWindow* mGamePausedWindow;

    CFPSWindow* mFPSWin;

    void updateUI();


    //  ----------- Camera management
    CCamera* mCam;
    //CTerrainCamera* mCam;
    int mCameraMode;
    CCameraController* mCamController;
    Ogre::Vector3 mCamPos;

    bool mFreeCam;


    // ------------ Gameplay Stuffs
    int mGameState;

    // bool mShipExploded;
    // CDelay* mExplodeDelay; // in CShip ?!

    // CDelay* mGameOverDelay;
    // void waitForInput();


    /// overridables game states :

//    virtual void prestart();
//    virtual void starting();

//    virtual void gameover();
//    virtual void gameWon();



    // refueling : moved to the Refueling Sequence, how neat
    // void refueling();
    // CSound3D *mSoundRefuel;

    // above clouds = frozen ship : should be moved to CShip ?
    bool mTooHigh;


    CParticleEmitterManager* mParticleEmitterManager;
    CParticleEmitter* mAirDustParticles;

    CExplosionManager* mExplosionManager;



    // ----------  Game objects
    CLevel* mLevel;

    CShip* mShip;
    CShipController* mShipController;


    // ---------- debug
//    CDebugWindow mDebugWindow;



};

#endif
