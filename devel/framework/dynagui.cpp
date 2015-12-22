
#include "dynagui.h"
#include "logger.h"

#include "screen.h"






// ======================================================
// ========== Defaults ==================================
// ======================================================

//#define DYNAGUI_METRICS_MODE Ogre::GMM_PIXELS
#define DYNAGUI_METRICS_MODE PIXELS

#define DYNAGUI_BASE_MATERIAL_NAME "DynaGUI"

#define DYNAGUI_FONT_NAME "LCD"

#define DYNAGUI_SCALE 1.28f

// ======================================================
// ======================================================
// ======================================================

#define DYNAGUI_BASEHEIGHT_RELATIVE 0.04
#define DYNAGUI_BASEHEIGHT_ABSOLUTE 20

#define DYNAGUI_BASEGAP_RELATIVE 0.02
#define DYNAGUI_BASEGAP_ABSOLUTE 8

#define DYNAGUI_TEXTSHIFT_RELATIVE 0.005
#define DYNAGUI_TEXTSHIFT_ABSOLUTE 3








/** @brief DynaGUI
  *
  * @todo: document this function
  */
DynaGUI::DynaGUI():
//    mMetricsMode( DYNAGUI_METRICS_MODE ),
    mBaseMaterialName( DYNAGUI_BASE_MATERIAL_NAME ),
    mFontName( DYNAGUI_FONT_NAME ),
    mScale( DYNAGUI_SCALE ),
    mBaseHeight( 0 ),
    mFontSize( 0 ),
    mMousePointer(0) {


    mViewportWidth = CScreen::getInstance()->getRenderWindow()->getWidth();
    mViewportHeight = CScreen::getInstance()->getRenderWindow()->getHeight();

    mMainOverlay=Ogre::OverlayManager::getSingleton().create( "DynaGUI" );

    mMainOverlay->show();


    //mMetricsMode==Ogre::GMM_PIXELS?DYNAGUI_BASEHEIGHT_ABSOLUTE:DYNAGUI_BASEHEIGHT_RELATIVE

    #if DYNAGUI_METRICS_MODE == PIXELS
    // if( mMetricsMode == Ogre::GMM_PIXELS ) {
        mRootWindow=new DWidget( this, 0, "RootWindow",
            Ogre::Vector4( 0,0, mViewportHeight, mViewportWidth), mBaseMaterialName );
        mBaseHeight = DYNAGUI_BASEHEIGHT_ABSOLUTE;
        mFontSize = mBaseHeight*mScale;
    //}
    #else
    //else {
        mRootWindow=new DWidget( this, 0, "RootWindow",
            Ogre::Vector4( 0,0,1,1), mBaseMaterialName );
        mBaseHeight = DYNAGUI_BASEHEIGHT_RELATIVE;
        mFontSize = mBaseHeight*mScale;
    //}
    #endif

    createMousePointer( 32, 32, "MousePointer" );
    hideMousePointer();

    mInputManager = CInputManager::getInstance();

}



/** @brief don't worry be cleany
  *
  */
DynaGUI::~DynaGUI() {

    Ogre::LogManager::getSingleton().getDefaultLog()->logMessage( "DynaGUI : deleting windows..." );

    for( std::list<DWindow*>::const_iterator it=mWindows.begin(); it != mWindows.end(); ++it ) {
        delete( (*it) );
    }


    Ogre::LogManager::getSingleton().getDefaultLog()->logMessage( "DynaGUI : deleting the main overlay..." );

    mMainOverlay->remove2D( mRootWindow->mOverlay ); // avoid crash in Ogre::Compositor something
    delete( mRootWindow ); // destroy all subwindows
    Ogre::OverlayManager::getSingleton().destroy( mMainOverlay );


    if( mMousePointer ) {
        Ogre::LogManager::getSingleton().getDefaultLog()->logMessage( "DynaGUI : deleting the mouse pointer..." );
        Ogre::OverlayManager::getSingleton().destroyOverlayElement( "MousePointer");
        Ogre::OverlayManager::getSingleton().destroy( "DynaGUI.MousePointer" );
    }

//    debugOverlays();

}


/** @brief injectMouse
  *
  * @todo: document this function
  */
