

#include "leveleditor.h"

#include "logger.h"

#include "sqlite3x.hpp"

#include "gameconstants.h"
#include "hopperinputactions.h"

#include "resourcemanager.h"
#include "particleemittermanager.h"

#include "timemanager.h"
#include "levelinfo.h"

#include "screen.h"

//TODO: cloudlayer height management
//TODO: sun management
//TODO: redesign the add object box
//TODO: level name management ( use TextInput  )
//TODO: add pitch and roll orientation
//TODO: add a cancel button in the objectoptions window.


/**
 * Level Editor.
 */
CLevelEditor::CLevelEditor() {

    LOG("\n============================\n" );
    LOG(" New Editor" );
    LOG("\n============================\n" );

    mInputManager=CInputManager::getInstance();

    mSceneManager=CSceneManager::getInstance();

    mSceneManager->setAmbientColor(0.0,0.0,0.0);
 //   mSceneManager->setShadowOn();

    //==================== Camera ============================

    mCamera=CCamera::getInstance();

    /*
    mPointer=new CActor( "editor_pointer", "repere.mesh" ); // "titsphere.mesh");
    mPointer->scale( 2, 2, 2 );
    */

    //mSceneManager->setFogOff();


    mRayQuery=mSceneManager->getOgreSceneManager()->createRayQuery( Ogre::Ray() );
    mRayQuery->setSortByDistance( true, 0);
    // mRayQuery->setQueryMask( ~0 ); // any entity
    mRayQuery->setQueryMask( CActor::BASIC_OBJECT ); // only CActors entity
    mRayQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);

//    mPointerDistance=100;

    mGUI=DynaGUI::getInstance();
    mGUI->showMousePointer();
    mMenuWindow=new CEditorMenuWindow();
    mInfoWindow=new CEditorInfoWindow();



    // mDebugWin=new CDebugWindow(4);
//    mDebugWin->setPosition( 0, 50 );

    // mSelectMode=ED_LEVEL;  // edition mode

    mEditedObject=-1; // ED_OBJ_NONE;  // special for object mode
    mObjectSelected=false;
    mMode=ED_SELECTMODE;
    mEditMode=EM_NOTHING;



    mGameType = LANDING_GAME;
    mLevelIndex=1;
    mMenuWindow->setLevelNum( mLevelIndex);

    mLevel=0;
    loadLevel();



    mCamController=new CEditorCameraController( mCamera );

    mReturn=GM_GAMEPLAY;

}


CLevelEditor::~CLevelEditor() {

    mSceneManager->getOgreSceneManager()->destroyQuery( mRayQuery );

    // delete( mDebugWin );



    delete( mCamController );

    delete(mMenuWindow );
    delete(mInfoWindow );

    mGUI->hideMousePointer();

    // delete(mPointer);
    delete(mLevel);

    // CParticleEmitterManager::kill();
    LOG("\n~CGame : killing the scene manager...");
    CSceneManager::kill();
    LOG("Ok.");

}

/**
 * manages the camera and the editor mode switching.
 */

int CLevelEditor::run() {

    /*
    if( mMode!=ED_CAMERAMODE ) {
        mGM->injectMouse();
    }
    */
    //mCamController->run();

    mTimeFactor = CTimeManager::getInstance()->getTimeFactor();


    switch( mMode ) {

        case ED_CAMERAMODE:
            manageCameraMode();
            break;

        case ED_SELECTMODE:
            mGUI->injectMouse();
            manageSelectMode();

            if(mInputManager->isReleased( IMR_ESCAPE )) {
                quit();
            }
            break;

        case ED_OBJECTOPTION:
            mGUI->injectMouse();
            manageObjectOptions();
            break;

        case ED_OBJECTCREATE:
            mGUI->injectMouse();
            manageObjectCreation();
            break;

        case ED_LEVELOPTION:
            mGUI->injectMouse();
            manageLevelOptions();
            break;

        case ED_TERRAINOPTION:
            mGUI->injectMouse();
            manageTerrainOptions();
            break;
/*
        case ED_TUNNEL_EDIT_MODE:
            mGM->injectMouse(true);
            manageTunnelEditMode();
            break;
*/
        case ED_EDITMODE:
            manageEditMode();
            break;


        case ED_DIALOGBOX:
            mGUI->injectMouse();
            manageDialogBox();
            break;
    }


    switch( mMenuWindow->getStatus() ) {

        case ES_QUIT:
            quit();
            break;
        case ES_TERRAIN:
            switchToTerrainOptionsMode();
            break;

        case ES_LEVEL:
            switchToLevelOptionsMode();
            break;

        case ES_NEXT:
            if( mLevelIndex < CLevelInfo::getMaxLevels( mGameType ) ) {
                mLevelIndex++;
                loadLevel();
            }
            else {
               createNewLevel();
            }
            break;

        case ES_PREVIOUS:
            if( mLevelIndex>1 ) {
                mLevelIndex--;
                loadLevel();
            }
            break;

        case ES_PREVIOUS_TYPE:
                switch( mGameType ) {

                    case RACING_GAME:
                        mGameType = LANDING_GAME;
                        mLevelIndex = 1;   // reset to 1 to avoid loading non existing levels
                        loadLevel();
                    break;

                    default:
                    case LANDING_GAME:
                    break;

                }
        break;

        case ES_NEXT_TYPE:
            switch( mGameType ) {
                case LANDING_GAME:
                    mGameType = RACING_GAME;
                    mLevelIndex = 1;   // reset to 1 to avoid loading non existing levels
                    loadLevel();
                break;

                case RACING_GAME:
                    mGameType = LANDING_GAME;
                    mLevelIndex = 1;   // reset to 1 to avoid loading non existing levels
                    loadLevel();
                default:
                break;

            }
        break;

        case ES_SAVE:
            saveLevel();
        break;

    }





    return( mReturn );
}




