

#include "astrum.h"

CAstrum::CAstrum( Ogre::Vector3 direction, Ogre::String material, float size ) {

    mDirection = direction;

    mNode=static_cast<Ogre::SceneNode*>(CSceneManager::getInstance()->getRootSceneNode()->createChild() );

    mBillboardSet=CSceneManager::getInstance()->getNewBillBoardSet("sun");

    mBillboardSet->setMaterialName( material );
    mBillboardSet->setDefaultDimensions( size, size );
    mBillboardSet->createBillboard( Ogre::Vector3(0,0,0) );
    mNode->attachObject(mBillboardSet);

    CCamera::getInstance()->addListener( this );

}


CAstrum::~CAstrum() {

    CSceneManager::getInstance()->getRootSceneNode()->removeChild( mNode );
    CSceneManager::getInstance()->destroyBillboardSet(mBillboardSet);
    CCamera::getInstance()->removeListener( this );

}


void CAstrum::setDirection( Ogre::Vector3 direction ) {


    direction.normalise();
    mDirection=direction;

    onCameraMoved( CCamera::getInstance()->getPosition()  );
}


void CAstrum::onCameraMoved( Ogre::Vector3 campos ) {
    mNode->setPosition( campos - mDirection * ASTRUM_DISTANCE_TO_CAMERA );

}

