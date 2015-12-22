

#include "shiporientationwindow.h"

#include "scenemanager.h"

CShipOrientationWindow::CShipOrientationWindow() {

    mGUI=DynaGUI::getInstance();

    // mWindow=mGM->createWindow( BetaGUI::MAX, BetaGUI::MAX );

    // mModelNode=new Ogre::SceneNode(0); // doesn't work anymore in Dagon :(

    mModelNode=new Ogre::SceneNode( CSceneManager::getInstance()->getOgreSceneManager(), "UIorientation_node" );

    mModelEntity=CSceneManager::getInstance()->createEntity( "orientationlander", "ghostlander.mesh" );
    mModelNode->attachObject( mModelEntity );

    mModelNode->setPosition( 8,-6,-15 );  // TODO: should be dependent on the screen ratio ?

    //mWindow->add3D( mModelNode );
    mGUI->add3D( mModelNode );


}

CShipOrientationWindow::~CShipOrientationWindow() {

    // mGM->destroyWindow( mWindow );
    mGUI->remove3D( mModelNode);

    CSceneManager::getInstance()->destroyEntity( mModelEntity );
    delete(mModelNode);

}

void CShipOrientationWindow::setOrientation( Ogre::Quaternion orientation ) {

    mModelNode->setOrientation( orientation );

}
