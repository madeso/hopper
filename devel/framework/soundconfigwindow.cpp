

#include "soundconfigwindow.h"
#include "soundmanager.h"
#include "optionmanager.h"

/** @brief User interface for the sound configuration
  *
  */
CSoundConfigWindow::CSoundConfigWindow()
{


    if( ! COptionManager::getInstance()->getOption( "volume", mVolume ) ) {
       mVolume=5;
    }

    mGUI=DynaGUI::getInstance();
    mWindow=mGUI->createWindow( DWindow::CENTERED, DWindow::CENTERED, "Options");
    mWindow->newLine();

    mVolumeText=mWindow->addText( "Volume : " + Ogre::StringConverter::toString(mVolume*10) +"%", 100 );
    mWindow->newLine();
    mVolumeDown=mWindow->addButton( "<", 30, this );
    mVolumeBar=mWindow->addProgressBar( "Hoper/BasicProgressBar", 100 );
    mVolumeUp=mWindow->addButton( ">", 30, this );

    mVolumeBar->setPercent( mVolume*10 );

    mWindow->newLine();
    mWindow->newLine();
    mWindow->addSpace( 150 );
    mOKButton=mWindow->addButton( "OK", 60, this );


    mDone=false;
}


CSoundConfigWindow::~CSoundConfigWindow() {
    mGUI->destroyWindow(mWindow);
}

/** @brief returns true if the user clicked on the OK button
  *
  */
bool CSoundConfigWindow::done()
{
    return( mDone );
}


void CSoundConfigWindow::onButtonPress( DButton* b ) {

    if( b==mVolumeDown && mVolume>0 ) mVolume--;
    if( b==mVolumeUp && mVolume<10 ) mVolume++;
    mVolumeText->setText( "Volume : " + Ogre::StringConverter::toString( mVolume*10 ) + "%" );
    mVolumeBar->setPercent( mVolume*10 );
    CSoundManager::getInstance()->setVolume( mVolume/10.f );

    if( b==mOKButton ) {
        COptionManager::getInstance()->setOption( "volume", mVolume );
        mDone=true;
    }

}
