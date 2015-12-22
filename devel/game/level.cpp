

#include "level.h"

#include "logger.h"

#include "sqlite3x.hpp"

#include "levelinfo.h"


/**
 * Levels properties and object definitions are stored in a sqlite database.
 *
 * Game Objects all derived from Actor / PhysActor.
 *
 * There are gameplay objects which have a class on their own
 * and generic objects.
 *
 */


CLevel::CLevel( int aLevelID, bool editorMode  ):
    mSun(0),
    mTerrain(0),
    mShip(0),
    mSceneManager(0),
    mShowBBs(false),
    mLevelID( aLevelID ),
    mHeightMap(0),
    mTerrainTexture(0),
    mDetailTexture(0),
    mFogColor(0),
    mGravity(0),
    mCloudLayer(0),
    mCloudLayerHeight(0),
    mLevelName( "" ),
    mHasTerrain(false),
    mHasClouds(false),
    mEditorMode(editorMode)  {


    mSceneManager=CSceneManager::getInstance();

    mSceneManager->setAmbientColor(0.08,0.08,0.08);
    mSceneManager->setShadowOn();

    LOG("\nCLevel : Loading level... ");

    initObjectDef();
    LOG("\nCLevel : info : found %d object definitions", mObjectDefList.size() );

    load();

    LOG("\nCLevel : info : created %d objects", mGameObjectList.size() );


    LOG("\nCLevel : creating sun... ");
    initSun();



}


CLevel::~CLevel() {


    LOG("\n~CLevel : Deleting objects...");
    wipeObjects();
    LOG("done.");

    if( mSun ) {
        LOG("\n~CLevel : Deleting sun...");
        delete(mSun);
        LOG("done.");
    }



    if(mCloudLayer) {
        LOG("\n~CLevel : Deleting clouds...");
        delete(mCloudLayer);
        LOG("done.");
    }


    if( mTerrain ) {
        LOG("\n~CLevel : Deleting terrain ... ");
        delete(mTerrain);
        LOG("done.");
    }

    LOG("~CLevel : done.");
}



float CLevel::getCloudLayerHeight() {
    return mCloudLayerHeight;
}


/**
 * Count objects of type aType in the level.
 */
int CLevel::getNumberOf( int aType ) {

    int count=0;
    GameObjectList::iterator i;

    for( i=mGameObjectList.begin(); i!=mGameObjectList.end(); ++i ){
            if( (*i).type==aType ) count++;
    }
    // return( LandingPadList.size() );
    return( count );

}

/**
 * Get the first actor of type aType in the level .
 * Mainly useful for unique objects :/
 */
CActor* CLevel::getFirst( int aType ) {
    GameObjectList::iterator i;
    for(  i=mGameObjectList.begin(); i!=mGameObjectList.end(); ++i ){
            if( (*i).type==aType ) return( (*i).actor );
    }

    return(NULL);
}

/**
 * Returns a pointer to the ship instance.
 */
CShip* CLevel::getShip() {

    return( static_cast<CShip *>(getFirst( O_SHIP )) );
    // return(mShip);
}


/**
 * Returns a pointer to the terrain instance.
 */
CGround* CLevel::getTerrain() {
    return(mTerrain);
}


/**
 * Toggles bounding boxes of all game objects in the level .
 */
void CLevel::toggleBBs() {

    mShowBBs=!mShowBBs;

    GameObjectList::const_iterator i;
    for( i=mGameObjectList.begin(); i!= mGameObjectList.end(); ++i ) {
        (*i).actor->showBoundingBox( mShowBBs );
    }
}

/**
 * Activate the terrain's physics properties .
 */
void CLevel::initTerrainPhysics() {

    if(mTerrain) mTerrain->initPhysics();

}


/**
 * Destroy all game objects of the level .
 */
