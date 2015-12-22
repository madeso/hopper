

#ifndef _CAMERACONTROLLERS_
#define _CAMERACONTROLLERS_


#include "camera.h"
#include "timemanager.h"
#include "actor.h"

class CCameraController {

    public:
    CCameraController( CCamera* cam ) {
        mCamera=cam;
        mTimeManager=CTimeManager::getInstance();
    }

    virtual ~CCameraController() {};

    virtual void run()=0;

    protected:
    CCamera* mCamera;
    CTimeManager* mTimeManager;

};



class CStaticCameraController:public CCameraController {

    public:
    CStaticCameraController( CCamera* cam, Ogre::Vector3 pos, Ogre::Vector3 lookat ): CCameraController( cam ) {

        mCamera->setPosition( pos );
        mCamera->lookAt( lookat );

    };

    void run() {};


};


#include "scenemanager.h"
class CTerrainCameraController: public CCameraController {

    public:
    CTerrainCameraController( CCamera* cam );

    void terrainCheck( Ogre::Vector3& pos );

    private:
    CSceneManager* mSceneMgr;

};


#include "inputmanager.h"

#define BEHINDCAM_MAX_PITCH_POS 5

class CBehindCameraController: public CTerrainCameraController {

    public:
    CBehindCameraController( CCamera* cam, CActor* actor  );

    void run();
//    void pitch( float aPitch );


    private:

    CActor* mActor;
    float mYaw;
    float mSetPitch, mPitch;
    float mPreviousDistance, mDistance;

    Ogre::Vector3 mCamPos;

    float mPitchPos[BEHINDCAM_MAX_PITCH_POS];
    int mCurrentPitchPos;
    CInputManager* mInputManager;

    void pitchUp();
    void pitchDown();

    Ogre::Vector3 mActorPos;  // for the laggy camera effect

};



class CFollowCameraController:public CCameraController {

    public:
    CFollowCameraController( CCamera* camera, CActor* actor, Ogre::Vector3 relpos ): CCameraController( camera ) {

        mRelPos=relpos;
        mActor=actor;
    }

    void run() {
        mCamera->setPosition( mActor->getPosition() + mRelPos );
        mCamera->lookAt( mActor->getPosition() );
    }

    private:
    Ogre::Vector3 mRelPos;
    CActor* mActor;

};


class CLookAtCameraController: public CCameraController {

    public:
    CLookAtCameraController( CCamera* camera, CActor* actor, Ogre::Vector3 pos ): CCameraController( camera ) {
        mActor=actor;
        mPosition=pos;
    }

    void run() {
        mCamera->setPosition( mPosition );
        mCamera->lookAt( mActor->getPosition() );
    }

    private:
    CActor* mActor;
    Ogre::Vector3 mPosition;

};



class CRotateAroundCameraController:public CCameraController {

    public:
    CRotateAroundCameraController( CCamera* camera, CActor* actor, int speed=1, float pitch=M_PI_2, float distance=10.f );
    void run();


    private:
    CActor* mActor;
    float mYaw;
    float mPitch;
    float mDistance;
    int mSpeed;


};


class CTrackCameraController: public CTerrainCameraController {

    public:
    CTrackCameraController( CCamera* camera );

    void run();
    void addKey( Ogre::Vector3 position, Ogre::Vector3 lookAt );

    private:
    struct SKey {
        Ogre::Vector3 position;
        Ogre::Vector3 lookAt;
    };

    float doCatmull( float p1, float p2, float p3, float p4, float t );
    CTrackCameraController::SKey& getKey( int index );

    std::vector< SKey > mKeys;
    int mIndex;
    float mTime;

};


class CEditorCameraController:public CCameraController {

    public:
    CEditorCameraController( CCamera* camera );

    void run();

    void translate(Ogre::Vector3 translate );
    Ogre::Ray getCameraToViewportRay( Ogre::Vector2 v );


    private:
    Ogre::Vector3 mCamTranslate;
    //float mMoveScale;
    float mCamTranslateScale;
    CInputManager* mInputManager;

};



#endif

