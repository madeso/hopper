

// ring.h

#ifndef _RING_H_
#define _RING_H_

//#include "staticphystrimesh.h"
#include "physicobject.h"


/// a ring object.
class CRing:public CStaticPhysTrimesh {

    public:
    CRing( Ogre::String aName, Ogre::Vector3 Pos, float aYaw );

    void activate();
    bool isActive();

    ~CRing();

    private:
    Ogre::AxisAlignedBox mAABB;
    bool mActivated;

};

#endif