void CLevel::wipeObjects() {

    GameObjectList::iterator i;

    for( i=mGameObjectList.begin(); i!=mGameObjectList.end(); ++i ) {
        if( (*i).actor) delete( (*i).actor );
    }

    mGameObjectList.clear();
    mGameObjectList.empty();

    //LOG("\n\tDone deleting objects");

}

/**
 * New sun ( directional light + billboard ).
 */
void CLevel::initSun() {

    // should be a Camera Listener !!
    // mSun=new CSun( Ogre::Vector3( mTerrain->getXSize()/3.0, 4000, mTerrain->getZSize()/3.0 ), "sun/sun1", 400.0 );

    mSun=new CSun( Ogre::Vector3( 0.2, -1, 0.3 ), Ogre::ColourValue( 0.5,0.5,0.5,0 ), "sun/sun1", 500 );

}


float CLevel::getTerrainXWidth() {
    if( mTerrain ) return( mTerrain->getXSize() );
    return(0);
}

float CLevel::getTerrainZWidth() {
    if( mTerrain) return( mTerrain->getZSize() );
    return(0);
}



/**
 * Changes the terrain properties.
 *
 * Warning : If the terrain shape is changed, all objects in the level are detroyed .
 *
 */
 void CLevel::changeTerrain( int aHeightMap, int aTerrainTexture, int aDetailTexture ) {
    // wipe all objects, because they could be burried
    // or compute their new height ?

    bool heightMapChanged=false;

    if( mHasTerrain ) {

        if( mHeightMap != aHeightMap ) heightMapChanged=true;

        mHeightMap=aHeightMap;
        mTerrainTexture=aTerrainTexture;
        mDetailTexture=aDetailTexture;

        delete(mTerrain);

        LOG( "\nCLevel::changeTerrain : cground : heightmap %d / terraintexture %d / detailtexture %d",aHeightMap, aTerrainTexture, aDetailTexture );

        mTerrain=new CGround( mHeightMap, mTerrainTexture, mDetailTexture );

        LOG( "\nCLevel::changeTerrain : Terrain changed !");

        if( heightMapChanged ) {
            float y;
            Ogre::Vector3 pos;
            GameObjectList::iterator i;
            for( i=mGameObjectList.begin(); i!=mGameObjectList.end(); ++i ) {

                pos=(*i).actor->getPosition();
                y=mTerrain->getHeightAt( pos.x, pos.z );
                pos.y=y;
                (*i).actor->setPosition(pos);

            }
        }

    }

 }


/**
 * Changes the color of the fog...
 */
    void CLevel::setFogColor( int red, int green, int blue ) {
        mFogColor=(red<<16)+(green<<8)+(blue);
        mSceneManager->setFogOn( red, green, blue );
    }

/**
 * Changes the level gravity...
 */
    void CLevel::setGravity( float gravity) {
        mGravity=gravity;
    }


float CLevel::getGravity() {
    return( mGravity );
}

void CLevel::getFogColor( int &red, int &green, int &blue ) {
    red=(mFogColor&&0xFF000)>>16;
    green=(mFogColor&&0xFF00)>>8;
    blue=(mFogColor&&0xFF);
}

void CLevel::getTerrainParams( int &heightmap, int &terraintexture, int &detailtexture ) {

    heightmap=mHeightMap;
    terraintexture=mTerrainTexture;
    detailtexture=mDetailTexture;
}




/**
 * loads a level.
 */
