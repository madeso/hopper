


/// gravity tutorial

#include "tutorialpartbox.h"
#include "hoppersounds.h"
#include "logger.h"
#include "soundmanager.h"

CTutorialpartBox::CTutorialpartBox(CShip* aShip):ITutPart() {

    mShip=aShip;
//    mSoundManager=CSoundManager::getInstance();
    mTutDelay=new CDelay(1000);

    // mSoundBeep=new CSound2D( SOUND_REFUEL_OK );

    mGoal=0;

}


CTutorialpartBox::~CTutorialpartBox() {
    LOG("\n\tEnd of TutorialPartBox");
    if(mGoal) delete(mGoal);


    //LOG("\n\t\tdeleting sound");
    // delete(mSoundBeep);

    mShip->thrust( false );
    mShip->setLinearVelocity( Ogre::Vector3( 0,0,0 ));
    mShip->setAngularVelocity( Ogre::Vector3(0,0,0));
    mShip->resetOrientation();

}


void CTutorialpartBox::init() {

 //   mTutDataMax=0;

 //   mTutNum=0;

    LOG("\nTutorial Part BOX init");

    mGoal=new CActor("TutGoal","tutorial_goal.mesh");
    mGoal->scale( 2, 2, 2 );


    nextPart();
}


void CTutorialpartBox::nextPart() {

    // Ogre::Vector3 shipPos;

    mShipStartPos=mShip->getPosition();

//            mPointerGoal->setRotation( 0,0,0);
    mGoal->setPosition( mShipStartPos + mTutData[mTutNum] );
        // shipPos.x+mTutData[mTutNum].x,shipPos.y+mTutData[mTutNum].y, shipPos.z+mTutData[mTutNum].z );

    tutOk=false;
    tutPartialOk=false;
    mGoal->getEntity()->setMaterialName("tutorial_goal/red");
    // mGoal->mEntity->setVisible(true);

}


int CTutorialpartBox::run() {

    if( (mShip->getPosition()-mShipStartPos).length() > 10 ) {
        mShip->setPosition( mShipStartPos );
        mShip->resetOrientation();
        mShip->setLinearVelocity( Ogre::Vector3( 0,0,0 ));
        mShip->setAngularVelocity( Ogre::Vector3(0,0,0));
    }

    if( tutPartOk() ) {

        mTutNum++;
        if(mTutNum > mTutDataMax) {
            return(TUTPART_DONE);
        }
        nextPart();
    }
    return(TUTPART_NOTHING);
}



bool CTutorialpartBox::tutPartOk() {

        float threshold=1;
        Ogre::Vector3 shipPos;

        shipPos=mShip->getPosition();


        if( !tutOk ) {
            //  next tut condition
            if( shipPos.positionEquals( mGoal->getPosition(), threshold )) {

                if( !tutPartialOk ) {
                    mGoal->getEntity()->setMaterialName("tutorial_goal/green");  // green
                    tutPartialOk=true;
                    mTutDelay->restart();
                }
                else {
                    if(mTutDelay->isOver() ) {
                        tutOk=true;
                        // mSoundManager->play(SOUND_REFUEL_OK);
                        CSoundManager::getInstance()->play2D( SOUND_LANDED, 1 );
                        // mSoundBeep->play();
                        return(true);
                    }
                }
            }
            else {
                if( tutPartialOk ) {
                   mGoal->getEntity()->setMaterialName("tutorial_goal/red");
                   // mGoal->mEntity->setVisible(false);
                   tutPartialOk=false;
                }
            }

        }

    return(false);


}

/*
bool CTutorialpartBox::getYawEnabled() {
    return(true);
}

bool CTutorialpartBox::getPitchEnabled() {
    return(true);
}

bool CTutorialpartBox::getRollEnabled() {
    return(true);
}

bool CTutorialpartBox::getThrustEnabled() {
    return(true);
}
*/

std::vector<std::string> CTutorialpartBox::getMessage() {
    return(mMessage);
}

