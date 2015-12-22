

#ifndef _GAMESEQUENCE_
#define _GAMESEQUENCE_

#include "Ogre.h"

class CGame;


class CGameSequence {

public:
    CGameSequence( CGame* game );
    virtual ~CGameSequence();

    virtual void init();
    virtual bool done();

protected:
    bool mDone;
    CGame* mGame;
    Ogre::String name;

private:

};


#include "delay.h"
class CTimedGameSequence: public CGameSequence {

public:
    CTimedGameSequence( CGame* game, int duration );
    virtual ~CTimedGameSequence();
    virtual void init();
    bool isOver();

private:
    CDelay* mDuration;

};



// ==========================================================
// Common game sequences
// ==========================================================

class CGameOverSequence: public CGameSequence {

public:
    CGameOverSequence( CGame* game );
    bool done();
};

#include "gamemessagewindow.h"

class CWaitForInputSequence: public CGameSequence {

public:
    CWaitForInputSequence( CGame* game );
    ~CWaitForInputSequence();
    bool done();
    void init();

private:
    CGameMessageWindow* mGameMessageWindow;

};

class CPauseSequence: public CTimedGameSequence {

public:
    CPauseSequence( CGame* game );
    bool done();

};

// ===========================================================
// Landing game sequences
// ============================================================
#include "sound.h"

class CLandingGameplaySequence : public CGameSequence {

public:
    CLandingGameplaySequence( CGame* game );
    bool done();
};


class CRefuelingSequence: public CGameSequence {

public:
    CRefuelingSequence( CGame* game );
    ~CRefuelingSequence();
    void init();
    bool done();

private:
    CSound3D* mSoundRefuel;
};


class CLandingWonSequence:  public  CGameSequence {

public:
    CLandingWonSequence( CGame* game );
    bool done();

};


// ============================================================
// Racing game sequences
// ============================================================
#include "CountDown.h"

class CRacingPreStartSequence: public CGameSequence {

public:
    CRacingPreStartSequence( CGame* game);
    ~CRacingPreStartSequence();
    bool done();

private:
    CGameMessageWindow* mMessageWindow;
};


class CRacingStartSequence : public CTimedGameSequence {

public:
    CRacingStartSequence( CGame* game );
    ~CRacingStartSequence();
    void init();
    bool done();

private:
    CCountDown* mCountDown;
    bool mDoGo;

};



class CRacingGameplaySequence : public CGameSequence {

public:
    CRacingGameplaySequence( CGame* game );
    bool done();
};


class CRacingWonSequence : public CGameSequence {

public:
    CRacingWonSequence( CGame* game );
    ~CRacingWonSequence() {};
    bool done();
};


// ===========================================================
// Tutorial game sequences
// ============================================================

class CTutorialCompleteSequence: public CTimedGameSequence {
public:
    CTutorialCompleteSequence( CGame* game );
    bool done();

};



class CTutorialGameplaySequence: public CGameSequence {

public:
    CTutorialGameplaySequence( CGame* game );
    bool done();

};


// ===================================================================

#endif


