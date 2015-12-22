

#include "screen.h"

#include "logger.h"
#include "configfile.h"
#include "version.h"

//#define USE_OGRE_CONFIG_DIALOG


CScreen::CScreen() {

    LOG("\nScreen init..." );

    mRoot=new Ogre::Root();
    if( !mRoot ) throw "Fatal : no Ogre::Root !!";

    Ogre::String windowTitle;
    windowTitle = "Hopper ";
    windowTitle += AutoVersion::YEAR;
    windowTitle += AutoVersion::MONTH;
    windowTitle += AutoVersion::DATE;
    windowTitle += AutoVersion::STATUS;

#ifdef USE_OGRE_CONFIG_DIALOG
    if(!mRoot->restoreConfig()){            // try to read conf file
        if(!mRoot->showConfigDialog()){     // show config dialog if no conf file
            delete mRoot;

            throw "fatal : no Ogre::root !!";
        }
    }

    mRoot->initialise(false,"Hopper");

    mRenderWindow = mRoot->initialise(true, windowTitle );




#else

    Ogre::RenderSystem* renderSystem;

    Ogre::NameValuePairList miscOptions;

    Ogre::String value;
    int width, height;
    bool fullscreen;
    Ogre::String renderer;

/* Ogre.cfg
Render System=Direct3D9 Rendering Subsystem

[OpenGL Rendering Subsystem]
Colour Depth=32
Display Frequency=60
FSAA=0
Full Screen=Yes
RTT Preferred Mode=FBO
VSync=No
Video Mode=1024 x 768
sRGB Gamma Conversion=No

[Direct3D9 Rendering Subsystem]
Allow NVPerfHUD=No
Anti aliasing=None
Floating-point mode=Fastest
Full Screen=No
Rendering Device=NVIDIA GeForce GTX 260
VSync=Yes
Video Mode=1024 x 768 @ 32-bit colour
sRGB Gamma Conversion=No
*/


    LOG("\n\tloading and parsing hopper.conf..." );

    CConfigFile conf("Hopper.conf");

    if( !conf.load() ) {

        conf.setValue( "Renderer", "Direct3D" );
        conf.setValue( "ScreenWidth", "1024" );
        conf.setValue( "ScreenHeight", "768" );
        conf.setValue( "Fullscreen", "false" );
        conf.setValue( "FOV","60" );
        conf.setValue( "FSAA", "0" );
        conf.setValue( "MainVolume","90" );
        conf.setValue( "nointro", "true" );

        if( !conf.save() )
            throw( "Fatal : Can't write the configuration file !" );

    }

    if( !conf.load() ) {
        throw( "Fatal : Can't read the configuration file !" );
    }


    conf.getValue("Renderer", renderer );

    if( renderer == "OpenGL" ) {
        renderSystem = mRoot->getRenderSystemByName( "OpenGL Rendering Subsystem" );
            miscOptions["ColourDepth"]="32";
			miscOptions["DisplayFrequency"]="60";
			miscOptions["RTT Preferred Mode"]="FBO";
			miscOptions["sRGB Gamma Conversion"]="No";
			miscOptions["sRGB Gamma Conversion"]="No";
			miscOptions["vsync"]="Yes";
			if( conf.getValue( "FSAA", value ) )
				miscOptions["FSAA"]=value;
			else
				miscOptions["FSAA"]="0";
    }
    else {
    	renderSystem = mRoot->getRenderSystemByName( "Direct3D9 Rendering Subsystem" );
    	    miscOptions["Allow NVPerfHUD"]="No";
			miscOptions["Anti aliasing"]="None";
			miscOptions["Floating-point mode"]="Fastest";
			miscOptions["vsync"]="Yes";

			if( conf.getValue( "FSAA", value ) )
				miscOptions["FSAA"]=value;
			else
				miscOptions["FSAA"]="0";
    }

    if( renderSystem == NULL ) throw ("Can't find the rendering system");

    mRoot->setRenderSystem( renderSystem );


    if( !( conf.getValue("ScreenWidth", width ) && conf.getValue("ScreenHeight", height ))  ) {
        width=1024;
        height=768;
    }

    if( ! conf.getValue( "Fullscreen", value ))
        fullscreen=true;
    else {
        if( value == "false" || value == "0" ) fullscreen=false;
        else fullscreen=true;
    }



    mRoot->initialise( false, "Hopper" );
    mRenderWindow = mRoot->createRenderWindow( windowTitle, width, height, fullscreen, &miscOptions );

	//mRoot->getRenderSystem()->setWaitForVerticalBlank(true);

#endif



    LOG("\ndone.");

}





Ogre::RenderWindow* CScreen::getRenderWindow() {

    return(mRenderWindow);

}


void CScreen::repositionWindow() {

    int x = 0;
    int y = 0;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    x = ( GetSystemMetrics(SM_CXSCREEN) - mRenderWindow->getWidth() ) / 2 ;
    y = ( GetSystemMetrics(SM_CYSCREEN) - mRenderWindow->getHeight() ) / 2 ;
#endif

    mRenderWindow->reposition( x, y );
}


Ogre::Root* CScreen::getRoot() {
    return(mRoot);
}



CScreen::~CScreen() {
	delete mRoot;
}


void CScreen::render() {

    mRenderWindow->update();
    // mRoot->_updateAllRenderTargets(); // no repaint if another window on top
   // mRenderWindow->swapBuffers();

}


float CScreen::getFPS() {
    //return(mRenderWindow->getAverageFPS());
    return(mRenderWindow->getLastFPS());
}

/*
void CScreen::setDebugText(Ogre::String aString ) {
    mRenderWindow->setDebugText( aString );
}
*/

void CScreen::shot() {
    mRenderWindow->writeContentsToTimestampedFile( "../screenshots/shot", ".png" );
}

int CScreen::getWidth() {
    return( mRenderWindow->getWidth() );
}

int CScreen::getHeight() {
    return( mRenderWindow->getHeight() ) ;
}