bool CLevel::load() {

#define ID 0
#define NAME 1
#define MAP 2
#define TEX 3
#define DETAILTEX 4
#define FOG_COLOR 5
#define GRAVITY 6
#define HAS_TERRAIN 7
#define HAS_CLOUDS 8
#define CLOUD_LAYER_HEIGHT 9
#define GAMETYPE 10
#define GAMEORDER 11

    int num=-1;
    int object_type;
    Ogre::String name;

    mGameType=0;

    sqlite3x::sqlite3_connection con("../data/levels.db");
    //LOG("\nDatabse opened.");

    {
        // level parameters

        sqlite3x::sqlite3_command cmd ( con,
            "select id, name, map, terraintex, detailtex, \
            fogcolor, gravity, has_terrain, has_cloudlayer, \
            cloud_layer_height, gametype, gameorder from level where id=?;");
        cmd.bind( 1, mLevelID );
        sqlite3x::sqlite3_reader reader=cmd.executereader();

		reader.read();
		LOG("\nCLevel::load : level#%d => name: %s ", reader.getint(ID), reader.getstring(NAME).c_str() );

		num=reader.getint(ID);

		mGameType = reader.getint( GAMETYPE );
		mLevelIndex = reader.getint( GAMEORDER );

		//if( num>0 ) {
		    mLevelName=reader.getstring(NAME);

            if( reader.getint(HAS_TERRAIN) != 0 ) {
                LOG("\nCLevel::load : Creating the terrain." );
                mHasTerrain=true;
                mHeightMap=reader.getint(MAP);
                mTerrainTexture=reader.getint(TEX);
                mDetailTexture=reader.getint(DETAILTEX);
                mTerrain=new CGround( mHeightMap, mTerrainTexture, mDetailTexture );
            }

            mFogColor= reader.getint(FOG_COLOR);
		    LOG("\nCLevel::load : fog color = %#x ", mFogColor);
            setFogColor( (mFogColor&0xFF0000)>>16, (mFogColor&0xFF00)>>8, (mFogColor&0xFF) );

		    LOG("\nCLevel::load : gravity = %f ", reader.getdouble(GRAVITY) );
		    mGravity=reader.getdouble(GRAVITY);


		    if( reader.getint(HAS_CLOUDS) != 0 ) {
                mHasClouds=true;
		        LOG("\nCLevel::load : Creating the cloudlayer at %d", reader.getint(CLOUD_LAYER_HEIGHT) );
		        mCloudLayerHeight=reader.getint(CLOUD_LAYER_HEIGHT);

		        if( mHasTerrain ) {
                    mCloudLayer=new CClouds(
                        Ogre::Vector3( mTerrain->getXSize()/2, mCloudLayerHeight, mTerrain->getZSize()/2 ),
                        Ogre::Vector3( mTerrain->getXSize(), 10, mTerrain->getZSize() ),
                        1000  );
		        }
		        else {
		            mCloudLayer=new CClouds( Ogre::Vector3(0,0,0), Ogre::Vector3( 1000,10,1000 ), 1000 );
		        }
		    }

/*
		}
		else {
		    LOG("\n****** no level #%d : falling back to default", mLevelID );
            // mLevelID=0;
		    mTerrain=new CGround( 0,0,0 );
   		    setFogColor( 127,127,127);
   		    mGravity=-4.0;
		    setGravity( mGravity);

		}
*/
        // ====== objects

        sqlite3x::sqlite3_command cmdall ( con, "select type, xpos, ypos, zpos, yaw, scale from levelobjects where levelid=?;");
        cmdall.bind( 1, mLevelID );
        sqlite3x::sqlite3_reader readall=cmdall.executereader();
        while(readall.read() ) {
            // LOG("\nCLevel::load : object type %d at %d,%d,%d", readall.getint(0), readall.getint(1),readall.getint(2),readall.getint(3) );
            if( mEditorMode )
                addEditorObject(
                    readall.getint(0),
                    Ogre::Vector3( readall.getdouble(1), readall.getdouble(2), readall.getdouble(3)),
                    readall.getdouble(4),
                    readall.getdouble(5) );
            else
                addActor(
                    readall.getint(0),
                    Ogre::Vector3( readall.getdouble(1), readall.getdouble(2), readall.getdouble(3)),
                    readall.getdouble(4),
                    readall.getdouble(5) );

        }


    }

    con.close();

    return(true);
#undef ID
#undef NAME
#undef MAP
#undef DETAILTEX
#undef TEX
#undef FOG_COLOR
#undef GRAVITY
#undef HAS_TERRAIN
#undef HAS_CLOUDS
#undef CLOUD_LAYER_HEIGHT
#undef GAMETYPE
#undef GAMEORDER
}


