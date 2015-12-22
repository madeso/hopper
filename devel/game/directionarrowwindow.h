
#ifndef _DIRECTIONARROWWINDOW_
#define _DIRECTIONARROWWINDOW_


#include "dynagui.h"

class CDirectionArrowWindow {

    public:
    CDirectionArrowWindow();
    ~CDirectionArrowWindow();

    void lookAt( Ogre::Vector3 lookat);

    private:
    DynaGUI* mGUI;
    DWindow* mWindow;
    Ogre::SceneNode* mModelNode;
    Ogre::Entity* mModelEntity;

};


#endif

