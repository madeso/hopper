
#ifndef _CPHYSICSMANAGER_
#define _CPHYSICSMANAGER_

#include "singleton.h"
#include "OgreOde_Core.h"
#include "collisionmanager.h"
#include "timemanager.h"

class CPhysicsManager:public SSingleton<CPhysicsManager> {

    public:

    CPhysicsManager();
    ~CPhysicsManager();

    void setCollisionManager( CCollisionManager* aCollisionManager );
    //void update(int aFrametime);
    void update( float aTimefactor );
    void pause( bool paused );
    // bool preStep(Ogre::Real time);  // callback before each step
    void setGravity( float gravity );
    OgreOde::World* getPhysicsWorld();
    OgreOde::Space* getPhysicsSpace();


    private:
          OgreOde::ForwardFixedInterpolatedStepHandler *mPhysStepper;
//      OgreOde::ForwardFixedInterpolatedQuickStepper *mPhysStepper;
//    OgreOde::ForwardFixedQuickStepper *mPhysStepper;
//    OgreOde::ExactVariableQuickStepper *mPhysStepper;
//    OgreOde::QuickStepper *mPhysStepper;
    //CCollisionManager* mCollisionManager;
    CTimeManager* mTimeManager;
    bool mPaused;
    OgreOde::World* mPhysicsWorld;
    OgreOde::SimpleSpace* mPhysicsSpace;

};


#endif

