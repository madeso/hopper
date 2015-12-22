

#ifndef _MENU_H_
#define _MENU_H_

#include "gamepart.h"

#include "game.h"

#include "level.h"
// #include "interfacemenu.h"
#include "scenemanager.h"
#include "inputmanager.h"
#include "timemanager.h"

#include "AIShipControllers.h"

#include "OgreOde_Core.h"

#include "optionswindow.h"
#include "debugwindow.h"
// #include "fpswindow.h"
#include "NewGameWindow.h"

#include "PoweredByWindow.h"

#include "CameraControllers.h"

/// The game menu.


class CMenu:public CGame, public DynaGUIButtonListener {

    public:

    CMenu();
    ~CMenu();

    int run();

    void gameplay();

    void onButtonPress( DButton* b );


    private:

    float mAlpha, mCamx, mCamy, mCamz;

    DynaGUI* mGUI;
    DWindow* mWindow;
    DWindow* mLogo;

    DButton* mButtonEditor;
    DButton* mButtonQuit;
    DButton* mButtonOptions;
    DButton* mButtonNewGame;

    COptionsWindow* mOptionsUI;
    CNewGameWindow* mNewGameWindow;
    CPoweredByWindow* mPoweredByWindow;


    int mReturnState;

    CLandingShipController* mAIship;

    float mMouseX, mMouseY;
    int mScreenWidth, mScreenHeight;

    void debugOverlays();

    CDelay* mCamDelay;
    int mCamMode;
    CCameraController* mCamController;


    CDebugWindow* mDebugWin;

    void makeUI();

    bool mBypassInput;



};


#endif

