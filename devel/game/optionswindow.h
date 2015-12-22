
#ifndef _INTERFACEOPTIONS_
#define _INTERFACEOPTIONS_

#include "dynagui.h"
#include "videoconfigwindow.h"
#include "soundconfigwindow.h"
#include "inputconfigwindow.h"


class COptionsWindow:public DynaGUIButtonListener {

    public:
    COptionsWindow();
    ~COptionsWindow();

    void onButtonPress( DButton* b );

    enum OPT_WINDOW { RUNNING, VIDEO, SOUND, INPUT, GAMEPLAY, DONE };

    bool done();

    private:
    DynaGUI* mGUI;
    DWindow* mWindow;

    DButton* mOK;

    DButton* mVideoButton;
    DButton* mSoundButton;
    DButton* mInputButton;
    DButton* mGameplayButton;

    CVideoConfigWindow* mVideoWindow;
    CSoundConfigWindow* mSoundWindow;
    CInputConfigWindow* mInputWindow;
//    BetaGUI::Window* mGameplayWindow;


    int mStatus;




};


#endif

