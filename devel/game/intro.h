

#ifndef _CINTRO_
#define _CINTRO_

#include "gamepart.h"
#include "timemanager.h"
#include "physicsmanager.h"
#include "scenemanager.h"
#include "inputmanager.h"
#include "physicobject.h"
#include "light.h"
#include "OgreOde_Core.h"
#include "delay.h"
#include "dynagui.h"

class CIntro:public IGamePart {  // , public CCollisionHandler {

    public:

    CIntro();
    ~CIntro();

    int run();

    bool collision( OgreOde::Contact* contact );

    private:

    CTimeManager* mTimeManager;
    CInputManager* mInputManager;
    CSceneManager* mSceneManager;

    CCamera* mCam;

    CPhysicsManager* mPhysicsManager;

    CDelay* mDelay;
    CDelay* mIntroDelay;

    // CSpotLight* mLight;
    Ogre::Light* mLight;

    CActor* mBol;
    CStaticPhysTrimesh* mCollBol;
    CDynamicPhysTrimesh* mTriangle;
    // CDynamicPhysCube* mCube;
    CDynamicPhysTrimesh* mSquare;
    CDynamicPhysTrimesh* mPentagon;
    CDynamicPhysTrimesh* mDisc;

    std::vector<CDynamicPhysTrimesh*> mPolygon;

    // CStaticPhysCube* mSlab;
    OgreOde::InfinitePlaneGeometry* mPlane;

    int mPolygonCount;

    DynaGUI* mGUI;
    // BetaGUI::Window* mLogo;
    DWindow* mLogo;


};


#endif


