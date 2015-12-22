
#ifndef _SHIPORIENTATIONWINDOW_
#define _SHIPORIENTATIONWINDOW_

#include "dynagui.h"

class CShipOrientationWindow {

    public:
    CShipOrientationWindow();
    ~CShipOrientationWindow();

    void setOrientation(Ogre::Quaternion orientation );

    private:
    DynaGUI* mGUI;
    DWindow* mWindow;
    Ogre::SceneNode* mModelNode;
    Ogre::Entity* mModelEntity;


};


#endif
