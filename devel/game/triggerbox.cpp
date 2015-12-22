

#include "triggerbox.h"
#include "triggerboxmanager.h"
#include "scenemanager.h"
#include "physicobject.h"

#include "logger.h"

CTriggerBox::CTriggerBox( Ogre::Vector3 position, Ogre::Vector3 dimensions ) {

    mAAB = Ogre::AxisAlignedBox(
        Ogre::Vector3( position - dimensions/2.f ),
        Ogre::Vector3( position + dimensions/2.f ));



    mQuery = CSceneManager::getInstance()->getOgreSceneManager()->createAABBQuery( mAAB );

    CTriggerBoxManager::getInstance()->addTriggerBox( this );

    mActive = true;

}

CTriggerBox::~CTriggerBox() {

    CTriggerBoxManager::getInstance()->removeTriggerBox( this );

    CSceneManager::getInstance()->getOgreSceneManager()->destroyQuery( mQuery );
}

void CTriggerBox::setActive( bool active ) {
    mActive = active;
}


void CTriggerBox::update() {

    if( mActive) {
        mQuery->setBox( mAAB );

        Ogre::SceneQueryResult &result = mQuery->execute();

        Ogre::SceneQueryResultMovableList::iterator it;
        for( it = result.movables.begin(); it != result.movables.end(); ++it ) {

            LOG("\nCTriggerBox::update() : movable %s (%p) => actor %p",
            (*it)->getName().c_str(), (void*)(*it), static_cast<CActor*>((*it)->getUserObject() ) );

            CActor* actor = static_cast<CActor*>((*it)->getUserObject() );
            if( actor )
                action( (*it) );

        }

    }

}

/*
void CTriggerBox::action( Ogre::MovableObject* obj ) {
    if( mEffectOwner )
        mEffectOwner->notifyEffect( obj );
}
*/

// ===============================================================================



CWindZone::CWindZone( Ogre::Vector3 pos, Ogre::Vector3 dimensions, Ogre::Vector3 velocity ):
    CTriggerBox( pos, dimensions )  {

    mVelocity = velocity;
    // only on dynamic physics actor :
    mQuery->setQueryTypeMask( Ogre::SceneManager::ENTITY_TYPE_MASK );
    mQuery->setQueryMask( CActor::DYNAMIC_OBJECT );

}

CWindZone::~CWindZone() {

}

void CWindZone::action( Ogre::MovableObject* obj) {

    static_cast<CDynamicPhysActor*>(obj->getUserObject())->addForce( mVelocity );
}


// ===============================================================================



CBlast::CBlast( Ogre::Vector3 pos, Ogre::Vector3 dimensions, float power ):
    CTriggerBox( pos, dimensions ) {

    mPower=power;
    mPosition = pos;

    // only on dynamic physics actor :
    mQuery->setQueryTypeMask( Ogre::SceneManager::ENTITY_TYPE_MASK );
    mQuery->setQueryMask( CActor::DYNAMIC_OBJECT );

}

CBlast::~CBlast() {}

void CBlast::action( Ogre::MovableObject* obj ) {

    LOG("\nCBlast::action() ... " );

    CDynamicPhysActor* actor = static_cast<CDynamicPhysActor*>(obj->getUserObject());
    actor->wake();
    // construct a vector from the center of the explosion to the objet :
    Ogre::Vector3 dir( actor->getPosition() - mPosition );
    dir.normalise();
    dir = dir * mPower;
    // add velocity
    actor->addForce( dir );

    LOG(" done." );

}


// ===============================================================================

CLevelChanger::CLevelChanger( ILevelChanger* changer, Ogre::Vector3 pos, Ogre::Vector3 dimensions, unsigned int levelNum ):
    CTriggerBox( pos, dimensions ) {

    mChanger = changer;
    mLevelNum = levelNum;

}


CLevelChanger::~CLevelChanger() {}

void CLevelChanger::action( Ogre::MovableObject* obj ) {
    // if( obj == ship ) notify CGame
    mChanger->notifyLevelChange( mLevelNum );

}







