

#ifndef _DEBUGWINDOW_
#define _DEBUGWINDOW_

//#include "guimanager.h"
#include "dynagui.h"

class CDebugWindow {

    public:
    // CDebugWindow();
    CDebugWindow(unsigned int nblines=1);
    ~CDebugWindow();

    void addLines( unsigned int nbLines );
    void setText( unsigned int line, Ogre::String text );

    private:
    std::vector<DText*> mLines;
    DynaGUI* mGUI;
    DWindow* mWindow;

};


#endif
