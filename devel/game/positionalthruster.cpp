
#include "positionalthruster.h"

#include "scenemanager.h"


/** @brief CPositionalThruster
  *
  */
CPositionalThruster::CPositionalThruster(
    Ogre::SceneNode* parent,
    const Ogre::String& name,
    const Ogre::Vector3& relpos,
    Placement placement ):
        mParentNode( parent ),
        mName( name ),
        mRelPos( relpos ),
        mPlacement( placement ),
        mNode(0),
        mUp(0),
        mDown(0),
        mSide(0) {


        mNode=mParentNode->createChildSceneNode( mRelPos );

        mUp=CSceneManager::getInstance()->createEntity( name + "up" , "minithrustup.mesh" );
        mNode->attachObject( mUp );
        mUp->setQueryFlags( 0x0 ); // avoid detection in actor queries

        mDown=CSceneManager::getInstance()->createEntity( name + "down" , "minithrustdown.mesh" );
        mNode->attachObject( mDown );
        mDown->setQueryFlags( 0x0 ); // avoid detection in actor queries

        if( mPlacement==FRONTRIGHT || mPlacement==REARRIGHT ) {
            mSide=CSceneManager::getInstance()->createEntity( name + "right" , "minithrustright.mesh" );
        }
        else {
            mSide=CSceneManager::getInstance()->createEntity( name + "left" , "minithrustleft.mesh" );
        }

        mNode->attachObject( mSide );
        mSide->setQueryFlags( 0x0 ); // avoid detection in actor queries
        mNode->scale(0.08,0.08,0.08);

}




/** @brief ~CPositionalThruster
  *
  */
CPositionalThruster::~CPositionalThruster() {
    CSceneManager::getInstance()->destroyEntity(mUp);
    CSceneManager::getInstance()->destroyEntity(mDown);
    CSceneManager::getInstance()->destroyEntity(mSide);

    mParentNode->removeChild(mNode);

}


void CPositionalThruster::cutPower() {
    mUp->setVisible( false );
    mDown->setVisible( false );
    mSide->setVisible( false );
    setPower( 0,0,0 );
}


void CPositionalThruster::setPower( float forceYaw, float forcePitch, float forceRoll ) {

//    if( vel.length() > 0.1 ) {

    switch( mPlacement ) {
        case FRONTLEFT:
            mUp->setVisible( forcePitch<0 || forceRoll>0 );
            mDown->setVisible( forcePitch>0 || forceRoll<0 );
            mSide->setVisible( forceYaw<0 );
        break;
        case FRONTRIGHT:
            mUp->setVisible( forcePitch<0 || forceRoll<0 );
            mDown->setVisible( forcePitch>0 || forceRoll>0 );
            mSide->setVisible( forceYaw>0 );
        break;
        case REARRIGHT:
            mUp->setVisible( forcePitch>0 || forceRoll<0 );
            mDown->setVisible( forcePitch<0 || forceRoll>0 );
            mSide->setVisible( forceYaw<0 );
        break;
        case REARLEFT:
            mUp->setVisible( forcePitch>0 || forceRoll>0 );
            mDown->setVisible( forcePitch<0 || forceRoll<0 );
            mSide->setVisible( forceYaw>0 );
        break;
    }


//    }
//    else
//        cutPower();

}



