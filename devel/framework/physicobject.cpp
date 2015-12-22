
#include "physicobject.h"
#include "logger.h"



CPhysActor::CPhysActor( Ogre::String name, Ogre::String modelname )
        :CActor( name, modelname ) {

    // mName=name;
    mWorld= CPhysicsManager::getInstance()->getPhysicsWorld();
    mSpace= CPhysicsManager::getInstance()->getPhysicsSpace();
    mPhysGeom=0;
    mPhysBody=0;

}


CPhysActor::~CPhysActor() {

    if ( mPhysGeom ) delete( mPhysGeom );

}



OgreOde::Geometry* CPhysActor::getPhysGeom() {
    return( mPhysGeom );
}

bool CPhysActor::isPhysEnabled() {
    return( mPhysGeom->isEnabled() );
}

void CPhysActor::physEnable() {
    mPhysGeom->enable();
}

void CPhysActor::physDisable() {
    mPhysGeom->disable();
}


Ogre::Vector3 CPhysActor::getVelocity() {
    if( mPhysBody ) return mPhysBody->getLinearVelocity();
    return( Ogre::Vector3(0,0,0) );
}


void CPhysActor::onCollision( float aDamage ) {

    damage( aDamage );


    return;
}



/*
void CPhysActor::onCollision( Ogre::Vector3 position, float penetrationDepth, int material ) {
    return;
}
*/
/*
void CPhysActor::onCollision( Ogre::Vector3 position, float speed, float otherObjectType ) {
}
*/


CStaticPhysActor::CStaticPhysActor( Ogre::Vector3 pos, float aYaw, float aScale,
                                    Ogre::String name, Ogre::String modelname )
        :CPhysActor( name, modelname ) {

    // set the position, orientation and scale before the physics init
    // ( static objects case : no OgreOde::Body )
    setPosition( pos );
    setScale( aScale, aScale, aScale ); // absolute
    // scale( aScale, aScale, aScale ); // relative
    setYaw(aYaw);
    mNode->_update( false, false );
    setQueryFlags( CActor::STATIC_OBJECT );

}



CStaticPhysCube::CStaticPhysCube( Ogre::Vector3 pos, float yaw, float scale,
                                  Ogre::String name, Ogre::String model  )
        :CStaticPhysActor( pos, yaw, scale, name, model ) {

    OgreOde::EntityInformer ei( mEntity, getNode()->_getFullTransform() );  // Ogre::Matrix4::getScale(getScale())  );
    mPhysGeom=ei.createSingleStaticBox( mWorld, mSpace );

    mPhysGeom->setUserObject( static_cast<CPhysActor*>(this) );

    //mPhysGeom->setMaxContacts( 1 );
    mPhysGeom->enable();
}


CStaticPhysSphere::CStaticPhysSphere( Ogre::Vector3 pos, float scale, Ogre::String name, Ogre::String model ):
    CStaticPhysActor( pos, 0, scale, name, model )  {

    mPhysGeom = new OgreOde::SphereGeometry( scale, mWorld, mSpace );

    mPhysGeom->setUserObject( static_cast<CPhysActor*>(this) );

    mPhysGeom->enable();

}



CStaticPhysTrimesh::CStaticPhysTrimesh( Ogre::Vector3 pos, float yaw, float scale,
                                        Ogre::String name, Ogre::String model )
        :CStaticPhysActor( pos, yaw, scale, name, model ) {

    OgreOde::EntityInformer ei( mEntity, getNode()->_getFullTransform() );
    mPhysGeom=ei.createStaticTriangleMesh( mWorld, mSpace );

    mPhysGeom->setUserObject( static_cast<CPhysActor*>(this) );

    //mPhysGeom->setMaxContacts( 1 );

}







CDynamicPhysActor::CDynamicPhysActor( Ogre::String name, Ogre::String modelname )
        :CPhysActor( name, modelname ) {


    mPhysBody = new OgreOde::Body( mWorld, name+"_body" );
    attachObject( mPhysBody );

    setQueryFlags( CActor::DYNAMIC_OBJECT );

}

CDynamicPhysActor::~CDynamicPhysActor() {
    delete( mPhysBody );
}

void CDynamicPhysActor::addForce( Ogre::Vector3 force ) {
    mPhysBody->addForce( force );
}

void CDynamicPhysActor::addRelativeForce( Ogre::Vector3 force ) {
    mPhysBody->addRelativeForce( force );
}

void CDynamicPhysActor::addRelativeTorque ( Ogre::Vector3 torque ) {
    mPhysBody->addRelativeTorque( torque );
}

Ogre::Vector3 CDynamicPhysActor::getLinearVelocity() {
    return( mPhysBody->getLinearVelocity() );
}

Ogre::Vector3 CDynamicPhysActor::getAngularVelocity() {
    return( mPhysBody->getAngularVelocity() );
}

Ogre::Quaternion CDynamicPhysActor::getOrientation() {
    return( mPhysBody->getOrientation() );
}

/*
OgreOde::Geometry* CDynamicPhysActor::getPhysGeom() {
    return( mPhysGeom );
}
*/
Ogre::Vector3 CDynamicPhysActor::getVelocity() {
    return( mPhysBody->getPointWorldVelocity( Ogre::Vector3( 0,0,0 ) ) );
}

Ogre::Vector3 CDynamicPhysActor::getPosition() {
    return( mPhysBody->getPosition() );
}

void CDynamicPhysActor::setPosition( Ogre::Vector3 pos ) {
    mPhysBody->setPosition( pos );
}

