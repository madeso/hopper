

#ifndef _CVIDEOCONFIGWINDOW_
#define _CVIDEOCONFIGWINDOW_

//#include "guimanager.h"
#include "dynagui.h"

class CVideoConfigWindow:public DynaGUIButtonListener {

    public:
    CVideoConfigWindow();
    ~CVideoConfigWindow();
    bool done();
    void onButtonPress( DButton* b);

    private:
    DynaGUI* mGUI;
    DWindow* mWindow;
    DButton* mToggleFullscreenButton;

    DButton* mPreviousResolutionButton;
    DText* mResolutionText;
    DButton* mNextResolutionButton;
    DButton* mSwitchResolution;

    Ogre::String mCurrentResolution;
    Ogre::StringVector mPossibleResolutions;

    DButton* mOKButton;

    int mResolutionIndex;
    int mScreenWidth;
    int mScreenHeight;

    bool mDone;

};


#endif

