
#ifndef _PHYSICOBJECT_
#define _PHYSICOBJECT_


#include "actor.h"
#include "physicsmanager.h"
#include "OgreOde_Core.h"




/** @brief base actor which is part of the physical simulation.
    don't instanciate directly !
    manages only the OgreOde::Geom
*/
class CPhysActor: public CActor {

    public:
    CPhysActor( Ogre::String name, Ogre::String modelname);
    virtual ~CPhysActor();

    OgreOde::Geometry* getPhysGeom();

    bool isPhysEnabled();
    void physEnable();
    void physDisable();
    Ogre::Vector3 getVelocity();

    // virtual void onCollision( float penetrationDepth );
    //void onCollision( Ogre::Vector3 position, float penetrationDepth, int othermaterial );
    virtual void onCollision( float damage );

    protected:
    OgreOde::Geometry* mPhysGeom;
    OgreOde::Body* mPhysBody;
    OgreOde::Space *mSpace;
    OgreOde::World *mWorld;

};




/**
    @brief base static actor which is part of the physical simulation.
*/
class CStaticPhysActor: public CPhysActor {

    public:
    CStaticPhysActor(Ogre::Vector3, float yaw, float scale, Ogre::String name, Ogre::String modelname);
    virtual ~CStaticPhysActor() {};
};



class CStaticPhysCube: public CStaticPhysActor {

    public:
    CStaticPhysCube( Ogre::Vector3 pos, float yaw, float scale, Ogre::String name, Ogre::String model );
    virtual ~CStaticPhysCube() {};
};


class CStaticPhysSphere: public CStaticPhysActor {
    public:
    CStaticPhysSphere( Ogre::Vector3 pos, float scale, Ogre::String name, Ogre::String model );
    virtual ~CStaticPhysSphere() {};
};


class CStaticPhysTrimesh: public CStaticPhysActor {

    public:
    CStaticPhysTrimesh( Ogre::Vector3 pos, float yaw, float scale, Ogre::String name, Ogre::String model );

    virtual ~CStaticPhysTrimesh() {};
};




/**
  @brief physActor with a OgreOde::Body
*/
class CDynamicPhysActor: public CPhysActor {

    public:
    CDynamicPhysActor( Ogre::String name, Ogre::String model);

    virtual ~CDynamicPhysActor();


    Ogre::Vector3 getLinearVelocity();
    Ogre::Vector3 getAngularVelocity();
    Ogre::Quaternion getOrientation();
    Ogre::Vector3 getVelocity();
    Ogre::Vector3 getPosition();

    void setPosition( Ogre::Vector3 pos );
    void setOrientation( Ogre::Quaternion orientation );
    void setLinearVelocity( Ogre::Vector3 velocity );
    void setAngularVelocity( Ogre::Vector3 velocity );
    float getLinearDamping();
    void setLinearDamping( float damping );
    float getAngularDamping();
    void setAngularDamping( float damping );
    void setAutoSleep(bool flag );
    void setAffectedByGravity( bool flag );

    void addForce(Ogre::Vector3 force );
    void addRelativeTorque ( Ogre::Vector3 torque );
    void addRelativeForce( Ogre::Vector3 force );

    void wake();

    OgreOde::Body* getPhysBody();

};




class CDynamicPhysCube:public CDynamicPhysActor {

    public:
    CDynamicPhysCube(Ogre::Vector3 pos, float scale, float mass, float density,
                        Ogre::String name, Ogre::String model);
    virtual ~CDynamicPhysCube() {};

};




class CDynamicPhysTrimesh: public CDynamicPhysActor {

    public:
    CDynamicPhysTrimesh(Ogre::Vector3 pos, float scale, float mass, float density,
                        Ogre::String name, Ogre::String model );

    virtual ~CDynamicPhysTrimesh() {};

};



class CDynamicPhysSphere: public CDynamicPhysActor {

    public:
    CDynamicPhysSphere(Ogre::Vector3 pos, float scale, float mass,
                        Ogre::String name, Ogre::String model );

    virtual ~CDynamicPhysSphere() {};
};


class CDynamicPhysCylinder: public CDynamicPhysActor {
    public:
    CDynamicPhysCylinder( Ogre::Vector3 pos, float scale, float mass, float density,
                        Ogre::String name, Ogre::String model );

    virtual ~CDynamicPhysCylinder() {};

};



#endif

