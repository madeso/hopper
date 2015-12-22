

#ifndef _WINDMILL_
#define _WINDMILL_

#include "physicobject.h"
#include "sound.h"

class CWindmill : public CStaticPhysCube {  //: public CDynamicPhysCube {

public:
    CWindmill( Ogre::String name, Ogre::Vector3 pos, float yaw );
    ~CWindmill();


private:
    CDynamicPhysTrimesh* mBlades;

    OgreOde::JointGroup* mJointGroup;
    OgreOde::AngularMotorJoint* mAMotor;
    OgreOde::PistonJoint* mPiston;
//    OgreOde::FixedJoint* mLink;

    CSound3D* mSoundLoop;

};


#endif