void DynaGUI::injectMouse(int x, int y, bool pressed) {

    float xx = ((float)(x))/mViewportWidth;
    float yy = ((float)(y))/mViewportHeight;



    #if DYNAGUI_METRICS_MODE == PIXELS
        if( mMousePointer ) mMousePointer->setPosition( x,y );
    #else
        if( mMousePointer ) mMousePointer->setPosition( xx, yy );
    #endif

/*
    if( mMousePointer ) {
        switch( mMetricsMode ) {
            case Ogre::GMM_PIXELS:
                mMousePointer->setPosition( x,y );
            break;

            case Ogre::GMM_RELATIVE_ASPECT_ADJUSTED:
            case Ogre::GMM_RELATIVE:
                mMousePointer->setPosition( xx, yy );
            break;
        }
    }
*/

    checkButtons( xx,yy, pressed );  // Ogre::Overlay::contains(x,y) is always in relative mode...

}


// this is a bad hack
#include "hopperinputactions.h"
void DynaGUI::injectMouse() {

    injectMouse( mInputManager->getMouseX(), mInputManager->getMouseY(),
        mInputManager->isLeftMouseButtonReleased() );

}


void DynaGUI::checkButtons( float x, float y, bool pressed ) {


    for( std::list<DWindow*>::const_iterator w=mWindows.begin(); w != mWindows.end(); ++w ) {
        if( (*w)->isVisible() ) {
            (*w)->checkButtons( x, y, pressed );
        }
    }

}



void DynaGUI::resolutionChanged() {

    for( std::list<DWindow*>::const_iterator w=mWindows.begin(); w != mWindows.end(); ++w ) {

        (*w)->computePosition();

    }

}


/** @brief hide the mouse pointer
  */
void DynaGUI::hideMousePointer() {
    if(mMousePointer) mMousePointer->hide();
}

/** @brief show the mouse pointer
  */
void DynaGUI::showMousePointer() {
    if(mMousePointer) mMousePointer->show();
}

/** @brief create the mouse pointer
  */
void DynaGUI::createMousePointer( float w, float h, Ogre::String material) {

    Ogre::Overlay* o=Ogre::OverlayManager::getSingleton().create("DynaGUI.MousePointer");

    o->setZOrder(649);

    mMousePointer=static_cast<Ogre::OverlayContainer*>(
                Ogre::OverlayManager::getSingleton().createOverlayElement(
                    "Panel", "MousePointer" ) );

    #if DYNAGUI_METRICS_MODE == PIXELS
        mMousePointer->setMetricsMode( Ogre::GMM_PIXELS );
    #else
        mMousePointer->setMetricsMode( Ogre::GMM_RELATIVE );
    #endif
    /*
    if( mMetricsMode == Ogre::GMM_PIXELS )
        mMousePointer->setMetricsMode( Ogre::GMM_PIXELS );
    else
        mMousePointer->setMetricsMode( Ogre::GMM_RELATIVE );
    */

    mMousePointer->setPosition( 0, 0 );
    mMousePointer->setDimensions( w, h );

    mMousePointer->setMaterialName( mBaseMaterialName + "." + material );

    o->add2D(mMousePointer);
    o->show();
    mMousePointer->show();


}


/** @brief createWindow
  *
  * @todo: create a normal window
  */
DWindow* DynaGUI::createWindow( DWindow::Placement horiz, DWindow::Placement vert, Ogre::String title ) {

    DWindow* win;

    win=new DWindow( this, "Window#"+ Ogre::StringConverter::toString( mWindows.size() ), horiz, vert, title );
    mWindows.push_back( win );

    return( win );

}

void DynaGUI::destroyWindow( DWindow* window ) {

    delete( window );
    mWindows.remove( window );


}

DWindow* DynaGUI::createImageWindow( Ogre::Vector4 xywh, Ogre::String materialName ) {

    DWindow* win;
    win = new DWindow( this,"ImageWindow#"+ Ogre::StringConverter::toString( mWindows.size() ), DWindow::MIN, DWindow::MIN, Ogre::StringUtil::BLANK );

    mWindows.push_back( win );

    win->setPosition( xywh.x, xywh.y );
    win->resize( xywh.z, xywh.w );
    win->setMaterial( materialName );

    return(win);
}


void DynaGUI::add3D( Ogre::SceneNode* node ) {
    mMainOverlay->add3D( node );
}

void DynaGUI::remove3D( Ogre::SceneNode* node ) {
    mMainOverlay->remove3D( node );
}


