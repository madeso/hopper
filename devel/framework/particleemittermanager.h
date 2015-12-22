

#ifndef _PARTICLEEMITTERMANAGER_
#define _PARTICLEEMITTERMANAGER_

#include "particleemitter.h"
#include "singleton.h"
#include "particleemitterpool.h"


class CParticleEmitterManager: public SSingleton<CParticleEmitterManager> {

    public:
    CParticleEmitterManager();
    ~CParticleEmitterManager();

    void addEmitterType( unsigned int emitterTypeID, Ogre::String aType );
    Ogre::String getTypeName( unsigned int emitterTypeID );


// are non-default particle pools useful ???
/*
    CParticleEmitterPool* createPool( Ogre::String name, unsigned int emitterTypeID, unsigned int maxEmitters );
    void destroyPool( Ogre::String name );
    CParticleEmitterPool* getDefaultPool( unsigned int emitterTypeID );
*/

    CParticleEmitter* createEmitter( unsigned int emitterTypeID, Ogre::SceneNode* node=0 );
    void destroyEmitter( CParticleEmitter* emitter );

    void emit( unsigned int emitterTypeID, unsigned int numParticles, Ogre::Vector3 position );


    void update();

    private:
    std::vector<std::string> mEmitterTypes;
    std::list<CParticleEmitter*> mEmitters;
    std::vector<CParticleEmitterPool*> mDefaultPools;
    // std::map< Ogre::String, CParticleEmitterPool* > mEmitterPools;

    int mCount;

};



#endif


