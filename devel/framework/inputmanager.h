


#ifndef _CINPUTMANAGER_
#define _CINPUTMANAGER_

#include "singleton.h"

// #include "Ogre.h"
//#include "delay.h"
#include "OIS/OIS.h"

#include "inputconstants.h"
#include "inputactions.h"
#include "inputconfiguration.h"

#define IM_MAX_INPUT_ACTIONS 50

/// Simple input manager.

class CInputManager : public SSingleton<CInputManager> {


    public:
    CInputManager();
    ~CInputManager();

    void update();

    void updateMouseButtonsRelease();

    void setNumberOfInputActions( int aNumberOfInputActions );

    bool isPressed( int action ) const;
    bool isReleased( int action ) const;
    int getAxisDelta( int action ) const;
    int getAxisValue( int action ) const;

    bool isRealAxis( int action ) const;

    bool isJoystickButtonPressed( int joynum, int button );
    int getJoystickAxisValue( int joynum, int axis );

    bool isKeyPressed( OIS::KeyCode key );

    bool appClosed();

    void getMouseDelta( float& mouseDeltaX, float& mouseDeltaY);
    float getMouseXDelta();
    float getMouseYDelta();
    void getMouseXY( int& mX, int& mY );
    long getMouseX();
    long getMouseY();
    float getMouseWheelDelta();

    bool isMouseButtonPressed( OIS::MouseButtonID );
    bool isLeftMouseButtonReleased();  // silly stuff to avoid messing with the OIS buffered input system
    bool isRightMouseButtonReleased();

    bool keyPressedDetected( OIS::KeyCode& keycode );
    bool mouseButtonDetected( OIS::MouseButtonID& mousebutton );
    bool joystickButtonDetected( int& joynum, int& button );
    bool mouseAxisDetected( IM_MOUSE_AXIS & axis );
    bool joystickAxisDetected( int& joynum, int& axis );
    std::string getKeyName( OIS::KeyCode key );
    std::string getMouseButtonName( OIS::MouseButtonID button );
    std::string getMouseAxisName( IM_MOUSE_AXIS axis );
    std::string getJoystickName ( int joynum );
    std::string getJoystickButtonName( int joynum, int button );
    std::string getJoystickAxisName( int joynum, int axis );

    void setAction( int action_id, CInputConfiguration::IC_TYPE type, CInputConfiguration::IC_METHOD method,
            int periph_num, int periph_value, int periph_value2, bool reversed,
            int minvalue, int maxvalue, int step );

    void windowResized();

    private :
    bool mMBLeftReleased;
    bool mMBRightReleased;
    bool mMBLeftWasPressed;
    bool mMBRightWasPressed;

    OIS::InputManager* mInputSystem;
    OIS::Mouse* mMouseSystem;
    OIS::Keyboard* mKeyboardSystem;

    std::vector<OIS::JoyStick*> mJoystick;
    int mNumJoysticks;

    OIS::MouseState* mMouseState;


    int mNumberOfInputActions;

    CAxisInputAction* mAction[IM_MAX_INPUT_ACTIONS];

    void debugJoysticks();

    bool mRet;

};


#endif







