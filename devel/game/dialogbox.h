
#ifndef _CDIALOGBOX_
#define _CDIALOGBOX_


#include "dynagui.h"

class CDialogBox:public DynaGUIButtonListener {

    public:
    CDialogBox( Ogre::String title );
    ~CDialogBox();

    void addLine(Ogre::String text );
    bool done();
    void addOKButton( Ogre::String caption );
    void onButtonPress( DButton* b );

    private:
    DynaGUI* mGUI;
    DWindow* mWindow;
    DButton* mOKButton;
    bool mDone;

};


#endif

