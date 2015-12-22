

#include "explosionmanager.h"
#include "logger.h"

CExplosionManager::CExplosionManager() {


}


CExplosionManager::~CExplosionManager() {

    for( std::list<IExplosion*>::iterator it = mExplosions.begin();
        it != mExplosions.end(); ++it ) {

            delete( (*it) );

    }

}


void CExplosionManager::createExplosion( Ogre::SceneNode* node, CExplosionManager::Type type ) {

    LOG("\nCExplosionManager::createExplosion() : new explosion type : %d", type );

    if( node ) {

        switch( type ) {
            case BASIC:
                mExplosions.push_back( new CBasicExplosion( node->getPosition() ) );
            break;

            case SMOKE:
                mExplosions.push_back( new CSmokeExplosion( node->getPosition() ) );
            break;

            case BLAST:
                mExplosions.push_back( new CBlastExplosion( node->getPosition() ) );
            break;

            default:
                mExplosions.push_back( new CBasicExplosion( node->getPosition() ) );
            break;
        }
    }

}



void CExplosionManager::update() {

     for( std::list<IExplosion*>::iterator it = mExplosions.begin();
        it != mExplosions.end(); ++it ) {

            if( (*it)->done() ) {
                delete( (*it) );
                it = mExplosions.erase( it );
            }

    }

}