DWidget* DynaGUI::getRootWindow() const {
    return( mRootWindow );
}

const Ogre::String& DynaGUI::getBaseMaterial() const {
    return( mBaseMaterialName );
}


const Ogre::String& DynaGUI::getFontName() const {
    return( mFontName );
}

float DynaGUI::getFontSize() const {
    return( mFontSize );
}


float DynaGUI::getScale() const {
    return( mScale );
}

/*
Ogre::GuiMetricsMode DynaGUI::getMetricsMode() const {
    return( mMetricsMode );
}
*/

int DynaGUI::getViewportWidth() {
    mViewportWidth = CScreen::getInstance()->getRenderWindow()->getWidth();
    return( mViewportWidth );
}

int DynaGUI::getViewportHeight() {
    mViewportHeight = CScreen::getInstance()->getRenderWindow()->getHeight();
    return( mViewportHeight );
}


void DynaGUI::debugOverlays() {

        Ogre::OverlayManager::OverlayMapIterator i(Ogre::OverlayManager::getSingletonPtr()->getOverlayIterator());
        Ogre::Overlay* o;
        Ogre::OverlayContainer* oe;

        Ogre::Log* log=Ogre::LogManager::getSingleton().getDefaultLog();


        log->logMessage( "----------------------------------------------------");

        while( i.hasMoreElements() ) {
            o=i.peekNextValue();

            log->logMessage( o->getName() );

            Ogre::Overlay::Overlay2DElementsIterator oeit(o->get2DElementsIterator() );

            while( oeit.hasMoreElements() ) {
                oe=oeit.getNext();
                log->logMessage( "  +" + oe->getName() );

                debugOverlayElements( 1, oe->getChildContainerIterator() );

            }

            i.moveNext();
        }

        log->logMessage("----------------------------------------------------");

}

void DynaGUI::debugOverlayElements( int level, Ogre::OverlayContainer::ChildContainerIterator it ) {

    Ogre::OverlayContainer* o;

    while( it.hasMoreElements() ) {

        o=it.getNext();

        Ogre::String s;
        for( int i=0; i<level; ++i ) { s += "    "; }

        Ogre::LogManager::getSingleton().getDefaultLog()->logMessage( s + "+" + o->getName() );

        debugOverlayElements( level+1, o->getChildContainerIterator() );
    }
}


// ***********************************************************
// ******************   DWidget   ****************************
// ***********************************************************


/** @brief container for Ogre::PanelOverlay
  *
  */
DWidget::DWidget( DynaGUI* gui, DWidget* parent,
    Ogre::String name, Ogre::Vector4 xywh, Ogre::String materialname ) {

    mMaterialName=materialname;
    mGUI=gui;


    if( parent != 0 ) {

        mParent = parent;
        mName = parent->getName() + "_" + name;

        mOverlay=static_cast<Ogre::OverlayContainer*>(
            Ogre::OverlayManager::getSingleton().createOverlayElement(
                "Panel", mName ) );

        #if DYNAGUI_METRICS_MODE == PIXELS
        mOverlay->setMetricsMode( Ogre::GMM_PIXELS );
        #else
        mOverlay->setMetricsMode( Ogre::GMM_RELATIVE );
        #endif
        /*
        if( mGUI->getMetricsMode() == Ogre::GMM_PIXELS )
            mOverlay->setMetricsMode( Ogre::GMM_PIXELS );
        else
            mOverlay->setMetricsMode( Ogre::GMM_RELATIVE );
        */
        mOverlay->setPosition( xywh.x, xywh.y );
        mOverlay->setDimensions( xywh.z, xywh.w );
        parent->addChild( static_cast<Ogre::OverlayElement*>(mOverlay) );

        setDynaMaterial(  mMaterialName );


    }
    else {  // root window

        mParent = 0;
        mName = Ogre::StringUtil::BLANK;

        mOverlay=static_cast<Ogre::OverlayContainer*>(
            Ogre::OverlayManager::getSingleton().createOverlayElement(
                "Panel", name ));

        #if DYNAGUI_METRICS_MODE == PIXELS
        mOverlay->setMetricsMode( Ogre::GMM_PIXELS );
        #else
        mOverlay->setMetricsMode( Ogre::GMM_RELATIVE );
        #endif
        /*
        if( mGUI->getMetricsMode() == Ogre::GMM_PIXELS )
            mOverlay->setMetricsMode( Ogre::GMM_PIXELS );
        else
            mOverlay->setMetricsMode( Ogre::GMM_RELATIVE );
        */

        mOverlay->setPosition( 0, 0 );
        mOverlay->setDimensions( xywh.z, xywh.w );

        Ogre::OverlayManager::getSingleton().getByName("DynaGUI")->add2D( mOverlay );

    }


    mOverlay->show();

}


