

#ifndef _CLIGHT_
#define _CLIGHT_

#include "actor.h"
#include "Ogre.h"


class CLight {

    public:

    CLight(Ogre::String aName);
    ~CLight();

    void setPosition( float x, float y, float z );  // no meaning for a directional light
    void setPosition( Ogre::Vector3 pos );  // no meaning for a directional light
    void setDirection( float x, float y, float z);

    void setColor( float red, float green, float blue );

    void setSpecular( float red, float green, float blue );

    void setAttenuation( float range, float constant, float linear=1, float quadratic=0 );

    bool isVisible();
    void setVisible( bool active );

    void setCastShadows( bool castshadows );



    protected:
    Ogre::Light* mLight;
    Ogre::String mName;

};


class CDirectionalLight:public CLight {
    public:
    CDirectionalLight(Ogre::String name );
};



class CPointLight:public CLight {

    public:
    CPointLight( Ogre::String name, Ogre::SceneNode* node );
    ~CPointLight();

    private:
    Ogre::SceneNode* mSceneNode;  // parent SceneNode
    Ogre::SceneNode* mLightNode;
};


class CSpotLight:public CLight {

    public:

    CSpotLight(  Ogre::String name, Ogre::SceneNode* node );
    ~CSpotLight();
    void setAngle( float aAngle );
    void setRange( float innerAngle, float outerAngle, float falloff );

    private:
    Ogre::SceneNode* mSceneNode; // parent SceneNode
    Ogre::SceneNode* mLightNode;

};


#endif

