

#include "mine.h"
#include "logger.h"
#include "hoppersounds.h"

#include "explosionmanager.h"

CMine::CMine( Ogre::String name, Ogre::Vector3 pos ): CDynamicPhysSphere( pos, 1, 1, name, "mine.mesh" ) {

    mPhysBody->setAffectedByGravity( false );
    mPhysBody->setAngularVelocity( Ogre::Vector3( 0.1,0.3,0.2 ));

    mSoundLoop = new CSound3D( SOUND_PROXMINE, true );
    mSoundLoop->setPosition( getPosition() );
    mSoundLoop->play();

    getNode()->scale( 0.75, 0.75, 0.75 ); // hack : resizing the mesh does not resize the collision sphere

}


CMine::~CMine() {

    LOG("\nCMine::~CMine()");
    mPhysGeom->setUserObject( 0 );
    delete(mSoundLoop);
}

void CMine::die() {

    LOG("\nCMine::die() !" );
    CExplosionManager::getInstance()->createExplosion( getNode(), CExplosionManager::BLAST );

}
