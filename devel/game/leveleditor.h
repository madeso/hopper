

#ifndef _CEDITOR_H_
#define _CEDITOR_H_


#include "inputmanager.h"

#include "camera.h"
#include "CameraControllers.h"

#include "gamepart.h"
#include "scenemanager.h"
#include "level.h"
//#include "ground.h"
//#include "ship.h"

#include "soundmanager.h"
// #include "interfaceeditor.h"
#include "gamemanager.h"

#include "editorwindows.h"
#include "debugwindow.h"

#include "dialogbox.h"

//enum { ED_LEVEL, ED_OBJECT, ED_TERRAIN, ED_COLOR, ED_GRAVITY, ED_LASTMODE    };
//enum { ED_OBJ_NONE, ED_OBJ_SHIP, ED_OBJ_LANDINGPAD, ED_OBJ_RING, ED_OBJ_MAX };

enum { ED_CAMERAMODE, ED_SELECTMODE, ED_EDITMODE, ED_OBJECTOPTION,
        ED_OBJECTCREATE, ED_LEVELOPTION, ED_TERRAINOPTION,
         ED_QUIT, ED_LOAD, ED_SAVE, ED_DIALOGBOX };


/// a fairly simple editor.

class CLevelEditor: public IGamePart {

public:

    CLevelEditor();
    ~CLevelEditor();
    virtual int run();


private:

    // System stuffs
    CInputManager* mInputManager;
    CSceneManager* mSceneManager;

    // windows
    CEditorMenuWindow* mMenuWindow;
    CEditorInfoWindow* mInfoWindow;
    CEditorLevelOptionsWindow* mLevelOptionsWindow;
    CEditorObjectCreationWindow* mObjectCreationWindow;
    CEditorObjectOptionsWindow* mObjectOptionsWindow;
    CEditorTerrainOptionsWindow* mTerrainOptionsWindow;
    CDialogBox* mDialogBox;

    CDebugWindow* mDebugWin;

    // Camera management
    CCamera* mCamera;

    CCameraController* mCamController;

    float mMouseXd, mMouseYd;
    float mCamTranslateScale;
 //   Ogre::Vector3 mCamTranslate;


    /// returns true if a game object has been picked
    /// and sets its name in the reference passed string
    bool pickObject( Ogre::String &s );

    /// returns mGameObjectList.end() if not found
    CLevel::GameObjectList::iterator getGameObjectIterator( Ogre::String aName );


    void switchToSelectMode();
    void switchToCameraMode();
    void switchToLevelOptionsMode();
    void switchToObjectOptionsMode();
    void switchToObjectCreationMode();
    void switchToEditMode();
    void switchToTerrainOptionsMode();
    void switchToDialogBox( Ogre::String title, Ogre::String message, Ogre::String caption );

    void manageCameraMode();
    void manageSelectMode();

    void manageLevelOptions();
    void manageObjectOptions();
    void manageObjectCreation();
    void manageEditMode();
    void manageTerrainOptions();
    void manageDialogBox();


    void deleteSelectedObject();


    void loadLevel();
    void saveLevel();
    void createNewLevel();

    // Game objects
    CLevel* mLevel;

    Ogre::RaySceneQuery* mRayQuery;

    DynaGUI* mGUI;


    // CActor* mPointer;
    Ogre::Vector3 mPointerPos;
    // float mPointerDistance;

    int mGameType;  /// current game type (i.e.: LANDING, RACING, ... )
    int mLevelIndex;  /// currently edited level
    int mLevelOrder; /// level number by gametype

    int mMode;
    int mEditMode;
    // int mSelectMode;  // edition mode
    int mEditedObject;  // special for object mode
    //SGameObject* mSelectedObject;
    bool mObjectSelected;
    CLevel::GameObjectList::iterator mSelectedObject;

    int mFogGreen, mFogRed, mFogBlue;  // fog color
    int mTerrainTexture;
    int mDetailTexture;
    int mHeightMap;
    float mGravity;

    float mTimeFactor;


    void quit();

    int mReturn; // game part status




};


#endif

