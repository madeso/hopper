

#ifndef _DYNAGUI_
#define _DYNAGUI_

/**
    DynaGUI is similar to BetaGUI with a twist :
        Elements are added to a window like characters are entered in a text editor.
        The window auto-resized itself while elements are added, 'line' by 'line'.
        Call newLine to begin at the next 'line'.
        Current Elements : Buttons, Checkboxes, Texts ( labels ), Progressbars
            and SubWidows for easy alignment.
        Currently, windows can't be resized or moved by the user.

        Ideas to implement :
            - image windows ( windows with a custom material )
            - subwindows as image containers ( pass a "custom" material to it )
            - sliders
            - effects on widows creation and destruction
*/

#include <string>
#include <vector>
#include "Ogre.h"
#include "OgreTextAreaOverlayElement.h"

#include "singleton.h"

#include "inputmanager.h"



/*
class DPressable {
    void notifyListener();
};
*/

/*
class Dragable {
};
*/

class DButton;

class DynaGUIButtonListener {
    public:
    virtual void onButtonPress( DButton* button )=0;
    virtual ~DynaGUIButtonListener() {};
};


class DynaGUI;

class DWidget {

    friend class DynaGUI; // for access to mOverlay ( in order to do remove2D for the root Window )

    public:
    DWidget( DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh, Ogre::String materialname);
    virtual ~DWidget();

    void addChild( Ogre::OverlayElement* child );
    void removeChild( Ogre::String name );

    void setPosition( float x, float y );
    void resize( float w, float h );
    void setDynaMaterial( Ogre::String materialname );
    void setMaterial( Ogre::String materialname );
    Ogre::String getName();
    void show();
    void hide();
    bool isVisible();

    protected:
    Ogre::OverlayContainer* mOverlay;
    Ogre::String mMaterialName;
    DynaGUI* mGUI;
    DWidget* mParent;
    Ogre::String mName;

};



class DText {

    public:

    DText( DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh, Ogre::String text, float Size=0 );
    ~DText();
    void setText( const Ogre::String& text );
    void setColour( Ogre::ColourValue colour );
    void setColour( float red, float green, float blue );

    Ogre::String getText();

    private:
    Ogre::String mText;
    Ogre::TextAreaOverlayElement* mOverlay;
    Ogre::String mName;
    DWidget* mParent;
};



class DProgressBar:public DWidget {

    public:
    DProgressBar(DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh,
        Ogre::String materialname);

    int getPercent();
    void setPercent( int percent );

    private:
    float mWidth, mHeight;
    int mPercent;


};




class DButton:public DWidget {

    public:
    DButton( DynaGUI* gui, DWidget* widget, Ogre::String name, Ogre::Vector4 xywh,
        Ogre::String caption, DynaGUIButtonListener* listener );
    virtual ~DButton();
    void setCaption( const Ogre::String& caption );
    Ogre::String getCaption();
    bool in( float x, float y );
    bool isActive();
    void activate();
    void deactivate();
    void notifyListener();

    private:
    DText* mCaption;
    bool mActive;
    DynaGUIButtonListener* mListener;

};


class DCheckbox:public DButton, public DynaGUIButtonListener {

    public:
    DCheckbox( DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh, bool checked );
    virtual ~DCheckbox();
    bool checked();
    void check();
    void uncheck();
    void onButtonPress( DButton* button );

    private:
    bool mChecked;
    DWidget* mCheckOverlay;

};



class DSubWindow;
class DImage;

class DWindow: public DWidget {

    friend class DynaGUI; // for access to checkButtons

    public:
    enum Placement { MIN, MAX, CENTERED };
    // constructor for "normal" windows ( directly in the root window )
    DWindow( DynaGUI* gui, Ogre::String name, DWindow::Placement horiz, DWindow::Placement vert, Ogre::String title );

    // constructor for SubWindows
    DWindow( DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh );


    virtual ~DWindow();

    void debugElements();


    DButton* addButton( Ogre::String caption, float width, DynaGUIButtonListener* listener );
    DCheckbox* addCheckbox( bool checked );
    void addSpace( float width );
    void newLine();
    DProgressBar* addProgressBar( Ogre::String material, float width );
    DText* addText( Ogre::String text, float width, float size = 0 );
    DSubWindow* addSubWindow( float width, float height );
    DImage* addImage( float width, float height, Ogre::String materialname );

    protected:
    void init();

    float mGap;
    float mInsertXPos;
    float mInsertYPos;
    float mLineHeight;
    float mWidth, mHeight;

    /**
        adjust the window size based on the given dimensions ( added elements )
        then reposition the window
    */
    virtual void computePosAndSize( float width, float height );

    /**
        change the window position based on its size and placement commands
    */
    void computePosition();

    private:

    DWidget* mTitleBar;
    DWindow::Placement mHorizPlacement;
    DWindow::Placement mVertPlacement;

    float mX, mY;

    std::vector<DText*> mTexts;
    std::vector<DProgressBar*> mProgressBars;
    std::vector<DSubWindow*> mSubWindows;
    std::vector<DButton*> mButtons;
    DText* mTitle;
    void checkButtons( float x, float y, bool pressed );

};



class DSubWindow: public DWindow {
    public:
    DSubWindow(DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh  );

    protected:
    // overridden from DWindow - SubWindows don't auto-resize.
    void computePosAndSize( float width, float height );

};


class DImage: public DSubWindow {
    public:
    DImage( DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh, Ogre::String materialname );

/*
    //overriden from DWidget to avoid the DynaGUI prefix
    void setMaterial( Ogre::String materialname );
*/
//private:

};


class DynaGUI : public SSingleton<DynaGUI> {

    public:
    //DynaGUI( Ogre::GuiMetricsMode mode, Ogre::String baseMaterialname, Ogre::String fontname, float scale=1.f );
    DynaGUI();
    ~DynaGUI();

    DWindow* createWindow( DWindow::Placement horiz, DWindow::Placement vert, Ogre::String title = Ogre::StringUtil::BLANK );
    void destroyWindow( DWindow* window );

    DWindow* createImageWindow( Ogre::Vector4 xywh, Ogre::String materialName );

    void add3D( Ogre::SceneNode* node );
    void remove3D( Ogre::SceneNode* node );

    void createMousePointer( float w, float h, Ogre::String material );
    void showMousePointer();
    void hideMousePointer();

    void injectMouse( int x, int y, bool pressed );
    void injectMouse();

    const Ogre::String& getBaseMaterial() const;
    DWidget* getRootWindow() const;
    const Ogre::String& getFontName() const;
    float getFontSize() const;
    float getScale() const;
    /*
    Ogre::GuiMetricsMode DynaGUI::getMetricsMode() const;
    */
    int getViewportWidth();
    int getViewportHeight();

    void debugOverlays();
    void debugOverlayElements( int level, Ogre::OverlayContainer::ChildContainerIterator it );

    /**
        re-position windows after a resolution change
        TODO : also change every element sizes based on the new resolution ?
    */
    void resolutionChanged();

    private:
    // Ogre::GuiMetricsMode mMetricsMode;
    Ogre::String mBaseMaterialName;
    Ogre::String mFontName;
    float mScale;
    float mBaseHeight;
    float mFontSize;
    Ogre::OverlayContainer* mMousePointer;

    std::list<DWindow*> mWindows;

    CInputManager* mInputManager;

    Ogre::Overlay* mMainOverlay;
    DWidget* mRootWindow;

    int mViewportWidth;
    int mViewportHeight;

    void checkButtons( float x, float y, bool pressed );
    void checkSubWindowsButtons( DSubWindow* subwin );



};



#endif

