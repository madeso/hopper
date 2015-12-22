



#ifndef _CSCREEN_
#define _CSCREEN_

#include "Ogre.h"
#include "singleton.h"


/// Manages the render target.

class CScreen : public SSingleton<CScreen> {


    public:
    CScreen();
    ~CScreen();

    void render();
    float getFPS();
    void setDebugText(Ogre::String aString );
    void shot();
    Ogre::RenderWindow* getRenderWindow();
    Ogre::Root* getRoot();

    int getWidth();
    int getHeight();

    void repositionWindow();


    private:
    Ogre::RenderWindow *mRenderWindow;
    Ogre::Root *mRoot;

};



#endif