void CDynamicPhysActor::setOrientation( Ogre::Quaternion orientation ) {
    mPhysBody->setOrientation( orientation );
}

float CDynamicPhysActor::getLinearDamping() {
    return( mPhysBody->getLinearDamping() );
}

void CDynamicPhysActor::setLinearDamping( float damping ) {
    mPhysBody->setLinearDamping( damping );
}

float CDynamicPhysActor::getAngularDamping() {
    return( mPhysBody->getAngularDamping() );
}

void CDynamicPhysActor::setAngularDamping( float damping ) {
    mPhysBody->setAngularDamping( damping );
}

void CDynamicPhysActor::setAutoSleep( bool flag ) {
    mPhysBody->setAutoSleep( flag );
}

void CDynamicPhysActor::setAngularVelocity( Ogre::Vector3 velocity ) {
    mPhysBody->setAngularVelocity( velocity );
}

void CDynamicPhysActor::setLinearVelocity( Ogre::Vector3 velocity ) {
    mPhysBody->setLinearVelocity( velocity );
}


void CDynamicPhysActor::setAffectedByGravity( bool flag ) {
    mPhysBody->setAffectedByGravity( flag );
}


OgreOde::Body* CDynamicPhysActor::getPhysBody() {
    return( mPhysBody );
}


void CDynamicPhysActor::wake() {
    mPhysBody->wake();
}

CDynamicPhysCube::CDynamicPhysCube( Ogre::Vector3 pos,
                                    float scale, float mass, float density,
                                    Ogre::String name, Ogre::String model )
        :CDynamicPhysActor( name, model ) {


    setScale( scale, scale, scale );

    OgreOde::EntityInformer ei( mEntity, getNode()->_getFullTransform() );
    mPhysBody=ei.createSingleDynamicBox( mass, mWorld, mSpace );

    mPhysGeom=mPhysBody->getGeometry( 0 );
    mPhysGeom->setUserObject( static_cast<CPhysActor*>(this) );
    //mPhysGeom->setMaxContacts( 1 );


    // don't call before !! ( call to OgreOde::Body )
    setPosition( pos );


}




CDynamicPhysTrimesh::CDynamicPhysTrimesh( Ogre::Vector3 pos,
        float scale, float mass, float density,
        Ogre::String name, Ogre::String model )
        :CDynamicPhysActor( name, model )  {

    setScale( scale, scale, scale );
    OgreOde::EntityInformer ei( mEntity, getNode()->_getFullTransform() );
    mPhysGeom = ei.createStaticTriangleMesh( mWorld, mSpace );

    OgreOde::BoxMass OdeMass( mass , ei.getSize() );
    OdeMass.setDensity( density, ei.getSize() );
    mPhysBody->setMass( OdeMass );

    mPhysGeom->setBody( mPhysBody );
    mPhysGeom->setUserObject( static_cast<CPhysActor*>(this) );
    //mPhysGeom->setMaxContacts( 1 );

    // don't call before !! ( call to OgreOde::Body )
    setPosition( pos );
    // set scale here
    // set orientation here

}


CDynamicPhysSphere::CDynamicPhysSphere( Ogre::Vector3 pos,
                                        float scale, float mass,
                                        Ogre::String name, Ogre::String model )
        :CDynamicPhysActor( name, model )  {

    setScale( scale, scale, scale );

    OgreOde::EntityInformer ei( mEntity, getNode()->_getFullTransform() );
    // mPhysGeom = ei.createStaticTriangleMesh(mWorld, mSpace);
    mPhysBody=ei.createSingleDynamicSphere( mass, mWorld, mSpace );

    // mPhysGeom->setBody(mPhysBody);
    mPhysGeom=mPhysBody->getGeometry( 0 );
    mPhysGeom->setUserObject( static_cast<CPhysActor*>(this) );
    //mPhysGeom->setMaxContacts( 1 );

    //LOG( "\n ********** dynamic sphere geom : %p ", mPhysGeom );

    // don't call before !! ( call to OgreOde::Body )
    setPosition( pos );


}


CDynamicPhysCylinder::CDynamicPhysCylinder(
    Ogre::Vector3 pos,
    float scale, float mass, float density,
    Ogre::String name, Ogre::String model )
    :CDynamicPhysActor( name, model ) {

    setScale( scale, scale, scale );

    LOG("\nCylinder : size x=%f, y=%f, z=%f", mEntity->getBoundingBox().getHalfSize().x,
        mEntity->getBoundingBox().getHalfSize().y, mEntity->getBoundingBox().getHalfSize().z );

    float radius=mEntity->getBoundingBox().getHalfSize().y;
    float length=mEntity->getBoundingBox().getHalfSize().z;

    OgreOde::CylinderMass cylinderMass( mass, Ogre::Vector3( 0,0,1 ), radius, length );
    cylinderMass.setDensity( density, Ogre::Vector3( 0,0,1), radius, length );

    mPhysBody->setMass( cylinderMass );

// TODO: find a better way to handle the size definition
// FIXME : I ask a cylinder, I got a capsule !! And a Z aligned one...
    mPhysGeom=new OgreOde::CylinderGeometry(
        radius,
        length,
        mWorld,
        mSpace );

/*
    Ogre::Quaternion q;
    q.FromAngleAxis( Ogre::Radian(M_PI_2), Ogre::Vector3( 1,0,0 ));
    mPhysGeom->setOrientation( q );
*/
    mPhysGeom->setBody( mPhysBody );
    mPhysGeom->setUserObject( static_cast<CPhysActor*>(this) );




    setPosition( pos );

}



