

#ifndef _IGAMEPART_
#define _IGAMEPART_

 enum {
    GM_NOTHING,
    GM_PAUSED,
    GM_GAMEPLAY,
    GM_GAMEWON,
    GM_GAMEOVER,
    GM_MENU,
    GM_QUIT,
    GM_HISCORE,
    GM_NEWGAME_LANDING,
    GM_NEWGAME_RACING,
    GM_NEWGAME_TUTORIAL,
    GM_EDITOR,
    GM_NEWGAME_TEST,
    GM_LEVEL_CHANGE
    };



/// One part at a time.

 class IGamePart {
    public:
    virtual ~IGamePart() {};
    virtual int run() { return GM_NOTHING; }

 };


 #endif

