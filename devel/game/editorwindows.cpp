
#include "editorwindows.h"

#include "sqlite3x.hpp"
#include "levelinfo.h"

CEditorBasicWindow::CEditorBasicWindow(
    DWindow::Placement hp,
    DWindow::Placement vp,
    Ogre::String title=Ogre::StringUtil::BLANK ) {

        mGUI=DynaGUI::getInstance();
        mWindow=mGUI->createWindow( hp, vp, title );

        mDone=false;
}

CEditorBasicWindow::~CEditorBasicWindow() {
    mGUI->destroyWindow( mWindow );
}

bool CEditorBasicWindow::done() {
    return( mDone );
}

void CEditorBasicWindow::show() {
    mWindow->show();
}

void CEditorBasicWindow::hide() {
    mWindow->hide();
}

// ------------------------------------------------------------------

/// Infobar

CEditorInfoWindow::CEditorInfoWindow():
    CEditorBasicWindow(DWindow::MIN, DWindow::MAX ) {

/*
    mGM=CGUIManager::getInstance();
    mWindow=mGM->createWindow( BetaGUI::MIN, BetaGUI::MAX );
*/
    mLevelLine=mWindow->addText( "level", 60 );
    mStatusLine=mWindow->addText( "status", 100 );
    mInfoLine=mWindow->addText( "Current level : x", 600 );

}

/*
CEditorInfoWindow::~CEditorInfoWindow() {
    mGM->destroyWindow( mWindow);
}
*/

void CEditorInfoWindow::setLevel( Ogre::String line ) {

    mLevelLine->setText( line );

}

void CEditorInfoWindow::setStatus( Ogre::String line ) {

    mStatusLine->setText( line );

}

void CEditorInfoWindow::setInfo( Ogre::String line ) {

    mInfoLine->setText( line );

}


// ------------------------------------------------------------------


/// Menu window
CEditorMenuWindow::CEditorMenuWindow():
    CEditorBasicWindow(DWindow::MIN, DWindow::MIN ) {

    // mGM->addText( mWindow, 60, "Level" );

    mButtonPreviousLevelType = mWindow->addButton( "<", 30, this );
    mTextLevelType = mWindow->addText( "Landing", 60 );
    mButtonNextLevelType = mWindow->addButton( ">", 30, this );

    mButtonPrevious=mWindow->addButton( "-", 30, this  );
    mTextLevel=mWindow->addText( "#00", 60 );
    mButtonNext=mWindow->addButton( "+",30, this );

    mWindow->addSpace( 20 );

    mButtonLevel=mWindow->addButton( "Level Opts", 100, this );

    mButtonTerrain=mWindow->addButton( "Terrain Opts", 120, this );

    mWindow->addSpace( 20 );

    mButtonSave=mWindow->addButton( "Save", 60, this );
    mWindow->addSpace( 40 );
    mButtonQuit=mWindow->addButton( "Quit", 100, this );

    mStatus=ES_NOTHING;

}

/*
CEditorMenuWindow::~CEditorMenuWindow() {

    mGM->destroyWindow(mWindow);

}
*/

void CEditorMenuWindow::setLevelNum( int aLevel ) {
    mTextLevel->setText( "Level #" + Ogre::StringConverter::toString( aLevel ));
}

void CEditorMenuWindow::setGameType( int aGameType ) {
    switch( aGameType ) {
        case LANDING_GAME:
            mTextLevelType->setText( "Landing" );
        break;
        case RACING_GAME:
            mTextLevelType->setText( "Racing" );
        break;
        case TUTORIAL_GAME:
            mTextLevelType->setText("Tutorial");
        break;
        default:
            mTextLevelType->setText("???" );
        break;
    }
}

EDITORSTATE CEditorMenuWindow::getStatus() {

    EDITORSTATE s=mStatus;

    mStatus=ES_NOTHING;

    return( s );
}

void CEditorMenuWindow::onButtonPress( DButton* b ) {

    if( b==mButtonTerrain) mStatus=ES_TERRAIN;
    if( b==mButtonLevel) mStatus=ES_LEVEL;
    if( b==mButtonSave) mStatus=ES_SAVE;
    if( b==mButtonQuit) mStatus=ES_QUIT;
    if( b==mButtonNext) mStatus=ES_NEXT;
    if( b==mButtonPrevious) mStatus=ES_PREVIOUS;

    if( b==mButtonPreviousLevelType ) mStatus = ES_PREVIOUS_TYPE;
    if( b==mButtonNextLevelType ) mStatus = ES_NEXT_TYPE;

}


// ------------------------------------------------------------------

/// Level options window

