

#include "resourcemanager.h"
#include "hoppersounds.h"
#include "logger.h"

#include "particleemittermanager.h"
#include "hopperparticles.h"

//TODO: This is only a resource loader : make it a real resource manager or move it to the game part.
// this is game dependent.

void CResourceManager::loadAll() {


        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../data/terrains","FileSystem","Terrain");
        // Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./data","FileSystem","General");
    	// Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./data/materials/programs","FileSystem","General");

        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../data/DynaGUI","FileSystem","GUI");

	    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../data/materials/scripts","FileSystem","General");
	    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../data/materials/textures","FileSystem","General");
	    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../data/models","FileSystem","General");
	    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../data/fonts","FileSystem","General");
        // Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./data/overlays","FileSystem","General");


        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../data/particles","FileSystem","General");

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();



        loadSounds();

//        initParticleSystems(); // they are initialised with each scenemanager construction

}


void CResourceManager::initParticleSystems() {
    CParticleEmitterManager::getInstance()->addEmitterType( PART_DARK_SMOKE, "darksmoke" );
    CParticleEmitterManager::getInstance()->addEmitterType( PART_TERRAIN_SMOKE, "smoke" );
    CParticleEmitterManager::getInstance()->addEmitterType( PART_COLLISION_SPARKS, "colsparks" );
    CParticleEmitterManager::getInstance()->addEmitterType( PART_THRUSTER_SPARKS, "thrustersparks" );
    CParticleEmitterManager::getInstance()->addEmitterType( PART_EXPLOSION, "explosion" );
    CParticleEmitterManager::getInstance()->addEmitterType( PART_AIRDUST, "airdust" );
    CParticleEmitterManager::getInstance()->addEmitterType( PART_PROJECTILE_HIT, "projectile" );
    CParticleEmitterManager::getInstance()->addEmitterType( PART_THRUSTER_SMOKE, "thruster_smoke" );
}



void CResourceManager::loadSounds() {
        CSoundManager::getInstance()->load( SOUND_THRUST, "../data/sounds/thrust.wav");
        CSoundManager::getInstance()->load( SOUND_THRUSTLOOP, "../data/sounds/thrust_loop.wav");
        CSoundManager::getInstance()->load( SOUND_GROUND_HIT, "../data/sounds/bonk.wav");
        CSoundManager::getInstance()->load( SOUND_LANDED, "../data/sounds/landed.wav");
        CSoundManager::getInstance()->load( SOUND_EXPLODE, "../data/sounds/explode.wav");
        CSoundManager::getInstance()->load( SOUND_REFUEL, "../data/sounds/refuel.wav");
        CSoundManager::getInstance()->load( SOUND_REFUEL_OK, "../data/sounds/heather_fuel_full.wav");
        CSoundManager::getInstance()->load( SOUND_OUTOFFUEL, "../data/sounds/heather_outoffuel.wav");
        CSoundManager::getInstance()->load( SOUND_SCRATCH, "../data/sounds/scratch.wav");
        CSoundManager::getInstance()->load( SOUND_FREEZING, "../data/sounds/freezing.wav");
        CSoundManager::getInstance()->load( SOUND_FAN, "../data/sounds/fan.wav");
        CSoundManager::getInstance()->load( SOUND_WRONGRING, "../data/sounds/wrong.wav");
        CSoundManager::getInstance()->load( SOUND_MOUSECLICK, "../data/sounds/down.wav");
        CSoundManager::getInstance()->load( SOUND_PLASTIC, "../data/sounds/hit_plastic.wav");
        CSoundManager::getInstance()->load( SOUND_METAL_HIT, "../data/sounds/metal_hit.wav");
        CSoundManager::getInstance()->load( SOUND_FUEL_LOW, "../data/sounds/heather_fuel_low.wav");
        CSoundManager::getInstance()->load( SOUND_HULL_CRITICAL, "../data/sounds/heather_hull_critical.wav");
        CSoundManager::getInstance()->load( SOUND_UNFREEZING, "../data/sounds/unfreeze.wav");
        CSoundManager::getInstance()->load( SOUND_PULSE, "../data/sounds/flak.wav");
        CSoundManager::getInstance()->load( SOUND_PROJECTILE_HIT, "../data/sounds/projectile_hit.wav");

        CSoundManager::getInstance()->load( SOUND_PROXMINE, "../data/sounds/proxmine.wav");
        CSoundManager::getInstance()->load( SOUND_TURRET, "../data/sounds/turet2.wav");
        CSoundManager::getInstance()->load( SOUND_WINDMILL, "../data/sounds/windm1.wav");

        CSoundManager::getInstance()->load( SOUND_THREE, "../data/sounds/heather_three.wav");
        CSoundManager::getInstance()->load( SOUND_TWO, "../data/sounds/heather_two.wav");
        CSoundManager::getInstance()->load( SOUND_ONE, "../data/sounds/heather_one.wav");
        CSoundManager::getInstance()->load( SOUND_GO, "../data/sounds/heather_go.wav");

        CSoundManager::getInstance()->load( SOUND_TRACTORBEAM, "../data/sounds/tractorbeam.wav" );
}






void CResourceManager::printResources(Ogre::String aResourceGroup ) {

    Ogre::StringVector list= *Ogre::ResourceGroupManager::getSingleton().listResourceNames ( aResourceGroup ) ;

    CLogger::getInstance()->log("\n----------------------------------------------------------" );
    CLogger::getInstance()->log("\nresource group \"%s\"has %d elements :\n", aResourceGroup.c_str(), list.size() );

    for(int i=0; i<list.size(); i++ ) {
        CLogger::getInstance()->log("\n+++ %s", list[i].c_str()) ;
    }
    CLogger::getInstance()->log("\n----------------------------------------------------------" );

}


void CResourceManager::printAllResources() {

    //Ogre default resources
    printResources( Ogre::ResourceGroupManager::getSingleton().AUTODETECT_RESOURCE_GROUP_NAME );
    // printResources( Ogre::ResourceGroupManager::getSingleton().BOOTSTRAP_RESOURCE_GROUP_NAME );
    printResources( Ogre::ResourceGroupManager::getSingleton().INTERNAL_RESOURCE_GROUP_NAME );


    //printResources( Ogre::ResourceGroupManager::getSingleton().DEFAULT_RESOURCE_GROUP_NAME  );
    //printResources( Ogre::ResourceGroupManager::getSingleton().getWorldResourceGroupName() );

    //Game explicit resouces
    printResources( "Terrain" );
    printResources( "General" );

}

void CResourceManager::loadSplash() {
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../data/materials/splash","FileSystem","Splash");
    Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Splash");
   // printResources( "Splash" );
}


void CResourceManager::unloadSplash() {
    Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("Splash");
}


/*
void CResourceManager::printResourceInfo( Ogre::String aResourceName ) {

    Ogre::Resource r=*Ogre::ResourceGroupManager::getSingleton().getByName(aResourceName);

    CLogger::getInstance()->log("\n Resource :\"%s\"  : group \"%s\" ", aResourceName.c_str(), r.getGroup().c_str() );


}
*/

/*
void CResourceManager::clearTerrain() {

     Ogre::ResourceGroupManager::getSingleton().clearResourceGroup("Hoper Scene Manager/Terrain");

}
*/



/*
CResourceManager::CResourceManager() {



}

IResource* CResourceManager::getResource( string aResourceName ) {

    map<string,IResource*>::iterator iter;

    iter= mResourceMap.find(aResourceName);
    if(iter != mResourceMap.end() ) return(iter->second);
    else return(NULL);


}
*/
