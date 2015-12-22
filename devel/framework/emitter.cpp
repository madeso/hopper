


#include "emitter.h"
#include "logger.h"


CEmitter::CEmitter(Ogre::String aName, Ogre::String aType, Ogre::SceneNode* aNode ) {

   mNode=aNode;
   mNodeCreated=false;
    init(  aName, aType );

}


CEmitter::CEmitter(Ogre::String aName, Ogre::String aType ) {
       mNode=CSceneManager::getInstance()->getRootSceneNode()->createChildSceneNode();
       mNodeCreated=true;
       init(  aName, aType );
}

CEmitter::~CEmitter() {
    //mSystem->getEmitter(0)->setEnabled(false);
    //mSystem->removeAllEmitters();
    CSceneManager::getInstance()->destroyParticleSystem(mSystem);
    if(mNodeCreated) CSceneManager::getInstance()->getRootSceneNode()->removeChild( mNode);
}

void CEmitter::init( Ogre::String aName, Ogre::String aType ) {

    mSystem=CSceneManager::getInstance()->createParticleSystem( aName, aType );

    LOG("\n========> particle system %s has %d emitters ", aName.c_str(), mSystem->getNumEmitters());

    mEmitter=mSystem->getEmitter(0);

    mNode->attachObject( mSystem);

    mSystem->setVisible(true);
    //mSystem->getEmitter(0)->setEnabled(false);

    mEmitter->setEnabled(false);

    mSystem->_update( 0 );  // mandatory to properly init the emitter or Ogre dies !!

    mSystem->createParticle();


    mMaxParticles=mSystem->getParticleQuota();

}

void CEmitter::update() {
    mSystem->_update( 0.0166);
}

void CEmitter::setPosition(Ogre::Vector3 aPosition ) {
   if(mNodeCreated) {
        mNode->setPosition( aPosition );

   }
}

Ogre::Vector3 CEmitter::getPosition() {
    return(mNode->getPosition());
}


void CEmitter::emit( bool aEmit ) {

    // mSystem->getEmitter(0)->setEnabled(aEmit);
    mEmitter->setEnabled(aEmit);

}

void CEmitter::addParticle() {

    if( mSystem->getNumParticles() <  mMaxParticles ) {
        mSystem->createParticle();
    }

}


