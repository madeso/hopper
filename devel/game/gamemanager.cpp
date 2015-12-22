

#include "gamemanager.h"

//#include "game.h"
#include "gameplaylanding.h"
#include "gameplayracing.h"
#include "gameplaytutorial.h"
#include "gameplaytest.h"

#include "leveleditor.h"
#include "menu.h"
#include "inputmanager.h"

#include "logger.h"
#include "screen.h"

#include "sqlite3x.hpp"

#include "optionmanager.h"

#include "intro.h"
#include "hopperinputactions.h" // for the anyKey function ( which could be in the input manager ? )

// #include "guimanager.h"
// #include "hoppersounds.h"  // for the GUI click sound

#include "levelinfo.h"



CGameManager::CGameManager() {

    mLevelOrder=1;

    mGameType=LANDING_GAME;

    mMaxLevels=0;

    mQuit=APP_RUN;

    getStartingPart();


    // CGUIManager::getInstance()->setClickSound( SOUND_MOUSECLICK );

}


CGameManager::~CGameManager() {
   delete(mPart);
}

int CGameManager::run() {

    mPartStatus=mPart->run();

    switch(mPartStatus) {

        case GM_GAMEOVER:  // wait for user input then return to menu
            //if( anyKey() ) {
                delete(mPart);
                mPart=new CMenu();
                mGameType = LANDING_GAME;
                mLevelOrder = 1;  // resets the next level for any game type
            //}
            break;

        case GM_GAMEWON:  // wait for user input then go to next level if any

            // if( anyKey() ) {

                delete(mPart);
                mLevelOrder++;

                if(mLevelOrder > mMaxLevels) {
                    mPart=new CMenu();
                }
                else {
                    mLevelNum = CLevelInfo::getLevelID( mGameType, mLevelOrder );

                    switch( mGameType ) {
                        case LANDING_GAME:
                            mPart=new CGameplayLanding(mLevelNum);
                            break;
                        case RACING_GAME:
                            mPart=new CGameplayRacing(mLevelNum);
                            break;
                        case TUTORIAL_GAME:
                            mPart=new CMenu();

                    }

                }
            // }
            break;

        case GM_NEWGAME_LANDING:
            delete(mPart);
            mGameType=LANDING_GAME;
            mMaxLevels = CLevelInfo::getMaxLevels( mGameType );
            mLevelOrder = 1;
            mLevelNum = CLevelInfo::getLevelID( mGameType, mLevelOrder );
            mPart=new CGameplayLanding(mLevelNum);
        break;

        case GM_NEWGAME_RACING:
            delete(mPart);
            mGameType = RACING_GAME;
            mMaxLevels = CLevelInfo::getMaxLevels( mGameType );
            mLevelOrder = 1;
            mLevelNum = CLevelInfo::getLevelID( mGameType, mLevelOrder );
            mPart=new CGameplayRacing(mLevelNum);
        break;

        case GM_NEWGAME_TUTORIAL:
            delete(mPart);
            mGameType = TUTORIAL_GAME;
            mMaxLevels = 1;
            mLevelOrder = 1;
            mLevelNum = CLevelInfo::getLevelID( mGameType, mLevelOrder );
            mPart=new CGameplayTutorial(mLevelNum);
        break;

        case GM_LEVEL_CHANGE:
        case GM_NEWGAME_TEST:
            delete(mPart);
            mLevelNum=0;
            mPart=new CGameplayTest(mLevelNum);
        break;

        case GM_EDITOR:
            delete(mPart);
            mLevelNum=1;
            mPart=new CLevelEditor();
        break;

        case GM_QUIT:
            mQuit=APP_QUIT;
        break;

        case GM_MENU:   // exiting to menu but don't wait for user input.
            delete(mPart);
            mPart=new CMenu();
            mLevelOrder=1;
        break;


    }

    // CTRL + X  : could be ALT + F4 !
    if( CInputManager::getInstance()->isKeyPressed( OIS::KC_X ) &&
        CInputManager::getInstance()->isKeyPressed( OIS::KC_LCONTROL )) {
            mQuit=APP_QUIT;
    }


    return(mQuit);
}

/*
// moved to CLevelInfo
int CGameManager::getMaxLevels( int gameType ) {

    int maxlvl=0;

    LOG("\nCGameManager::getMaxLevels()" );

    sqlite3x::sqlite3_connection con("../data/levels.db");
    //CLogger::getInstance()->log("\nDatabse opened.");
    {
        sqlite3x::sqlite3_command cmd ( con, "select count(*) from level where gametype = ?;" );
        cmd.bind( 1, gameType );

        maxlvl=cmd.executeint();
    }

    con.close();

    LOG(" => got %d", maxlvl );

    return(maxlvl);
}


int CGameManager::getNextLevelNum( int gameType, int gameOrder ) {

    int lvl=1;

    LOG("\nCGameManager::getNextLevelNum()" );

    sqlite3x::sqlite3_connection con("../data/levels.db");
    {
        //CLogger::getInstance()->log("\nDatabse opened.");
        sqlite3x::sqlite3_command cmd ( con, "select id from level where gametype = ? and gameorder = ?;" );
        cmd.bind( 1, gameType );
        cmd.bind( 2, gameOrder );

        lvl=cmd.executeint();

    }
    con.close();

    LOG(": got level %d for gameType = %d and gameOrder = %d", lvl, gameType, gameOrder );

    return( lvl );

}
*/

bool CGameManager::anyKey() {

    if( CInputManager::getInstance()->isReleased(IMR_SPACE ) ||
        CInputManager::getInstance()->isReleased(IMR_MB1) ||
        CInputManager::getInstance()->isReleased(IMR_MB2)
    ) {
            return(true);
    };

    return(false);
}



void CGameManager::getStartingPart() {

    std::string part;
    mPart=0;
    COptionManager::getInstance()->getOption( "dev_gamestart", part );

    if ( part=="menu" ) {
        LOG("\n******* Starting with the menu ***********");
        mPart=new CMenu();
    }

    if ( part=="intro" ) {
        LOG("\n******* Starting with the intro ***********");
        mPart=new CIntro();
    }
/*
    if( mPart==0 ) {
        LOG("\n******* mPart bug !***********");
        mPart=new CMenu();
    }
*/

}


