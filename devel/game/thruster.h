

#ifndef _THRUSTER_
#define _THRUSTER_

#include "Ogre.h"
// #include "spotlight.h"
#include "light.h"
#include "sound.h"
#include "particleemitter.h"

class CThruster {

    public:

    CThruster( Ogre::SceneNode* parentnode, const Ogre::String& name,
        const Ogre::Vector3& relpos ); // , const Ogre::Vector3& dir, float size );

    ~CThruster();

    void setActive( bool active );
    bool isActive();
    void setPower( float power );
    float getPower();


    private:
    Ogre::SceneNode* mNode;
    Ogre::Entity* mEntity;
    Ogre::SceneNode* mParentNode;
    Ogre::Vector3 mRelPos;

    float mPower;
    bool mActive;


    //CPointLight* mThrustLight;
    CSpotLight* mThrustLight;
    CSound3D* mSoundThrust;
    CSound3D* mSoundThrustloop;

    Ogre::SceneNode* mSmokeTestPoint;
    CParticleEmitter* mTerrainSmokeEmitter;

};



#endif


