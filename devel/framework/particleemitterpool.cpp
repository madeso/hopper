


#include "particleemitterpool.h"
// #include "particleemittermanager.h"

CParticleEmitterPool::CParticleEmitterPool(
    Ogre::String name,Ogre::String emitterType, unsigned int maxEmitters )
    :mName( name ),
    mMaxEmitters( maxEmitters ),
    mEmitterType( emitterType ) {


    //mEmitters.reserve( mMaxEmitters );
    mEmitters.resize( mMaxEmitters );

    for( std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it ) {
        (*it)=0;
    }


    // precreate emitters

    int n=0;

    for( std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it ) {
        (*it)=new CParticleEmitter(
            mName + "_emitter#" + Ogre::StringConverter::toString( n ),
            mEmitterType  );
        n++;
    }

    mPoolIterator=mEmitters.begin();

}


CParticleEmitterPool::~CParticleEmitterPool() {

    for( std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it ) {
        //if( (*it) != 0 )
            delete( (*it) );
    }

}

void CParticleEmitterPool::emit( unsigned int aNumParticles, Ogre::Vector3 position ) {

/*
    int n=0;

    for( std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it ) {

        if( (*it) != 0 ) {

            if( (*it)->numberOfParticles() == 0 ) {

                (*it)->setPosition( position );
                (*it)->addParticle( aNumParticles );
                break;
            }

        }
        else {

            (*it) = new CParticleEmitter(
                mName + "_emitter#" + Ogre::StringConverter::toString( n ),
                mEmitterTypeID  );

            (*it)->setPosition( position );
            (*it)->addParticle( aNumParticles );
        }


       n++;
    }

*/

    (*mPoolIterator)->setPosition( position );
    (*mPoolIterator)->addParticle( aNumParticles );
    (*mPoolIterator)->emit( true );

    ++mPoolIterator;
    if( mPoolIterator == mEmitters.end() )
        mPoolIterator=mEmitters.begin();

}


/*
void CParticleEmitterPool::emit( unsigned int emitterTypeID, unsigned int numParticles, Ogre::Vector3 position ) {


    if( (*mPoolIterator) != 0 )
        delete( (*mPoolIterator) );


    (*mPoolIterator)=new CParticleEmitter(
        mName + "_emitter#" + Ogre::StringConverter::toString( std::distance(mEmitters.begin(), mPoolIterator) ),
        CParticleEmitterManager::getInstance()->getTypeName( emitterTypeID )  );


    (*mPoolIterator)->setPosition( position );
    (*mPoolIterator)->addParticle( numParticles );
    (*mPoolIterator)->emit( true );

    ++mPoolIterator;
    if( mPoolIterator == mEmitters.end() )
        mPoolIterator=mEmitters.begin();
}
*/

void CParticleEmitterPool::update() {

    for( std::vector<CParticleEmitter*>::const_iterator it = mEmitters.begin(); it != mEmitters.end(); ++it ) {
        //if( (*it) != 0 ) {
            (*it)->update();
            // (*it)->emit( false );
        //}
    }
}



