

#ifndef _GAMESUQUENCEQUEUE_
#define _GAMESUQUENCEQUEUE_

#include "GameSequences.h"



struct SGameSequenceData {

    Ogre::String name;
    CGameSequence* sequence;
    Ogre::String dependency;

};


class CGameSequenceManager {

public:

    CGameSequenceManager();
    ~CGameSequenceManager();

    void addSequence( Ogre::String name, CGameSequence* sequence, Ogre::String dependency = Ogre::StringUtil::BLANK );

    void run();

private:

    std::list<SGameSequenceData> mGameSequenceDatas;

};



#endif



