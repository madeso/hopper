

#ifndef _CPOSITIONALTHRUSTER_
#define _CPOSITIONALTHRUSTER_

#include "Ogre.h"

class CPositionalThruster {

    public:

    typedef enum Placement { FRONTLEFT, FRONTRIGHT, REARRIGHT, REARLEFT };

    CPositionalThruster( Ogre::SceneNode* parent,
        const Ogre::String& name,
        const Ogre::Vector3& relpos,
        Placement placement );

    ~CPositionalThruster();

    void setPower( float forceYaw, float forcePitch, float forceRoll );
    void cutPower();


    private:
    Ogre::SceneNode* mParentNode;
    Ogre::String mName;
    Ogre::Vector3 mRelPos;
    Placement mPlacement;
    Ogre::SceneNode* mNode;
    Ogre::Entity* mUp;
    Ogre::Entity* mDown;
    Ogre::Entity* mSide;



};

#endif

