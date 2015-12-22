
#ifndef _SPLASH_
#define _SPLASH_


#include "Ogre.h"
// #include "OgreTextAreaOverlayElement.h"

// #include "camera.h"
// #include "scenemanager.h"


class Splash {

    public:
    Splash();
    ~Splash();

    private:
    Ogre::Camera* mCam;
    Ogre::Overlay* mOverlay;
    Ogre::OverlayContainer* o;
    Ogre::SceneManager* mSM;

};


#endif

