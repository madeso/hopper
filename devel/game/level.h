

#ifndef _CLEVEL_H_
#define _CLEVEL_H_

#include "ground.h"
#include "ship.h"

// gameplay objects
#include "landingpad.h"
#include "ring.h"
#include "fan.h"
#include "turret.h"
#include "mine.h"
#include "sun.h"
#include "windmill.h"

#include "scenemanager.h"

#include "clouds.h"

// #include "hopedit.h"


#define MAX_LANDINGPADS 10
#define MAX_RINGS 10

#include "objecttypes.h"
#include "singleton.h"
#include <vector>


/// a game object.
struct SGameObject  {

    int type;           // object definition id
    std::string name;
    // CPhysActor* actor;
    CActor* actor;      // object instance ref
    float yaw;
    float scale;
    bool special;       // if true : can't be scaled in the editor

};

/// a game object definition.
struct SObjectDef  {

    int type;
    std::string name;
    float scale;
    std::string model;
    int phystype;
    int material;
    bool destroyable;
    float life;
    bool special;

};


/// Levels loading/saving and manages level objects.
class CLevel : public SSingleton<CLevel> {

    friend class CLevelEditor;

public:

    CLevel(); // random
    CLevel(int aLevelID, bool editorMode = false );

    ~CLevel();

    CGround* getTerrain();
    CShip* getShip();

    int getNumberOf(int aType);

    void toggleBBs();

    void initTerrainPhysics();

    typedef std::vector<SGameObject > GameObjectList;
    typedef std::vector<SObjectDef > ObjectDefList;

    ObjectDefList mObjectDefList;
    GameObjectList mGameObjectList;

    float getTerrainXWidth();
    float getTerrainZWidth();

    float getCloudLayerHeight();

    CActor* getActor( const Ogre::String aName );

protected:
    void save( int aGameType, int aLevelIndex );
    bool load();

    void createNew( int aLevelID );

    //void addShip(float x, float y, float z);
    //void addLandingPad( float x, float y, float z);
    //void addRing( float x, float y, float z);


    void changeTerrain( int aHeightMap, int aTerrainTexture, int aDetailTexture );
    void setFogColor( int red, int green, int blue );
    void setGravity( float gravity);

    float getGravity();
    void getFogColor( int &red, int &green, int &blue );
    void getTerrainParams( int &heightmap, int &terraintexture, int &detailtexture );

    void addActor( int type, Ogre::Vector3 pos, float yaw, float scale );

    void addEditorObject(int aType, Ogre::Vector3 aPos, float aYaw, float aScale );

private:
    CSceneManager* mSceneManager;

    CSun* mSun;

    CGround* mTerrain;
    CShip* mShip;

    bool mShowBBs;

    int mGameType;
    int mLevelIndex;
    int mLevelID;

    // Terrain stuffs
    int mHeightMap;
    int mTerrainTexture;
    int mDetailTexture;

    // Level options
    int mFogColor;
    float mGravity;
    Ogre::String mLevelName;

    bool mHasTerrain;
    bool mHasClouds;


    CClouds* mCloudLayer;
    float mCloudLayerHeight;

    void initTerrain();
    void initSun();

    void initObjectDef();

    void wipeObjects();

    CActor* getFirst( int aType );

    void getUniqueName( std::string &name);

    bool mEditorMode;  // if true, objects are added/created as basic CActors

};




#endif