void DWidget::addChild( Ogre::OverlayElement* child ) {
    mOverlay->addChild( child );
}

void DWidget::removeChild( Ogre::String name ) {
    mOverlay->removeChild( name );
}


DWidget::~DWidget() {
    if( mParent ) mParent->removeChild( mName );
    Ogre::OverlayManager::getSingleton().destroyOverlayElement( mOverlay );
}


Ogre::String DWidget::getName() {
    return( mOverlay->getName() );
}

void DWidget::setDynaMaterial(Ogre::String materialname) {
    mOverlay->setMaterialName( mGUI->getBaseMaterial() + "." + materialname );
    //mOverlay->_update();
}

void DWidget::setMaterial( Ogre::String materialname ) {
    mOverlay->setMaterialName( materialname );
}

void DWidget::resize( float w, float h) {
    mOverlay->setDimensions( w, h );
}


void DWidget::setPosition( float x, float y ) {

    mOverlay->setPosition( x, y );

}


void DWidget::show() {
    mOverlay->show();
}

void DWidget::hide() {
    mOverlay->hide();
}

bool DWidget::isVisible() {
    return( mOverlay->isVisible() );
}

// ***********************************************************
// ******************   DWindow   ****************************
// ***********************************************************

/** @brief DWindow constructor for "normal" window ( attached to the root window )
  */
DWindow::DWindow( DynaGUI* gui, Ogre::String name,
    DWindow::Placement horiz, DWindow::Placement vert, Ogre::String title)
    :DWidget( gui, gui->getRootWindow(),
            name,
            Ogre::Vector4( 0, 0, 0, 0 ),
            "Window" ) {

    init();

    // if there's a title string then create the title bar, and shift the vertical insert position.
    if( title != Ogre::StringUtil::BLANK ) {
        mInsertYPos = mLineHeight + mGap;

        mTitleBar=new DWidget( mGUI, this,  "TitleBar", Ogre::Vector4( mX, mY, mWidth, mLineHeight ), "Window.TitleBar" );
        mTitle=new DText( mGUI, this, "TitleText", Ogre::Vector4( mX,mY, mWidth, mLineHeight ), title );
    }

    mHorizPlacement=horiz;
    mVertPlacement=vert;

    //computePosAndSize( 0, 0);


}

/** DWindow constructor for sub-windows
*/
DWindow::DWindow( DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh )
    :DWidget( gui, parent, name, xywh, "SubWindow" ) {

    init();

    mWidth=xywh.z * mGUI->getScale();   // subwindows : the dimensions are given
    mHeight= xywh.w * mGUI->getScale();

    mHorizPlacement=DWindow::MIN;
    mVertPlacement=DWindow::MIN;

}


/// common init for windows and sub-windows
void DWindow::init() {

    #if DYNAGUI_METRICS_MODE == PIXELS
        mGap=DYNAGUI_BASEGAP_ABSOLUTE * mGUI->getScale();
        mLineHeight=DYNAGUI_BASEHEIGHT_ABSOLUTE * mGUI->getScale();
    #else
        mGap=DYNAGUI_BASEGAP_RELATIVE * mGUI->getScale();
        mLineHeight=DYNAGUI_BASEHEIGHT_RELATIVE * mGUI->getScale();
    #endif

/*
    if( mGUI->getMetricsMode() == Ogre::GMM_PIXELS ) {
        mGap=DYNAGUI_BASEGAP_ABSOLUTE * mGUI->getScale();
        mLineHeight=DYNAGUI_BASEHEIGHT_ABSOLUTE * mGUI->getScale();
    }
    else {
        mGap=DYNAGUI_BASEGAP_RELATIVE * mGUI->getScale();
        mLineHeight=DYNAGUI_BASEHEIGHT_RELATIVE * mGUI->getScale();
    }
*/

    mX=mY=0;

    mInsertXPos=mGap;
    mInsertYPos=mGap;

    mTitleBar=0;
    mTitle=0;

    mWidth=0;
    mHeight=0;
}




