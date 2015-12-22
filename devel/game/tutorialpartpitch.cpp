/// Pitch tutorial

#include "tutorialpartpitch.h"

#include "logger.h"
//#include "pitchthrustshipcontroller.h"
#include "PlayerControlledShipControllers.h"

CTutorialpartPitch::CTutorialpartPitch(CShip* aShip):CTutorialpartBox(aShip) {

    mShipController=new CPitchThrustShipController(mShip);

}


CTutorialpartPitch::~CTutorialpartPitch() {
    // delete( mShipController );
}


void CTutorialpartPitch::init() {

    LOG("\nTutorial Part Pitch init");

    mTutDataMax=4;

    mTutData[0].x=0;
    mTutData[0].y=2;
    mTutData[0].z=-2;

    mTutData[1].x=0;
    mTutData[1].y=5;
    mTutData[1].z=-4;

    mTutData[2].x=0;
    mTutData[2].y=-3;
    mTutData[2].z=2;

    mTutData[3].x=0;
    mTutData[3].y=-4;
    mTutData[3].z=3;

    mTutData[4].x=0;
    mTutData[4].y=4;
    mTutData[4].z=4;


    mTutNum=0;

    CTutorialpartBox::init();

    mMessage.push_back("Picth tutorial");
    // mMessage.push_back(" ");
    mMessage.push_back("* Pitch orientation enabled ");
    mMessage.push_back("* Yaw & Roll orientation disabled");
    mMessage.push_back("* Thrust enabled");
    mMessage.push_back(" ");
    mMessage.push_back("Goal : Stabilize in each cube for 1 second");
    // mMessage.push_back("Use : the left mouse button to thrust, mouse up/down");
    // mMessage.push_back(" ");
    // mMessage.push_back(" *** Press space to continue ***");

}


std::vector<std::string> CTutorialpartPitch::getMessage() {
    return(mMessage);
}

