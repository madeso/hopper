

#include "particleemittermanager.h"
#include "scenemanager.h"
#include "logger.h"

CParticleEmitterManager::CParticleEmitterManager() {
    mCount=0;
    mEmitterTypes.resize( 10 );  // huh ?
    mDefaultPools.resize( 10 );
    mDefaultPools.assign( 10, static_cast<CParticleEmitterPool*>(0) );

}


CParticleEmitterManager::~CParticleEmitterManager() {

    LOG("\nParticleEmitterManager : destroying..." );

    mEmitterTypes.clear();


    for( std::list<CParticleEmitter*>::iterator it=mEmitters.begin(); it != mEmitters.end(); ++it ) {
        delete( (*it) );
    }
    mEmitters.clear();

/*
    std::map< Ogre::String, CParticleEmitterPool* >::iterator it;
    for( it = mEmitterPools.begin(); it != mEmitterPools.end(); ++it ) {
        delete( (*it) );
    }
    mEmitterPools.clear();
*/

    std::vector<CParticleEmitterPool*>::iterator it;
    for( it = mDefaultPools.begin(); it != mDefaultPools.end(); ++it ) {
        if( (*it) )
            delete( (*it) );
    }
    mDefaultPools.clear();

    LOG("\n done." );

}


void CParticleEmitterManager::addEmitterType( unsigned int emitterTypeID, Ogre::String aType ) {


    if( emitterTypeID > 10 && emitterTypeID > mEmitterTypes.size() ) {
        mEmitterTypes.resize( emitterTypeID+1 );
        mDefaultPools.resize( emitterTypeID+1 );
    }

    mEmitterTypes.at(emitterTypeID) = aType;


    // createPool( "DefaultPool#" + emitterTypeID, emitterTypeID, 10 );

    mDefaultPools.at(emitterTypeID) = new CParticleEmitterPool(
        "DefaultPool#" + emitterTypeID,
        mEmitterTypes[emitterTypeID],
         10 );

}


Ogre::String CParticleEmitterManager::getTypeName( unsigned int emitterTypeID ) {
    return( mEmitterTypes.at( emitterTypeID ));
}




void CParticleEmitterManager::emit(
    unsigned int emitterTypeID, unsigned int numParticles, Ogre::Vector3 position ) {

    mDefaultPools[emitterTypeID]->emit( numParticles, position );
    // CParticleEmitterPool* pool=getDefaultPool( emitterTypeID );
    // if( pool ) pool->emit( numParticles, position );
}

/*
CParticleEmitterPool* CParticleEmitterManager::getDefaultPool( unsigned int emitterTypeID ) {

    std::map< Ogre::String, CParticleEmitterPool* >::const_iterator it;

    it=mEmitterPools.find( "DefaultPool#"+ emitterTypeID );
    if( it != mEmitterPools.end() )
        return( (*it).second );
    else
        return( 0 );

}



void CParticleEmitterManager::destroyPool( Ogre::String name ) {

    std::map< Ogre::String, CParticleEmitterPool* >::iterator it;
    it=mEmitterPools.find( name );

    if( it != mEmitterPools.end() ) {
        delete( (*it).second );
        mEmitterPools.erase( it );
    }

}

CParticleEmitterPool* CParticleEmitterManager::createPool(
    Ogre::String name, unsigned int emitterTypeID, unsigned int maxEmitters ) {

    Ogre::String type=mEmitterTypes[emitterTypeID];

    CParticleEmitterPool* pool = new CParticleEmitterPool( name, type, maxEmitters );

    mEmitterPools.insert( std::pair<Ogre::String, CParticleEmitterPool*>( name, pool ) );

    return( pool );

}



*/


// TODO : emitter can be of three types : ponctual (emit(false) after update()), continuous or with a fixed duration.

CParticleEmitter* CParticleEmitterManager::createEmitter( unsigned int emitterTypeID, Ogre::SceneNode* node ) {




    Ogre::String type=mEmitterTypes[emitterTypeID];
    Ogre::String name=type + Ogre::StringConverter::toString(mCount++);

    CParticleEmitter* emitter;

    if( node )
        emitter = new CParticleEmitter( name, type, node );
    else
        emitter = new CParticleEmitter( name, type );

    mEmitters.push_back( emitter );

    return( emitter );
}



void CParticleEmitterManager::destroyEmitter( CParticleEmitter* emitter ) {

    mEmitters.remove( emitter );
    delete( emitter );

}


void CParticleEmitterManager::update() {

    // update single emitters
    for( std::list< CParticleEmitter* >::const_iterator it = mEmitters.begin(); it != mEmitters.end(); ++it ) {
        (*it)->update();
        (*it)->emit(false);
    }

    // update default pools
    std::vector<CParticleEmitterPool*>::const_iterator it;
    for( it = mDefaultPools.begin(); it != mDefaultPools.end(); ++it ) {
        if(*it) (*it)->update();
    }

/*
    // update other pools
    std::map< Ogre::String, CParticleEmitterPool* >::const_iterator it;
    for( it = mEmitterPools.begin(); it != mEmitterPools.end(); ++it ) {
        (*it).second->update();
    }
*/
}


