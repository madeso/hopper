

#ifndef _CCAMERA_
#define _CCAMERA_

#include "Ogre.h"
#include "singleton.h"
#include <list>


/// a somewhat simple camera


class CCamera : public SSingleton<CCamera> {


public:

    class Listener {

    public:
        // Listener() {};
        // virtual ~Listener()=0;
        virtual void onCameraMoved( Ogre::Vector3 pos )=0;
    };


    CCamera();
    ~CCamera();

    void lookAt( float x, float y, float z );
    void lookAt( Ogre::Vector3 pos);
    void setPosition( float x, float y, float z );
    void setPosition( Ogre::Vector3 pos);

    void moveRelative( Ogre::Vector3 vec);
    Ogre::Ray getCameraToViewportRay( Ogre::Vector2 v );

    Ogre::Vector3 getInFrontPosition(float distance );


    void yaw(float aYaw );
    void pitch( float aPitch );
    void roll( float aRoll );

    Ogre::Quaternion getOrientation();
    Ogre::Vector3 getPosition();


    void addListener( CCamera::Listener* listener );
    void removeListener( CCamera::Listener* listener );

    Ogre::Vector3 getVelocity();
    Ogre::Vector3 getDirection();


private:
    Ogre::Viewport* mViewport;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneManager;

    Ogre::Vector3 mPreviousPosition;

    void notifyListeners();
    std::list<CCamera::Listener*> mListeners;


};


#endif

