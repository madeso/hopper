


#include "PlayerControlledShipControllers.h"

#include "gameconstants.h"

#include "logger.h"

CPlayerControlledShipController::CPlayerControlledShipController( CShip* ship): CShipController( ship ) {

    mInputManager=CInputManager::getInstance();

    mTimeManager=CTimeManager::getInstance();

}


void CPlayerControlledShipController::manageYaw() {
    mShip->yaw( (float)mInputManager->getAxisDelta( IMA_YAW )/50.f * mTimeManager->getTimeFactor() );
}


void CPlayerControlledShipController::managePitch() {
    // send the delta pitch in radians :
    mShip->pitch( mInputManager->getAxisDelta( IMA_PITCH )/200.f * mTimeManager->getTimeFactor());
}


void CPlayerControlledShipController::manageRoll() {
    // send the delta roll in radians :
    mShip->roll( -mInputManager->getAxisDelta( IMA_ROLL )/200.f * mTimeManager->getTimeFactor());
}


void CPlayerControlledShipController::manageThrust() {

    if( mInputManager->isRealAxis( IMP_THRUST ) ) {
        mShip->thrust( (float)mInputManager->getAxisValue( IMP_THRUST )/100.f );
    } else {
        mShip->thrust( (bool)mInputManager->isPressed(IMP_THRUST) );
    }


}

void CPlayerControlledShipController::manageFuelConsumption() {
    if( mShip->isThrusting() )
        mShip->consumeFuel( FUEL_CONSUMPTION_NORMAL * mTimeManager->getTimeFactor() );
}


void CPlayerControlledShipController::manageRecoverMode() {

    if(mInputManager->isPressed(IMP_RECOVER )) {
        mShip->recover();
    }

}

void CPlayerControlledShipController::manageFire() {

    if( mInputManager->isPressed( IMR_FIRE )) {
        mShip->fire();
    }
}


void CPlayerControlledShipController::manageTractorBeam() {

    if( mInputManager->isKeyPressed( OIS::KC_T )) {
        mShip->engageTractorBeam();
    }
    else {
        mShip->disengageTractorBeam();
    }

    if( mInputManager->isKeyPressed( OIS::KC_R )) {
        mShip->releaseTractorBeam();
    }
}


void CPlayerControlledShipController::run() {

    manageYaw();

    managePitch();
    manageRoll();


    manageThrust();
    manageFuelConsumption();

    manageRecoverMode();

    manageFire();
    manageTractorBeam();

    mShip->update();

}

