


#include "clouds.h"

#include "scenemanager.h"

#include "logger.h"


CClouds::CClouds( Ogre::Vector3 pos, Ogre::Vector3 size, int aMax , Ogre::String material ) {

    float x,y,z;

    mX=pos.x;
    mZ=pos.z;
    mHeight=pos.y;

    mNode=static_cast<Ogre::SceneNode*>(CSceneManager::getInstance()->getRootSceneNode()->createChild() );
    // mNode=CSceneManager::getInstance()->getSceneManager()->createSceneNode("cloudlayer");

    LOG("\nCreating the clouds billboardset...");
    mBillboardSet=CSceneManager::getInstance()->getNewBillBoardSet("clouds");
    LOG("done.");

    mBillboardSet->setPoolSize( aMax );

    mBillboardSet->setMaterialName( "hoper/cloud" );
    // mBillboardSet->setBillboardType( Ogre::BBT_ORIENTED_SELF); // BBT_ORIENTED_COMMON );
    mBillboardSet->setDefaultDimensions(128,128 );


    // create a bunch of clouds
    for(int i=0; i<aMax; i++ ) {
        x=rand()%(int)size.x-size.x/2;
        y=rand()%(int)size.y;           // thickness
        z=rand()%(int)size.z-size.z/2;

        mBillboardSet->createBillboard( Ogre::Vector3( x,y,z) );

    }


    mNode->attachObject(mBillboardSet);

    mNode->setPosition( mX, mHeight, mZ );

}


CClouds::~CClouds() {
    CSceneManager::getInstance()->getRootSceneNode()->removeChild( mNode );
    // CSceneManager::getInstance()->getSceneManager()->destroySceneNode("cloudlayer");
    CSceneManager::getInstance()->destroyBillboardSet(mBillboardSet);
}

/*
CClouds::setSize( float aSize ) {
    mSize=aSize;
}
*/


void CClouds::setPos( Ogre::Vector3 pos ) {
    mX=pos.x;
    mZ=pos.z;
    mNode->setPosition( mX, mHeight, mZ );
}

void CClouds::setHeight( float aHeight ) {
    mHeight=aHeight;
    mNode->setPosition( mX, mHeight, mZ );
}

/*
void CClouds::update( Ogre::Vector3 camPos) {

    LOG("\n ~~~ hey clouds !");
    mNode->setPosition( camPos.x, mHeight, camPos.z );

}
*/
