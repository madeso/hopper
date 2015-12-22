

#ifndef _NEGAMEWINDOW_
#define _NEGAMEWINDOW_


#include "dynagui.h"
#include "gamepart.h"

class CNewGameWindow: DynaGUIButtonListener {

public:
    CNewGameWindow();
    ~CNewGameWindow();

    bool done();
    int getSelection();

    void onButtonPress( DButton* b );

private:
    bool mDone;
    int mSelection;

    DWindow* mWindow;

    DButton* mTutorialButton;
    DButton* mLandingButton;
    DButton* mRacingButton;
    DButton* mCancelButton;


};



#endif


