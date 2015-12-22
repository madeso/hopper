

#include "splash.h"
#include "screen.h"
// #include "inputmanager.h"
#include "logger.h"

Splash::Splash() {


    mSM=CScreen::getInstance()->getRoot()->createSceneManager(
            Ogre::ST_INTERIOR, "Splash Scene Manager");

    mCam=mSM->createCamera("splash_cam");
    CScreen::getInstance()->getRenderWindow()->addViewport(mCam);
    // mViewport->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.3));


    mOverlay=Ogre::OverlayManager::getSingleton().create("Splash");
    mOverlay->show();

        o=static_cast<Ogre::OverlayContainer*>
            (Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "splash_pic"));

        o->setMaterialName( "splash" );



        o->setMetricsMode(Ogre::GMM_RELATIVE_ASPECT_ADJUSTED);
//        o->setPosition(25,25);
        // o->setDimensions( 450,450 );
        o->setDimensions( 4500,4500 );


        /*
        o->setMetricsMode(Ogre::GMM_PIXELS);
        o->setPosition(0,0);
        o->setDimensions( 450, 450 );
        */

        /*
        o->setMetricsMode(Ogre::GMM_RELATIVE);
        o->setPosition(0.25,0.25);
        o->setDimensions( 0.5,0.5 );
        */

        o->setHorizontalAlignment ( Ogre::GHA_CENTER );
        o->setVerticalAlignment ( Ogre::GVA_CENTER );

/*
        o->setHorizontalAlignment ( Ogre::GHA_LEFT );
        o->setVerticalAlignment ( Ogre::GVA_TOP );
*/
/*
        o->setHorizontalAlignment ( Ogre::GHA_RIGHT );
        o->setVerticalAlignment ( Ogre::GVA_BOTTOM );
*/

        mOverlay->add2D(o);
        o->show();


    CScreen::getInstance()->getRoot()->renderOneFrame();

}




Splash::~Splash() {
    Ogre::OverlayManager::getSingleton().destroyOverlayElement("splash_pic");
    Ogre::OverlayManager::getSingleton().destroy("Splash");


    //mSM->destroyCamera(mCam);
    CScreen::getInstance()->getRenderWindow()->removeAllViewports();
    mSM->destroyCamera("splash_cam");
    CScreen::getInstance()->getRoot()->destroySceneManager(mSM);


}





