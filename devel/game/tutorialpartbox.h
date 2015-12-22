

#ifndef _CTUTORIALPARTBOX_
#define _CTUTORIALPARTBOX_

#include "tutpart.h"
#include "Ogre.h"
//#include "sound.h"


// #define MAX_DATA 10

class CTutorialpartBox:public ITutPart {

    public:

    CTutorialpartBox(CShip* ship);
   ~CTutorialpartBox();

    CShipController* getShipController() { return mShipController; }

    protected:

    virtual int run();
    virtual void init();
    virtual void nextPart();
    virtual bool tutPartOk();


/*
    virtual bool getThrustEnabled();
    virtual bool getYawEnabled();
    virtual bool getPitchEnabled();
    virtual bool getRollEnabled();
*/
    virtual std::vector<std::string> getMessage();
    std::vector<std::string> mMessage;

    Ogre::Vector3 mTutData[11];
    int mTutDataMax;
    int mTutNum;


    Ogre::Vector3 mShipPos;
    Ogre::Vector3 mShipStartPos;
    CShip* mShip;
    CActor* mGoal;

    bool tutOk;
    bool tutPartialOk;

    // CSoundManager* mSoundManager;
    // CSound2D* mSoundBeep;
    CDelay* mTutDelay;

    CShipController* mShipController;

};

#endif

