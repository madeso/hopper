

#include "ship.h"
#include "scenemanager.h"
#include "logger.h"
#include "gameconstants.h"
#include "hoppersounds.h"
#include "soundmanager.h"
#include "projectilemanager.h"

CShip::CShip( Ogre::String name, Ogre::Vector3 pos )
    :CDynamicPhysTrimesh( pos, 1, 2.0, 1.0, name, "hoper11.mesh") {

    //LOG("\n =======> %s has skeleton : %d", mEntity->getName().c_str() , mEntity->hasSkeleton() );

    LOG("\nCShip::CShip() ");

    castShadows();

    setLinearDamping(LINEAR_DAMPING);

    setAngularDamping(ANGULAR_DAMPING);

    setAutoSleep(false);


    // =============== debugging ===================
//    mPhysBody->setAffectedByGravity(false);
    // =============== debugging ===================


    forceYaw=0;
    forcePitch=0;
    forceRoll=0;


    mRecoverMode=false;
    mGodMode=false;
    mStatus=0;
    mFrozen=false;

    mAnounceFuel=false;
    mAnounceHull=false;

    mHull=100.0;
    mFuel=100.0;

    mFireDelay=new CDelay( 350 );

    // ----------------------------------------------------------------------------
    LOG("\nCShip::CShip() : creating thrusters");
    mMainThruster=new CThruster( mNode, name + "_mainthruster", Ogre::Vector3( 0,0,0 ) );


    //TODO : find a better way to give a position to these minithrusters !! ( look at tag nodes ?)
    mFrontLeftPositionalThruster=new CPositionalThruster(
        mNode, name + "_positionalfrontleft", Ogre::Vector3(-0.8,0.5,-0.7),
        CPositionalThruster::FRONTLEFT );

    mFrontRightPositionalThruster=new CPositionalThruster(
        mNode, name + "_positionalfrontright", Ogre::Vector3( 0.8,0.5,-0.7),
        CPositionalThruster::FRONTRIGHT );

    mRearLeftPositionalThruster=new CPositionalThruster(
        mNode, name + "_positionalrearleft", Ogre::Vector3( -0.8,0.6,1.4 ),
        CPositionalThruster::REARLEFT );

    mRearRightPositionalThruster=new CPositionalThruster(
        mNode, name + "_positionalrearright", Ogre::Vector3(0.8,0.6,1.4),
        CPositionalThruster::REARRIGHT );

    LOG("ok.");

    mLandTestNode=getNode()->createChildSceneNode(Ogre::Vector3(0, mAABB.getMinimum().y-0.5f, 0 ));

    mCanonPos=getNode()->createChildSceneNode( Ogre::Vector3( 0, 0, -2.f ));

    // mTractorBeamNode=getNode()->createChildSceneNode(Ogre::Vector3(0, mAABB.getMinimum().y-0.5f, 0 ));
    mTractorBeam = new CTractorBeam( static_cast<CDynamicPhysActor*>(this),
        Ogre::Vector3(0, mAABB.getMinimum().y, 0 ) );

    mColliding = 0;

    LOG("\n =============== Ship : init done. ==========" );
}


CShip::~CShip() {


    delete( mMainThruster);

    delete( mFrontLeftPositionalThruster );
    delete( mFrontRightPositionalThruster );
    delete( mRearLeftPositionalThruster );
    delete( mRearRightPositionalThruster );


    delete( mFireDelay );

    getNode()->removeChild( mLandTestNode);
    getNode()->removeChild( mCanonPos );

    // getNode()->removeChild( mTractorBeamNode );
    delete( mTractorBeam );

    LOG("\n...done deleting CShip ");
}

Ogre::Vector3 CShip::getTestPointPosition() {
    return( getPosition() + mLandTestNode->getPosition() );
}


void CShip::thrust(bool thrusting ) {

    if( thrusting )
        thrust( 1.f );
    else
        thrust( 0.f );

}



void CShip::thrust( float power ) {

    if( mStatus != STATUS_DEAD ) {
        mMainThruster->setPower( power );
        mThrusting = (power>0);
    }
}


void CShip::setYaw( float aYaw ) {
    mYaw=aYaw;
}

void CShip::setPitch( float aPitch ) {
    mPitch=aPitch;
}

void CShip::setRoll( float aRoll ) {
    mRoll=aRoll;
}


