

#include "CameraControllers.h"
#include "logger.h"


CTerrainCameraController::CTerrainCameraController( CCamera* cam ): CCameraController( cam ) {

    mSceneMgr = CSceneManager::getInstance();

}


void CTerrainCameraController::terrainCheck( Ogre::Vector3& pos ) {

    float height = mSceneMgr->getHeightAt( pos.x, pos.z ) + 1.f;

    if( pos.y < height ) pos.y = height;

}


// -----------------------------------------------------------------------------

CRotateAroundCameraController::CRotateAroundCameraController(
    CCamera* camera, CActor* actor, int speed, float pitch, float dist )
    :CCameraController( camera ) {

    mActor=actor;
    mSpeed=speed;
    mYaw=mActor->getYaw();
    mPitch=pitch;
    mDistance=dist;


}


void CRotateAroundCameraController::run() {

    mYaw += (float)mSpeed/10.f*M_PI/180.f * mTimeManager->getTimeFactor();

    mCamera->setPosition( mActor->getPosition()
        + Ogre::Vector3(
        mDistance * sin( mYaw ) * sin( mPitch ),
        mDistance * cos( mPitch),
        mDistance * cos(mYaw) * sin(mPitch) )
    );
    mCamera->lookAt( mActor->getPosition() );


}



// -----------------------------------------------------------------------------
#define BEHIND_DISTANCE 10.f

CBehindCameraController::CBehindCameraController( CCamera* cam, CActor* actor ): CTerrainCameraController( cam ) {

    mActor=actor;

    mYaw=mActor->getYaw();
    mSetPitch=mPitch=70.0*M_PI/180.0;
    //mDistance=BEHIND_DISTANCE;
    mActorPos = actor->getPosition();

    mCamPos = mActorPos + Ogre::Vector3(
        BEHIND_DISTANCE * sin( mYaw ) * sin( mPitch ),
        BEHIND_DISTANCE * cos( mPitch),
        BEHIND_DISTANCE * cos(mYaw) * sin(mPitch)
    );

    mPitchPos[0]=M_PI/8.f;
    mPitchPos[1]=3.f*M_PI/8.f;
    mPitchPos[2]=M_PI/2.f;
    mPitchPos[3]=5.f*M_PI/8.f;
    mPitchPos[4]=7.f*M_PI/8;

    mCurrentPitchPos=1;
    mSetPitch=mPitchPos[mCurrentPitchPos];

    mInputManager=CInputManager::getInstance();


}



#include "hopperinputactions.h"

void CBehindCameraController::run() {

    //float tf=mTimeManager->getTimeFactor();

    if( mInputManager->isReleased( IMR_CAMPITCH_UP ) ) {
        pitchUp();
    } else {
        if( mInputManager->isReleased( IMR_CAMPITCH_DOWN ) ) {
            pitchDown();
        }
    }


    mYaw=mYaw + ( mActor->getYaw() - mYaw ) / 50.f;
    mPitch=mPitch + ( mSetPitch - mPitch ) / 50.f;


    mCamPos= mActorPos + Ogre::Vector3(
        BEHIND_DISTANCE * sin( mYaw ) * sin( mPitch ),
        BEHIND_DISTANCE * cos( mPitch ),
        BEHIND_DISTANCE * cos( mYaw ) * sin( mPitch )
    );


    terrainCheck( mCamPos );

    mCamera->setPosition( mCamPos );
    mCamera->lookAt( mActorPos );

    mActorPos = mActorPos + ( mActor->getPosition() - mActorPos ) / 10.f;



}
#undef BEHIND_DISTANCE

/*
void CBehindCameraController::pitch( float aPitch ) {

    mSetPitch -= aPitch;
    if( mSetPitch < BEHINDCAM_MIN_PITCH ) mSetPitch=BEHINDCAM_MIN_PITCH;
    if( mSetPitch > BEHINDCAM_MAX_PITCH ) mSetPitch=BEHINDCAM_MAX_PITCH;

    // LOG("\nBehindCam pitch = %f°", mSetPitch*180.f/M_PI );

}
*/

void CBehindCameraController::pitchUp() {
    if( mCurrentPitchPos>0 ) {
        mCurrentPitchPos--;
        mSetPitch=mPitchPos[mCurrentPitchPos];
    }
}

void CBehindCameraController::pitchDown() {
    if( mCurrentPitchPos < (BEHINDCAM_MAX_PITCH_POS-1) ) {
        mCurrentPitchPos++;
        mSetPitch=mPitchPos[mCurrentPitchPos];
    }
}

// -----------------------------------------------------------------------------


CTrackCameraController::CTrackCameraController( CCamera* camera ):CTerrainCameraController( camera ) {
    mIndex = 0;
    mTime = 0;
}

