
#include "physicsmanager.h"
#include "screen.h"
#include "scenemanager.h"
#include "gameconstants.h"
#include "logger.h"


CPhysicsManager::CPhysicsManager() {

    mTimeManager=CTimeManager::getInstance();


   //-----  Ode stuffs ------------

    mPhysicsWorld = new OgreOde::World(CSceneManager::getInstance()->getOgreSceneManager() );

    // Try with OgreOde::OgreSceneManagerSpace
    mPhysicsSpace = new OgreOde::SimpleSpace( mPhysicsWorld, 0 );
    mPhysicsWorld->setDefaultSpace( mPhysicsSpace );


    mPhysicsWorld->setGravity(Ogre::Vector3(0,GRAVITY,0));

	mPhysicsWorld->setCFM(10e-5);
	mPhysicsWorld->setERP(1.0f);
	mPhysicsWorld->setAutoSleep(true);
	mPhysicsWorld->setContactCorrectionVelocity(1.0);


// one
	// mPhysStepper = new OgreOde::ForwardFixedInterpolatedStepHandler(mPhysicsWorld );
	mPhysStepper = new OgreOde::ForwardFixedInterpolatedStepHandler( mPhysicsWorld,
                    OgreOde::StepHandler::QuickStep,
                    Ogre::Real( 1.f/60.f ), // step_size
                    Ogre::Real( 1.f/60.f ), // frame_rate
                    Ogre::Real( 10.f/60.f ), // max_interval
                    Ogre::Real( 1.0 ) // time_scale
                    );

	// mPhysStepper = new OgreOde::ForwardFixedInterpolatedQuickStepper(0.01666); // PHYSICS_STEP); // 1.f/75.f);

	//  mPhysStepper = new OgreOde::QuickStepper(PHYSICS_STEP); // 1.f/75.f);
    //	mPhysStepper = new OgreOde::ForwardFixedQuickStepper(PHYSICS_STEP); // 1.f/75.f);
    //	mPhysStepper->setAutomatic(OgreOde::Stepper::AutoMode_PostFrame,CScreen::getInstance()->getRoot());

	// mPhysStepper->setAutomatic(OgreOde::Stepper::AutoMode_NotAutomatic, CScreen::getInstance()->getRoot());

//    mPhysStepper->setStepListener(this);
//   OgreOde::World::getSingleton().setShowDebugObjects(true);


    // OgreOde::World::getSingleton().setCollisionListener(mCollisionManager);

//    OgreOde::World::getSingleton().getDefaultSpace()->setAutoCleanup(true);

//    CLogger::getInstance()->log("\n*** Ode default space autocleanup : %d",
//            OgreOde::World::getSingleton().getDefaultSpace()->getAutoCleanup() );

//   for memory : OgreOde::TriangleMeshGeometry :: setTriangleMeshCollisionListener(this);

/*==============================================================
   Real const time_step = 0.01;
   Real const frame_rate = 1.0 / 60.0;
   Real const max_frame_time = 1.0 / 4.0;
   Real const time_scale = 1.0;
   mStepper = new OgreOde::ForwardFixedStepHandler(
                                       mWorld,
                                       StepHandler::QuickStep,
                                       time_step,
                                       max_frame_time,
                                       time_scale );

   mStepper->setAutomatic( StepHandler::AutoMode_NotAutomatic, mRoot );
   // Register it with the stepper, so we can (for example) add forces before each step
   mStepper->setStepListener( mLightningWheels );

================================================================*/

    mPaused=false;
}


CPhysicsManager::~CPhysicsManager() {

    LOG("\n\tkilling the collision manager...");
//    CCollisionManager::kill();
    LOG("OK.");

    LOG("\n\tdeleting the phys stepper...");
    delete(mPhysStepper);
    LOG("OK.");

    LOG("\n\tdeleting the physics world...");
    // OgreOde::Wold deletes the default space
    delete( mPhysicsWorld );
    LOG("OK.");

}


void CPhysicsManager::setCollisionManager( CCollisionManager* aCollisionManager ) {

    mPhysicsWorld->setCollisionListener( aCollisionManager );
}


void CPhysicsManager::update( float aTimefactor) { // aFrametime) {

    if( ! mPaused ) {

        //float vStep=mTimeManager->getElapsed()/1000.f;
        // float vStep=aFrametime/1000.f;

        // if( vStep <0.01666 ) vStep=0.01666; // 60 FPS
        // if( vStep > 0.5 ) vStep=0.5; // 2 fps

 //   CLogger::getInstance()->log("\nstep=%f",vStep );

        if (mPhysStepper->step( 0.01666 * aTimefactor )) {
            // OgreOde::World::getSingleton().synchronise();
            mPhysicsWorld->synchronise();
        }

    }

}


void CPhysicsManager::pause( bool paused ) {
    mPaused=paused;
    mPhysStepper->pause( paused );
}


OgreOde::World* CPhysicsManager::getPhysicsWorld() {
    return mPhysicsWorld;
}

OgreOde::Space* CPhysicsManager::getPhysicsSpace() {
    return ( mPhysicsWorld->getDefaultSpace() );
}


void CPhysicsManager::setGravity( float gravity ) {

    mPhysicsWorld->setGravity(Ogre::Vector3(0,gravity,0));
}