bool CLevelEditor::pickObject( Ogre::String &s ) {

    CScreen* theScreen = CScreen::getInstance();

    Ogre::Vector2 xy(
        (float)mInputManager->getMouseX() / (float)theScreen->getWidth(),
        (float)mInputManager->getMouseY() / (float)theScreen->getHeight() );

    mRayQuery->setRay( mCamera->getCameraToViewportRay( xy ) );
    Ogre::RaySceneQueryResult &result = mRayQuery->execute();
    if( result.size() > 0) {
        Ogre::RaySceneQueryResult::iterator i = result.begin();
        if( (*i).movable != NULL ) {

            s=(*i).movable->getName();
            return(true);
        }

    }

    return(false);
}


CLevel::GameObjectList::iterator CLevelEditor::getGameObjectIterator( Ogre::String aName ) {

    CLevel::GameObjectList::iterator o;


    for(o=mLevel->mGameObjectList.begin(); o!=mLevel->mGameObjectList.end(); ++o ) {

        //if( static_cast<CActor*>((*i).movable) == (*o).actor ) {
        if( aName == (*o).name ) {
            return( o );
        }

    }

    return(mLevel->mGameObjectList.end());
}



void CLevelEditor::switchToSelectMode() {
    mMode=ED_SELECTMODE;
    mInfoWindow->setStatus( "[Sel. Mode]" );
    mGUI->showMousePointer();
    mMenuWindow->show();
}

void CLevelEditor::switchToCameraMode() {
    mMode=ED_CAMERAMODE;
    mInfoWindow->setStatus( "[Cam. mode]" );
    mGUI->hideMousePointer();
    mMenuWindow->hide();
}

void CLevelEditor::switchToLevelOptionsMode() {
    mLevelOptionsWindow=new CEditorLevelOptionsWindow();
    mLevelOptionsWindow->setFogColor( mFogRed, mFogGreen, mFogBlue );
    mLevelOptionsWindow->setGravity( mGravity );
    mMode=ED_LEVELOPTION;
    mInfoWindow->setStatus("[Level Opt.]");
    mMenuWindow->hide();
}

void CLevelEditor::switchToTerrainOptionsMode() {
    mTerrainOptionsWindow=new CEditorTerrainOptionsWindow();
    mTerrainOptionsWindow->setTerrainParams( mHeightMap, mTerrainTexture, mDetailTexture );
    mMode=ED_TERRAINOPTION;
    mInfoWindow->setStatus("[Terrain Op.]");
    mMenuWindow->hide();
}

void CLevelEditor::switchToObjectOptionsMode() {
    mObjectOptionsWindow=new CEditorObjectOptionsWindow();
    mMode=ED_OBJECTOPTION;
    mInfoWindow->setStatus( "[Obj. Opt.]" );
    mMenuWindow->hide();
}

