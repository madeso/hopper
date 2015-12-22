
#include "dialogbox.h"

CDialogBox::CDialogBox(Ogre::String title) {

    mGUI=DynaGUI::getInstance();
    mWindow=mGUI->createWindow( DWindow::CENTERED, DWindow::CENTERED, title );
    mWindow->newLine();

    mOKButton=0;
    mDone=false;

}

CDialogBox::~CDialogBox() {
    mGUI->destroyWindow( mWindow);
}

void CDialogBox::addLine( Ogre::String line ) {
    mWindow->addText( line, line.length()*10 );
    mWindow->newLine();
}

void CDialogBox::addOKButton( Ogre::String caption ) {
    if( !mOKButton )
        mOKButton=mWindow->addButton( caption,caption.length()*20, this );
}

bool CDialogBox::done() {
    return( mDone );
}

void CDialogBox::onButtonPress( DButton* b) {

    if(b==mOKButton ) {
        mDone=true;
    }

}
