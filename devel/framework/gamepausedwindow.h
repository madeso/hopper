

#ifndef _CGAMEPAUSEDWINDOW_
#define _CGAMEPAUSEDWINDOW_

#include "basewindow.h"
#include "optionswindow.h"

class CGamePausedWindow: public CBaseWindow {

    public:

    CGamePausedWindow();
    ~CGamePausedWindow();

    bool done();
    bool returnToMenu();
    void onButtonPress( DButton* b );

    private:
    DButton* mMenuButton;
    DButton* mOptionsButton;

    COptionsWindow* mOptionsWindow;

    enum { WAITING, DO_OPTIONS, BACK_TO_MENU, FINISHED };
    int mStatus;
    bool mReturnToMenu;

};


#endif


