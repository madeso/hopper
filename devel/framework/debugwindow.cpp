
#include "debugwindow.h"

#include "logger.h"

CDebugWindow::CDebugWindow( unsigned int nblines ) {

    mGUI = DynaGUI::getInstance();
    mWindow=mGUI->createWindow( DWindow::MIN, DWindow::CENTERED, "Debug" );
    mWindow->newLine();

    LOG("\n CDebugWindow : mLines.size() = %d ", mLines.size() );

    addLines( nblines );

    LOG("\n CDebugWindow : mLines.size() = %d ", mLines.size() );

}


CDebugWindow::~CDebugWindow() {

    mGUI->destroyWindow( mWindow );

}


void CDebugWindow::addLines( unsigned int nblines ) {

    int s=mLines.size();

    if( nblines > s ) {
        mLines.resize( nblines, 0 );
        int newsize=mLines.size();
        for( int i=s; i<newsize; i++ ) {
            mLines[i]=mWindow->addText( "...", 200 );
            mWindow->newLine();
        }
    }

}


void CDebugWindow::setText( unsigned int line, Ogre::String text ) {

    if( line<mLines.size() ) {
        mLines[line]->setText( text );
    }

}