// left / right orientation
void CShip::yaw( float aYaw) {
    mYaw+=aYaw;
}

// up / down orientation
void CShip::pitch( float aPitch) {
    mPitch+=aPitch;

    if(mPitch >  3.0/2.f ) mPitch= 3.0/2.f;
    if(mPitch < -3.0/2.f ) mPitch=-3.0/2.f;
}

// roll
void CShip::roll( float aRoll) {
    mRoll+=aRoll;

    if(mRoll >  3.0/2.f ) mRoll= 3.0/2.f;
    if(mRoll < -3.0/2.f ) mRoll=-3.0/2.f;


}

bool CShip::isUp() {
    Ogre::Vector3 v,w;
   // Ogre::Quaternion q;

    w=Ogre::Vector3(0,1,0);
    v=getOrientation().yAxis();

    return v.directionEquals( w, Ogre::Radian(3.1415f/180.0f) ) ;
}

bool CShip::isIdle() {
    return ( (getLinearVelocity().length() + getAngularVelocity().length() ) < 0.005 ) ;
}

bool CShip::isThrusting() {
    return( mThrusting );
}

void CShip::recover() {
    mRecoverMode=true;
}


Ogre::Quaternion CShip::getPitchRollOrientation() {
    return( Q1*Q2 );
}

void CShip::update() {

    if(mRecoverMode) {

        mPitch=0;
        mRoll=0;

        impulseYaw=ANGULAR_FORCE_RECOVER;
        impulsePitch=ANGULAR_FORCE_RECOVER*1.5f;
        impulseRoll=ANGULAR_FORCE_RECOVER;
    }
    else {

        impulseYaw=ANGULAR_FORCE;
        impulsePitch=ANGULAR_FORCE*1.5f;
        impulseRoll=ANGULAR_FORCE;
    }


    // compute the aimed-at orientation

    Q1.FromAngleAxis( Ogre::Radian(mRoll), Ogre::Vector3( 0,0,1) );
    Q2.FromAngleAxis ( Ogre::Radian(mPitch), Ogre::Vector3( 1,0,0) );
    Q3.FromAngleAxis ( Ogre::Radian(mYaw), Ogre::Vector3( 0,1,0) );
    aimQ=Q3*Q1*Q2;   // the order is important.
    aimQ.normalise();

    curQ=getOrientation();
    curQ.normalise();

    // take 3 points ( axes ) in the two different orientations ( current and aimed-at )
    // We really only need two points, but it's more consistent looking like that ! :)

    AimYaw=aimQ*Ogre::Vector3(0,0,1); // a point in front of the ship. we're aiming for it
    CurYaw=curQ*Ogre::Vector3(0,0,1); // the same point in its current  position

    AimPitch=aimQ*Ogre::Vector3(0,1,0);
    CurPitch=curQ*Ogre::Vector3(0,1,0);

    AimRoll=aimQ*Ogre::Vector3(1,0,0);
    CurRoll=curQ*Ogre::Vector3(1,0,0);


    if( ( mColliding || mStatus==STATUS_DEAD ) && !mRecoverMode ) {
        forcePitch=0;
        forceYaw=0;
        forceRoll=0;

        mFrontLeftPositionalThruster->cutPower();
        mFrontRightPositionalThruster->cutPower();
        mRearLeftPositionalThruster->cutPower();
        mRearRightPositionalThruster->cutPower();

    }
    else {



        static const Ogre::Radian tolerance( 2*3.1415928/180.0 );  // degrees of tolerance
        static const Ogre::Radian tolerance2( 1*3.1415928/180.0 );  // degrees of tolerance
        #define THRESHOLD 1.0*3.1415928/180.0
        // #define THRESHOLD 1.0*3.1415928/180


        // Yaw :
        // Find the yaw direction to go from the current position to the aimed-at position
        // CurRoll is used here as the normal of the carrefully chosen plane passing through the CurYaw point

        mYawPs=CurRoll.dotProduct( AimYaw );
        if( ( mYawPs>THRESHOLD || mYawPs<-THRESHOLD ) || ( AimYaw.directionEquals(-CurYaw,tolerance ) ) ) {
            forceYaw=mYawPs*impulseYaw;
        }
        else forceYaw=0;

        // Roll
        mRollPs=CurPitch.dotProduct( AimRoll );
        if( ( mRollPs>THRESHOLD || mRollPs<-THRESHOLD ) || ( AimRoll.directionEquals(-CurRoll, tolerance)  ) ) {
            forceRoll=mRollPs*impulseRoll;
        }
        else forceRoll=0;


        // Pitch
        mPitchPs=CurYaw.dotProduct( AimPitch );
        if( ( mPitchPs>THRESHOLD || mPitchPs<-THRESHOLD ) || ( AimPitch.directionEquals(-CurPitch, tolerance ) ) ) {
            forcePitch=mPitchPs*impulsePitch;
        }
        else forcePitch=0;


        // ==================================================================


        mFrontLeftPositionalThruster->setPower( forceYaw, forcePitch, forceRoll );
        mFrontRightPositionalThruster->setPower( forceYaw, forcePitch, forceRoll );
        mRearLeftPositionalThruster->setPower( forceYaw, forcePitch, forceRoll );
        mRearRightPositionalThruster->setPower( forceYaw, forcePitch, forceRoll );

    } // collision mode = no orientation

    mRecoverMode=false;
    //mColliding=false;
    mColliding--;
    if( mColliding < 0 ) mColliding = 0;
}