void CLevelEditor::switchToObjectCreationMode() {
    mObjectCreationWindow=new CEditorObjectCreationWindow();

    CLevel::ObjectDefList::const_iterator i;
    // populate the window with the objectdeflist from CLevel
    for( i = mLevel->mObjectDefList.begin();
         i != mLevel->mObjectDefList.end(); ++i ) {
            mObjectCreationWindow->addObject( (*i).name );
    }

    mInfoWindow->setStatus( "[Add Obj. ]" );
    mMenuWindow->hide();
    mMode=ED_OBJECTCREATE;
}


void CLevelEditor::switchToDialogBox( Ogre::String title, Ogre::String message, Ogre::String caption ) {
    mMenuWindow->hide();
    mDialogBox = new CDialogBox( title );
    mDialogBox->addLine( message );
    mDialogBox->addOKButton( caption );
    mMode = ED_DIALOGBOX;
}

void CLevelEditor::switchToEditMode() {

    mMode=ED_EDITMODE;
    mInfoWindow->setStatus( "[Edit Mode]" );
    mGUI->hideMousePointer();
    mMenuWindow->hide();
}



void CLevelEditor::manageSelectMode() {

     //if( mInputManager->isReleased( IMR_MB1 ) ) {
     if( mInputManager->isLeftMouseButtonReleased() ) {

        //mDebugWin->setText( 0, "left clic !" );
        LOG("\nCLevelEditor::manageSelectMode() : Mouse Left click" );

        Ogre::String pickedObjectName;

        if( pickObject( pickedObjectName ) ){

            // mDebugWin->setText( 1, pickedObjectName );
            LOG("\nCLevelEditor::manageSelectMode() : Picked object : %s", pickedObjectName.c_str() );

            CLevel::GameObjectList::iterator o=getGameObjectIterator( pickedObjectName);
            if( o != mLevel->mGameObjectList.end() ) {

                // an object has been clicked on :

                if ( mObjectSelected && mSelectedObject==o ) {  // if this object is already selected, deselect it
                    (*o).actor->showBoundingBox( false );
                    mObjectSelected=false;
                    mInfoWindow->setInfo( "nothing selected" );
                }  else {

                    // unselect any previously selected object
                    if( mObjectSelected ) {
                        (*mSelectedObject).actor->showBoundingBox( false );
                        mObjectSelected=false;
                    }

                    // and select the object
                    (*o).actor->showBoundingBox( true );
                    mSelectedObject=o;
                    mObjectSelected=true;
                    mInfoWindow->setInfo( "Selected : " + mSelectedObject->name );
                }


            }

        }
        // else mDebugWin->setText( 1, "picked nothing" );

     }


    //if( mInputManager->isReleased( IMR_MB2 ) ) {
    if( mInputManager->isRightMouseButtonReleased() ) {

        // mDebugWin->setText( 0, "right clic !" );

        if( mObjectSelected ) {
            // afficher la fenêtre d'options de l'objet
            switchToObjectOptionsMode();
        }
        else {
            // afficher la fenêtre de création d'objet
            switchToObjectCreationMode();
        }

    }

    if( mInputManager->isReleased ( IMR_SPACE ) ) {
        // mDebugWin->setText( 0, "space pressed !" );
        switchToCameraMode();
    }


}


void CLevelEditor::manageCameraMode() {

    //updateCamera();
    mCamController->run();

    if(    mInputManager->isReleased( IMR_MB1)
        || mInputManager->isReleased( IMR_MB2 )
        || mInputManager->isReleased( IMR_SPACE )  ) {

        switchToSelectMode();
    }
}



void CLevelEditor::manageObjectOptions() {

    EDITORMESSAGE message;

    if( mObjectOptionsWindow->done() ) {

        message=mObjectOptionsWindow->getMessage();
        delete(mObjectOptionsWindow);

        switch( message ) {
            case EM_MOVEOBJECT:
                mEditMode=EM_MOVEOBJECT;
                switchToEditMode();
                break;

            case EM_ROTATEOBJECT:
                /*
                if( (*mSelectedObject).special ) {
                    mInfoWindow->setInfo( "Special objects can't be rotated !");
                    switchToSelectMode();
                } else {
                */
                    mEditMode=EM_ROTATEOBJECT;
                    switchToEditMode();
                // }
                break;

            case EM_SCALEOBJECT:
                if( (*mSelectedObject).special ) {
                    mInfoWindow->setInfo("Special objects can't be scaled !");
                    switchToSelectMode();
                } else {
                    mEditMode=EM_SCALEOBJECT;
                    switchToEditMode();
                }
                break;

            case EM_DELETEOBJECT:
                deleteSelectedObject();
                switchToSelectMode();
                break;

            default:
                switchToSelectMode();
                break;
        }

    }

}


