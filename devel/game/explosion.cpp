

#include "explosion.h"
#include "particleemittermanager.h"
#include "hopperparticles.h"
#include "hoppersounds.h"
#include "soundmanager.h"


CBasicExplosion::CBasicExplosion( Ogre::Vector3 pos ):IExplosion() {

    mDone = false;
    CParticleEmitterManager::getInstance()->emit( PART_EXPLOSION, 50, pos ); // using default pool

    CSoundManager::getInstance()->play3D( SOUND_EXPLODE, pos, 1 );
}

//----------------------------------------------------------------

CBasicExplosion::~CBasicExplosion() {
}

//----------------------------------------------------------------

bool CBasicExplosion::done() {

    return( true ); // using default pool


}

//=================================================================


CSmokeExplosion::CSmokeExplosion( Ogre::Vector3 pos ):CBasicExplosion( pos ) {

    mExplodeDelay = new CDelay( 3000 );
    mPos = pos;
}

//----------------------------------------------------------------

CSmokeExplosion::~CSmokeExplosion() {

    delete( mExplodeDelay );

};

//----------------------------------------------------------------

bool CSmokeExplosion::done() {

    if( mExplodeDelay->isOver() ) {
        mDone=true;
        CParticleEmitterManager::getInstance()->emit( PART_DARK_SMOKE, 20, mPos );
    }

    return( mDone );
}

//----------------------------------------------------------------

CBlastExplosion::CBlastExplosion( Ogre::Vector3 pos ):CBasicExplosion( pos ) {

    mBlast = new CBlast( pos, Ogre::Vector3( 10, 10, 10 ), 1000 );
    mBlastDelay = new CDelay( 1000 );
    mDone = false;
}

CBlastExplosion::~CBlastExplosion() {
    delete( mBlast );
    delete( mBlastDelay );
}

bool CBlastExplosion::done() {

    if( mBlastDelay->isOver() ) {
        mDone = true;
    }

    return( mDone );
}

//----------------------------------------------------------------



