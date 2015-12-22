
/// sun

#include "sun.h"
#include "logger.h"

CSun::CSun( Ogre::Vector3 direction, Ogre::ColourValue colour,
  Ogre::String material, float size ): CAstrum( direction, material, size ) {


    // mLight->setPosition(pos.x, pos.y, pos.z );
    // mLight->setDirection( 0.2, -1, 0.3 );  // eek !
    // mLight->setColor( (float)0.5,(float)0.5,(float)0.5 );

//    mDirection = direction;


    mLight=new CDirectionalLight("sun");
    mLight->setDirection( mDirection.x, mDirection.y, mDirection.z );  // eek !
    mLight->setColor( colour.r, colour.g, colour.b );
    mLight->setSpecular( 1,1,1 );

/*
    mNode=static_cast<Ogre::SceneNode*>(CSceneManager::getInstance()->getRootSceneNode()->createChild() );

    mBillboardSet=CSceneManager::getInstance()->getNewBillBoardSet("sun");

    mBillboardSet->setMaterialName( material );
   // mBillboard->setBillboardType( Ogre::BBT_ORIENTED_COMMON );
    mBillboardSet->setDefaultDimensions( size, size );
    mBillboardSet->createBillboard( Ogre::Vector3(0,0,0) );

    mNode->attachObject(mBillboardSet);
*/
//    CCamera::getInstance()->addListener( this );

}

CSun::~CSun() {

/*
    CSceneManager::getInstance()->getRootSceneNode()->removeChild( mNode );
    CSceneManager::getInstance()->destroyBillboardSet(mBillboardSet);
    CCamera::getInstance()->removeListener( this );
*/
    delete(mLight);

}


void CSun::setDirection( Ogre::Vector3 dir ) {

    dir.normalise();
    mDirection=dir;
    mLight->setDirection( mDirection.x, mDirection.y, mDirection.z );

    onCameraMoved( CCamera::getInstance()->getPosition()  );

}