/**
 *  saves the current level.
 * ( only called from the editor )
 */
void CLevel::save( int aGameType, int aLevelIndex ) {


    int object_type;
    float x,y,z;
    std::string levelname;

    mGameType = aGameType;
    mLevelIndex = aLevelIndex;

    switch( mGameType ) {
        case LANDING_GAME:
            levelname = "landing" + Ogre::StringConverter::toString( mLevelIndex);
        break;
        case RACING_GAME:
            levelname = "racing" + Ogre::StringConverter::toString( mLevelIndex);
        break;
        case TUTORIAL_GAME:
            levelname = "tutorial" + Ogre::StringConverter::toString( mLevelIndex);
        break;
        default:
            levelname = "level type " + Ogre::StringConverter::toString( mGameType) + " #" + Ogre::StringConverter::toString( mLevelIndex );
        break;
    }

            LOG("\n------------------------------------------------------");
            LOG("\nCLevel::save : Saving level %d", mLevelID);

    try {

        sqlite3x::sqlite3_connection con("../data/levels.db");
        //LOG("\nDatabse opened.");

		sqlite3x::sqlite3_transaction trans(con);
        //LOG("\nTransaction begun.");

		{
		    // level parameters
			sqlite3x::sqlite3_command cmdlevel ( con,
                "replace into level ( \
                id, name, gametype, gameorder, map, terraintex, detailtex, fogcolor, gravity, \
                has_terrain, has_cloudlayer, cloud_layer_height ) \
                values ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? );");
			cmdlevel.bind( 1, mLevelID );
			cmdlevel.bind( 2, levelname );
			cmdlevel.bind( 3, mGameType );
			cmdlevel.bind( 4, mLevelIndex );
            cmdlevel.bind( 5, mHeightMap );
            cmdlevel.bind( 6, mTerrainTexture );
            cmdlevel.bind( 7, mDetailTexture );
            cmdlevel.bind( 8, mFogColor );
            cmdlevel.bind( 9, mGravity );
            cmdlevel.bind( 10, mHasTerrain );
            cmdlevel.bind( 11, mHasClouds );
            cmdlevel.bind( 12, mCloudLayerHeight );
			cmdlevel.executenonquery();
//			cmdlevel.close();

            LOG("\nCLevel::save : level parameters saved");


            // deleting old objects
            sqlite3x::sqlite3_command cmdpurge ( con, "delete from levelobjects where levelid=?" );
            cmdpurge.bind( 1, mLevelID );
            cmdpurge.executenonquery();
            LOG("\nCLevel::save : levelobjects table purged");

            sqlite3x::sqlite3_command cmdobj ( con,
                "insert into levelobjects ( \
                levelid, type, xpos, ypos, zpos, yaw, scale ) \
                values ( ?,?,?,?,?,?,? );" );

            GameObjectList::const_iterator i;
            for( i=mGameObjectList.begin(); i!=mGameObjectList.end(); ++i ) {
                x=(*i).actor->getPosition().x;
                y=(*i).actor->getPosition().y;
                z=(*i).actor->getPosition().z;

                cmdobj.bind( 1, mLevelID );
                cmdobj.bind( 2, (*i).type );
                cmdobj.bind( 3, x );
                cmdobj.bind( 4, y );
                cmdobj.bind( 5, z );
                cmdobj.bind( 6, (*i).yaw );
                cmdobj.bind( 7, (*i).scale );

                cmdobj.executenonquery();
            }


		}
		trans.commit();
        // LOG("\ncommitted\n");

        con.close();

    }
    catch( std::exception &e) {
        LOG("\nCLevel::save : Exception while saving level : %s", e.what());
        throw "Exception while saving level";
    }


    LOG("\nCLevel::save : Save OK !\n---------------------------------------------");

}


