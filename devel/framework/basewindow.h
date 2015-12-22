

#ifndef _CBASEWINDOW_
#define _CBASEWINDOW_

// #include "guimanager.h"
#include "DynaGUI.h"

class CBaseWindow: public DynaGUIButtonListener {

    public:
    CBaseWindow( Ogre::String caption ) {

        mGUI= DynaGUI::getInstance();
        mWindow = mGUI->createWindow( DWindow::CENTERED, DWindow::CENTERED, caption );
        mWindow->newLine();

        mDone=false;

    }

    ~CBaseWindow() {
        mGUI->destroyWindow( mWindow );
    }

    bool done() {
        return( mDone );
    }

    void onButtonPress( DButton* b ) {
    }

    protected:
    DynaGUI* mGUI;
    DWindow* mWindow;
    DButton* mOKButton;
    bool mDone;



};

#endif


