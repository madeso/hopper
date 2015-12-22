
#include "inputmanager.h"
#include "screen.h"
#include "logger.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #include "windows.h"
#elif defined OIS_LINUX_PLATFORM
    #include <X11/Xlib.h>
   // void checkX11Events();
#endif

#include <sstream>


CInputManager::CInputManager() {

    std::size_t handle;


    CScreen::getInstance()->getRenderWindow()->getCustomAttribute ( "WINDOW", &handle );

    LOG("\nInputManager init...");

    mInputSystem = OIS::InputManager::createInputSystem( handle );
    mMouseSystem=static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, false));
    mKeyboardSystem=static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, false));

    mNumJoysticks=mInputSystem->numJoysticks();

    for( int num=0; num<mNumJoysticks; num++ ) {
        mJoystick.push_back( static_cast<OIS::JoyStick*>(mInputSystem->createInputObject(OIS::OISJoyStick, false )) );

    }
    debugJoysticks();


    mMouseState=(OIS::MouseState*)&mMouseSystem->getMouseState();

    mMouseState->width=CScreen::getInstance()->getRenderWindow()->getWidth();
    mMouseState->height=CScreen::getInstance()->getRenderWindow()->getHeight();

    for( int a=0; a<IM_MAX_INPUT_ACTIONS; a++ ) {
        mAction[a] = 0;
    }


    mMBLeftWasPressed=false;
    mMBRightWasPressed=false;

};


CInputManager::~CInputManager() {

    OIS::InputManager::destroyInputSystem( mInputSystem );

}

void CInputManager::setNumberOfInputActions( int aNumberOfInputActions ) {
    mNumberOfInputActions=aNumberOfInputActions;
}

bool CInputManager::isPressed( int action ) const {
    return( mAction[action]->isPressed() );
}

bool CInputManager::isReleased( int action ) const {
    return( mAction[action]->isReleased() );
}


int CInputManager::getAxisDelta( int action ) const {
    return( mAction[action]->getAxisDelta() );
}

int CInputManager::getAxisValue( int action ) const {
    return( mAction[action]->getAxisValue() );
}


bool CInputManager::isRealAxis( int action ) const {
    return( mAction[action]->isRealAxis() );
}


bool CInputManager::isKeyPressed( OIS::KeyCode key ) {
    return( mKeyboardSystem->isKeyDown( key ) );
}


bool CInputManager::isJoystickButtonPressed( int joynum, int button ) {

    if( joynum < mNumJoysticks ) {
        OIS::JoyStick* joy=mJoystick[joynum];

         if( button <= joy->buttons() ) // buttons start at 1
            return( joy->getJoyStickState().buttonDown( button ) );
    }

    return(false);

}


int CInputManager::getJoystickAxisValue( int joynum, int axis ) {
    if( joynum < mNumJoysticks ) {

        OIS::JoyStick* stick=mJoystick[joynum];

        if( axis < stick->axes() )
            return( stick->getJoyStickState().mAxes[axis].abs );
    }
    return(0);
}




bool CInputManager::appClosed() {

    return(false);

}


void CInputManager::windowResized() {
    mMouseState->width=CScreen::getInstance()->getRenderWindow()->getWidth();
    mMouseState->height=CScreen::getInstance()->getRenderWindow()->getHeight();
}



void CInputManager::update() {

    mKeyboardSystem->capture();
    mMouseSystem->capture();

    mMouseState=(OIS::MouseState*)&mMouseSystem->getMouseState();

    updateMouseButtonsRelease();


    for( std::vector<OIS::JoyStick*>::const_iterator i=mJoystick.begin(); i!=mJoystick.end(); ++i ) {
        (*i)->capture();
    }

    for( int a=0; a<mNumberOfInputActions; a++ ) {
            mAction[a]->update();
    }

}


/// only for left and right mouse buttons
void CInputManager::updateMouseButtonsRelease() {

    bool down;


    mMBLeftReleased = false;
    mMBRightReleased = false;

    down = mMouseState->buttonDown( OIS::MB_Left );
    if( down ) {
        mMBLeftWasPressed = true;

    } else {
        if( mMBLeftWasPressed ) mMBLeftReleased = true;
        mMBLeftWasPressed = false;
    }


    down = mMouseState->buttonDown( OIS::MB_Right );
    if( down ) {
        mMBRightWasPressed = true;

    } else {
        if( mMBRightWasPressed ) mMBRightReleased = true;
        mMBRightWasPressed = false;
    }

}



