
#ifndef _CSCENEMANAGER_
#define _CSCENEMANAGER_
#include "singleton.h"
#include "camera.h"
// #include "Ogre.h"
#include "OgreOde_Core.h"
// #include "OgreOde_Prefab.h"
#include "OgreTerrainSceneManager.h"
#include "OgreHeightmapTerrainPageSource.h"

/// manages the scene ( camera, fog, shadow, gravity, light,... ).
class CSceneManager: /* public Ogre::TerrainSceneManager, */ public SSingleton<CSceneManager> {

    private:
        Ogre::SceneManager* mOgreSceneManager;
        Ogre::TerrainSceneManager* mSceneManager;
        CCamera* mCamera;
   //     OgreOde::World* mPhysicsWorld;
   //     OgreOde::SimpleSpace* mPhysicsSpace;

        // Ogre::HeightmapTerrainPageSource* mHeightmapPageSource;

    public:
        CSceneManager();
        ~CSceneManager();

        Ogre::SceneManager* getOgreSceneManager();
        Ogre::TerrainSceneManager* getTerrainSceneManager();

        float getHeightAt( float x, float y );

        Ogre::SceneNode* getRootSceneNode();
        Ogre::Entity* createEntity( Ogre::String name, Ogre::String modelname );

        bool hasEntity( Ogre::String name );

        void destroyEntity( Ogre::Entity* e );
        Ogre::Camera* createCamera( Ogre::String name );
        void destroyCamera( Ogre::Camera* camera );
        void destroyBillboardSet(Ogre::BillboardSet* billboardset );
        void destroyParticleSystem( Ogre::ParticleSystem* particlesystem );

        Ogre::ParticleSystem* createParticleSystem( Ogre::String name, Ogre::String type );

        Ogre::Light* createLight( Ogre::String name );
        void destroyLight( Ogre::Light* light );

        bool getShowBoundingBoxes();
        void showBoundingBoxes( bool show );

        // void addSceneNode(Ogre::SceneNode* aSceneNode);

        void setShadowOn();
        void setFogOn(int red, int greeen, int blue );
        //void setGravity( float gravity );
        void setAmbientColor( float red, float green, float blue );
   /*
        CCamera *getCamera();
        CCamera *createCamera();
        bool hasCamera();
    */


     //   OgreOde::World* getPhysicsWorld();
     //   OgreOde::Space* getPhysicsSpace();

 //       void clearScene();
        void setFogOff();
        Ogre::BillboardSet* getNewBillBoardSet( Ogre::String name);

/*
        bool init();
        bool CSceneManager::initPhysics();
*/


};


#endif
