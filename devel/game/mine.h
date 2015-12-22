
#ifndef _MINE_
#define _MINE_

#include "physicobject.h"
#include "sound.h"

class CMine: public CDynamicPhysSphere {

public:

    CMine( Ogre::String name, Ogre::Vector3 pos );
    virtual ~CMine();

    void die();

private:
    CSound3D* mSoundLoop;

};





#endif