/** @brief can we be clean ? yes we can !
  */
DWindow::~DWindow() {

    // LOG("\n~DWindow() ");

    if( mTitle ) delete( mTitle );
    if( mTitleBar) delete( mTitleBar );


    // LOG("\nPurge Buttons..." );

    // purge Buttons ( this include checkboxes )
    for( std::vector<DButton*>::const_iterator it=mButtons.begin(); it != mButtons.end(); ++it ) {
        delete( (*it) );
    }

    // purge texts
    for( std::vector<DText*>::const_iterator it=mTexts.begin(); it != mTexts.end(); ++it ) {
        delete( (*it) );
    }

    // purge progress bars
    for( std::vector<DProgressBar*>::const_iterator it=mProgressBars.begin(); it != mProgressBars.end(); ++it ) {
        delete( (*it) );
    }

    for( std::vector<DSubWindow*>::const_iterator it=mSubWindows.begin(); it != mSubWindows.end(); ++it ) {
        delete( (*it) );
    }

    // LOG("\n~DWindow() : done" );

}


void DWindow::debugElements() {

    LOG("\nWindow %s", mTitle->getText().c_str() );

    for( std::vector<DButton*>::const_iterator it=mButtons.begin(); it != mButtons.end(); ++it ) {
        LOG("\n  Button : %s", (*it)->getCaption().c_str() );
    }

    // purge texts
    for( std::vector<DText*>::const_iterator it=mTexts.begin(); it != mTexts.end(); ++it ) {
        LOG("\n  Text : %s", (*it)->getText().c_str() );
    }

    // purge progress bars
    for( std::vector<DProgressBar*>::const_iterator it=mProgressBars.begin(); it != mProgressBars.end(); ++it ) {
        LOG("\n  Progress Bar" );
    }

    for( std::vector<DSubWindow*>::const_iterator it=mSubWindows.begin(); it != mSubWindows.end(); ++it ) {
        (*it)->debugElements();
    }

}



void DWindow::computePosAndSize( float width, float height ) {

    mInsertXPos += width + mGap;

    if( mInsertXPos > mWidth )
        mWidth=mInsertXPos;

    mInsertYPos += height;

    mHeight=mInsertYPos + mLineHeight + mGap + mGap;

    resize( mWidth, mHeight );


    if( mTitleBar )
        mTitleBar->resize( mWidth, mLineHeight );
    // mTitle->resize( mWidth, mLineHeight );


    computePosition();

/*
    if( mGUI->getMetricsMode() == Ogre::GMM_PIXELS ) {

        switch( mHorizPlacement ) {
            case MIN: mX=0; break;
            case CENTERED: mX=(mGUI->getViewportWidth()-mWidth)/2.f; break;
            case MAX: mX=mGUI->getViewportWidth()-mWidth; break;
        }

        switch( mVertPlacement ) {
            case MIN: mY=0; break;
            case CENTERED: mY=(mGUI->getViewportHeight()-mHeight)/2.f; break;
            case MAX: mY=mGUI->getViewportHeight()-mHeight; break;
        }

    }
    else {
        switch( mHorizPlacement ) {
            case MIN: mX=0; break;
            case CENTERED: mX=(1.f-mWidth)/2.f; break;
            case MAX: mX=1.f-mWidth; break;
        }

        switch( mVertPlacement ) {
            case MIN: mY=0; break;
            case CENTERED: mY=(1.f-mHeight)/2.f; break;
            case MAX: mY=1.f-mHeight; break;
        }

    }


    setPosition( mX, mY );
    */

}


void DWindow::computePosition() {

    #if DYNAGUI_METRICS_MODE == PIXELS
    // if( mGUI->getMetricsMode() == Ogre::GMM_PIXELS ) {

        switch( mHorizPlacement ) {
            case MIN: mX=0; break;
            case CENTERED: mX=(mGUI->getViewportWidth()-mWidth)/2.f; break;
            case MAX: mX=mGUI->getViewportWidth()-mWidth; break;
        }

        switch( mVertPlacement ) {
            case MIN: mY=0; break;
            case CENTERED: mY=(mGUI->getViewportHeight()-mHeight)/2.f; break;
            case MAX: mY=mGUI->getViewportHeight()-mHeight; break;
        }

    //}
    #else
    //else {
        switch( mHorizPlacement ) {
            case MIN: mX=0; break;
            case CENTERED: mX=(1.f-mWidth)/2.f; break;
            case MAX: mX=1.f-mWidth; break;
        }

        switch( mVertPlacement ) {
            case MIN: mY=0; break;
            case CENTERED: mY=(1.f-mHeight)/2.f; break;
            case MAX: mY=1.f-mHeight; break;
        }

    //}
    #endif

    setPosition( mX, mY );
}



