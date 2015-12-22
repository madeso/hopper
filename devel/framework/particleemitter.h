

#ifndef _PARTICLEEMITTER_
#define _PARTICLEEMITTER_

#include "Ogre.h"



class CParticleEmitter {

    //friend class CParticleEmitterPool;
    //friend class CParticleEmitterManager;

    //protected :
    public:

        CParticleEmitter( Ogre::String aName, Ogre::String aType, Ogre::SceneNode* aNode );
        CParticleEmitter( Ogre::String aName, Ogre::String aType  );
        ~CParticleEmitter();

        void emit( bool aEmit );
        void setPosition( Ogre::Vector3 aPosition );
        Ogre::Vector3 getPosition();
        void addParticle( unsigned int numParticles=1 );
        void update();


    private:

        Ogre::SceneNode* mNode;
        bool mNodeCreated;

        int mMaxParticles;
        Ogre::ParticleSystem* mSystem;
        Ogre::ParticleEmitter* mEmitter;

        void init( Ogre::String aName, Ogre::String aType );
};


#endif


