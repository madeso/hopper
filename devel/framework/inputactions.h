

#ifndef _CINPUTACTIONS_
#define _CINPUTACTIONS_

// #include "inputmanager.h"
class CInputManager;

#include "inputconstants.h"
#include <string>

/// Two kinds of input actions : both are axes.
/// the first kind is controlled by one button ( key, mouse button or joystick button )
/// the second kind is controlled by two buttons or 1 axis. ( 2 keys, etc... or a mouse axis or a joystick axis )

// TODO: add parameters : speed (step), min and max of axes, deadzone for joysticks and sensitivity for mouse

class CAxisInputAction {

    public:
    CAxisInputAction( bool reversed, int minvalue, int maxvalue, int step );
    int getAxisDelta() const;
    int getAxisValue() const;

    void setAxisMin( int min );
    void setAxisMax( int max );
    void setAxisStep( int step );

    bool isPressed();
    bool isReleased();
    inline void increment();
    inline void decrement();

    virtual void update()=0;

    bool isRealAxis();

    protected:
    int mAxisValue;
    int mAxisDelta;
    int mAxisMin;
    int mAxisMax;
    int mReversed;
    bool mPressed;
    bool mPreviouslyPressed;
    CInputManager* mInputManager;
    int mStep;
    bool mRealAxis;


};


class CKeysAxisInputAction: public CAxisInputAction {

    public:
    CKeysAxisInputAction( OIS::KeyCode inc, OIS::KeyCode dec, bool reversed, int minvalue, int maxvalue, int step );
    void update();

    private:
    OIS::KeyCode mKeyInc;
    OIS::KeyCode mKeyDec;


};

class CJoystickButtonAxisInputAction: public CAxisInputAction {
    public:
    CJoystickButtonAxisInputAction( int joynum, int inc, int dec, bool reversed, int minvalue, int maxvalue, int step );
    void update();

    private:
    int mJoyNum;
    int mInc;
    int mDec;

};

class CJoystickAxisInputAction: public CAxisInputAction {

    public:
    CJoystickAxisInputAction( int joynum, int joyaxis, bool reversed,  int minvalue, int maxvalue, int step );
    void update();
  //  void setDeadZone( int percent );

    private:
    int mJoystickNum;
    int mJoystickAxis;

};


class CJoystickRelativeAxisInputAction: public CAxisInputAction {

    public:
    CJoystickRelativeAxisInputAction( int joynum, int joyaxis, bool reversed,  int minvalue, int maxvalue, int step  );
    void update();
//    void setDeadZone( int percent );

    private:
    int mJoystickNum;
    int mJoystickAxis;

};


class CMouseButtonAxisInputAction: public CAxisInputAction {
    public:
    CMouseButtonAxisInputAction( OIS::MouseButtonID inc, OIS::MouseButtonID dec, bool reversed,  int minvalue, int maxvalue, int step );
    void update();

    private:
    OIS::MouseButtonID mInc;
    OIS::MouseButtonID mDec;
};

class CMouseAxisInputAction: public CAxisInputAction {
    public:
    CMouseAxisInputAction( IM_MOUSE_AXIS axis, bool reversed,  int minvalue, int maxvalue, int step );
    void update();
 //   void setSensitivity( int percent );

    private:
    IM_MOUSE_AXIS mAxis;
    int mSensitivity;

};


class CKeyInputAction: public CAxisInputAction {
    public:
    CKeyInputAction( OIS::KeyCode key);
    void update();

    private:
    OIS::KeyCode mKey;
};


class CMouseButtonInputAction: public CAxisInputAction {
    public:
    CMouseButtonInputAction( OIS::MouseButtonID button );
    void update();

    private:
    OIS::MouseButtonID mButton;

};

class CJoystickButtonInputAction: public CAxisInputAction {
    public:
    CJoystickButtonInputAction( int joynum, int button );
    void update();

    private:
    int mJoyNum;
    int mButton;

};



#endif