/** @brief adds a line of text
  *
  */
DText* DWindow::addText( Ogre::String text, float width, float size ) {

    width = width * mGUI->getScale();
    size = size * mGUI->getScale();

    DText* t=new DText(
        mGUI,
        this,
        "Text#" + Ogre::StringConverter::toString(mTexts.size()),
        Ogre::Vector4( mInsertXPos, mInsertYPos, width, mLineHeight ),
        text, size    );

    mTexts.push_back( t );

    computePosAndSize( width, 0 );

    return( t );

}


/** @brief addButton
  *
  * @todo: document this function
  */
DButton* DWindow::addButton(Ogre::String caption, float width, DynaGUIButtonListener* listener ) {

    width=width * mGUI->getScale();

    DButton* b=new DButton( mGUI, this,
        "Button#" + Ogre::StringConverter::toString( mButtons.size()),
        Ogre::Vector4( mInsertXPos, mInsertYPos, width, mLineHeight + mGap ),
        caption, listener );


    mButtons.push_back( b );

    computePosAndSize( width, 0 );

    return(b);

}


/** @brief newLine
  *
  * @todo: document this function
  */
void DWindow::newLine() {

    //mInsertXPos = mGap;
    mInsertXPos = 0;

    computePosAndSize(0, mLineHeight+mGap );

    // reset the lineheight
    #if DYNAGUI_METRICS_MODE == PIXELS
    // if( mGUI->getMetricsMode() == Ogre::GMM_PIXELS )
        mLineHeight=DYNAGUI_BASEHEIGHT_ABSOLUTE*mGUI->getScale();
    #else
    //else
        mLineHeight=DYNAGUI_BASEHEIGHT_RELATIVE*mGUI->getScale();
    #endif
}


/** @brief addSpace
  *
  * @todo: document this function
  */
void DWindow::addSpace( float width) {

   // width=(width-mGap) * mGUI->getScale();  // mGap is re-added below
    computePosAndSize( width, 0 );
}




/** @brief addProgressBar
  *
  * @todo: document this function
  */
DProgressBar* DWindow::addProgressBar( Ogre::String material, float width) {

    width=width * mGUI->getScale();

    DProgressBar* p=new DProgressBar( mGUI, this,
        "ProgressBar#" + Ogre::StringConverter::toString( mProgressBars.size() ),
        Ogre::Vector4( mInsertXPos, mInsertYPos, width, mLineHeight ),
        material );

    mProgressBars.push_back( p );

    computePosAndSize( width, 0 );

    return( p );

}


DSubWindow* DWindow::addSubWindow( float width, float height ) {

    width=width * mGUI->getScale();
    height=height * mGUI->getScale();

    DSubWindow* w=new DSubWindow(
        mGUI,
        this,
        "SubWindow#" + Ogre::StringConverter::toString( mSubWindows.size() ),
        Ogre::Vector4( mInsertXPos, mInsertYPos, width, height ) );


    if( height > mLineHeight )
        mLineHeight=height;

    mSubWindows.push_back( w );

    computePosAndSize( width, 0 );

    return( w );
}


DImage* DWindow::addImage( float width, float height, Ogre::String materialname ) {


    width=width * mGUI->getScale();
    height=height * mGUI->getScale();

    DImage* img=new DImage( mGUI, this, "DImage#" + Ogre::StringConverter::toString( mSubWindows.size()),
        Ogre::Vector4( mInsertXPos, mInsertYPos, width, height ), materialname  );

    mLineHeight = height;

    mSubWindows.push_back( img );

    computePosAndSize( width, 0 );

    return( img );

}



/** @brief addCheckbox
  *
  * @todo: document this function
  */
