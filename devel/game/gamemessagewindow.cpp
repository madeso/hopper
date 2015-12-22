

#include "gamemessagewindow.h"


CGameMessageWindow::CGameMessageWindow( int timeout ):
 mTimeOut( timeout ) {

    mGUI=DynaGUI::getInstance();
    mWindow=mGUI->createWindow( DWindow::CENTERED, DWindow::MIN );
    mMessage=mWindow->addText( "", 100 );

    mWindow->setPosition( 350, 200 );

    mWindow->hide();

    mHideDelay = new CDelay( timeout );
}



CGameMessageWindow::~CGameMessageWindow() {

    mGUI->destroyWindow( mWindow );
    delete(mHideDelay);
}


void CGameMessageWindow::setMessage( std::string message ) {


    mMessage->setText( message );
    mHideDelay->restart();
    show();

}


void CGameMessageWindow::show() {

    mWindow->show();

}

void CGameMessageWindow::hide() {

    mWindow->hide();

}

void CGameMessageWindow::update() {

    if( mTimeOut > 0 ) {
        if( mHideDelay->isOver() ) {
            hide();
        }
    }

}
