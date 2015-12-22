

#include "fpswindow.h"

#include "logger.h"

CFPSWindow::CFPSWindow() {

    mGUI = DynaGUI::getInstance();

    mWindow = mGUI->createWindow( DWindow::MAX, DWindow::MIN );

    mText = mWindow->addText( "999", 30 );

}


CFPSWindow::~CFPSWindow() {

    mGUI->destroyWindow( mWindow );

}


void CFPSWindow::setFPS( int fps ) {

    mText->setText( Ogre::StringConverter::toString( fps ) );
}
