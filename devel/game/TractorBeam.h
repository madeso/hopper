



#ifndef _TRACTORBEAM_
#define _TRACTORBEAM_


#include "Ogre.h"
#include "sound.h"
#include "physicobject.h"

class CTractorBeam {


public:

    enum status { INACTIVE=0, ACTIVE, LOCKED, LINKED };

    CTractorBeam( CDynamicPhysActor* owner, Ogre::Vector3 relativePosition );
    ~CTractorBeam();

    void update();

    void setActive(bool active);

    void unlink();

    bool isActive();
    bool isLocked();

    CDynamicPhysActor* getLockedActor();

private:

    Ogre::Entity* mEntity;
    Ogre::SceneNode* mNode;


    bool mActivated;
//    bool mLocked;
    Ogre::SceneNode* mParent;
    CSound3D* mActiveSound;
    CSound3D* mLockedSound;

    Ogre::RaySceneQuery* mRayQuery;

    CDynamicPhysActor* mLockedActor;
    CDynamicPhysActor* mOwner;

    int mStatus;

    void switchToInactiveMode();

    void switchToActiveMode();
    void manageActiveMode();

    void switchToLockedMode();
    void manageLockedMode();

    void switchToLinkedMode();
    void manageLinkedMode();

    OgreOde::BallJoint* mJoint;


};


#endif


