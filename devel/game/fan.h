

#ifndef _CFAN_
#define _CFAN_

#include "Ogre.h"
//#include "staticphyscube.h"
#include "physicobject.h"
#include "sound.h"
#include "triggerbox.h"

class CFan: public CStaticPhysCube {

    public:
    CFan(Ogre::String aName, Ogre::Vector3 Pos, float aYaw);
    ~CFan();
    //Ogre::AxisAlignedBox mAABB;

    private:
    CSound3D* mSound;
    CWindZone* mTrigger;

};


#endif

