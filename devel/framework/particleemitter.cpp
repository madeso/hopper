

#include "particleemitter.h"
#include "scenemanager.h"
//#include "logger.h"


CParticleEmitter::CParticleEmitter(Ogre::String aName, Ogre::String aType, Ogre::SceneNode* aNode ) {

   mNode=aNode;
   mNodeCreated=false;
    init( aName, aType );

}


CParticleEmitter::CParticleEmitter(Ogre::String aName, Ogre::String aType ) {
       mNode=CSceneManager::getInstance()->getRootSceneNode()->createChildSceneNode();
       mNodeCreated=true;
       init( aName, aType );
}

CParticleEmitter::~CParticleEmitter() {
    //mSystem->getEmitter(0)->setEnabled(false);
    //mSystem->removeAllEmitters();
    CSceneManager::getInstance()->destroyParticleSystem(mSystem);
    if(mNodeCreated) CSceneManager::getInstance()->getRootSceneNode()->removeChild( mNode);
}

void CParticleEmitter::init( Ogre::String aName, Ogre::String aType ) {

    mSystem=CSceneManager::getInstance()->createParticleSystem( aName, aType );

    mEmitter=mSystem->getEmitter(0);

    mNode->attachObject( mSystem);

    mSystem->setVisible(true);


    mEmitter->setEnabled(false);

    mSystem->_update( 0 );  // mandatory to properly init the emitter or Ogre dies !!

    mSystem->createParticle();


    mMaxParticles=mSystem->getParticleQuota();

    //LOG("\nCParticleEmitter::init() : mMaxParticles = %d for type %s", mMaxParticles, aType.c_str() );

}

void CParticleEmitter::update() {
    mSystem->_update( 0.0166);  // TODO : should ask the time manager...
}

void CParticleEmitter::setPosition(Ogre::Vector3 aPosition ) {
   if(mNodeCreated) {
        mNode->setPosition( aPosition );

   }
}

Ogre::Vector3 CParticleEmitter::getPosition() {
    return(mNode->getPosition());
}


void CParticleEmitter::emit( bool aEmit ) {

    // mSystem->getEmitter(0)->setEnabled(aEmit);
    mEmitter->setEnabled(aEmit);

}

void CParticleEmitter::addParticle( unsigned int numParticles ) {

    while( numParticles > 0 ) {

        if( mSystem->getNumParticles() <  mMaxParticles ) {
            mSystem->createParticle();
        }

        numParticles--;
    }

}