void CTrackCameraController::addKey( Ogre::Vector3 position, Ogre::Vector3 lookAt ) {
    SKey k;
    k.position = position;
    k.lookAt = lookAt;
    mKeys.push_back( k );


}


float CTrackCameraController::doCatmull( float p1, float p2, float p3, float p4, float t ) {
   return( 0.5 * ((-p1 + 3*p2 -3*p3 + p4)*t*t*t
       + (2*p1 -5*p2 + 4*p3 - p4)*t*t
       + (-p1+p3)*t
       + 2*p2) );
}

CTrackCameraController::SKey& CTrackCameraController::getKey( int index ) {
    if( index < 0 ) index = mKeys.size() - index;
    if( index > mKeys.size()-1 ) index = index - mKeys.size();

    return( mKeys[ index ] );
}

void CTrackCameraController::run() {

    // LOG("\nCTrackCameraController::run()");

    Ogre::Vector3 position;
    Ogre::Vector3 lookAt;

    SKey& k1 = getKey( mIndex - 2 );
    SKey& k2 = getKey( mIndex - 1 );
    SKey& k3 = getKey( mIndex );
    SKey& k4 = getKey( mIndex + 1 );

    position.x = doCatmull ( k1.position.x, k2.position.x, k3.position.x, k4.position.x, mTime );
    position.y = doCatmull ( k1.position.y, k2.position.y, k3.position.y, k4.position.y, mTime );
    position.z = doCatmull ( k1.position.z, k2.position.z, k3.position.z, k4.position.z, mTime );
    lookAt.x = doCatmull ( k1.lookAt.x, k2.lookAt.x, k3.lookAt.x, k4.lookAt.x, mTime );
    lookAt.y = doCatmull ( k1.lookAt.y, k2.lookAt.y, k3.lookAt.y, k4.lookAt.y, mTime );
    lookAt.z = doCatmull ( k1.lookAt.z, k2.lookAt.z, k3.lookAt.z, k4.lookAt.z, mTime );

    mTime += CTimeManager::getInstance()->getFrametime()/3000.f;
    if( mTime > 1.f ) {
        mIndex++;
        mTime = 0;
        LOG("\nCTrackCameraController::run() index = %d / %d", mIndex, mKeys.size() );
    }
    if( mIndex > mKeys.size()-1 ) mIndex = 0;


    terrainCheck( position );

    mCamera->setPosition( position );
    mCamera->lookAt( lookAt );

}


#include "hopperinputactions.h"

CEditorCameraController::CEditorCameraController( CCamera* camera ): CCameraController( camera)  {

    mInputManager=CInputManager::getInstance();

}


void CEditorCameraController::run() {

    if( mInputManager->isPressed( IMP_EDITOR_ACCEL )) {
        mCamTranslateScale=10;
    }
    else {
        mCamTranslateScale=1;
    }
    mCamTranslateScale *= mTimeManager->getTimeFactor();

    mCamTranslate=Ogre::Vector3(0,0,0);

    if( mInputManager->isPressed( IMP_FORWARD ) ) {
        // mCamera->moveForward();
        mCamTranslate+=Ogre::Vector3( 0,0, -mCamTranslateScale );
    }
    if( mInputManager->isPressed( IMP_BACKWARD ) ) {
        mCamTranslate+=Ogre::Vector3( 0,0, mCamTranslateScale );
      //  mCamera->moveBackward();
    }
    if( mInputManager->isPressed( IMP_LEFT ) ) {
        mCamTranslate+=Ogre::Vector3( -mCamTranslateScale, 0, 0 );
      //  mCamera->moveLeft();
    }
    if( mInputManager->isPressed( IMP_RIGHT ) ) {
        mCamTranslate+=Ogre::Vector3( mCamTranslateScale, 0, 0 );
      //  mCamera->moveRight();
    }

    float mouseXd, mouseYd;

    mInputManager->getMouseDelta(mouseXd,mouseYd);

    mCamera->yaw( -mouseXd * mTimeManager->getTimeFactor()/5.f);
    mCamera->pitch( -mouseYd * mTimeManager->getTimeFactor()/5.f);


    translate(mCamTranslate);

}


void CEditorCameraController::translate(Ogre::Vector3 translate ) {

    Ogre::Quaternion q=mCamera->getOrientation();

    mCamera->moveRelative( translate*mTimeManager->getTimeFactor() );

    Ogre::Vector3 dir(0,0,-1);
    dir=q*dir;

    mCamera->lookAt( mCamera->getPosition() + dir );


}


Ogre::Ray CEditorCameraController::getCameraToViewportRay( Ogre::Vector2 v ) {

    return( mCamera->getCameraToViewportRay( v ));

}


// -----------------------------------------------------------------------------




