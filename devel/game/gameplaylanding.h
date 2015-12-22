
// gameplay landing

#ifndef _CGAMEPLAYLANDING_
#define _CGAMEPLAYLANDING_


#include "game.h"
// #include "sound.h"



class CGameplayLanding: public CGame {

    private:
    Ogre::Vector3 mNextLandingPadPosition;
    int mNumLandingPads;

    public:
        CGameplayLanding(int aLevelNum);
        ~CGameplayLanding();
        void gameplay();
        void computeLandingPads();
        void landingTest();

};


#endif



