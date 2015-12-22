

#include "ring.h"

#include "logger.h"

CRing::CRing( Ogre::String aName, Ogre::Vector3 Pos, float aYaw ):CStaticPhysTrimesh( Pos, aYaw, 7, aName, "ring.mesh" ) {

    castShadows();
// LOG("\n\n\n gasp \n\n\n" );
    // scale( 7,7,7 );
    //scale( 10,10,10 );
    // pitch( Ogre::Radian(3.141592/2.0), Ogre::Node::TS_LOCAL );

    // setYaw( aYaw);
    // yaw(aYaw);
    //yaw( mYaw, Ogre::Node::TS_LOCAL );

    // setInitialPosition( Pos );  //  warning : immovable objects has to be positionned before physics init !! ( no phys body ! )

    // initPhysicTrimesh();



//    _update(false,false);  // force update to fill the AABB
    mAABB=getAABB() ;

    mActivated=false;

    mEntity->setMaterialName("ring/blue");



}


void CRing::activate() {
    if( !mActivated ) {
        mActivated=true;

        mEntity->getSubEntity(1)->setMaterialName( "ring/green" );
    }
}


bool CRing::isActive() {
        return mActivated;
}

CRing::~CRing() {

}