CEditorLevelOptionsWindow::CEditorLevelOptionsWindow():
    CEditorBasicWindow( DWindow::CENTERED, DWindow::CENTERED, "Level Options" ) {

    mWindow->newLine();
    mWindow->addText( "Name:", 100 );
    mTextName=mWindow->addText( "<name>", 120 );
    //mLevelNameInput=mWindow->addTextInput( );
    mWindow->newLine();
    mWindow->addText( "Type:", 100 );
    mTextType=mWindow->addText( "<type>", 80 );
    mWindow->newLine();

    mWindow->addText( "Fog Color:", 100 );

    mButtonRedMinus=mWindow->addButton( "<", 30, this );
    mTextRed=mWindow->addText( "red", 30);
    mButtonRedPlus=mWindow->addButton( ">", 30, this );

    mButtonGreenMinus=mWindow->addButton( "<", 30, this);
    mTextGreen=mWindow->addText( "green", 30 );
    mButtonGreenPlus=mWindow->addButton( ">",30, this);

    mButtonBlueMinus=mWindow->addButton( "<", 30, this);
    mTextBlue=mWindow->addText( "blue", 30 );
    mButtonBluePlus=mWindow->addButton( ">", 30, this);

    mWindow->newLine();

    mWindow->addText( "Gravity:", 100 );
    mButtonGravityMinus=mWindow->addButton( "<", 30, this);
    mTextGravity=mWindow->addText( "g", 30 );
    mButtonGravityPlus=mWindow->addButton( ">", 30, this);

    mWindow->newLine();
    mWindow->newLine();
    mButtonWipeObjects=mWindow->addButton( "Wipe All Objects", 150, this );

    mWindow->addSpace( 50);
    mOKButton=mWindow->addButton( "OK", 50, this );

    mColorChanged=false;
    mWipeObjects=false;

}


void CEditorLevelOptionsWindow::onButtonPress( DButton* b ) {

    // mColorChanged=false;

    if( b==mButtonRedMinus && mRed>0 ) { mRed--; mColorChanged=true; }
    if( b==mButtonRedPlus && mRed<16 ){  mRed++; mColorChanged=true; }

    mTextRed->setText( Ogre::StringConverter::toString( mRed ));

    if( b==mButtonGreenMinus && mGreen>0 ) { mGreen--; mColorChanged=true; }
    if( b==mButtonGreenPlus && mGreen<16 ) { mGreen++; mColorChanged=true; }

    mTextGreen->setText( Ogre::StringConverter::toString( mGreen ));

    if( b==mButtonBlueMinus && mBlue>0 ) { mBlue--; mColorChanged=true; }
    if( b==mButtonBluePlus && mBlue<16 ) {mBlue++; mColorChanged=true; }

    mTextBlue->setText( Ogre::StringConverter::toString( mBlue ));

    if( b==mButtonGravityMinus && mGravity>-25 ) { mGravity--; }
    if( b==mButtonGravityPlus && mGravity<-1 ) { mGravity++; }

    mTextGravity->setText( Ogre::StringConverter::toString( mGravity ));

    if( b==mOKButton ) {
        mDone=true;
    }

    if (b==mButtonWipeObjects) {
        mDone=true;
        mWipeObjects=true;
    }


}


bool CEditorLevelOptionsWindow::getColorChanged() {
    bool c=mColorChanged;
    mColorChanged=false;
    return(c);
}


void CEditorLevelOptionsWindow::setFogColor( int red, int green, int blue ) {
    mRed=red/16; mGreen=green/16; mBlue=blue/16;
    onButtonPress(0);
}

void CEditorLevelOptionsWindow::getFogColor( int &red, int &green, int &blue ) {
    red=mRed*16; green=mGreen*16; blue=mBlue*16;
}

void CEditorLevelOptionsWindow::setGravity( float gravity ) {
    mGravity=gravity;
    onButtonPress(0);
}

float CEditorLevelOptionsWindow::getGravity() {
    return( mGravity );
}

bool CEditorLevelOptionsWindow::getWipeObjects() {
    return( mWipeObjects );
}

// ------------------------------------------------------------------

/// Terrain options window

CEditorTerrainOptionsWindow::CEditorTerrainOptionsWindow():
    CEditorBasicWindow( DWindow::CENTERED, DWindow::CENTERED, "Terrain Options" ) {

    mWindow->newLine();
    mWindow->addText( "Terrain heightmap:", 150 );
    mButtonPreviousHeightmap=mWindow->addButton( "<", 30, this );
    mTextHeightmap=mWindow->addText( "<heightmap>", 30 );
    mButtonNextHeightmap=mWindow->addButton( ">", 30, this );
    mWindow->newLine();

    mWindow->addText( "Terrain texture:", 150 );
    mButtonPreviousTexture=mWindow->addButton( "<", 30,  this );
    mTextTexture=mWindow->addText( " <texture> ", 30 );
    mButtonNextTexture=mWindow->addButton( ">", 30, this );
    mWindow->newLine();

    mWindow->addText( "Terrain detail tex.:", 150 );
    mButtonPreviousDetail=mWindow->addButton( "<", 30, this );
    mTextDetail=mWindow->addText( "<det. tex.>", 30 );
    mButtonNextDetail=mWindow->addButton( ">", 30, this );
    mWindow->newLine();

    mWindow->newLine();
    mWindow->addSpace( 100 );
    mOKButton=mWindow->addButton( "OK", 50, this );



    sqlite3x::sqlite3_connection con("../data/levels.db");

    mMaxHeightmap=con.executeint( "select count(*) from terrainheightmap;" );
    mMaxTexture=con.executeint( "select count(*) from terraintexture;" );
    mMaxDetail=con.executeint( "select count(*) from terraindetailtexture;" );

    con.close();

    mHeightmapChanged=false;
    mTextureChanged=false;
    mDetailChanged=false;

}


