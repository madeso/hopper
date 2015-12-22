

/// Orientation tutorial


#include "tutorialpartorientation.h"
#include "logger.h"
#include "hoppersounds.h"
#include "soundmanager.h"
// #include "nothrustshipcontroller.h"
#include "PlayerControlledShipControllers.h"

CTutorialpartOrientation::CTutorialpartOrientation(CShip *aShip ):ITutPart() {

    // mSoundManager=CSoundManager::getInstance();
    mTutDelay=new CDelay(1000);
    mShip=aShip;

    // mSoundOk=new CSound2D( SOUND_REFUEL_OK );

    mMessage.push_back("Orientation tutorial");
    mMessage.push_back("* Orientation enabled");
    mMessage.push_back("* Thrust disabled");
    mMessage.push_back("");
    mMessage.push_back("---------- Goal ---------");
    mMessage.push_back("Orient the ship to match the red ship orientaton");
    mMessage.push_back("");
    //mMessage.push_back("Use the mouse to orient the ship's attitude");
    //mMessage.push_back("Use Q/D to rotate left/right");
   // mMessage.push_back("");
   // mMessage.push_back("*** Press <Space> to continue ***");

   mGoal=0;

   mShipController=new CNoThrustShipController(mShip);


}

CTutorialpartOrientation::~CTutorialpartOrientation() {

    // delete(mShipController);
    if(mGoal) delete(mGoal);


    mShip->setAffectedByGravity(true);
    mShip->thrust(false);
    mShip->resetOrientation();
    mShip->setLinearVelocity( Ogre::Vector3(0,0,0));
    mShip->setAngularVelocity( Ogre::Vector3(0,0,0));

    //delete(mSoundOk);
}


CShipController* CTutorialpartOrientation::getShipController() {
    return( mShipController);
}


void CTutorialpartOrientation::init() {

    LOG("\nTutorial part Orientation init");

    mGoal=new CActor( "tut_pointer_goal", "hopper11.mesh" );
    //mGoal->getEntity()->setMaterialName("arrow/arrowred");
    //mGoal->getEntity()->setCastShadows(false);


    mTutData[0].x=0;
    mTutData[0].y=45;
    mTutData[0].z=0;

    mTutData[1].x=0;
    mTutData[1].y=0;
    mTutData[1].z=50;

    mTutData[2].x=90;
    mTutData[2].y=0;
    mTutData[2].z=0;

    mTutData[3].x=20;
    mTutData[3].y=-20;
    mTutData[3].z=20;

    mTutData[4].x=110;
    mTutData[4].y=0;
    mTutData[4].z=50;

    mTutData[5].x=40;
    mTutData[5].y=35;
    mTutData[5].z=10;

    mTutData[6].x=-10;
    mTutData[6].y=-15;
    mTutData[6].z=0;

    mTutData[7].x=20;
    mTutData[7].y=10;
    mTutData[7].z=-20;

    mTutData[8].x=0;
    mTutData[8].y=0;
    mTutData[8].z=0;

    mTutNum=0;
    mTutDataMax=8;



    nextPart();

}

void CTutorialpartOrientation::nextPart() {

        Ogre::Vector3 shipPos;

        shipPos=mShip->getPosition();

        mShip->setAffectedByGravity(false);
        mShip->resetOrientation();
        mShip->setLinearVelocity( Ogre::Vector3( 0,0,0 ));

        // mPointerGoal->setRotation( 0, 45.f*3.1415f/180.f,0.f);
        mGoal->orient( mTutData[mTutNum].x*3.1415/180.f,mTutData[mTutNum].y*3.1415/180.f ,mTutData[mTutNum].z*3.1415/180.f);
        mGoal->setPosition( Ogre::Vector3(shipPos.x, shipPos.y+3.f, shipPos.z ));
        tutOk=false;
        tutPartialOk=false;
        mGoal->getEntity()->setMaterialName("tutorial_goal/translucent_red");
}



int CTutorialpartOrientation::run() {

    mShip->setAffectedByGravity( false );  // force it because setAffectedByGravity(true) is used as a pause system in CGameplayTutorial

    if( tutPartOk() ) {

        mTutNum++;
        if(mTutNum > mTutDataMax) {
            return(TUTPART_DONE);
        }
        nextPart();
    }

}


bool CTutorialpartOrientation::tutPartOk() {

    float a;
    Ogre::Radian threshold( 5.0f * 3.1415 / 180.f );
    Ogre::Vector3 shipPos;


    shipPos=mShip->getPosition();
    mGoal->setPosition( Ogre::Vector3(shipPos.x, shipPos.y /* +3.f */ , shipPos.z ) );

          //  mPointerCurrent->setPosition(shipPos.x, shipPos.y, shipPos.z );
          //  mPointerCurrent->setOrientation( mShip->getPitchRollOrientation() );
          /*
            a=-(mShip->mYaw+(90.f*3.1415/180.f));
            mPointerGoal->setPosition(shipPos.x-sin(a)*3.f, shipPos.y, shipPos.z+cos(a)*3.f) ;
            */


        if( !tutOk ) {
            //  next tut condition
            if( mShip->getOrientation().equals( mGoal->getOrientation(), threshold )) {

                if( !tutPartialOk ) {
                    mGoal->getEntity()->setMaterialName("tutorial_goal/translucent_green");
                    tutPartialOk=true;
                    mTutDelay->restart();
                }
                else {
                    if(mTutDelay->isOver() ) {
                        tutOk=true;
                        // mSoundManager->play(SOUND_REFUEL_OK);
                        // mSoundOk->play();
                        CSoundManager::getInstance()->play2D( SOUND_LANDED, 1 );
                        return(true);
                    }
                }
            }
            else {
                if( tutPartialOk ) {
                   mGoal->getEntity()->setMaterialName("tutorial_goal/translucent_red");
                   tutPartialOk=false;
                }
            }

        }

    return(false);
}

/*

bool CTutorialpartOrientation::getYawEnabled() {
    return(true);
}

bool CTutorialpartOrientation::getPitchEnabled() {
    return(true);
}

bool CTutorialpartOrientation::getRollEnabled() {
    return(true);
}

bool CTutorialpartOrientation::getThrustEnabled() {
    return(false);
}
*/

std::vector<std::string> CTutorialpartOrientation::getMessage() {
    return(mMessage);

}