void CLevelEditor::deleteSelectedObject() {

    if( mObjectSelected ) {
        delete((*mSelectedObject).actor);
        mLevel->mGameObjectList.erase( mSelectedObject );
        mObjectSelected=false;
    }

}



void CLevelEditor::manageObjectCreation() {

    if( mObjectCreationWindow->done() ) {

        int ret=mObjectCreationWindow->getSelected();

        if( ret>=0 ) {

            Ogre::Vector3 pos( mCamera->getInFrontPosition(30) );
            float heightmin=mLevel->getTerrain()->getHeightAt( pos.x, pos.z );

            if( pos.y<heightmin ) pos.y=heightmin;

            // mDebugWin->setText( 3, "got:" + Ogre::StringConverter::toString( mObjectCreationWindow->getSelected() ));

            mLevel->addEditorObject(
                mLevel->mObjectDefList[ ret ].type,
                pos,
                0,
                mLevel->mObjectDefList[ ret ].scale
            );

        }

        delete(mObjectCreationWindow);
        switchToSelectMode();
    }

}


void CLevelEditor::manageEditMode() {

    float deltax, deltay;
    mInputManager->getMouseDelta(deltax, deltay );


    float deltaWheel=mInputManager->getMouseWheelDelta();
    if( deltaWheel < 0 ) deltaWheel = -1*3.1415928/180.f * mTimeFactor;
        else if (deltaWheel>0) deltaWheel = 1*3.1415928/180.f * mTimeFactor;
    if( mInputManager->isKeyPressed( OIS::KC_LCONTROL )) deltaWheel *= 10;
        else if( mInputManager->isKeyPressed( OIS::KC_LSHIFT )) deltaWheel /= 10;

    Ogre::Vector3 pos( (*mSelectedObject).actor->getPosition() );
    float alpha;
    float s;
    Ogre::Vector3 move;
    move = mCamera->getOrientation() * Ogre::Vector3( deltax, 0, deltay );
    move.y = 0;
    //move.normalise();



    switch( mEditMode ) {

        case EM_MOVEOBJECT:
            if( deltax || deltay || deltaWheel ) {
                pos += move;
                //pos.x+=deltax;
                //pos.z+=deltay;
                pos.y+=deltaWheel;

                mInfoWindow->setInfo( "pos :" + Ogre::StringConverter::toString( pos ));

                (*mSelectedObject).actor->setPosition(pos);
            }
        break;

        case EM_ROTATEOBJECT:
            if( deltaWheel != 0 ) {
                if( mInputManager->isKeyPressed( OIS::KC_LCONTROL )) deltaWheel *= 10;
                else if( mInputManager->isKeyPressed( OIS::KC_LSHIFT )) deltaWheel /= 10;
                alpha=fmodf( (*mSelectedObject).yaw+deltaWheel, 2*M_PI );
                // mDebugWin->setText( 3, "yaw :" + Ogre::StringConverter::toString( alpha ));
                mInfoWindow->setInfo( "yaw :" + Ogre::StringConverter::toString( alpha ));

                (*mSelectedObject).actor->setYaw( alpha );
                (*mSelectedObject).yaw=alpha;
            }
        break;

        case EM_SCALEOBJECT:
            s=(*mSelectedObject).scale;
            if( deltaWheel != 0 && s > 0.1) {
                s+=deltaWheel;
                // mDebugWin->setText( 3, "scale :" + Ogre::StringConverter::toString( s ));
                mInfoWindow->setInfo( "scale :" + Ogre::StringConverter::toString( s ));

                (*mSelectedObject).scale=s;
                (*mSelectedObject).actor->setScale(s,s,s);
            }
        break;

    }


    if( mInputManager->isReleased( IMR_MB1 ) || mInputManager->isReleased( IMR_MB2 ) ) {
        switchToSelectMode();
    }

}


void CLevelEditor::manageLevelOptions() {

    if( mLevelOptionsWindow->getColorChanged() ) {
        mLevelOptionsWindow->getFogColor( mFogRed, mFogGreen, mFogBlue );
        mLevel->setFogColor( mFogRed, mFogGreen, mFogBlue );
    }


    if( mLevelOptionsWindow->done() ) {

        if( mLevelOptionsWindow->getWipeObjects()) {
            mLevel->wipeObjects();
        }

        mGravity=mLevelOptionsWindow->getGravity();

        delete(mLevelOptionsWindow);
        switchToSelectMode();
    }

}


