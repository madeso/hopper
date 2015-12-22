
/// orientation tutorial


#include "tutpart.h"
#include "Ogre.h"
#include "ship.h"
#include "actor.h"
// #include "sound.h"

// #define MAX_DATA 8

class CTutorialpartOrientation:public ITutPart {

    public:

    CTutorialpartOrientation(CShip* ship);
   ~CTutorialpartOrientation();

    private:

    int run();
    void init();
    void nextPart();
    bool tutPartOk();

    bool getThrustEnabled();
    bool getYawEnabled();
    bool getPitchEnabled();
    bool getRollEnabled();

    std::vector<std::string> getMessage();
    std::vector<std::string> mMessage;

    Ogre::Vector3 mTutData[11];
    int mTutDataMax;
    int mTutNum;


    Ogre::Vector3 mShipPos;
    CShip* mShip;
    CActor* mGoal;

    bool tutOk;
    bool tutPartialOk;

    // CSoundManager* mSoundManager;
    // CSound2D *mSoundOk;
    CDelay *mTutDelay;

    CShipController* mShipController;
    CShipController* getShipController();

/*
    int mTutMax;
    int mTutNum;
    Ogre::Vector3 mTutData[10];

    Ogre::Vector3 mShipPos;

    CShip* mShip;
    CActor* mSampleShip;

    bool tutOk;
    bool tutPartialOk;

    CSoundManager* mSoundManager;
*/

};

