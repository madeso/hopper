
#include "scenemanager.h"
#include "actor.h"

#include "logger.h"
// #include "soundmanager.h"
//#include "hoppersounds.h"
#include "explosionmanager.h"

CActor::CActor(Ogre::String name, Ogre::String modelname ) { //:Ogre::SceneNode(CSceneManager::getInstance(), name) {

    mSceneManager=CSceneManager::getInstance(); // ->getOgreSceneManager();
    mRoot=mSceneManager->getRootSceneNode();

    mNode=mRoot->createChildSceneNode( name );

    mEntity=mSceneManager->createEntity( name, modelname);

    //LOG("\nattaching entity to node '%s'...", name.c_str() );
    mNode->attachObject(mEntity);
    //LOG("done.");

//    mEntity->setNormaliseNormals(true);
    mEntity->setUserObject( this );
    mEntity->setQueryFlags( BASIC_OBJECT );

    mName=name;

    mYaw=mPitch=mRoll=0;

    mLife=100;
    mDead=false;
    mDestroyable=false;

}


CActor::~CActor() {

    mRoot->removeAndDestroyChild( mName );

    if( mEntity ) {
        mSceneManager->destroyEntity( mEntity );
    }

}



Ogre::String CActor::getName() {
    return(mName);
}


void CActor::castShadows() {
        // if( mCastShadows ) {
    mEntity->getMesh()->buildEdgeList();
    mEntity->getMesh()->prepareForShadowVolume();
    mEntity->setCastShadows(true);
        // }
        // else   mEntity->setCastShadows(false);
}

void CActor::setQueryFlags( uint32 flags ) {
    mEntity->setQueryFlags( flags );
}

uint32 CActor::getQueryFlags() {
    return( mEntity->getQueryFlags() );
}


void CActor::setVisible( bool aVisible ) {
//        mVisible=aVisible;
    mNode->setVisible( aVisible );
}

void CActor::setCastShadows( bool castshadow ) {
     mEntity->setCastShadows(castshadow);

}




Ogre::Vector3 CActor::getPosition() {
    return( mNode->getPosition());
}

void CActor::setPosition( Ogre::Vector3 pos ) {
    mNode->setPosition(pos);
}

void CActor::scale( Ogre::Vector3 scalefactor ) {
    mNode->scale( scalefactor );
}

void CActor::scale( float widthfactor, float depthfactor, float heightfactor ) {
    mNode->scale( widthfactor, depthfactor, heightfactor );
}

void CActor::setScale( Ogre::Vector3 scale ) {
    mNode->setScale( scale );
}

void CActor::setScale( float xscale, float yscale, float zscale ) {
    mNode->setScale( xscale, yscale, zscale );
}

void CActor::setOrientation( Ogre::Quaternion o ) {
    mNode->setOrientation( o );
}

/*
void CActor::yaw( float aYaw) {
  mNode->yaw(Ogre::Radian(mYaw));
}
*/

void CActor::setYaw( float aYaw ) {
//    yaw( (Ogre::Radian)aYaw); // , Ogre::Node::TS_LOCAL );
    mNode->resetOrientation();
    mYaw=aYaw;
    mNode->yaw( (Ogre::Radian)mYaw );
    mNode->pitch( (Ogre::Radian)mPitch );
    mNode->roll( (Ogre::Radian)mRoll );
}

void CActor::setPitch( float aPitch ) {

    mNode->resetOrientation();
    mPitch=aPitch;
    mNode->yaw( (Ogre::Radian)mYaw );
    mNode->pitch( (Ogre::Radian)mPitch );
    mNode->roll( (Ogre::Radian)mRoll );

}

void CActor::setRoll( float aRoll ) {

    mNode->resetOrientation();
    mRoll=aRoll;
    mNode->yaw( (Ogre::Radian)mYaw );
    mNode->pitch( (Ogre::Radian)mPitch );
    mNode->roll( (Ogre::Radian)mRoll );

}

float CActor::getPitch() {
    return(mPitch);
}

float CActor::getRoll() {
    return(mRoll);
}

float CActor::getYaw() {
    return(mYaw);
}


void CActor::orient( float aYaw, float aPitch, float aRoll ) {
    mYaw=aYaw;
    mPitch=aPitch;
    mRoll=aRoll;

    Ogre::Quaternion Q1,Q2,Q3,Q;

        Q1.FromAngleAxis( (Ogre::Radian)aRoll, Ogre::Vector3( 0,0,1) );
        Q2.FromAngleAxis ( (Ogre::Radian)aPitch, Ogre::Vector3( 1,0,0) );
        Q3.FromAngleAxis ( (Ogre::Radian)aYaw, Ogre::Vector3( 0,1,0) );
        Q=Q3*Q1*Q2;

        Q.normalise();
        // Ogre::Node::setOrientation( Q );
        mNode->setOrientation( Q );

}


Ogre::Quaternion CActor::getOrientation() {
    return( mNode->getOrientation() );
}

Ogre::AxisAlignedBox CActor::getAABB() {
    // mNode->_updateBounds();
    mNode->_update(false,false);
    return(mNode->_getWorldAABB());
}

Ogre::SceneNode* CActor::getNode() {
    return(mNode);
}

Ogre::Entity* CActor::getEntity() {
    return(mEntity);
}


void CActor::showBoundingBox( bool show ) {
    mNode->showBoundingBox( show);
}

void CActor::attachObject( Ogre::MovableObject* node ) {
    mNode->attachObject( node );
}



// ==================== destructability


void CActor::setDestroyable( bool destroyable ) {
    mDestroyable=destroyable;
}

bool CActor::isDestroyable() {
    return( mDestroyable );
}


void CActor::setLife( float life ) {
    mLife=life;
}

bool CActor::isDead() {
    return( mDead);
}


void CActor::damage(float aDamage ) {

    if( mDestroyable && !mDead ) {

        mLife -= aDamage;

        if( mLife <= 0 ) {
            mLife = 0;
            // TODO : explosion types (sound, particles ), damaged model ?
            // generate an explosion here
            // CExplosionManager::getInstance()->createExplosion( getNode() );
            // mEntity->setMaterialName( "destroyed" );

            die();
            mDead=true;
        }

    }

}



void CActor::die() {
    CExplosionManager::getInstance()->createExplosion( getNode() );
    mEntity->setMaterialName( "destroyed" );
}