DCheckbox* DWindow::addCheckbox(bool checked) {


    DCheckbox* cb = new DCheckbox( mGUI, this,
        "Checkbox#" + Ogre::StringConverter::toString( mButtons.size() ),
        Ogre::Vector4( mInsertXPos, mInsertYPos, mLineHeight, mLineHeight ),
        "Button" );

    mButtons.push_back( cb );

    computePosAndSize( mLineHeight,0  );

    return( cb );

}



void DWindow::checkButtons( float x, float y, bool pressed ) {

    for( std::vector<DButton*>::const_iterator b = mButtons.begin(); b != mButtons.end(); ++b ) {

            if( (*b)->in( x, y ) ) {
                if( !(*b)->isActive() ) (*b)->activate();
                if( pressed ) (*b)->notifyListener();
            }
            else {
                if( (*b)->isActive() ) (*b)->deactivate();
            }

    }

    for( std::vector<DSubWindow*>::const_iterator sub = mSubWindows.begin(); sub != mSubWindows.end(); ++sub ) {

        (*sub)->checkButtons( x, y, pressed );

    }

}



// ***********************************************************
// *******************   DText   *****************************
// ***********************************************************
/** @brief DText
  */
DText::DText( DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh,  Ogre::String text, float size ) {

    mName = parent->getName() + "_" + name ;

    mParent = parent;

    mOverlay=static_cast<Ogre::TextAreaOverlayElement*>(
        Ogre::OverlayManager::getSingleton().createOverlayElement(
            "TextArea", mName ) );


    float shift=0;

    #if DYNAGUI_METRICS_MODE == PIXELS

    //if( gui->getMetricsMode() == Ogre::GMM_PIXELS ) {
        shift=DYNAGUI_TEXTSHIFT_ABSOLUTE*gui->getScale();
        mOverlay->setMetricsMode( Ogre::GMM_PIXELS);
    //}
    #else
    //else {
        shift=DYNAGUI_TEXTSHIFT_RELATIVE*gui->getScale();
        mOverlay->setMetricsMode( Ogre::GMM_RELATIVE );
    //}
    #endif


    mOverlay->setPosition( xywh.x+shift, xywh.y+shift );
    mOverlay->setDimensions( xywh.z, xywh.w );
    mParent->addChild( static_cast<Ogre::OverlayElement*>(mOverlay) );


    mOverlay->setFontName( gui->getFontName() );


    if( size > 0)
        mOverlay->setCharHeight( size );
    else
        mOverlay->setCharHeight( gui->getFontSize() );

    //mOverlay->setColourTop( Ogre::ColourValue( 0.8,0.8,1.0 ));
    //mOverlay->setColourBottom( Ogre::ColourValue( 0.4,0.4,0.6 ));
    mOverlay->setColour( Ogre::ColourValue( 0.7, 0.7, 0.7 ));

    mOverlay->setEnabled( true );

    setText( text );

}

DText::~DText() {
    mParent->removeChild( mName );
    Ogre::OverlayManager::getSingleton().destroyOverlayElement( mOverlay );
}


/** @brief setText
  *
  * @todo: document this function
  */
void DText::setText(const Ogre::String& text)
{
    mText=text;

    mOverlay->setCaption( text );
    //mOverlay->_update();

}

Ogre::String DText::getText() {
    return( mText);
}

void DText::setColour( Ogre::ColourValue colour ) {
    mOverlay->setColour( colour );
}

void DText::setColour( float red, float green, float blue ) {
    mOverlay->setColour( Ogre::ColourValue( red, green, blue ));
}



// ***********************************************************
// *****************    DButton   ****************************
// ***********************************************************


/** @brief DButton
  *
  * @todo: document this function
  */
DButton::DButton(DynaGUI* gui, DWidget* parent, Ogre::String name,
    Ogre::Vector4 xywh, Ogre::String caption, DynaGUIButtonListener* listener )
    :DWidget( gui, parent, name, xywh, "Button" ) {

    if( !caption.empty())
        mCaption=new DText( gui, this, name + "_caption", Ogre::Vector4( 0,0, xywh.z, xywh.w ), caption );
    else
        mCaption=0;

    mActive=false;
    mListener=listener;

}

DButton::~DButton() {
    if (mCaption) delete( mCaption );
}

/** @brief setCaption
  *
  * @todo: document this function
  */
void DButton::setCaption(const Ogre::String& caption) {
    if( mCaption )
        mCaption->setText( caption );
}

