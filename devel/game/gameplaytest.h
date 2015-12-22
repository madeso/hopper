

#ifndef _GAMEPLAYTEST_
#define _GAMEPLAYTEST_

#include "game.h"
#include "tunnels.h"
// #include "CSGTunnels.h"

class CGameplayTest: public CGame {

    public:
    CGameplayTest( int aLevelNum );
    ~CGameplayTest();
    void gameplay();

    private:
    CTunnels* mTunnels;
    //CSGTunnels* mCSGTunnels;

};


#endif


