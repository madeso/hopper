


#ifndef _EXPLOSIONMANAGER_
#define _EXPLOSIONMANAGER_

#include "singleton.h"
#include "explosion.h"
#include <list>


class CExplosionManager: public SSingleton<CExplosionManager> {

public:

    enum Type { BASIC, SMOKE, BLAST };

    CExplosionManager();
    ~CExplosionManager();

    void update();
    void createExplosion( Ogre::SceneNode* node, CExplosionManager::Type type = BASIC );

private:
    std::list<IExplosion*> mExplosions;


};



#endif


