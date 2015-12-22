
#include "directionarrowwindow.h"
#include "scenemanager.h"

CDirectionArrowWindow::CDirectionArrowWindow() {
    mGUI=DynaGUI::getInstance();

    //mWindow=mGM->createWindow( BetaGUI::CENTERED, BetaGUI::MIN, BetaGUI::NONE );
    //mWindow->setSize( 200, 100 );
    //mWindow=mGM->createZBufferClearingWindow( Ogre::Vector4( 300, 0, 200, 100 ) );

    // mModelNode=new Ogre::SceneNode(0); // doesn't work anymore in Dagon :(
    mModelNode=new Ogre::SceneNode( CSceneManager::getInstance()->getOgreSceneManager(), "UIarrow_node" );

    mModelEntity=CSceneManager::getInstance()->createEntity( "UIarrow", "arrow.mesh" );
    mModelNode->attachObject( mModelEntity );

    mModelNode->setScale( 0.5,0.5,0.5 );
    mModelNode->setPosition( 0,7, -15 );

    mGUI->add3D( mModelNode );
}


CDirectionArrowWindow::~CDirectionArrowWindow() {

    mGUI->remove3D( mModelNode );
    //mGM->destroyWindow( mWindow );

    CSceneManager::getInstance()->destroyEntity( mModelEntity );
    delete(mModelNode);

}


void CDirectionArrowWindow::lookAt(Ogre::Vector3 lookat ) {
        mModelNode->lookAt( lookat, Ogre::Node::TS_WORLD );
}

