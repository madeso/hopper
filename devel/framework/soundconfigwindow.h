

#ifndef _CSOUNDCONFIGWINDOW_
#define _CSOUNDCONFIGWINDOW_

#include "dynagui.h"

class CSoundConfigWindow:public DynaGUIButtonListener {

    public:
    CSoundConfigWindow();
    ~CSoundConfigWindow();
    bool done();
    void onButtonPress( DButton* b);

    private:
    DynaGUI* mGUI;
    DWindow* mWindow;
    DButton* mVolumeUp;
    DButton* mVolumeDown;
    DText* mVolumeText;
    DProgressBar* mVolumeBar;
    int mVolume;
    bool mDone;
    DButton* mOKButton;

};


#endif


