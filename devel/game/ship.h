


#ifndef _SHIP_
#define _SHIP_

#include "physicobject.h"
#include "OgreOde_Core.h"
#include "thruster.h"
#include "positionalthruster.h"

#include "TractorBeam.h"


/// main controllable actor

class CShip: public CDynamicPhysTrimesh {

public:
    enum ship_status { STATUS_OK, STATUS_FROZEN, STATUS_DEAD };

    /*
    enum ship_ability { NO_THRUST=1, NO_ORIENTATION=2, NO_DAMAGE=4, NO_FUEL_CONSUMPTION=8, NO_CANNON=16 };
    enum ship_status {
        FROZEN = NO_THRUST | NO_ORIENTATION,
        COLLIDING = NO_ORIENTATION,
        GOD = NO_DAMAGE | NO_FUEL_CONSUMPTION,
        BROKEN = NO_THRUST | NO_ORIENTATION | NO_DAMAGE | NO_FUEL_CONSUMPTION | NO_CANNON,
        OUT_OF_FUEL = NO_THRUST | NO_ORIENTATION | NO_FUEL_CONSUMPTION
    };
    */


    CShip(Ogre::String name, Ogre::Vector3 pos );
    ~CShip();

    void yaw(float aYaw);
    void pitch(float aPitch);
    void roll(float aRoll);

    /// overloaded from CActor
    void setYaw( float aYaw );

    /// overloaded from CActor
    void setPitch( float aPitch);

    /// overloaded from CActor
    void setRoll( float aRoll );

    void update();

    void recover();
    void addForces(float time);

    void thrust(bool thrusting );
    void thrust( float power );

    bool isUp();
    bool isIdle();
    bool isThrusting();

    void orient();

    void onCollision( float damage );
    float getHullCondition();
    float getFuel();
    void refuel( float fuel);
    void consumeFuel( float aFuel );
    void setGameOverMode();
    void explode();
    void resetOrientation();

    Ogre::AxisAlignedBox mAABB;
    Ogre::AxisAlignedBox* getAABB();

    Ogre::Quaternion getPitchRollOrientation();

    void setGodMode( bool god );

    void freeze();
    void unfreeze();
    bool isFrozen();

    void fire();
    void engageTractorBeam();
    void disengageTractorBeam();
    void releaseTractorBeam();

    Ogre::Vector3 getTestPointPosition();
    Ogre::Vector3 getProjectileSpawnPosition();

    int getStatus();

protected:
        Ogre::Quaternion curQ, aimQ, Q1,Q2,Q3;

private:
    CThruster* mMainThruster;

    float mHull;
    float mFuel;

    float forceYaw, forcePitch, forceRoll;

    bool mRecoverMode;

    int mColliding;

    bool mAnounceFuel;
    bool mAnounceHull;

    bool mGodMode;
    int mStatus;

    bool mThrusting;

    float impulseYaw;
    float impulsePitch;
    float impulseRoll;

    Ogre::Vector3 AimYaw; // a point in front of the ship. the position we're aiming for
    Ogre::Vector3 CurYaw; // a point in front of the ship. current position.

    Ogre::Vector3 AimPitch;
    Ogre::Vector3 CurPitch;

    Ogre::Vector3 AimRoll;
    Ogre::Vector3 CurRoll;

    float mYawPs;
    float mPitchPs;
    float mRollPs;

    CPositionalThruster* mFrontLeftPositionalThruster;
    CPositionalThruster* mFrontRightPositionalThruster;
    CPositionalThruster* mRearLeftPositionalThruster;
    CPositionalThruster* mRearRightPositionalThruster;

    // Ogre::SceneNode* mProjectileSpawnNode;
    Ogre::SceneNode* mLandTestNode;
    Ogre::Entity* mLandTestDebug;

    CTractorBeam* mTractorBeam;
    // Ogre::SceneNode* mTractorBeamNode;

    Ogre::SceneNode* mCanonPos;
    CDelay* mFireDelay;

    bool mFrozen;

};


#endif