float CShip::getFuel() {
    return(mFuel);
}

void CShip::refuel(float aFuel) {
    mFuel+=aFuel;
    if(mFuel>100) mFuel=100;
    if( mFuel>15 ) mAnounceFuel=false;
}

void CShip::consumeFuel( float aFuel ) {

    if(mThrusting && mFuel>0 ) {
        mFuel=mFuel-aFuel;
    }

    if( !mAnounceFuel && mFuel<15 ) {
        CSoundManager::getInstance()->play2D( SOUND_FUEL_LOW, 1 );
        mAnounceFuel=true;
    }

    if( mFuel <= 0 && mStatus != STATUS_DEAD ) {
        CSoundManager::getInstance()->play2D( SOUND_OUTOFFUEL,1 );
        setGameOverMode();
    }

}

void CShip::fire() {

    if( mFireDelay->isOver() ) {
        mFireDelay->restart();
        CProjectileManager::getInstance()->createProjectile(
            //mCanonPos->getWorldPosition(),  // from Ogre 1.4.7 to Ogre 1.6.1 : no more getWorldPosition() !
            mCanonPos->_getDerivedPosition(),
            getOrientation().zAxis(), getLinearVelocity() );
    }

}

void CShip::engageTractorBeam() {
    mTractorBeam->setActive( true );
    mTractorBeam->update();
}

void CShip::disengageTractorBeam() {
    mTractorBeam->setActive( false );
}

void CShip::releaseTractorBeam() {
    mTractorBeam->unlink();
}

void CShip::onCollision( float aDamage ) {

/*
    if( aDamage > BIG_CONTACT_THRESHOLD ) {
        mHull -= aDamage * BIG_CONTACT_MULTIPLICATOR;
    }
    else {
         if( aDamage > SMALL_CONTACT_THRESHOLD ) {
            mHull -= aDamage * SMALL_CONTACT_MULTIPLICATOR;
         }
    }
*/
    //if( aDamage > 0.5 ) {
        mHull -= aDamage;
    //}

    if( !mGodMode && mStatus != STATUS_DEAD ) {

        if( mHull <= 0 ) {
            explode();

            setGameOverMode();
        }

        if( mHull < 15 && !mAnounceHull ) {
            CSoundManager::getInstance()->play2D( SOUND_HULL_CRITICAL,1 );
            mAnounceHull=true;
        }
    }

    mColliding=5;

}

int CShip::getStatus() {
    return mStatus;
}


void CShip::setGodMode( bool god ) {
    mGodMode = god;
}

float CShip::getHullCondition() {
    return( mHull );
}

void CShip::setGameOverMode() {

    mStatus=STATUS_DEAD;

    mMainThruster->setPower(0);

    setAngularDamping( 0.0 );
    forceYaw=0;
    forcePitch=0;
    forceRoll=0;
    mFrontLeftPositionalThruster->cutPower();
    mFrontRightPositionalThruster->cutPower();
    mRearLeftPositionalThruster->cutPower();
    mRearRightPositionalThruster->cutPower();
}

