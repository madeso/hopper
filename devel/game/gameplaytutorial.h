
/// tutorial gameplay

#ifndef _CGAMEPLAYTUTORIAL_
#define _CGAMEPLAYTUTORIAL_

// #include "gamepart.h"
#include "game.h"

#include "tutpart.h"

// #include "interfacetutorialmessage.h"
#include "dialogbox.h"






class CGameplayTutorial: public CGame {

    private:

       // void coreShipHandling(bool thrust, bool fuelconsumption, bool yaw, bool pitch, bool roll );
        int mTutState;

        enum { TUT_NONE, TUT_MESSAGE, TUT_RUNNING, TUT_COMPLETE };

        CActor* mPointerGoal;
        CActor* mPointerCurrent;
        Ogre::Vector3 shipPos;

        bool tutPartialOk,tutOk;

        CDelay* mTutDelay;
/*
        bool orientationOk();
        void orientationInit(int tutNum);
        void gravityInit(int tutNum);
        bool gravityOk();
*/
        void messageInit( std::vector<std::string> lines );
        void messageEnd();
        void nextPart();

        int mTutNum;

        Ogre::Vector3 mTutData[10];

        ITutPart* mPart[5];
        int mPartNum;

        //CInterfaceTutorialmessage* mMessageBox;
        CDialogBox* mMessageBox;
        DynaGUI* mGUI;

        std::vector<std::string> mMessageComplete;


    public:
        CGameplayTutorial(int aLevelNum);
        ~CGameplayTutorial();
        void gameplay();
        // void computeLandingPads();


};




#endif