Ogre::String DButton::getCaption() {
    if( mCaption )
        return( mCaption->getText() );
    else
        return( Ogre::StringUtil::BLANK );
}

bool DButton::in( float x, float y ) {

    if( ( x > mOverlay->_getDerivedLeft() ) &&
        ( x < ( mOverlay->_getDerivedLeft() + mOverlay->_getRelativeWidth() ) ) &&
        ( y > mOverlay->_getDerivedTop() ) &&
        ( y < ( mOverlay->_getDerivedTop() + mOverlay->_getRelativeHeight() ) )
    ) return( true );

    return( false );
    // return( mOverlay->contains( x,y ) );

}


bool DButton::isActive() {
    return( mActive );
}


void DButton::activate() {
    mActive=true;
    setDynaMaterial( "Button.Active" );
}

void DButton::deactivate() {
    mActive=false;
    setDynaMaterial( "Button" );
}


void DButton::notifyListener() {
    if( mListener )
        mListener->onButtonPress( this );
}





// ***********************************************************
// *****************    DProgressBar   ***********************
// ***********************************************************


/** @brief DProgressBar
  *
  * @todo: document this function
  */
DProgressBar::DProgressBar( DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh,
    Ogre::String material )
    : DWidget( gui, parent, name, xywh, "ProgressBar" ) {

    mWidth=xywh.z;
    mHeight=xywh.w;

    mPercent=100;
    setPercent( mPercent );
}


int DProgressBar::getPercent() {
    return( mPercent );
}

void DProgressBar::setPercent( int percent ) {
    mPercent=percent;
    if( mPercent > 100 ) mPercent=100;
    if( mPercent < 0 ) mPercent=0;
    resize( mWidth * (float)mPercent / 100.f , mHeight );
}




// ***********************************************************
// ******************   DCheckBox   *************************
// ***********************************************************


/** @brief DCheckbox
  */
DCheckbox::DCheckbox( DynaGUI* gui, DWidget* parent, Ogre::String name, Ogre::Vector4 xywh
    , bool checked )
    :DButton( gui, parent, name, xywh, Ogre::StringUtil::BLANK, this ) {


    mCheckOverlay=new DWidget( gui, parent, name + "Check", xywh, "Checkbox" );

    mChecked=checked;
    if( mChecked ) check();
    else uncheck();



}

DCheckbox::~DCheckbox() {
    delete( mCheckOverlay );
}


/** @brief checks the checkbox. huh ?
  */
void DCheckbox::check() {
    mChecked=true;
    // setCaption( "X" );
    mCheckOverlay->setDynaMaterial( "Checkbox.Checked" );
}


void DCheckbox::uncheck() {
    mChecked=false;
    // setCaption( " " );
    mCheckOverlay->setDynaMaterial( "Checkbox" );
}

/** @brief check if checked
  */
bool DCheckbox::checked() {
    return( mChecked );
}


void DCheckbox::onButtonPress( DButton* button ) {

    if( mChecked ) {
        uncheck();
        mChecked=false;
    }
    else {
        check();
        mChecked=true;
    }

}





// ***********************************************************
// ****************   DSubWindow   **************************
// ***********************************************************


/** @brief DSubWindow
  *
  */
DSubWindow::DSubWindow( DynaGUI* gui, DWidget* parent, Ogre::String name,
    Ogre::Vector4 xywh /*, Ogre::String material */ )
    :DWindow( gui, parent, name, xywh ) {

/*
    if( !material.empty() ) {
        mImage=new DWidget( gui, parent, name + "_Image", xywh );
    }
*/

}

// override of DWindow::computePosAndSize ( no window resizing )
void DSubWindow::computePosAndSize( float width, float height ) {

    mInsertXPos += width;
    mInsertYPos += height;

    mHeight=mInsertYPos + mLineHeight + mGap;
}




// ***********************************************************
// ****************   DImage        **************************
// ***********************************************************


DImage::DImage( DynaGUI* gui, DWidget* parent, Ogre::String name,
    Ogre::Vector4 xywh, Ogre::String materialname )
    : DSubWindow( gui, parent, name, xywh ) {

    setMaterial( materialname );

}

/*
// override from DOverlay to avoid the addition of the base material name
void DImage::setMaterial( Ogre::String materialname ) {
    mOverlay->setMaterialName( materialname );
}
*/



