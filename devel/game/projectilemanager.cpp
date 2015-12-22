

#include "projectilemanager.h"


CProjectileManager::CProjectileManager():
mProjectileCount(0) {

}

CProjectileManager::~CProjectileManager() {

    std::list<CProjectile*>::iterator it = mProjectiles.begin();

    for( it = mProjectiles.begin(); it != mProjectiles.end() ; ++it ) {
        delete( (*it) );
    }

}


void CProjectileManager::update() {

    std::list<CProjectile*>::iterator it;

    for( it = mProjectiles.begin(); it != mProjectiles.end(); ++it ) {
        (*it)->update();
        if( (*it)->isDead() ) {
            delete( (*it) );
            it=mProjectiles.erase( it );
        }
    }

}


void CProjectileManager::createProjectile( Ogre::Vector3 spawnPosition,
    Ogre::Vector3 direction, Ogre::Vector3 velocity  ) {

    mProjectiles.push_back( new CPulse(
        "projectile" + Ogre::StringConverter::toString( mProjectileCount ++ ),
        spawnPosition,
        direction,
        velocity  ));

}
