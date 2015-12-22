


#ifndef _PARTICLE_EMITER_POOL_
#define _PARTICLE_EMITER_POOL_


#include "particleemitter.h"

class CParticleEmitterPool {

    public:

    CParticleEmitterPool( Ogre::String name, Ogre::String emitterType, unsigned int maxEmitters );
    // CParticleEmitterPool( Ogre::String name, unsigned int maxEmitters );

    ~CParticleEmitterPool();

    void emit( unsigned int aNumParticles, Ogre::Vector3 position );
    // void emit( unsigned int particleTypeID, unsigned int numParticles, Ogre::Vector3 position );

    void update();

    private:
    Ogre::String mName;
    unsigned int mMaxEmitters;
    Ogre::String mEmitterType;

    std::vector<CParticleEmitter*> mEmitters;
    std::vector<CParticleEmitter*>::iterator mPoolIterator;

};


#endif


