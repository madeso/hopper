

#include "scenemanager.h"
#include "screen.h"
#include "logger.h"
#include "gameconstants.h"


CSceneManager::CSceneManager() { // :Ogre::TerrainSceneManager("Hopper scene manager") {

    mOgreSceneManager=CScreen::getInstance()->getRoot()->createSceneManager(
        Ogre::ST_EXTERIOR_CLOSE, "Hopper Scene Manager");

}

CSceneManager::~CSceneManager() {

    // delete(mCamera);
    CCamera::kill();

    CScreen::getInstance()->getRoot()->destroySceneManager( mOgreSceneManager );
    CScreen::getInstance()->getRenderWindow()->removeAllViewports();
}


Ogre::SceneNode* CSceneManager::getRootSceneNode() {
    return( mOgreSceneManager->getRootSceneNode() );
}

Ogre::Entity* CSceneManager::createEntity( Ogre::String name, Ogre::String modelname ) {
    return( mOgreSceneManager->createEntity( name, modelname ));
}

bool CSceneManager::hasEntity( Ogre::String name ) {
    return( mOgreSceneManager->hasEntity( name ) );
}

void CSceneManager::destroyEntity( Ogre::Entity* e ) {
    mOgreSceneManager->destroyEntity( e );
}

Ogre::Camera* CSceneManager::createCamera( Ogre::String name ) {
    return( mOgreSceneManager->createCamera( name ));
}

void CSceneManager::destroyCamera( Ogre::Camera* camera ) {
    mOgreSceneManager->destroyCamera( camera);
}


Ogre::BillboardSet* CSceneManager::getNewBillBoardSet( Ogre::String name ) {
    return( mOgreSceneManager->createBillboardSet( name ));
}


void CSceneManager::destroyBillboardSet( Ogre::BillboardSet* billboardset ) {
    mOgreSceneManager->destroyBillboardSet( billboardset );
}

void CSceneManager::destroyParticleSystem( Ogre::ParticleSystem* particlesystem ) {
    mOgreSceneManager->destroyParticleSystem( particlesystem );
}

Ogre::ParticleSystem* CSceneManager::createParticleSystem( Ogre::String name, Ogre::String type ) {
    return(mOgreSceneManager->createParticleSystem( name, type ));
}

Ogre::Light* CSceneManager::createLight( Ogre::String name ) {
    return(mOgreSceneManager->createLight( name ));
}

void CSceneManager::destroyLight( Ogre::Light* light ) {
    mOgreSceneManager->destroyLight( light);
}

bool CSceneManager::getShowBoundingBoxes() {
    return( mOgreSceneManager->getShowBoundingBoxes());
}

void CSceneManager::showBoundingBoxes( bool show ) {
    mOgreSceneManager->showBoundingBoxes( show);
}


void CSceneManager::setShadowOn() {

    mOgreSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
 //   mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    mOgreSceneManager->setShadowColour(Ogre::ColourValue(0.5,0.5,0.5));

    mOgreSceneManager->setShadowTextureSettings(512, 2);

    mOgreSceneManager->setShadowFarDistance(900);

  //  mSceneManager->setShowDebugShadows(true);

}

void CSceneManager::setAmbientColor( float red, float green, float blue ) {

    mOgreSceneManager->setAmbientLight(Ogre::ColourValue(red,green,blue));

}

void CSceneManager::setFogOn( int red, int green, int blue ) {
    Ogre::ColourValue fadeColour( red / 256.0, green / 256.0 , blue / 256.0 );
    mOgreSceneManager->setFog( Ogre::FOG_LINEAR, fadeColour, 0.001, 500, 1500);
    CScreen::getInstance()->getRenderWindow()->getViewport(0)->setBackgroundColour(fadeColour);
}

void CSceneManager::setFogOff() {
    Ogre::ColourValue fadeColour(0.2, 0.2, 0.2);
    mOgreSceneManager->setFog( Ogre::FOG_NONE, fadeColour, .001, 1000, 1500);
    CScreen::getInstance()->getRenderWindow()->getViewport(0)->setBackgroundColour(fadeColour);
}


Ogre::SceneManager* CSceneManager::getOgreSceneManager() {
   //  CLogger::getInstance()->log(" ( SceneManager::mSceneManager = %p ) ",mSceneManager );
    return(mOgreSceneManager);
}

Ogre::TerrainSceneManager* CSceneManager::getTerrainSceneManager() {
    // ceci n'est pas un hack (!)
    return(  (Ogre::TerrainSceneManager*) mOgreSceneManager);
}



float CSceneManager::getHeightAt( float x, float y ) {
    return( static_cast<Ogre::TerrainSceneManager*>(mOgreSceneManager)->getHeightAt( x, y) );
}
