
#include "GameSequenceManager.h"

#include "logger.h"

// -------------------------------------------------------------------------

/** @brief CGameSequenceManager
  *
  * @todo: document this function
  */
CGameSequenceManager::CGameSequenceManager() {

}

/** @brief ~CGameSequenceManager
  *
  * @todo: document this function
  */
CGameSequenceManager::~CGameSequenceManager() {

    std::list< SGameSequenceData >::iterator it;

    for( it = mGameSequenceDatas.begin(); it != mGameSequenceDatas.end(); ++it ) {
        delete( (*it).sequence );
        it = mGameSequenceDatas.erase( it );
    }
}


/** @brief createQueue
  *
  * @todo: document this function
  */
void CGameSequenceManager::addSequence( Ogre::String name, CGameSequence* sequence, Ogre::String dependency ) {

    SGameSequenceData seq;

    seq.name = name;
    seq.sequence = sequence;
    seq.dependency = dependency;

    mGameSequenceDatas.push_back( seq );

    LOG("\nCGameSequenceManager::addSequence() \"%s\"", name.c_str() );

}



/** @brief run
  *
  * @todo: document this function
  */
void CGameSequenceManager::run() {

    std::list< SGameSequenceData >::iterator it;
    std::list< SGameSequenceData >::iterator it_dep;

    for( it = mGameSequenceDatas.begin(); it != mGameSequenceDatas.end(); ++it ) {

        if( (*it).dependency != Ogre::StringUtil::BLANK ) continue;

        if( (*it).sequence->done() ) {

            LOG("\nCGameSequenceManager::run() => removing \"%s\"", (*it).name.c_str() );
            // remove dependency in other sequences
            for( it_dep = mGameSequenceDatas.begin(); it_dep != mGameSequenceDatas.end(); ++it_dep ) {
                if( (*it_dep).dependency == (*it).name ) {
                    (*it_dep).dependency = Ogre::StringUtil::BLANK;
                    LOG("\nCGameSequenceManager::run() => removed dependency on \"%s\"", (*it_dep).name.c_str() );
                    (*it_dep).sequence->init();
                }
            }

            // remove this sequence from the list

            delete( (*it).sequence );
            it = mGameSequenceDatas.erase( it );
        }

    }


}


