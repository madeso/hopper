

#ifndef _CPROJECTILEMANAGER_
#define _CPROJECTILEMANAGER_

#include "singleton.h"
#include "projectiles.h"


class CProjectileManager: public SSingleton<CProjectileManager> {

    public:

    CProjectileManager();
    ~CProjectileManager();

    void createProjectile( Ogre::Vector3 spawnPosition, Ogre::Vector3 direction, Ogre::Vector3 velocity );

    void update();

    private:
    int mProjectileCount;
    std::list<CProjectile*> mProjectiles;

};



#endif