void CLevel::createNew( int aLevelID ) {

    LOG("\n****** no level #%d : falling back to default", mLevelID );

    mHasTerrain = true;
    changeTerrain( 0,0,0 );

    wipeObjects();

    setFogColor( 0,0,0 );
    // setGravity( mGravity );

    /*
    delete( mCloudLayer );
    mCloudLayer = 0;
    mHasClouds = false;
    mCloudLayerHeight = 100;
    */
    mLevelID = aLevelID;

    // save();

}



/**
 * Adds a new actor instance to the object list.
 * Special object types ( with associated classes ) are selected here.
 */
void CLevel::addActor( int aType, Ogre::Vector3 pos, float yaw, float scale  ) {

   SGameObject tempobj;
    std::stringstream s;

    std::string name="object";
    std::string model="object.mesh";
    float defaultscale=1;
    int phystype;
    int material;
    bool destroyable;
    float life;
    bool special;


// find the base name and model to load
    ObjectDefList::iterator i;

    for( i=mObjectDefList.begin(); i!= mObjectDefList.end(); ++i ) {
        if( (*i).type==aType ) {
            model=(*i).model;
            name=(*i).name;
            defaultscale=(*i).scale;
            phystype=(*i).phystype;
            material=(*i).material;
            destroyable=(*i).destroyable;
            life=(*i).life;
            special=(*i).special;
            break;
        }

    }

    getUniqueName( name );

    //LOG("\n ===> adding %s ( type %d ) at %.2f,%.2f,%.2f  yaw=%.2f°, scale=%.2f", name.c_str(), aType, pos.x,pos.y,pos.z, yaw*180.0/3.1415, scale );

    tempobj.name=name;
    tempobj.special = special;

        switch(aType) {
            case O_SHIP:
                // if( getShip() != NULL ) return;  // if The ship already exists do nothing
                tempobj.actor=new CShip( name, pos );
                tempobj.special=true;
                break;
            case O_LANDINGPAD:
                tempobj.actor=new CLandingPad ( name, pos );
                tempobj.special=true;
                break;
            case O_RING:
                tempobj.actor=new CRing( name, pos, yaw );
                tempobj.special=true;
                break;
            case O_MINE:
                tempobj.actor=new CMine( name, pos );
                tempobj.special=true;
                break;
            case O_FAN:
                tempobj.actor=new CFan( name, pos, yaw );
                tempobj.special=true;
                break;
            case O_TURRET:
                tempobj.actor=new CTurret( name, pos );
                tempobj.special=true;
                break;
            case O_WINDMILL:
                tempobj.actor=new CWindmill( name, pos, yaw );
                tempobj.special=true;
                break;

            default:
                // addObject( name, model, x, y, z, scale );
                if( !scale ) scale=defaultscale;
                tempobj.yaw=yaw;
                tempobj.scale=scale;

                switch( phystype ) {

                    case 0:
                        tempobj.actor=new CDynamicPhysTrimesh( pos, scale, 2.0, 1.0, name, model );
                        break;
                    case 1:
                        tempobj.actor=new CDynamicPhysCube( pos, scale, 2.0, 1.0, name, model );
                        break;
                    case 2:
                        tempobj.actor=new CDynamicPhysSphere( pos, scale, 2.0, name, model );
                        break;
                    case 3:
                        tempobj.actor=new CDynamicPhysCylinder( pos, scale, 2.0, 1.0, name, model );
                        break;
                    case 5:
                        tempobj.actor=new CStaticPhysTrimesh( pos, yaw, scale , name, model );
                        break;
                    case 6:
                        tempobj.actor=new CStaticPhysCube( pos, yaw, scale , name, model );
                        break;

                    default:
                        tempobj.actor=new CStaticPhysTrimesh( pos, yaw, scale , name, model );
                        break;

                }

                tempobj.special=false;

                break;
      }


    tempobj.type=aType;

    tempobj.actor->castShadows();

    OgreOde::Geometry* geom=static_cast<CPhysActor*>(tempobj.actor)->getPhysGeom();

    //if( geom ) geom->setUserData( aType );
    // now UserData holds the material type for the object
    if( geom ) geom->setUserData( material );   // for collisions

    CActor* actor=static_cast<CActor*>(tempobj.actor);
    actor->setDestroyable( destroyable );
    actor->setLife( life );


    mGameObjectList.push_back( tempobj );

    //LOG(" ... done.");
}

