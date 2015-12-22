

#include "windmill.h"
#include "hoppersounds.h"
#define WINDMILL_SCALE 2.5f

CWindmill::CWindmill( Ogre::String name, Ogre::Vector3 pos, float yaw ):
    //CDynamicPhysCube( pos, WINDMILL_SCALE, 1, 1, name + "_foot", "windmill_foot.mesh" ) {
    CStaticPhysCube( pos, 0, WINDMILL_SCALE, name, "windmill_foot.mesh" ) {


    mBlades = new CDynamicPhysTrimesh( pos + Ogre::Vector3( 0,4*WINDMILL_SCALE, 0.5*WINDMILL_SCALE ), WINDMILL_SCALE, 1, 1, name + "_blades", "windmill_blades.mesh" );
    mBlades->castShadows();
    //mBlades->setScale( WINDMILL_SCALE, WINDMILL_SCALE, WINDMILL_SCALE );

    mJointGroup = new OgreOde::JointGroup(CPhysicsManager::getInstance()->getPhysicsWorld());

   // mLink = new OgreOde::FixedJoint(  CPhysicsManager::getInstance()->getPhysicsWorld() );
   // mLink->attach( this->getPhysBody() );



    mPiston = new OgreOde::PistonJoint( CPhysicsManager::getInstance()->getPhysicsWorld(), mJointGroup );

    //mPiston->attach( this->getPhysBody(), mBlades->getPhysBody() );
    mPiston->attach( mBlades->getPhysBody() );
    mPiston->setAnchor( mBlades->getPosition() );
    mPiston->setAxis( Ogre::Vector3( 0,0, 1 ) );

    // set these for the blades not to move away from the support !
    mPiston->setParameter( OgreOde::Joint::Parameter_LowStop,  0, 1 );
    mPiston->setParameter( OgreOde::Joint::Parameter_HighStop, 0, 1 );


    mAMotor = new OgreOde::AngularMotorJoint( CPhysicsManager::getInstance()->getPhysicsWorld(), mJointGroup );

    mAMotor->attach( mBlades->getPhysBody() );

    mAMotor->setAxisCount(1);
    mAMotor->setAxis(0, OgreOde::AngularMotorJoint::RelativeOrientation_FirstBody, Ogre::Vector3( 0,0,1 ) );
    // mAMotor->addTorque( 0,0,1 );

    mAMotor->setParameter( OgreOde::Joint::Parameter_MotorVelocity, 2.f );
    mAMotor->setParameter( OgreOde::Joint::Parameter_MaximumForce, 1000.f*WINDMILL_SCALE ); // 0 ( inactive ) by default


    mSoundLoop = new CSound3D( SOUND_WINDMILL, true );
    mSoundLoop->setPosition( mBlades->getPosition() );
    mSoundLoop->play();

}


CWindmill::~CWindmill() {

    //mLink->detach();
    //delete(mLink);
    mPiston->detach();
    delete( mPiston );
    mAMotor->detach();
    delete( mAMotor );
    delete( mJointGroup );
    delete(mBlades);

    delete( mSoundLoop );

}

#if 0

CWindmill::CWindmill( Ogre::String name, Ogre::Vector3 pos, float yaw ):
    CDynamicPhysCube( pos, 1, 1, 1, name + "_foot", "windmill_foot.mesh" ) {


    mBlades = new CDynamicPhysTrimesh( pos + Ogre::Vector3( 0,3, 0.5 ), 1, 1, 1, name + "_blades", "windmill_blades.mesh" );
    mBlades->setCastShadows( true );

    mJointGroup = new OgreOde::JointGroup(CPhysicsManager::getInstance()->getPhysicsWorld());

    mLink = new OgreOde::FixedJoint(  CPhysicsManager::getInstance()->getPhysicsWorld() );
    mLink->attach( this->getPhysBody() );



    mPiston = new OgreOde::PistonJoint( CPhysicsManager::getInstance()->getPhysicsWorld(), mJointGroup );

    mPiston->attach( this->getPhysBody(), mBlades->getPhysBody() );
    mPiston->setAnchor( mBlades->getPosition() );
    mPiston->setAxis( Ogre::Vector3( 0,0, 1 ) );

    // set these for the blades not to move away from the support !
    mPiston->setParameter( OgreOde::Joint::Parameter_LowStop,  0, 1 );
    mPiston->setParameter( OgreOde::Joint::Parameter_HighStop, 0, 1 );


    mAMotor = new OgreOde::AngularMotorJoint( CPhysicsManager::getInstance()->getPhysicsWorld(), mJointGroup );

    mAMotor->attach( this->getPhysBody(), mBlades->getPhysBody() );

    mAMotor->setAxisCount(1);
    mAMotor->setAxis(0, OgreOde::AngularMotorJoint::RelativeOrientation_SecondBody, Ogre::Vector3( 0,0,1 ) );
    // mAMotor->addTorque( 0,0,1 );

    mAMotor->setParameter( OgreOde::Joint::Parameter_MotorVelocity, 2.f );
    mAMotor->setParameter( OgreOde::Joint::Parameter_MaximumForce, 50.f ); // 0 ( inactive ) by default

}


CWindmill::~CWindmill() {

    mLink->detach();
    delete(mLink);
    mPiston->detach();
    delete( mPiston );
    mAMotor->detach();
    delete( mAMotor );
    delete( mJointGroup );
    delete(mBlades);

}

#endif


