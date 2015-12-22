

#ifndef _CASTRUM_
#define _CASTRUM_

#include "Ogre.h"
#include "scenemanager.h"
#include "light.h"


#define ASTRUM_DISTANCE_TO_CAMERA 3000.f

/**
    Billboard in the sky ( relative to the camera )
    Note: astrum is the latin for 'heavenly body' ( i.e. a star, a planet, a galaxy, etc... )
*/


class CAstrum: public CCamera::Listener {

public:
    CAstrum( Ogre::Vector3 direction, Ogre::String material, float size );
    virtual ~CAstrum();

    virtual void setDirection( Ogre::Vector3 direction );
    void onCameraMoved( Ogre::Vector3 campos );

protected:
    Ogre::Vector3 mDirection;

private:
    Ogre::BillboardSet* mBillboardSet;
    Ogre::SceneNode* mNode;


};


#endif

