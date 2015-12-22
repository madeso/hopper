

#include "thruster.h"
#include "scenemanager.h"
#include "hoppersounds.h"
#include "particleemittermanager.h"
#include "hopperparticles.h"
#include "scenemanager.h"
#include "logger.h"

/** @brief CThruster
  *
  */
CThruster::CThruster(
    Ogre::SceneNode* parentnode,
    const Ogre::String& name,
    const Ogre::Vector3& relpos ):
//    const Ogre::Vector3& dir,
//    float size ):
        mNode(0),
        mEntity(0),
        mParentNode( parentnode),
        mRelPos(relpos),
//        mDir(dir),
//        mSize(size),
        mPower(1),
        mActive(false),
        mThrustLight(0),
        mSoundThrust(0),
        mSoundThrustloop(0),
        mSmokeTestPoint(0),
        mTerrainSmokeEmitter(0)  {


    mNode=mParentNode->createChildSceneNode( mRelPos );
    mEntity=CSceneManager::getInstance()->createEntity( name, "mainthrust3.mesh" );  // mainthrust2 for the 3D shape thruster
    mNode->attachObject( mEntity );
    // mNode->setDirection(  mDir );
    // mNode->scale( mSize );

    mEntity->setQueryFlags( 0x0 );  // no collision detection

    mSmokeTestPoint=mParentNode->createChildSceneNode( Ogre::Vector3(0,-3,0) );
    //Ogre::Entity* e = CSceneManager::getInstance()->createEntity( "testpoint", "minithrustdown.mesh" );
    //mSmokeTestPoint->attachObject( e );

    LOG("\nCThruster::CTruster() : creating terrain smoke emitter...");
    mTerrainSmokeEmitter=CParticleEmitterManager::getInstance()->createEmitter( PART_THRUSTER_SMOKE );
    LOG("ok");


// spotlight
    mThrustLight=new CSpotLight( name+"_light", mNode );
    mThrustLight->setRange( 45, 120, 1.0 );

// pointlight
//    mThrustLight=new CPointLight( name+"_light", mNode );

// common properties
    mThrustLight->setColor( (float)0.2, (float)0.8, (float)1.0 );
    mThrustLight->setVisible(false);
    mThrustLight->setCastShadows(false);
    mThrustLight->setAttenuation( 5.0, 0.9, 0.1, 0.01 );


    mSoundThrust=new CSound3D( SOUND_THRUST, false );
    mSoundThrustloop=new CSound3D( SOUND_THRUSTLOOP, true );

    setActive( mActive );

}


/** @brief ~CThruster
  *
  */
CThruster::~CThruster() {

LOG("\n\t~CThruster() " );
    delete(mSoundThrustloop);
    delete(mSoundThrust);
    delete(mThrustLight);

    mParentNode->removeChild( mSmokeTestPoint );
    CParticleEmitterManager::getInstance()->destroyEmitter(mTerrainSmokeEmitter);

    CSceneManager::getInstance()->destroyEntity(mEntity);
    mParentNode->removeChild( mNode );
LOG(" done." );
}



/** @brief setPower
  *
  */
void CThruster::setPower(float power) {
    mPower=power;

    if( mPower > 1 ) mPower=1;


    if( mPower > 0 ) {


        if( !mActive ) {
            mActive = true;
            mNode->setVisible( mActive );
            mThrustLight->setVisible( mActive );
            mSoundThrust->setPosition( mNode->_getDerivedPosition() );
            mSoundThrust->play();
            mSoundThrust->setVolume( 1 );
            mSoundThrustloop->play();
        }



        // ======== thruster scale
        mNode->setScale( 1, mPower, 1 );


        // =========  thrust loop sound :


        mSoundThrustloop->setPosition( mNode->_getDerivedPosition());

        mSoundThrustloop->setVolume( mPower/2.0 );


        // ============== smoke

        mSmokeTestPoint->setPosition( 0, mPower*-5.f, 0 );

        Ogre::Vector3 testPoint( mSmokeTestPoint->_getDerivedPosition() );

        float terrain_height=CSceneManager::getInstance()->getHeightAt( testPoint.x, testPoint.z );
        // LOG("\nTerrain_height = %f , testPoint.y = %f ", terrain_height, testPoint.y );
        if(  terrain_height > testPoint.y ) {
            mTerrainSmokeEmitter->setPosition( Ogre::Vector3( testPoint.x, terrain_height, testPoint.z) );
            mTerrainSmokeEmitter->emit( true );
            mTerrainSmokeEmitter->update();

        }


    }
    else {
        if( mActive ) {
            // setActive( false );
            mActive = false;
            mSoundThrustloop->stop();
            mNode->setVisible( mActive );
            mThrustLight->setVisible( mActive );
        }
    }

}



/** @brief getPower
  *
  */
float CThruster::getPower() {
    return( mPower );
}


/** @brief setActive
  *
  */
void CThruster::setActive(bool active) {

    mActive=active;
    mNode->setVisible( mActive );
    mThrustLight->setVisible( mActive );

    if( mActive ) {
//        mSoundThrust->setPosition( mNode->getWorldPosition() );
// from Ogre 1.4.7 to Ogre 1.6.1 : no more getWorldPosition() !
        mSoundThrust->setPosition( mNode->_getDerivedPosition() );
        mSoundThrust->play();
        mSoundThrust->setVolume( 1 );
        mSoundThrustloop->play();
    }
    else {
        mSoundThrustloop->stop();
    }

}



/** @brief isActive
  *
  */
bool CThruster::isActive() {
    return( mActive );
}



