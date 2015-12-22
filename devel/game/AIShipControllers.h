


#ifndef _AISHIPCONTROLLERS_
#define _AISHIPCONTROLLERS_


#include "shipcontroller.h"


class CBreakShipController: public CShipController {

    public:
        CBreakShipController( CShip* ship );
        ~CBreakShipController();

        void run();

        bool done();

    private:
        bool mDone;

};



#include "debugwindow.h"

class CAutoHoverShipController : public CShipController {

public:
    CAutoHoverShipController( CShip* ship );
    ~CAutoHoverShipController();

    void run();

private:
    float mHoverAltitude;

    CDebugWindow* mDebugWindow;
    CBreakShipController* mBreakShipController;

    void hover();

};





#include "level.h"
#include "delay.h"
//#include "line.h"

class CLandingShipController: public CShipController {

    public:
    CLandingShipController(CShip* aShip, CLevel* aLevel );
    ~CLandingShipController();

    void run();
    Ogre::Vector3 getGoalPosition();

    private:

    Ogre::Vector3 mShipPos;
    // CShip* mShip;
    CGround* mTerrain;
    CLevel* mLevel;

    CDelay* mLandedDelay;
    bool mLanded;

    float mYaw, mPitch, mRoll;

    bool adjustValue( float &value, float goal, float threshold, float step );

    CActor* mGoalCube;

    std::vector<Ogre::Vector3> mGoalPos;
    std::vector<Ogre::Vector3>::iterator mGoal;

/*
    CLine* mLineDir;
    CLine* mLineVelocity;
    CLine* mLinePitchRoll;
    CLine* mLineVectorx;
    CLine* mLineVelocity2D;
*/
    float mPrevDist2D;

    bool doThrust;

};






#endif

