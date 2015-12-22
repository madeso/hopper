

#ifndef _TURRET_
#define _TURRET_

#include "physicobject.h"
#include "delay.h"

class CTurret: public CStaticPhysTrimesh {

public:
    CTurret( Ogre::String name, Ogre::Vector3 pos );
    ~CTurret();

    void update( CActor* actor );

private:
    //CStaticPhysTrimesh* mTurretStand;
    //CStaticPhysTrimesh* mTurretCannon;

    // Ogre::Vector2 mHeading; /// current heading vector
    Ogre::Radian mYaw; /// current yaw angle

    CDelay* mFireDelay;
    CDelay* mFirePause;
    int mFireCount;


    void die();

};


#endif