void CInputManager::getMouseDelta( float& mouseDeltaX, float& mouseDeltaY) {

    mouseDeltaX=mMouseState->X.rel/100.0;
    mouseDeltaY=mMouseState->Y.rel/100.0;

}


float CInputManager::getMouseXDelta() {
    return( mMouseState->X.rel );
}


float CInputManager::getMouseYDelta() {
    return( mMouseState->Y.rel );
}


void CInputManager::getMouseXY( int& mX, int& mY ) {
    mX=mMouseState->X.abs;
    mY=mMouseState->Y.abs;
}


long CInputManager::getMouseX() {
    return( mMouseState->X.abs );
}

long CInputManager::getMouseY() {
    return( mMouseState->Y.abs );
}


float CInputManager::getMouseWheelDelta() {
    return(mMouseState->Z.rel );
}

bool CInputManager::isMouseButtonPressed( OIS::MouseButtonID button ) {
    return( mMouseState->buttonDown( button ));
}

bool CInputManager::isLeftMouseButtonReleased() {
    return( mMBLeftReleased );
}

bool CInputManager::isRightMouseButtonReleased() {
    return( mMBRightReleased );
}




bool CInputManager::keyPressedDetected( OIS::KeyCode& keycode ) {

    for( int k=1; k<0xD3; k++ ) {  // from 'ESC' to 'delete' = ignoring special keys
        if( isKeyPressed( (OIS::KeyCode)k ) ) {
            keycode=(OIS::KeyCode)k;
            return( true );
        }
    }
    return( false );

}


bool CInputManager::mouseButtonDetected( OIS::MouseButtonID& mousebutton ) {
    for( int i=0; i<8; i++ ) {
        if( isMouseButtonPressed( (OIS::MouseButtonID)i ) ) {
            mousebutton=(OIS::MouseButtonID)i;
            return( true );
        }
    }
    return( false );
}


bool CInputManager::joystickButtonDetected( int& joynum, int& button ) {
    for( int j=0; j<mNumJoysticks; j++ ) {
        for( int b=0; b<mJoystick[j]->buttons(); b++ ) {
            if( isJoystickButtonPressed( j, b ) ) {
                joynum=j;
                button=b;
                return( true );
            }
        }
    }
    return( false );
}


bool CInputManager::mouseAxisDetected( IM_MOUSE_AXIS& axis ) {

   if( getMouseXDelta() ) {
       axis=MOUSE_X_AXIS;
       return(true);
   }

   if( getMouseYDelta() ) {
       axis=MOUSE_Y_AXIS;
       return(true);
   }

   if( getMouseWheelDelta() ) {
       axis=MOUSE_Z_AXIS;
       return(true);
   }

   return(false);
}


bool CInputManager::joystickAxisDetected( int& joynum, int& axis ) {
    for( int j=0; j<mNumJoysticks; j++ ) {
        for( int a=0; a<mJoystick[j]->axes(); a++ ) {
            int v = getJoystickAxisValue( j, a );
            if( v > (OIS::JoyStick::MAX_AXIS*0.9)
             || v < (OIS::JoyStick::MIN_AXIS*0.9)) {
                joynum=j;
                axis=a;
                LOG("\nJoy#%d Axis#%d detected with value=%d", j, a, v );
                return( true );
            }
        }
    }
    return(false);
}



std::string CInputManager::getKeyName( OIS::KeyCode key ) {
    return( mKeyboardSystem->getAsString( key ) );
}


std::string CInputManager::getMouseButtonName( OIS::MouseButtonID button ) {

    std::stringstream name;

    if( button==OIS::MB_Left ) name << "Left Mouse Button";
    else
        if( button==OIS::MB_Right ) name << "Right Mouse Button";
        else
            if( button==OIS::MB_Middle ) name << "Middle Mouse Button";
            else
                name << "Mouse Button #" << (int)button;

    return( name.str() );
}


std::string CInputManager::getMouseAxisName( IM_MOUSE_AXIS axis ) {

    std::string name="?";
    if( axis == MOUSE_X_AXIS ) name="Mouse X axis";
    if( axis == MOUSE_Y_AXIS ) name="Mouse Y axis";
    if( axis == MOUSE_Z_AXIS ) name="Mouse Z axis (wheel)";

    return( name );
}


