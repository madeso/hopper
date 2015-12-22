#include "Ogre.h"
// #include "actor.h"
#include "math.h"
#include "screen.h"
#include "camera.h"
#include "logger.h"

#include "scenemanager.h"
#include "timemanager.h"

CCamera::CCamera() {

    mCamera=CSceneManager::getInstance()->createCamera("Camera");

    mCamera->setNearClipDistance(0.1);

    mViewport=CScreen::getInstance()->getRenderWindow()->addViewport(mCamera);
    mViewport->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.3));

    mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth())/Ogre::Real(mViewport->getActualHeight()));

    mCamera->setFOVy(Ogre::Radian(60.0f*3.141592f/180.0f));


}

CCamera::~CCamera() {
    CSceneManager::getInstance()->destroyCamera(mCamera);

    for( std::list<CCamera::Listener*>::const_iterator i=mListeners.begin();
        i != mListeners.end(); ++i ) {
        mListeners.remove( (*i));
    }

}


void CCamera::addListener( CCamera::Listener* listener ) {
    mListeners.push_back( listener );
}


void CCamera::removeListener( CCamera::Listener* listener ) {
    mListeners.remove( listener );
}


void CCamera::notifyListeners() {

    for( std::list<CCamera::Listener*>::const_iterator i=mListeners.begin();
        i != mListeners.end(); ++i ) {
        (*i)->onCameraMoved( mCamera->getPosition() );
    }

}



void CCamera::yaw(float aYaw ) {
    mCamera->yaw( Ogre::Radian(aYaw) );
}

void CCamera::pitch(float aPitch) {
    mCamera->pitch( Ogre::Radian(aPitch));
}

void CCamera::roll( float aRoll ) {
    mCamera->roll( Ogre::Radian(aRoll) );
}


void CCamera::lookAt( float x, float y, float z ) {

    mCamera->lookAt(Ogre::Vector3(x,y,z));
}

void CCamera::lookAt( Ogre::Vector3 pos ) {
    mCamera->lookAt( pos );
}


Ogre::Quaternion CCamera::getOrientation() {
    return(mCamera->getOrientation());
}

Ogre::Vector3 CCamera::getPosition() {
    return( mCamera->getPosition() );
}


void CCamera::setPosition( float mX, float mY, float mZ ) {

    mPreviousPosition = mCamera->getPosition();

    mCamera->setPosition( mX, mY, mZ );

    notifyListeners();


}

void CCamera::setPosition( Ogre::Vector3 pos ) {

    mPreviousPosition = mCamera->getPosition();
    mCamera->setPosition( pos );
    notifyListeners();
}


void CCamera::moveRelative( Ogre::Vector3 vec ) {
    mPreviousPosition = mCamera->getPosition();
    mCamera->moveRelative( vec );
    notifyListeners();
}


Ogre::Ray CCamera::getCameraToViewportRay( Ogre::Vector2 v ) {

    return( mCamera->getCameraToViewportRay( v.x, v.y ) );
}

Ogre::Vector3 CCamera::getDirection() {
    return( mCamera->getDirection() );
}


Ogre::Vector3 CCamera::getInFrontPosition(float dist ) {
    Ogre::Quaternion q=mCamera->getOrientation();
    Ogre::Vector3 p( 0,0,-dist );
    p=q*p;
    p+=mCamera->getPosition();

    return(p);
}


Ogre::Vector3 CCamera::getVelocity() {

    float tf = CTimeManager::getInstance()->getTimeFactor();

    Ogre::Vector3 velocity = (mPreviousPosition - mCamera->getPosition()); // * tf;

    return( velocity );

}