// ---------------------------------------------------------------------------

/// Special method for adding objects in the level editor
void CLevel::addEditorObject(int aType, Ogre::Vector3 aPos, float aYaw, float aScale ) {

    SGameObject tempobj;
    std::stringstream s;

    std::string name="object";
    std::string model="object.mesh";
    float scale;
    bool special;

// find the base name and model to load
    ObjectDefList::iterator i;

    for( i=mObjectDefList.begin(); i!= mObjectDefList.end(); ++i ) {
        if( (*i).type==aType ) {
            model=(*i).model;
            name=(*i).name;
            scale=(*i).scale;
            special=(*i).special;
            break;
        }

    }

    getUniqueName( name );

    tempobj.name=name;

    // if( aScale <= 0 ) tempobj.scale = scale;
    // else
        tempobj.scale = aScale;

    tempobj.yaw = aYaw;

    tempobj.special=special;
    tempobj.type=aType;

    tempobj.actor=new CActor( name, model );
    tempobj.actor->setPosition( aPos );
   // tempobj.actor->setScale( scale, scale, scale );
    tempobj.actor->scale( tempobj.scale, tempobj.scale, tempobj.scale );
    //tempobj.actor->scale( scale, scale, scale );
    tempobj.actor->setYaw( aYaw );
    tempobj.actor->castShadows();

    mGameObjectList.push_back( tempobj );

}

CActor* CLevel::getActor( const Ogre::String aName ) {

    CLevel::GameObjectList::const_iterator o;
    for(o=mGameObjectList.begin(); o!=mGameObjectList.end(); ++o ) {
        if( (*o).name == aName ) return( (*o).actor);
    }
    return( NULL);
}


// obtain a unique name
void CLevel::getUniqueName(std::string &name) {
        std::stringstream s;
        int n=0;

        do {
            s.str("");
            s << name << n;
            n++;

//           LOG( "===> %s <===, %d \n", s.str().c_str(), CSceneManager::getInstance()->getOgreSceneManager()->hasSceneNode(s.str()) );


        } while ( CSceneManager::getInstance()->hasEntity(s.str()) );

    name=s.str();
    LOG( "===> %s <===\n", name.c_str());
}


/**
 * Populates the object definition list.
 * Each object type have some properties which are stored in memory.
 * Direct mapping of the objectdef table from the sqlite database.
 */

void CLevel::initObjectDef() {

    SObjectDef def;

    LOG("\nCLevel::initObjectDef : Reading objects definition...");

    sqlite3x::sqlite3_connection con("../data/levels.db");


    {
        sqlite3x::sqlite3_command cmd ( con,
            "select type, name, scale, model, phystype, material, destroyable, life, special from objectdef;");

        sqlite3x::sqlite3_reader reader=cmd.executereader();
        while(reader.read() ) {

            def.type=reader.getint(0);
            def.name=reader.getstring(1);
            def.scale=reader.getdouble(2);
            def.model=reader.getstring(3);
            def.phystype=reader.getint(4);
            def.material=reader.getint(5);
            def.destroyable=(bool)reader.getint(6);
            def.life=(float)reader.getint(7);
            def.special=(bool)reader.getint(8);
           // LOG("\n\t => %s ( model %s ) : type %d ", def.name.c_str(), def.model.c_str(), def.type );
            mObjectDefList.push_back(def);
        }


    }


    con.close();

    //LOG("\t\nDone.");
    LOG(" Done.");
}


