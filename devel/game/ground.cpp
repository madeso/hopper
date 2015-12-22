

#include "ground.h"

#include "logger.h"
#include "resourcemanager.h"
#include "physicobject.h"
#include "hoppercollisionmaterials.h"

CGround::CGround(int aHeightMap, int aTerrainTexture, int aDetailTexture ) {


    mNodes_per_side=513;
  //  mNodes_per_side=1025;
  //  mNodes_per_side=2049;
    mScale.x=2048;
    mScale.z=2048;
    mScale.y=256;

    LOG("\nCGround::CGround() : new Terrain ");

    mHeightMap=aHeightMap;
    mTerrainTexture=aTerrainTexture;
    mDetailTexture=aDetailTexture;

    mPhysicsManager=CPhysicsManager::getInstance();

    mOgreTerrainSceneManager=CSceneManager::getInstance()->getTerrainSceneManager();

    mPhysTerrain=0;

    initTerrain();


/*
    String terrain_file;
    terrain_file="terrain" + Ogre::StringConverter::toString( aLevelNum) + ".cfg";
    mOgreTerrainSceneManager->setWorldGeometry( terrain_file );
*/



//        initPhysics();


}



CGround::~CGround() {

    // CLogger::getInstance()->log("\n pouet ");

    if(mPhysTerrain )  delete(mPhysTerrain);

    // CLogger::getInstance()->log("\n tut ");



/*
    CLogger::getInstance()->log("\n pouet ");

    Ogre::MaterialPtr mp=mOgreTerrainSceneManager->getTerrainMaterial();
    if( ! mp.isNull() ) {
        CLogger::getInstance()->log("\n pouet ");
        mp->unload();
            CLogger::getInstance()->log("\n pouet ");
    }
*/


//    mOgreTerrainSceneManager->clearScene(); // destroys everything including cameras

//    mOgreTerrainSceneManager->shutdown();



}



float CGround::getHeightAt( float x, float y) {


 //   return(0);
    return (CSceneManager::getInstance()->getHeightAt( x, y));
    // return( mOgreTerrainSceneManager->getHeightAt( x,y ));
    // return(mPhysTerrain->getHeightAt( Ogre::Vector3(x, 0 ,y ) ) );


}

fptr CGround::getHeightAtPtr() {
    return( &getHeightAt );
}


Ogre::Real CGround::heightAt( const Ogre::Vector3 &position) {

    return(mPhysTerrain->getHeightAt( position ) );
    // return( mOgreTerrainSceneManager->getHeightAt( position.x, position.z ));

}


float CGround::getXSize() {
    return( mScale.x );
}

float CGround::getZSize() {
    return(mScale.z);
}

float CGround::getMaxHeight() {
    return( mScale.y );
}



