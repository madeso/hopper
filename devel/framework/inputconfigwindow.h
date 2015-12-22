


#ifndef _CINPUTCONFIGWINDOW_
#define _CINPUTCONFIGWINDOW_

#include "dynagui.h"
#include "inputconfigurationmanager.h"
#include "inputmanager.h"

#include "delay.h"
#include <string>

class CWaitForInputWindow {

    public:
    CWaitForInputWindow( CInputConfiguration::SInputActionConfig& config );
    ~CWaitForInputWindow();
    bool done();
    bool cancelled();
    CInputConfiguration::SInputActionConfig& getInputActionConfig();


    private:
    DynaGUI* mGUI;
    DWindow* mWindow;
    CInputManager* mInputManager;

    DText* mLine1;
    DText* mLine2;
    DText* mLine3;

    CInputConfiguration::SInputActionConfig mInputActionConfig;

    CDelay* mInputDelay;

    enum { CHECK_PRIMARY, CHECK_SECONDARY, WAIT_BEFORE_SECONDARY, DO_CHECK_SECONDARY, DONE, CANCELLED, FINISHED };

    int mStatus;
    bool mDone;
    bool mCancelled;

    void checkInput();
    void checkForSecondaryInput();
    Ogre::String methodName( bool secondary );

};





class CInputConfigWindow:public DynaGUIButtonListener {

public:
    CInputConfigWindow();
    ~CInputConfigWindow();

    void onButtonPress(DButton* b);
    bool done();

private:

    DynaGUI* mGUI;

    DWindow* mWindow;
    CInputConfiguration* mConfig;

    std::vector<CInputConfiguration::SInputActionConfig> mInputActionConfig;
    CInputManager* mInputManager;
    int mNumberOfActions;


    bool mDone;

    DButton* mOKButton;
    std::vector<DButton*> mRedefineButton;
    std::vector<DCheckbox*> mAxisModeButton;  // should be a DCheckBox
    std::vector<DCheckbox*> mAxisReversedButton;  // shoud be a DCheckBox
    std::vector<DText*> mMethodName;


    DButton* mNextConfigButton;
    DButton* mPreviousConfigButton;

    DText* mConfignameText;

    bool mWaitForInput;
    int mWaitForInputAction;
    CWaitForInputWindow* mWaitForInputWindow;

    int mConfigMax;
    int mCurrentConfig;

    void buildWindow();
    void updateWindow();
    void initWaitForInput( int action );
    void checkInput();
    void checkForSecondaryInput( int action );
    Ogre::String methodName( int action ); // , std::string& method );
    void applyConfig();
    void loadCurrentConfig();

/*
    int mMouseSensitivity;
    BetaGUI::Button* mMouseSensitivityUp;
    BetaGUI::Button* mMouseSensitivityDown;
    BetaGUI::Progressbar* mMouseSensitivityBar;
    BetaGUI::Text* mMouseSensitivityText;
*/

};



#endif

