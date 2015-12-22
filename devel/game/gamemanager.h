

#ifndef _CGAMEMANAGER_
#define _CGAMEMANAGER_

#include "gamepart.h"

#define APP_QUIT 1
#define APP_RUN 2

// #include "logger.h"


/// Manages the application states .

class CGameManager {

    private:
        int mStatus;
        IGamePart* mPart;
        int mPartStatus;
        int mGameType;

        bool mPaused;

        int mMaxLevels;
        int mLevelNum;
        int mLevelOrder;
        //int getMaxLevels( int levelType );
        //int getNextLevelNum( int gameType, int gameorder );

        void getStartingPart();

        int mQuit;

        bool anyKey();



    public :
        CGameManager();
        ~CGameManager();
        int run();


};

#endif