void CGround::initTerrain() {

//    Ogre::String terrain_string;
    unsigned char buff[2048];
    int buffsize;

    std::stringstream terrain_string;

    terrain_string << "WorldTexture = terraintexture" << Ogre::StringConverter::toString( mTerrainTexture ) << ".png" << "\n";
    terrain_string << "DetailTexture = terraindetail" << Ogre::StringConverter::toString( mDetailTexture ) << ".png" << "\n";
    terrain_string << "DetailTile=" << "4" << "\n";
    terrain_string << "PageSource=" << "Heightmap" << "\n";
    terrain_string << "Heightmap.image= terrainmap" << Ogre::StringConverter::toString( mHeightMap ) << ".png" << "\n";
   // terrain_string << "CustomMaterialName=terrain/" << Ogre::StringConverter::toString( mTerrainTexture ) << "\n";
   // terrain_string << "Heightmap.raw.size=" << Ogre::StringConverter::toString( mNodes_per_side ) << "\n";
   // terrain_string << "Heightmap.raw.bpp=1"  << "\n";

  /*
# If you use RAW, fill in the below too
# RAW-specific setting - size (horizontal/vertical)
Heightmap.raw.size=257
# RAW-specific setting - bytes per pixel (1 = 8bit, 2=16bit)
Heightmap.raw.bpp=2
# Use this if you want to flip the terrain (eg Terragen exports raw upside down)
#Heightmap.flip=true
  */

    terrain_string << "PageSize="<< Ogre::StringConverter::toString( mNodes_per_side ) << "\n";
    terrain_string << "TileSize="<< "33" << "\n";
    terrain_string << "MaxPixelError=" << "3" << "\n";
    terrain_string << "PageWorldX=" << Ogre::StringConverter::toString( mScale.x ) << "\n";
    terrain_string << "PageWorldZ="<< Ogre::StringConverter::toString( mScale.z ) << "\n";
    terrain_string << "MaxHeight=" << Ogre::StringConverter::toString( mScale.y ) <<"\n";
    terrain_string << "MaxMipMapLevel=" << "1" << "\n";
    terrain_string << "VertexNormals=yes\n";
    terrain_string << "VertexColors=yes\n";
    terrain_string << "UseTriStrips=no\n";
    terrain_string << "LODMorphStart=0.1\n";


    buffsize=terrain_string.str().length()+1;
    memcpy( buff, terrain_string.str().c_str(), buffsize  );

/*
    CLogger::getInstance()->log("\n\nInitialising Terrain with :\n" );
    CLogger::getInstance()->log("----------------------------------------------\n" );
    CLogger::getInstance()->log("%s",buff ); // terrain_string.str().c_str() );
    CLogger::getInstance()->log("----------------------------------------------\n" );
*/

    Ogre::DataStreamPtr terrain_stream(
                new Ogre::MemoryDataStream( buff, buffsize ));


    mOgreTerrainSceneManager->setWorldGeometry( terrain_stream );


}



void CGround::initPhysics() {

    /*
    mNodes_per_side=mOgreTerrainSceneManager->getPageSize();
    mScale=mOgreTerrainSceneManager->getScale();

    float tilesize=mOgreTerrainSceneManager->getTileSize();

   CLogger::getInstance()->log("\nTerrain : mScale = %f,%f,%f / mNodes_per_side = %f, tilesize=%f \n",
                            mScale.x, mScale.y, mScale.z, mNodes_per_side, tilesize );
*/


    // ================ Ode terrain init

    Ogre::Vector3 OdeScale;

    OdeScale.x=mScale.x/(float)(mNodes_per_side-1);
    OdeScale.y=mScale.y;
    OdeScale.z=mScale.z/(float)(mNodes_per_side-1);


/*
WARNING
ground.cpp:58: warning: passing `float' for converting 3 of
    `OgreOde::TerrainGeometry::TerrainGeometry(OgreOde::Space*, const Ogre::Vector3&, int, bool, Ogre::Real)'
*/

    CLogger::getInstance()->log("\nOde Terrain : mScale = %f,%f,%f / mNodes_per_side = %f \n",
                                            OdeScale.x, OdeScale.y, OdeScale.z, mNodes_per_side );
/*
    mPhysTerrain = new OgreOde::TerrainGeometry(
                CSceneManager::getInstance()->getPhysicsWorld()->getDefaultSpace(),
											OdeScale,
											mNodes_per_side,
											false);// true if plsm2
*/
// LOG("\n******* tut");

    mPhysTerrain = new OgreOde::TerrainGeometry(
        mPhysicsManager->getPhysicsWorld(),
        mPhysicsManager->getPhysicsSpace(),
        OdeScale,
        (int)mNodes_per_side, (int)mNodes_per_side,
        mScale.x, mScale.z,
        false /* centered */, 100 /*thickness*/ );


// LOG("\n*******pouet");

    mPhysTerrain->setUserData( MAT_TERRAIN );
    //mPhysTerrain->setMaxContacts( 100 );
    // recent version of OgreOde calls itself if no listener :
   // mPhysTerrain->setHeightListener(this);  // call back on heightAt();




}
