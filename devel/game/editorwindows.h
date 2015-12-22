
#ifndef _EDITORWINDOWS_
#define _EDITORWINDOWS_

#include "dynagui.h"

enum EDITORSTATE { ES_NOTHING, ES_TERRAIN, ES_LEVEL, ES_OBJECT, ES_COLOR, ES_PREVIOUS, ES_NEXT, ES_SAVE, ES_QUIT, ES_PREVIOUS_TYPE, ES_NEXT_TYPE };
enum EDITORMESSAGE { EM_NOTHING, EM_MOVEOBJECT, EM_ROTATEOBJECT, EM_SCALEOBJECT, EM_DELETEOBJECT };

class CEditorBasicWindow {
    public:
    CEditorBasicWindow(DWindow::Placement hp, DWindow::Placement vp, Ogre::String title );
    ~CEditorBasicWindow();
    bool done();

    void show();
    void hide();

    protected:

    DynaGUI* mGUI;
    DWindow* mWindow;
    bool mDone;

};


class CEditorInfoWindow: public CEditorBasicWindow {

    public:
    CEditorInfoWindow();

    void setInfo( Ogre::String line );
    void setLevel( Ogre::String line );
    void setStatus( Ogre::String line );

    private:
    DText* mLevelLine;
    DText* mStatusLine;
    DText* mInfoLine;


};



class CEditorMenuWindow: public CEditorBasicWindow, public DynaGUIButtonListener {

    public:
    CEditorMenuWindow();

    EDITORSTATE getStatus();

    void onButtonPress( DButton* b );

    void setLevelNum( int aLevel );

    void setGameType( int aGameType );

    private:
    DButton* mButtonTerrain;
    DButton* mButtonLevel;
    DButton* mButtonPrevious;
    DButton* mButtonNext;

    DButton* mButtonPreviousLevelType;
    DText* mTextLevelType;
    DButton* mButtonNextLevelType;

    DText* mTextLevel;

    DButton* mButtonSave;
    DButton* mButtonQuit;

    EDITORSTATE mStatus;

};


class CEditorLevelOptionsWindow:public CEditorBasicWindow, public DynaGUIButtonListener {

    public:
    CEditorLevelOptionsWindow();
//    ~CEditorLevelOptionsWindow();
    void onButtonPress( DButton* b );

    void setFogColor( int red, int green, int blue );
    void getFogColor( int &red, int &green, int &blue );
    void setGravity( float gravity );
    float getGravity();
    bool getColorChanged();
    bool getWipeObjects();

    private:
    DButton* mOKButton;
    DText* mTextName;
    DText* mTextType;
    DButton* mButtonRedMinus;
    DButton* mButtonRedPlus;
    DText* mTextRed;
    DButton* mButtonGreenMinus;
    DButton* mButtonGreenPlus;
    DText* mTextGreen;
    DButton* mButtonBlueMinus;
    DButton* mButtonBluePlus;
    DText* mTextBlue;

    DButton* mButtonGravityMinus;
    DText* mTextGravity;
    DButton* mButtonGravityPlus;

    DButton* mButtonWipeObjects;

    int mRed, mGreen, mBlue;
    float mGravity;
    bool mWipeObjects;
    bool mColorChanged;


};


class CEditorTerrainOptionsWindow:public CEditorBasicWindow, public DynaGUIButtonListener {

    public:
    CEditorTerrainOptionsWindow();
//    ~CEditorLevelOptionsWindow();
    void onButtonPress( DButton* b );

    void setTerrainParams( int heightmap, int texture, int detail );
    // void getTerrainParams( int &heightmap, int &texture, int &detail );
    int getHeightmap();
    int getTexture();
    int getDetail();

    bool getHeightmapChanged();
    bool getTextureChanged();
    bool getDetailChanged();


    private:
    DButton* mOKButton;
    DButton* mButtonPreviousHeightmap;
    DButton* mButtonNextHeightmap;
    DButton* mButtonPreviousTexture;
    DButton* mButtonNextTexture;
    DButton* mButtonPreviousDetail;
    DButton* mButtonNextDetail;

    DText* mTextHeightmap;
    DText* mTextTexture;
    DText* mTextDetail;

    int mHeightmap;
    int mTexture;
    int mDetail;

    int mMaxHeightmap;
    int mMaxTexture;
    int mMaxDetail;

    bool mHeightmapChanged;
    bool mTextureChanged;
    bool mDetailChanged;

};


class CEditorObjectCreationWindow:public CEditorBasicWindow, public DynaGUIButtonListener {

    public:
    CEditorObjectCreationWindow();
//    ~CEditorObjectCreationWindow();
    void onButtonPress( DButton* b );

    void addObject( Ogre::String name );
    int getSelected();

    private:
    std::vector<DButton*> mButton;
    int mSelected;
    DButton* mButtonCancel;

};


class CEditorObjectOptionsWindow:public CEditorBasicWindow, public DynaGUIButtonListener {
    public:
    CEditorObjectOptionsWindow();
//    ~CEditorObjectOptionsWindow();
    void onButtonPress( DButton* b );
    EDITORMESSAGE getMessage();

    private:
    EDITORMESSAGE mMessage;
    DButton* mButtonMove;
    DButton* mButtonRotate;
    DButton* mButtonScale;
    DButton* mButtonDelete;

};



#endif


