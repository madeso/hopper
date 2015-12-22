

#ifndef _EXPLOSION_
#define _EXPLOSION_

/** base explosion
  *
  * this could be inherited by other explosions types
  * ( with smoke after the explosion, explosion with area effect on objects, etc... )
  */

#include "Ogre.h"
#include "delay.h"
// #include "particleemitter.h"


class IExplosion {

public:
    virtual ~IExplosion() {}; // =0;

    virtual bool done()=0;

};




class CBasicExplosion : public IExplosion {

public:
    CBasicExplosion( Ogre::Vector3 pos );
    virtual ~CBasicExplosion();

    bool done();


protected:
    bool mDone;
    //CParticleEmitter* mEmitter;

};

// ===============================================================

class CSmokeExplosion:public CBasicExplosion {

public:
    CSmokeExplosion( Ogre::Vector3 pos );
    virtual ~CSmokeExplosion();

    bool done();

private:
    CDelay* mExplodeDelay;
    Ogre::Vector3 mPos;

};

// ===============================================================

#include "triggerbox.h"

class CBlastExplosion: public CBasicExplosion {

public:
    CBlastExplosion( Ogre::Vector3 pos );
    virtual ~CBlastExplosion();

    bool done();

private:
    CDelay* mBlastDelay;
    CBlast* mBlast;

};

#endif




