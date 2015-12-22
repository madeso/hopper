
#include "CSGTunnels.h"
#include "scenemanager.h"

#include "logger.h"

CSGTunnels::CSGTunnels( Ogre::Vector3 pos ) {

    mRoom1Node=CSceneManager::getInstance()->getRootSceneNode()->createChildSceneNode( "room1_node" );
    mCubeNode=CSceneManager::getInstance()->getRootSceneNode()->createChildSceneNode( "cube_node" );
    mResultNode=CSceneManager::getInstance()->getRootSceneNode()->createChildSceneNode( "result_node" );

    mRoom1=CSceneManager::getInstance()->createEntity( "room1_entity", "room.mesh" );
    mCube = CSceneManager::getInstance()->createEntity( "cube_entity", "cylinder6.mesh") ;

    mRoom1Node->attachObject( mRoom1 );
    mCubeNode->attachObject( mCube );

    mRoom1Node->setPosition( 0,0,0 );
    mCubeNode->setPosition( 0,0,0 );

    LOG("\n CSG operation... ");

    cvEntityDifference( "result_mesh", mRoom1, mCube );
    // cvEntityIntersection( "result_mesh", mRoom1, mCube );
    // cvEntityUnion( "result_mesh", mRoom1, mCube );

    LOG("done.");

    mResult = CSceneManager::getInstance()->createEntity( "result_entity", "result_mesh" );
    mResultNode->attachObject( mResult );

    mRoom1Node->setPosition( pos );
    mCubeNode->setPosition( pos );

    mResultNode->setPosition( pos + Ogre::Vector3( 10,0,0) );

}


CSGTunnels::~CSGTunnels() {

    CSceneManager::getInstance()->getRootSceneNode()->removeAndDestroyChild( "room1_node" );
    CSceneManager::getInstance()->getRootSceneNode()->removeAndDestroyChild( "cube_node" );
    CSceneManager::getInstance()->getRootSceneNode()->removeAndDestroyChild( "result_node" );

    CSceneManager::getInstance()->destroyEntity( mRoom1 );
    CSceneManager::getInstance()->destroyEntity( mCube );
    CSceneManager::getInstance()->destroyEntity( mResult );

}

