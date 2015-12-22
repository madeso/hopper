


#ifndef _SHIPCONTROLLERS_
#define _SHIPCONTROLLERS_


#include "ShipController.h"


#include "inputmanager.h"
#include "timemanager.h"
#include "hopperinputactions.h"

class CPlayerControlledShipController: public CShipController {

    public:
    CPlayerControlledShipController( CShip* ship);

    void run();

    void manageYaw();
    void managePitch();
    void manageRoll();
    void manageFuelConsumption();
    void manageThrust();
    void manageRecoverMode();
    void manageTractorBeam();
    void manageFire();

    protected:
    CInputManager* mInputManager;
    CTimeManager* mTimeManager;
    // float mMouseXd, mMouseYd;

};


class CNoThrustShipController: public CPlayerControlledShipController {

    public:
    CNoThrustShipController( CShip* ship ): CPlayerControlledShipController( ship ) { };

    void run() {

        manageYaw();

//        mInputManager->getMouseDelta(mMouseXd,mMouseYd);

        managePitch();
        manageRoll();

        manageFuelConsumption();
        manageRecoverMode();

        mShip->update();

    }



};


class CPitchThrustShipController: public CPlayerControlledShipController {

    public:
    CPitchThrustShipController( CShip* ship ): CPlayerControlledShipController( ship ) {};

    void run() {
        manageThrust();
//        mInputManager->getMouseDelta(mMouseXd,mMouseYd);
        managePitch();
        mShip->update();
    }


};


class CRollThrustShipController:public CPlayerControlledShipController {

    public:
    CRollThrustShipController( CShip* ship ):CPlayerControlledShipController( ship ) {};

    void run() {
        manageThrust();
//        mInputManager->getMouseDelta(mMouseXd,mMouseYd);
        manageRoll();
        mShip->update();
    };


};


class CThrustOnlyShipController: public CPlayerControlledShipController {

    public:

    CThrustOnlyShipController( CShip* ship ): CPlayerControlledShipController( ship ) {};

    void run() {
        manageThrust();
        mShip->update();
    };


};


#endif


