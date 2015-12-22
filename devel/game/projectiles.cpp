
#include "projectiles.h"
#include "hoppercollisionmaterials.h"
#include "soundmanager.h"
#include "hoppersounds.h"

CProjectile::CProjectile( Ogre::Vector3 pos,
                          float scale,
                          float mass,
                          Ogre::String name,
                          Ogre::String model )
        : CDynamicPhysSphere( pos, scale, mass, name, model ),
        mDead( false),
        mTTL( 0 ),
        mLight(0) {

    setLinearDamping( 0 );
    setAffectedByGravity( false );

}


CProjectile::~CProjectile() {
    if( mTTL ) delete( mTTL );
    if( mLight ) delete( mLight );
}


void CProjectile::onCollision( float impactForce ) {
    mDead=true;
}


bool CProjectile::isDead() {
    return( mDead );
}




CPulse::CPulse( Ogre::String name, Ogre::Vector3 pos, Ogre::Vector3 dir, Ogre::Vector3 velocity )
        : CProjectile( pos, 0.5 /*scale*/, 3.0/*mass*/, name, "pulse2.mesh" ) {
/*
    float vel=velocity.z;
    if( vel < 0 ) vel=-vel;
    if( vel < 20 ) vel=20;
*/
    setLinearVelocity( -30.f * dir );
    setAngularVelocity( Ogre::Vector3( 2,2,2 ));
    mTTL=new CDelay( 4000 );

    mLight=new CPointLight( name + "_light", mNode );
    mLight->setColor( 0.5,1,0.5 );
    mLight->setAttenuation( 20, 0.75, 1, 0 );

    CSoundManager::getInstance()->play3D( SOUND_PULSE, pos, 0.5 );

    mPhysGeom->setUserData( MAT_PROJECTILE );


}


void CPulse::update() {

    if( mTTL->isOver() ) mDead=true;

}




CMissile::CMissile( Ogre::String name, Ogre::Vector3 pos, Ogre::Vector3 dir )
        : CProjectile( pos, 1.0, 1.0, name, "missile.mesh" ) {

    setLinearVelocity( -10.f * dir );
    mTTL=new CDelay( 4000 );
}


void CMissile::update() {

    int time = CTimeManager::getInstance()->getCurrent();

    if( mTTL->isOver() ) mDead=true;

}



/*
CPulse* CProjectileFactory::createPulse( Ogre::Vector3 pos, Ogre::Vector3 dir ) {
    return( new CPulse(
        "pulse" + Ogre::StringConverter::toString( mProjectileCount ++ ),
        pos, dir ) );
}



CRocket* CProjectileFactory::createRocket(Ogre::Vector3 pos, Ogre::Vector3 dir) {
     return( new CRocket(
        "rocket" + Ogre::StringConverter::toString( mProjectileCount ++ ),
        pos, dir ) );
}

*/


