


#include "light.h"

#include "scenemanager.h"
#include "logger.h"


CLight::CLight(Ogre::String aName) {  // :CActor() {

        mName=aName;

        // LOG("\n ************ CLight() %s", mName.c_str() );

        mLight = CSceneManager::getInstance()->createLight(mName);

}


CLight::~CLight() {
    CSceneManager::getInstance()->destroyLight( mLight );
}

void CLight::setDirection( float x, float y, float z ) {
    mLight->setDirection( x,y,z);
}

void CLight::setPosition( float x, float y, float z ) {
    mLight->setPosition( x,y,z);
}

void CLight::setPosition( Ogre::Vector3 aPosition ) {
    mLight->setPosition( aPosition );
}

void CLight::setColor( float red, float green, float blue ) {
    mLight->setDiffuseColour( red, green, blue );
}


void CLight::setSpecular( float red, float green, float blue ) {
    mLight->setSpecularColour( red, green, blue );
}


void CLight::setAttenuation( float range, float constant, float linear, float quadratic ) {
    mLight->setAttenuation( range, constant, linear, quadratic );
}

bool CLight::isVisible() {
    return( mLight->isVisible());
}

void CLight::setVisible(bool visible ) {
    mLight->setVisible( visible );
}

void CLight::setCastShadows( bool cast ) {
        mLight->setCastShadows( cast );
}

CDirectionalLight::CDirectionalLight(Ogre::String name):CLight( name ) {
			mLight->setType(Ogre::Light::LT_DIRECTIONAL);
            mLight->setDirection( 0.2,-1.0, 0.3 );
			mLight->setDiffuseColour(Ogre::ColourValue( 0.5,0.5,0.5) );
			mLight->setSpecularColour( 1,1,1 );
}



CPointLight::CPointLight(Ogre::String aName, Ogre::SceneNode* aSceneNode  ):CLight(aName ) {

    mSceneNode=aSceneNode;
    mLightNode=mSceneNode->createChildSceneNode( mName+"_node", Ogre::Vector3(0,0,0));

    mLight->setType( Ogre::Light::LT_POINT );

    mLight->setDiffuseColour( 1.0, 1.0, 1.0 );
    mLight->setSpecularColour( 1,1,1 );
    mLight->setCastShadows(false);
    mLight->setAttenuation ( 20.0, 1.0, 0.1, 0.0 );


    //LOG("\n attaching light '%s' to lightNode '%s' ... ", mLight->getName().c_str(), mLightNode->getName().c_str() );
    mLightNode->attachObject( mLight );
    //LOG("done.");
   // mLight->setPowerScale( 0.01 );

}


CPointLight::~CPointLight() {

    // FIXME: is this really working ?
    mSceneNode->removeAndDestroyChild( mName+"_node" );
}


CSpotLight::CSpotLight(Ogre::String aName, Ogre::SceneNode* aSceneNode  ):CLight( aName) {

    mSceneNode=aSceneNode;
    mLightNode=mSceneNode->createChildSceneNode( mName+"_node", Ogre::Vector3(0,0,0));

    mLight->setType( Ogre::Light::LT_SPOTLIGHT );
    mLight->setDirection( Ogre::Vector3(0,-1,0) );
    mLight->setSpotlightOuterAngle  (  Ogre::Radian(3.1415/2.0)  ) ;
    mLight->setDiffuseColour( 1.0, 1.0, 1.0 );
    mLight->setSpecularColour( 1,1,1 );
    mLight->setCastShadows(false);
    mLight->setAttenuation ( 5.0, 0.5, 0.1, 0.0 );

    mLightNode->attachObject( mLight );

//        mSceneNode->attachObject( mLight );

}

CSpotLight::~CSpotLight() {
    // FIXME: is this really working ?
    mSceneNode->removeAndDestroyChild( mName+"_node" );
}

/// set spot angle : in degrees
void CSpotLight::setAngle( float aAngle ) {
    mLight->setSpotlightOuterAngle  (  Ogre::Radian(aAngle * 3.1415 / 180.0 )  ) ;
}

void CSpotLight::setRange( float innerAngle, float outerAngle, float falloff ) {
    mLight->setSpotlightRange(
        Ogre::Radian( innerAngle * 3.1415 / 180.0),
        Ogre::Radian( outerAngle * 3.1415 / 180.0),
        falloff );
}





