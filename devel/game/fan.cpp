

#include "fan.h"

#include "logger.h"
#include "hoppersounds.h"
#include "triggerbox.h"

CFan::CFan(Ogre::String aName, Ogre::Vector3 Pos, float aYaw):CStaticPhysCube(Pos, aYaw, 2, aName,"vent.mesh") {

    castShadows();

    Ogre::AxisAlignedBox aab=getAABB() ;

    Ogre::Vector3 pmin,pmax;
    pmin=aab.getMinimum();
    pmax=aab.getMaximum();

    pmin.y=pmax.y;
    pmax.y+=5;

    mTrigger = new CWindZone(
        Pos + Ogre::Vector3( 0, (pmax.y - pmin.y) / 2 , 0 ),
        Ogre::Vector3( pmax.x - pmin.x, pmax.y-pmin.y, pmax.z-pmin.z ),
        Ogre::Vector3( 0,200,0 ) );

    mSound=new CSound3D( SOUND_FAN, true );
    mSound->setPosition(Pos);
    mSound->play();


}

CFan::~CFan() {
    delete(mSound);
    delete( mTrigger );
}


