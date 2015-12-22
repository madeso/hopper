

#include "landingpad.h"
#include "hoppersounds.h"
#include "logger.h"


CLandingPad::CLandingPad(Ogre::String aName, Ogre::Vector3 Pos ): CStaticPhysCube( Pos, 0, 1, aName, "piste2.mesh" ) {

    mName=aName;

    castShadows();

  // --------- setPosition before creating the immovable physic box. ( and don't move the node too ) !!
   //  setInitialPosition(Ogre::Vector3(x,y,z));

   // initPhysicCube();
   // initPhysicImmovableTrimesh();

    // ========= AABB ===================
    mAABB=getAABB();


    Ogre::Vector3 pmin,pmax;
    pmin=mAABB.getMinimum();
    pmax=mAABB.getMaximum();

//    pmax.y+=0.1;
//    mAABB.setMaximum(pmax);

    mSoundManager=CSoundManager::getInstance();
    // =================================

//    mSoundLanded=new CSound3D( SOUND_LANDED );
    mShipLanded=false;


    // ====== Attached Light ==========================

    mLight=new CPointLight( mName + "_light" , getNode() );
    mLight->setPosition( Ogre::Vector3( 0, pmax.y-getPosition().y , 0 ) );
    mLight->setColor( 1,0,0 );
    mLight->setVisible(true);


}


CLandingPad::~CLandingPad() {
    delete(mLight);
//    delete( mSoundLanded);
}


void CLandingPad::setShipLanded() {
    mShipLanded=true;
    mLight->setColor( 0,1,0 );
    mEntity->getSubEntity(2)->setMaterialName( "piste2/green" );

    //mSoundManager->play3D( SOUND_LANDED, getPosition(), 1);
    mSoundManager->play2D( SOUND_LANDED, 1);

}


bool CLandingPad::hasShipLanded() {
    return( mShipLanded );
}