void CLevelEditor::manageTerrainOptions() {

    if( mTerrainOptionsWindow->getHeightmapChanged() ){
        mHeightMap=mTerrainOptionsWindow->getHeightmap();
        mLevel->changeTerrain( mHeightMap, mTerrainTexture, mDetailTexture );
    }
    if( mTerrainOptionsWindow->getTextureChanged() ) {
        mTerrainTexture=mTerrainOptionsWindow->getTexture();
        mLevel->changeTerrain( mHeightMap, mTerrainTexture, mDetailTexture );
    }
    if( mTerrainOptionsWindow->getDetailChanged() ) {
        mDetailTexture=mTerrainOptionsWindow->getDetail();
        mLevel->changeTerrain( mHeightMap, mTerrainTexture, mDetailTexture );
    }

    if (mTerrainOptionsWindow->done() ) {
        delete(mTerrainOptionsWindow);
        switchToSelectMode();
    }

}


void CLevelEditor::manageDialogBox() {

    if( mDialogBox->done() ) {
        delete( mDialogBox );
        switchToSelectMode();
    }

}

/*
void CLevelEditor::manageTunnelEditMode() {

    // right-clicking with nothing selected opens a menu :
    // create a cube ( 6 walls )




    // right-clicking on a wall opens a context menu with choices:
    //      select edge

}
*/


void CLevelEditor::quit() {

    if( mMode==ED_OBJECTCREATE ) {
        delete( mObjectCreationWindow );
    }
    if( mMode==ED_OBJECTOPTION  ) {
        delete( mObjectOptionsWindow );
    }
    if( mMode==ED_LEVELOPTION ) {
        delete( mLevelOptionsWindow );
    }
    if( mMode==ED_TERRAINOPTION ) {
        delete( mTerrainOptionsWindow );
    }

    mReturn=GM_MENU;

}


void CLevelEditor::loadLevel() {

    if(mLevel) delete(mLevel);

    int levelID = CLevelInfo::getLevelID( mGameType, mLevelIndex );


    mInfoWindow->setStatus( "Loading level #" + Ogre::StringConverter::toString( levelID ) );
    mLevel=new CLevel( levelID , true );
    mInfoWindow->setStatus( "Load OK" );
    mMenuWindow->setLevelNum( mLevelIndex);
    mMenuWindow->setGameType( mGameType );
    mObjectSelected=false;

    mLevel->getFogColor( mFogRed, mFogGreen, mFogBlue );
    mGravity=mLevel->getGravity();
    mLevel->getTerrainParams( mHeightMap, mTerrainTexture, mDetailTexture );


    CActor* ship=mLevel->getFirst( O_SHIP);
    if( ship )  {
        mCamera->setPosition( ship->getPosition() - Ogre::Vector3( 10, -5, 10 ) );
        mCamera->lookAt( ship->getPosition() );
    }
    else {
        LOG("\n CLevelEditor() : *** No Ship ?!!!! ***" );
        mCamera->setPosition( 0,0,0 );
        mCamera->lookAt( 0,0,1 );
    }

}


void CLevelEditor::saveLevel() {


    // TODO: test the presence of (at least ) a ship
    // and landing pads or rings depending of the level type
    // display a dialog box (and don't save ) if any of the condition failed.

    bool dialogDisplay = false;

    if( !mLevel->getFirst( O_SHIP ) ) {
        switchToDialogBox( "Error", "You forgot to add a ship in the level !", "aah" );
        return;
    }

    if( mGameType == LANDING_GAME && mLevel->getNumberOf( O_LANDINGPAD ) == 0  ) {
        switchToDialogBox( "Error", "There's no landing pad !", "oops" );
        return;
    }


    if( mGameType == RACING_GAME && mLevel->getNumberOf( O_RING ) == 0 ) {
        switchToDialogBox( "Error", "There's no ring !", "eek!" );
        return;
    }

    mInfoWindow->setStatus( "Saving level #" + Ogre::StringConverter::toString( mLevelIndex) );
    mLevel->save( mGameType, mLevelIndex );
    switchToDialogBox( "Yes !", "Level saved !", "OK" );
    mInfoWindow->setStatus( "Save OK" );

}



void CLevelEditor::createNewLevel() {

    // mGameType = LANDING_GAME;
    int aLevelID = CLevelInfo::getNewLevelID(); // get a *unused* level ID ( max levelID + 1 )

    // reset the current level, but set it to a new levelID
    mLevel->createNew( aLevelID );

    //  and load the new level.
    // loadLevel();


}



