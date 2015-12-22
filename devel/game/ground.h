

#ifndef _GROUND_
#define _GROUND_

// #include "actor.h"

#include "physicsmanager.h"
#include "scenemanager.h"


typedef float (* fptr)(float a, float b);


/// the terrain : from a heightmap.

class CGround : public OgreOde::TerrainGeometryHeightListener {

    public:

    CGround( int aHeightmap, int aTexture, int aDetailtexture );
    ~CGround();
    static float getHeightAt(float x, float y );
    Ogre::Real heightAt( const Ogre::Vector3 &position);

    float getXSize();
    float getZSize();
    float getMaxHeight();
    fptr getHeightAtPtr();

    void initPhysics();

    private:

    CPhysicsManager* mPhysicsManager;
    OgreOde::TerrainGeometry *mPhysTerrain;
    Ogre::Vector3 mScale;
    float mNodes_per_side;
    Ogre::TerrainSceneManager *mOgreTerrainSceneManager;
    void initTerrain();
    // void initPhysTerrain();
    int mHeightMap;
    int mTerrainTexture;
    int mDetailTexture;

};



#endif

