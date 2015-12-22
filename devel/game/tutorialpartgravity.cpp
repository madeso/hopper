
/// gravity tutorial

#include "tutorialpartgravity.h"

#include "logger.h"

// #include "thrustonlyshipcontroller.h"
#include "PlayerControlledShipControllers.h"

CTutorialpartGravity::CTutorialpartGravity(CShip* aShip):CTutorialpartBox(aShip) {

    LOG("\nCTutorialpartGravity::CTutorialpartGravity() ...");
    mShipController=new CThrustOnlyShipController( mShip );

    LOG("done.");

}


CTutorialpartGravity::~CTutorialpartGravity() {
    //delete(mShipController);
}




void CTutorialpartGravity::init() {

    LOG("\nTutorial Part Gravity init");

    mTutDataMax=4;

    mTutData[0].x=0;
    mTutData[0].y=2;
    mTutData[0].z=0;

    mTutData[1].x=0;
    mTutData[1].y=5;
    mTutData[1].z=0;

    mTutData[2].x=0;
    mTutData[2].y=-3;
    mTutData[2].z=0;

    mTutData[3].x=0;
    mTutData[3].y=4;
    mTutData[3].z=0;

    mTutData[4].x=0;
    mTutData[4].y=-4;
    mTutData[4].z=0;


    mTutNum=0;

    CTutorialpartBox::init();

    mMessage.push_back("Gravity tutorial");
    // mMessage.push_back(" ");
    mMessage.push_back("* Orientation disabled");
    mMessage.push_back("* Thrust enabled");
    // mMessage.push_back(" ");
    mMessage.push_back("Goal :");
    mMessage.push_back("  Stabilize in each cube for 1 second");
    mMessage.push_back("  Use the left mouse button to thrust");
    //mMessage.push_back(" ");
    //mMessage.push_back(" *** Press space to continue ***");

}
/*
bool CTutorialpartGravity::getYawEnabled() {
    return(false);
}

bool CTutorialpartGravity::getPitchEnabled() {
    return(false);
}

bool CTutorialpartGravity::getRollEnabled() {
    return(false);
}

bool CTutorialpartGravity::getThrustEnabled() {
    return(true);
}
*/
std::vector<std::string> CTutorialpartGravity::getMessage() {
    return(mMessage);
}

