

//  race gameplay

#ifndef _CGAMEPLAYRACING_
#define _CGAMEPLAYRACING_

// #include "gamepart.h"
#include "game.h"
#include "gamemessagewindow.h"


/**
  * Racing gameplay
  *
  */

class CGameplayRacing: public CGame {

    public:
        CGameplayRacing(int aLevelNum);
        ~CGameplayRacing();

    private:
        CRing* mNextRing;
        void gameplay();
        void computeRings();
        void ringTest();

        /// overriden from game for the countdown
        void prestart();
        /// overriden from game for the countdown
        void starting();

        /// overriden from CGame
        void preend();
        /// overriden from CGame
        void ending();


        CGameMessageWindow* mGameMessageWindow;
        CDelay* mWrongRingDelay;
        int mNumRings;
        std::vector<CRing*> mRings;

        CCountDown* mCountDown;

};


#endif