std::string CInputManager::getJoystickName ( int joynum ) {

    if( joynum > (mNumJoysticks-1) )
        return( "?" );
    else
        return( mJoystick[joynum]->vendor() );
}

std::string CInputManager::getJoystickButtonName( int joynum, int button ) {
    std::stringstream name;

    name << "Joy#" << joynum << " button#" << button; // << "(" << getJoystickName( joynum ) << ")";

    return( name.str() );
}


std::string CInputManager::getJoystickAxisName( int joynum, int axis ) {
    std::stringstream name;
    if( joynum < mNumJoysticks && axis < mJoystick[joynum]->axes() )
        name << "Joy#" << joynum << " axis#" << axis; //  << " (" << getJoystickName( joynum ) << ")";
    else
        name << "?";

    return( name.str() );
}


void CInputManager::debugJoysticks() {

    LOG("\n ************ %d joysticks detected", mNumJoysticks );
    for( int num=0; num<mNumJoysticks; num++ ) {
        LOG("\n ******** Joystick %d (%s) has %d buttons", num, mJoystick[num]->vendor().c_str(), mJoystick[num]->buttons() );
        LOG(" and %d axes", mJoystick[num]->axes() );
        LOG(" and %d hats", mJoystick[num]->hats() );
    }

}


/** @brief set mapping for an action
  *
  */
void CInputManager::setAction(
        int action_id, CInputConfiguration::IC_TYPE type, CInputConfiguration::IC_METHOD method,
        int periph_num, int periph_value, int periph_value2, bool reversed,
        int minvalue, int maxvalue, int step )
{


    if( mAction[action_id] != 0 ) {
        delete( mAction[action_id] );
    }

    if( type != CInputConfiguration::AXIS ) {  // not an axis
        switch( method ) {

            case CInputConfiguration::KEY:
                mAction[action_id]=new CKeyInputAction( (OIS::KeyCode) periph_value );
            break;

            case CInputConfiguration::MOUSE_BUTTON:
                mAction[action_id]=new CMouseButtonInputAction( (OIS::MouseButtonID) periph_value );
            break;

            case CInputConfiguration::JOYSTICK_BUTTON:
                mAction[action_id]=new CJoystickButtonInputAction( periph_num, periph_value );
            break;

            case CInputConfiguration::MOUSE_AXIS:
                mAction[action_id]=new CMouseAxisInputAction( (IM_MOUSE_AXIS)periph_value, reversed,0,1,1 );
            break;

            case CInputConfiguration::JOYSTICK_AXIS:
                mAction[action_id]=new CJoystickAxisInputAction( periph_num, periph_value, reversed,0,1,1 );
            break;

            case CInputConfiguration::JOYSTICK_AXIS_RELATIVE:
                mAction[action_id]=new CJoystickRelativeAxisInputAction( periph_num, periph_value, reversed,0,1,1 );
            break;
        }
    }
    else {
        switch( method ) {
            case CInputConfiguration::KEY:
                mAction[action_id]=new CKeysAxisInputAction(
                    (OIS::KeyCode)periph_value, (OIS::KeyCode)periph_value2,
                    reversed, minvalue, maxvalue, step );
            break;

            case CInputConfiguration::MOUSE_BUTTON:
                mAction[action_id]=new CMouseButtonAxisInputAction(
                    (OIS::MouseButtonID)periph_value, (OIS::MouseButtonID)periph_value2,
                    reversed, minvalue, maxvalue, step );
            break;

            case CInputConfiguration::JOYSTICK_BUTTON:
                mAction[action_id]=new CJoystickButtonAxisInputAction(
                    periph_num, periph_value, periph_value2, reversed, minvalue, maxvalue, step );
            break;

            case CInputConfiguration::MOUSE_AXIS:
                mAction[action_id]=new CMouseAxisInputAction(
                    (IM_MOUSE_AXIS)periph_value, reversed, minvalue, maxvalue, step );
            break;

            case CInputConfiguration::JOYSTICK_AXIS:
                mAction[action_id]=new CJoystickAxisInputAction(
                    periph_num, periph_value, reversed, minvalue, maxvalue, step );
            break;

            case CInputConfiguration::JOYSTICK_AXIS_RELATIVE:
                mAction[action_id]=new CJoystickRelativeAxisInputAction(
                    periph_num, periph_value, reversed, minvalue, maxvalue, step );
            break;

        }
    }

}



