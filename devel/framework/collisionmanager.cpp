
#include "collisionmanager.h"
#include "physicsmanager.h"

//TODO: add material management

CCollisionManager::CCollisionManager() {
    // mCollisionHandler=0;
    CPhysicsManager::getInstance()->setCollisionManager( this );
}

CCollisionManager::~CCollisionManager() {
    CPhysicsManager::getInstance()->setCollisionManager( NULL );
}

/*
void CCollisionManager::setCollisionHandler( CCollisionHandler *h ) {
    mCollisionHandler=h;
}
*/

bool CCollisionManager::collision( OgreOde::Contact* contact ) {

	OgreOde::Geometry * const g1 = contact->getFirstGeometry();
	OgreOde::Geometry * const g2 = contact->getSecondGeometry();

    // collisions between 2 static objects are not interesting !
    if( ( !g1->getBody() ) && ( !g2->getBody() ) )
        return( false);

  //  return( mCollisionHandler->collision(contact) );
  return( true );

}

