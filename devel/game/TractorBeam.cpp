

#include "TractorBeam.h"

#include "scenemanager.h"
#include "hoppersounds.h"

#include "logger.h"



CTractorBeam::CTractorBeam( CDynamicPhysActor* owner, Ogre::Vector3 relativePosition  ) {

    mOwner = owner;
    mParent = mOwner->getNode();

    mNode = mParent->createChildSceneNode( relativePosition );
    mEntity = CSceneManager::getInstance()->createEntity( "tractorbeam2", "tractorbeam2.mesh" );
    mNode->attachObject( mEntity );
    mNode->setScale( 1, 1.5, 1 );
    mNode->setInheritOrientation( false );  // the tractor beam points down always
    mEntity->setVisible( false );
    mEntity->setQueryFlags( 0x01 ); // 0x01 = CActor::BASIC_OBJECT

    // mActiveSound = new CSound3D( SOUND_TRACTORBEAM, mNode, true );
    mActiveSound = new CSound3D( SOUND_TRACTORBEAM, true );
    mLockedSound = new CSound3D( SOUND_TRACTORBEAM, true );


    mRayQuery=CSceneManager::getInstance()->getOgreSceneManager()->createRayQuery( Ogre::Ray() );
    mRayQuery->setSortByDistance( true, 2);
    // mRayQuery->setQueryMask( ~0 ); // any entity
    mRayQuery->setQueryMask( CActor::DYNAMIC_OBJECT ); // only CActors entity
    mRayQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);

    mLockedActor = 0;

    mStatus = INACTIVE;
    mActivated = false;

    mJoint = 0;
}


CTractorBeam::~CTractorBeam() {


    delete( mRayQuery );
    delete( mActiveSound );
    delete( mLockedSound );

    if( mJoint ) {
        mJoint->detach();
        delete( mJoint );
    }

    mNode->setAutoTracking( false, 0 );

    CSceneManager::getInstance()->destroyEntity(mEntity);
    mParent->removeChild( mNode );


}


void CTractorBeam::update() {

    switch( mStatus ) {

        //case INACTIVE:
        //    manageInactiveMode();
        //break;

        case ACTIVE:
            manageActiveMode();
        break;

        case LOCKED:
            manageLockedMode();
        break;

        //case LINKED:
        //    manageLinkedMode();
        //break;

    }


}



void CTractorBeam::setActive( bool active ) {

    if( active && !mActivated ) {
        mActivated = true;
        switchToActiveMode();
    } else {

        if( !active && mActivated ) {
            mActivated = false;
            switchToInactiveMode();
        }

    }


}



bool CTractorBeam::isActive() {
    return( mStatus == ACTIVE );
}



bool CTractorBeam::isLocked() {
    return( mLockedActor != 0 );
}



CDynamicPhysActor* CTractorBeam::getLockedActor() {
    return( mLockedActor );
}



void CTractorBeam::switchToInactiveMode() {

    if( mStatus == ACTIVE || mStatus == LOCKED ) {
        mActiveSound->stop();
        mLockedSound->stop();
        mEntity->setVisible( false );
        mStatus = INACTIVE;
    }

}

void CTractorBeam::switchToActiveMode() {

    if( mStatus == INACTIVE || mStatus == LOCKED )  {
        mEntity->setVisible( true );
        mStatus = ACTIVE;
        mEntity->setMaterialName( "tractorbeam2/active" );
        mLockedSound->stop();
        mActiveSound->play();

        mNode->setAutoTracking( false );
        mNode->resetOrientation();
        mNode->setScale( 1, 1.5, 1 );
    }

}



void CTractorBeam::manageActiveMode() {

    mActiveSound->setPosition( mNode->_getDerivedPosition() );

    //if( mLockedActor ) mLockedActor->showBoundingBox( false );
    mLockedActor = 0;

    mRayQuery->setRay( Ogre::Ray( mNode->_getDerivedPosition(), Ogre::Vector3(0,-1,0) ));

    Ogre::RaySceneQueryResult &result = mRayQuery->execute();

    if( result.size() > 1) {   // assume the ship is always the first returned object

        Ogre::RaySceneQueryResult::iterator i = result.begin();
        ++i;  // jump to the second object
        if( (*i).movable != NULL ) {

            mLockedActor = static_cast<CDynamicPhysActor*>((*i).movable->getUserObject() );

            if( (mNode->_getDerivedPosition() - mLockedActor->getPosition()).length() < 3.f ) {
                //mLockedActor->showBoundingBox( true );
                switchToLockedMode();
            }
            else
                mLockedActor = 0;
        }

    }

}



void CTractorBeam::switchToLockedMode() {

    mActiveSound->stop();
    mLockedSound->play();

    mEntity->setMaterialName( "tractorbeam2/locked" );

    mNode->setAutoTracking( true, mLockedActor->getNode(), Ogre::Vector3(0,-1,0) );

    mStatus = LOCKED;
}




void CTractorBeam::manageLockedMode() {

    mLockedSound->setPosition( mNode->_getDerivedPosition() ) ;


    mRayQuery->setRay( Ogre::Ray( mNode->_getDerivedPosition(), mNode->getOrientation() * Ogre::Vector3(0,-1,0) ));

    Ogre::RaySceneQueryResult &result = mRayQuery->execute();

    if( result.size() > 1) {   // assume the ship is always the first returned object

        Ogre::RaySceneQueryResult::iterator i = result.begin();
        ++i;  // jump to the second object
        if( (*i).movable != NULL ) {
            mLockedActor = static_cast<CDynamicPhysActor*>((*i).movable->getUserObject() );
            if( (mNode->_getDerivedPosition() - mLockedActor->getPosition() ).length() > 4.f ) {
                switchToLinkedMode();
            }
        }
        else {
            switchToActiveMode();
        }
    }
}


void CTractorBeam::switchToLinkedMode() {

    mLockedSound->stop();
    // mEntity->setVisible( false );
    mNode->setScale( 0.2, 1.5, 0.2 );
    CSoundManager::getInstance()->play2D( SOUND_GO );
    mStatus = LINKED;

    /* create the ode joints */
    mJoint = new OgreOde::BallJoint( CPhysicsManager::getInstance()->getPhysicsWorld() );
    mJoint->attach( mOwner->getPhysBody(), mLockedActor->getPhysBody() );

    // mNode->setAutoTracking( true, mLockedActor->getNode(), Ogre::Vector3(0,-1,0) );

    //LOG("\nDamping : linear : %f, angular : %f", mLockedActor->getLinearDamping(), mLockedActor->getAngularDamping() );

    mLockedActor->setLinearDamping( 0.3f );
    mLockedActor->setAngularDamping( 0.5f );

}


void CTractorBeam::manageLinkedMode() {
    // mNode->lookAt( mLockedActor->getPosition(), Ogre::Node::TS_WORLD );
}

void CTractorBeam::unlink() {

    if( mStatus == LINKED ) {
        // mNode->setAutoTracking( false, 0 );
        //mNode->resetOrientation();
        //mNode->setScale( 1, 1.5, 1 );

        mLockedActor->setLinearDamping( 0 );
        mLockedActor->setAngularDamping( 0 );

        if( mJoint ) {
            mJoint->detach();
            delete( mJoint );
            mJoint = 0;
        }

        mEntity->setVisible( false );


        CSoundManager::getInstance()->play2D( SOUND_MOUSECLICK );

        mStatus = INACTIVE;
    }
}




