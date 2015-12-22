


#ifndef _LANDING_PAD_
#define _LANDING_PAD_

// #include "staticphyscube.h"
#include "physicobject.h"
//#include "pointlight.h"
//#include "spotlight.h"
#include "light.h"
#include "soundmanager.h"

/// a landing pad.

class CLandingPad:public CStaticPhysCube {


    public :

    CLandingPad();
    CLandingPad( Ogre::String aName, Ogre::Vector3 aPos );
    ~CLandingPad();

    void setShipLanded();
    bool hasShipLanded();

    Ogre::AxisAlignedBox mAABB;




    private:
    Ogre::String mName;
    bool mShipLanded;
    // CSound3D* mSoundLanded;
    //CSpotLight* mLight;
    CPointLight* mLight;
    CSoundManager* mSoundManager;

};



#endif