void CEditorTerrainOptionsWindow::setTerrainParams( int heightmap, int texture, int detail ) {
     mHeightmap=heightmap;
     mTexture=texture;
     mDetail=detail;

     onButtonPress(0);

}

int CEditorTerrainOptionsWindow::getHeightmap() {
    return( mHeightmap );
}

int CEditorTerrainOptionsWindow::getTexture() {
    return(mTexture);
}

int CEditorTerrainOptionsWindow::getDetail() {
    return( mDetail );
}

/*
void CEditorTerrainOptionsWindow::getTerrainParams( int &heightmap, int &texture, int &detail ) {
     heightmap=mHeightmap;
     texture=mTexture;
     detail=mDetail;
}
*/

bool CEditorTerrainOptionsWindow::getHeightmapChanged() {
    bool c=mHeightmapChanged;
    mHeightmapChanged=false;
    return(c);
}

bool CEditorTerrainOptionsWindow::getTextureChanged() {
    bool c=mTextureChanged;
    mTextureChanged=false;
    return(c);
}

bool CEditorTerrainOptionsWindow::getDetailChanged() {
    bool c=mDetailChanged;
    mDetailChanged=false;
    return(c);
}


void CEditorTerrainOptionsWindow::onButtonPress( DButton* b ) {

    if( b==mButtonPreviousHeightmap && mHeightmap>0 ) { mHeightmap--; mHeightmapChanged=true; }
    if( b==mButtonNextHeightmap && mHeightmap<mMaxHeightmap ) { mHeightmap++; mHeightmapChanged=true; }

    mTextHeightmap->setText( Ogre::StringConverter::toString( mHeightmap ));

    if( b==mButtonPreviousTexture && mTexture>0 ) { mTexture--; mTextureChanged=true; }
    if( b==mButtonNextTexture && mTexture<mMaxTexture ) { mTexture++; mTextureChanged=true; }

    mTextTexture->setText( Ogre::StringConverter::toString( mTexture ));

    if( b==mButtonPreviousDetail && mDetail>0 ) { mDetail--; mDetailChanged=true; }
    if( b==mButtonNextDetail && mDetail<mMaxDetail ) { mDetail++; mDetailChanged=true; }

    mTextDetail->setText( Ogre::StringConverter::toString( mDetail ));

    if( b==mOKButton ) {
        mDone=true;
    }

}





// ------------------------------------------------------------------

/// Object creation window

CEditorObjectCreationWindow::CEditorObjectCreationWindow():
    CEditorBasicWindow( DWindow::CENTERED, DWindow::CENTERED, "New object" ) {

    // populate with objectdefs
    mWindow->newLine();
    mButtonCancel=mWindow->addButton( "Cancel", 60, this );
    mWindow->newLine();

    mSelected=-1;

}

void CEditorObjectCreationWindow::addObject( Ogre::String name ) {

    DButton* b;

    b=mWindow->addButton( name, 100, this );

    mButton.push_back( b );

    if( mButton.size()%4==0 ) {
        mWindow->newLine();
    }

}



int CEditorObjectCreationWindow::getSelected() {
    return( mSelected );
}



void CEditorObjectCreationWindow::onButtonPress( DButton* b ) {

    if( b==mButtonCancel ) {

        mSelected=-1;

    } else {

        int n;
        std::vector<DButton*>::const_iterator i;

        for( n=0, i=mButton.begin();
            i != mButton.end(); n++, ++i ) {
                if( (*i)==b) {
                    mSelected=n;
                    break;
                }
        }

    }

    mDone=true;
}



// ------------------------------------------------------------------

/// Object options window

CEditorObjectOptionsWindow::CEditorObjectOptionsWindow():
    CEditorBasicWindow( DWindow::CENTERED, DWindow::CENTERED, "Object Options" ) {

        mWindow->newLine();
        mButtonMove=mWindow->addButton( "Move", 100, this );
        mWindow->newLine();
        mButtonRotate=mWindow->addButton( "Rotate", 100, this );
        mWindow->newLine();
        mButtonScale=mWindow->addButton( "Scale",100, this );
        mWindow->newLine();
        mWindow->newLine();
        mButtonDelete=mWindow->addButton( "Delete", 100, this );

    mMessage=EM_NOTHING;
}


EDITORMESSAGE CEditorObjectOptionsWindow::getMessage() {
    return( mMessage );
}

void CEditorObjectOptionsWindow::onButtonPress( DButton* b) {

    if( b==mButtonMove ) mMessage=EM_MOVEOBJECT;
    if( b==mButtonRotate ) mMessage=EM_ROTATEOBJECT;
    if( b==mButtonScale ) mMessage=EM_SCALEOBJECT;
    if( b==mButtonDelete ) mMessage=EM_DELETEOBJECT;

    mDone=true;

}










