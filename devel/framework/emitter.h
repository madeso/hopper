

#ifndef _EMITTER_H_
#define _EMITTER_H_

#include "Ogre.h"
#include "scenemanager.h"


/// Particle emitter.


class CEmitter {

    public :

        CEmitter( Ogre::String aName, Ogre::String aType, Ogre::SceneNode* aNode );
        CEmitter( Ogre::String aName, Ogre::String aType  );
        ~CEmitter();

        void emit( bool aEmit );
        void setPosition( Ogre::Vector3 aPosition );
        Ogre::Vector3 getPosition();
        void addParticle();
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