void CShip::explode() {
    Ogre::String s;
    CSoundManager::getInstance()->play3D( SOUND_EXPLODE, getPosition(), 1 );

    //LOG("\n Ship : # of sub-entities : %d", mEntity->getNumSubEntities() );
    for(int i=0; i<mEntity->getNumSubEntities(); i++ ) {
        s=mEntity->getSubEntity(i)->getMaterialName();
        // LOG("\n   Sub entity %d => material %s",i, s.c_str() );
        if(s.find('_') != std::string::npos )  s=s.erase( s.find('_'), s.length());
        s=s+ "_burnt";
        mEntity->getSubEntity(i)->setMaterialName( s );
    }
    mFrontLeftPositionalThruster->cutPower();
    mFrontRightPositionalThruster->cutPower();
    mRearLeftPositionalThruster->cutPower();
    mRearRightPositionalThruster->cutPower();


}

Ogre::AxisAlignedBox* CShip::getAABB() {
    Ogre::Vector3 vmin, vmax;

        mAABB=mEntity->getWorldBoundingBox(false);
        return( &mAABB );
}

void CShip::addForces(float timefactor) {

    float af=timefactor;
    float lf=timefactor;

    if( mStatus != STATUS_DEAD ) {

        //if( !mColliding ) {
            addRelativeTorque ( Ogre::Vector3( forcePitch*af, forceYaw*af, forceRoll*af ));
        //}
        //else
        //    mColliding=false;   // don't stay in collision mode

        if(mThrusting ) {
            addRelativeForce( Ogre::Vector3( 0,THRUST_FORCE*mMainThruster->getPower()*lf*0.6,0 ));
        }

    }

}


void CShip::orient() {

    Ogre::Quaternion Q1,Q2,Q3,Q;

    if ( !mColliding ) {

            Q1.FromAngleAxis( (Ogre::Radian)mRoll, Ogre::Vector3( 0,0,1) );

            Q2.FromAngleAxis ( (Ogre::Radian)mPitch, Ogre::Vector3( 1,0,0) );

            Q3.FromAngleAxis ( (Ogre::Radian)mYaw, Ogre::Vector3( 0,1,0) );

            Q=Q3*Q1*Q2;

            Q.normalise();
            setOrientation( Q );
    }
   //mCollisionMode=false;

}


void CShip::resetOrientation() {
    setOrientation( Ogre::Quaternion::IDENTITY );
    mYaw=0;
    mPitch=0;
    mRoll=0;
    setAngularVelocity( Ogre::Vector3(0,0,0) );
    setLinearVelocity( Ogre::Vector3(0,0,0) );
}


void CShip::freeze() {
    Ogre::String s;

    CSoundManager::getInstance()->play3D( SOUND_FREEZING, getPosition(), 1 );

    LOG("\n Ship : # of sub-entities : %d", mEntity->getNumSubEntities() );
    for(int i=0; i<mEntity->getNumSubEntities(); i++ ) {
        s=mEntity->getSubEntity(i)->getMaterialName();
        LOG("\n   Sub entity %d => material %s",i, s.c_str() );
        mEntity->getSubEntity(i)->setMaterialName(s+"_frozen");
    }

    forcePitch=0;
    forceYaw=0;
    forceRoll=0;

    mFrontLeftPositionalThruster->cutPower();
    mFrontRightPositionalThruster->cutPower();
    mRearLeftPositionalThruster->cutPower();
    mRearRightPositionalThruster->cutPower();

    setAngularDamping(1.0f);
    mFrozen=true;
}


void CShip::unfreeze() {
    std::string s;

    CSoundManager::getInstance()->play3D( SOUND_UNFREEZING, getPosition(), 1 );

    for(int i=0; i<mEntity->getNumSubEntities(); i++ ) {
        s=mEntity->getSubEntity(i)->getMaterialName();
        LOG("\n   Sub entity %d => material %s",i, s.c_str() );
        // mEntity->getSubEntity(i)->setMaterialName( s.erase( s.length()-7, s.length())  );
        if(s.find('_') != std::string::npos )  s=s.erase( s.find('_'), s.length());
        mEntity->getSubEntity(i)->setMaterialName( s );
    }
    mFrozen=false;
    setAngularDamping(ANGULAR_DAMPING);
}


bool CShip::isFrozen() {
    return(mFrozen);
}



