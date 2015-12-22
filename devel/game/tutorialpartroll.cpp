/// gravity tutorial

#include "tutorialpartroll.h"
#include "logger.h"
//#include "rollthrustshipcontroller.h"
#include "PlayerControlledShipControllers.h"

CTutorialpartRoll::CTutorialpartRoll(CShip* aShip):CTutorialpartBox(aShip) {

    mShipController=new CRollThrustShipController( mShip );

}


CTutorialpartRoll::~CTutorialpartRoll() {
    // delete( mShipController );
}


void CTutorialpartRoll::init() {

    LOG("\nTutorial Part Roll init");

    mTutDataMax=4;

    mTutData[0].x=2;
    mTutData[0].y=2;
    mTutData[0].z=0;

    mTutData[1].x=4;
    mTutData[1].y=5;
    mTutData[1].z=0;

    mTutData[2].x=2;
    mTutData[2].y=-3;
    mTutData[2].z=0;

    mTutData[3].x=-1;
    mTutData[3].y=-4;
    mTutData[3].z=0;

    mTutData[4].x=-3;
    mTutData[4].y=-4;
    mTutData[4].z=0;


    mTutNum=0;

    CTutorialpartBox::init();

    mMessage.push_back("Roll tutorial");
  //  mMessage.push_back(" ");
    mMessage.push_back("* Roll orientation enabled ");
    mMessage.push_back("* Yaw & Pitch orientation disabled");
    mMessage.push_back("* Thrust enabled");
    mMessage.push_back(" ");
    mMessage.push_back("Goal : Stabilize in each cube for 1 second");
    mMessage.push_back("Use : the left mouse button to thrust, mouse left/right");
  //  mMessage.push_back(" ");
  //  mMessage.push_back(" *** Press space to continue ***");

}

bool CTutorialpartRoll::getYawEnabled() {
    return(false);
}

bool CTutorialpartRoll::getPitchEnabled() {
    return(false);
}

bool CTutorialpartRoll::getRollEnabled() {
    return(true);
}

bool CTutorialpartRoll::getThrustEnabled() {
    return(true);
}

std::vector<std::string> CTutorialpartRoll::getMessage() {
    return(mMessage);
}

