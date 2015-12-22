


#include "inputmanager.h"
#include "logger.h"



CAxisInputAction::CAxisInputAction( bool reversed, int minvalue, int maxvalue, int step ) {
     mAxisValue=0;
     mAxisDelta=0;
     mAxisMax=maxvalue;
     mAxisMin=minvalue;
     mReversed=reversed;
     mPressed=false;
     mPreviouslyPressed=false;
     mStep=step;
     mRealAxis=false;
     mInputManager=CInputManager::getInstance();
}


int CAxisInputAction::getAxisDelta() const {
    return( mAxisDelta );
}


int CAxisInputAction::getAxisValue() const {
    return( mAxisValue );
}

void CAxisInputAction::setAxisMin( int min ) {
    mAxisMin=min;
}

void CAxisInputAction::setAxisMax( int max ) {
    mAxisMax=max;
}

void CAxisInputAction::setAxisStep( int step ) {
    mStep=step;
}

bool CAxisInputAction::isPressed() {
    return( mPressed );
}

bool CAxisInputAction::isReleased() {
    //return( mReleased );
    if( mPressed ) { mPreviouslyPressed=true; return(false ); }
    else {
        if( mPreviouslyPressed )  { mPreviouslyPressed=false; return( true ); }
    }

    return( false );

}

bool CAxisInputAction::isRealAxis() {
    return( mRealAxis );
}

void CAxisInputAction::increment() {
    mAxisDelta=mReversed?-mStep:mStep;
    mAxisValue+=mAxisDelta;
    if( mAxisValue>mAxisMax ) mAxisValue=mAxisMax;
    if( mAxisValue<mAxisMin ) mAxisValue=mAxisMin;
}


void CAxisInputAction::decrement() {
    mAxisDelta=mReversed?mStep:-mStep;
    mAxisValue+=mAxisDelta;
    if( mAxisValue<mAxisMin ) mAxisValue=mAxisMin;
    if( mAxisValue>mAxisMax ) mAxisValue=mAxisMax;
}



CKeysAxisInputAction::CKeysAxisInputAction(
    OIS::KeyCode inc, OIS::KeyCode dec, bool reversed,
     int minvalue, int maxvalue, int step )
    :CAxisInputAction( reversed, minvalue, maxvalue, step  ) {

    //mAxisMin=-100;
    //mAxisMax=100;
    mKeyInc=inc;
    mKeyDec=dec;
    //mStep=1;

}

void CKeysAxisInputAction::update() {


    if( mInputManager->isKeyPressed( mKeyInc ) ) {
        mPressed=true;
       increment();
    }
    else
        if( mInputManager->isKeyPressed( mKeyDec ) ) {
            mPressed=true;
            decrement();
        }
        else {
            mAxisDelta=0;
            mPressed=false;
        }

}



CMouseButtonAxisInputAction::CMouseButtonAxisInputAction(
    OIS::MouseButtonID inc, OIS::MouseButtonID dec,
    bool reversed,  int minvalue, int maxvalue, int step )
    :CAxisInputAction(reversed, minvalue, maxvalue, step ) {

}

void CMouseButtonAxisInputAction::update() {


    if( mInputManager->isMouseButtonPressed( mInc ) ) {
        mPressed=true;
        increment();
    }
    else
        if( mInputManager->isMouseButtonPressed( mDec ) ) {
            mPressed=true;
            decrement();
        }
        else {
            mAxisDelta=0;
            mPressed=false;
        }

}



CMouseAxisInputAction::CMouseAxisInputAction(
    IM_MOUSE_AXIS axis, bool reversed,  int minvalue, int maxvalue, int step )
    :CAxisInputAction( reversed, minvalue, maxvalue, step ) {

    mAxis=axis;
    mSensitivity=4;
    mRealAxis=true;
    //mAxisMax=90;
    //mAxisMin=-90;
}

void CMouseAxisInputAction::update() {

    switch( mAxis ) {
        case MOUSE_X_AXIS:
            mAxisDelta=(int)mInputManager->getMouseXDelta(); // (int)(mInputManager->getMouseXDelta()/mSensitivity);
            break;
        case MOUSE_Y_AXIS:
            mAxisDelta=(int)mInputManager->getMouseYDelta(); // (int)(mInputManager->getMouseYDelta()/mSensitivity);
            break;
        case MOUSE_Z_AXIS:
            mAxisDelta=(int)(mInputManager->getMouseWheelDelta()/120.f);  // 120 here : is this universal ??
    }

    if(mReversed) mAxisDelta=-mAxisDelta;
    mAxisValue+=mAxisDelta;
    if( mAxisValue > mAxisMax ) { mAxisValue=mAxisMax; }
    if( mAxisValue < mAxisMin ) { mAxisValue=mAxisMin; }

    if( mAxisDelta==0 ) {
        mPressed=false;
    }
    else {
        mPressed=true;
    }

}


CJoystickButtonAxisInputAction::CJoystickButtonAxisInputAction(
    int joynum, int inc, int dec, bool reversed, int minvalue, int maxvalue, int step )
    :CAxisInputAction( reversed, minvalue, maxvalue, step ) {

    mJoyNum=joynum;
    mInc=inc;
    mDec=dec;
    // mStep=1;

}

void CJoystickButtonAxisInputAction::update() {

    mAxisDelta=0;

    if( mInputManager->isJoystickButtonPressed( mJoyNum, mInc ) ) {
        mPressed=true;
        increment();
    }
    else
        if( mInputManager->isJoystickButtonPressed( mJoyNum, mDec ) ) {
            mPressed=true;
            decrement();
        }
        else {
            mPressed=false;
        }


}



CJoystickAxisInputAction::CJoystickAxisInputAction(
    int joynum, int joyaxis, bool reversed, int minvalue, int maxvalue, int step )
    :CAxisInputAction( reversed, minvalue, maxvalue, step ) {

    //mAxisMax=32767;
    //mAxisMin=-32768;
    mJoystickAxis=joyaxis;
    mJoystickNum=joynum;
    mRealAxis=true;
}


void CJoystickAxisInputAction::update() {

    int newValue=mInputManager->getJoystickAxisValue( mJoystickNum, mJoystickAxis );
    // newValue = newValue / (float)(OIS::JoyStick::MAX_AXIS*mAxisMax);
    //newValue=newValue*314/mAxisMax;
    //newValue = (float)newValue/(float)OIS::JoyStick::MAX_AXIS*(3.141592/2.f)*100.f;


    mAxisDelta=(int)((float)(newValue-mAxisValue)*(float)(314.f/32767.f));  // *PI*100

    if(mReversed) mAxisDelta=-mAxisDelta;
    mAxisValue=newValue;

    if( mAxisValue == 0 ) mPressed=false;
    else mPressed=true;


   // LOG("\n Joy#%d : AxisValue=%d / AxisDelta=%d", mJoystickNum, mAxisValue, mAxisDelta );


}


CJoystickRelativeAxisInputAction::CJoystickRelativeAxisInputAction(
    int joynum, int axis, bool reversed,  int minvalue, int maxvalue, int step )
    :CAxisInputAction(reversed, minvalue, maxvalue, step ) {


    // mAxisMax=32767;
    mJoystickAxis=axis;
    mJoystickNum=joynum;
    // mStep=4;
}


void CJoystickRelativeAxisInputAction::update() {

    mAxisValue=mInputManager->getJoystickAxisValue( mJoystickNum, mJoystickAxis )*100/mAxisMax;
    if( mAxisValue > OIS::JoyStick::MAX_AXIS/2 ) {
        mAxisDelta=mStep;
        mPressed=true;
    }
    else
        if( mAxisValue < OIS::JoyStick::MIN_AXIS/2 ) {
            mAxisDelta=-mStep;
        }
        else {
            mAxisDelta=0;
            mPressed=false;
        }

    if( mReversed ) mAxisDelta=-mAxisDelta;


}


CKeyInputAction::CKeyInputAction( OIS::KeyCode key  )
    :CAxisInputAction( false, 0,1,1  ) {

    mKey=key;
    //mAxisMin=0;
    //mAxisMax=100;
    // mAxisValue=0;

}


void CKeyInputAction::update() {

    if( mInputManager->isKeyPressed( mKey ) ) {
         mPressed=true;
         //increment();
    }
    else {
        // decrement();
        mPressed=false;
    }

}


CMouseButtonInputAction::CMouseButtonInputAction( OIS::MouseButtonID button )
    :CAxisInputAction( false,0,1,1 ) {
    mButton=button;
    //mAxisMin=0;
    //mAxisMax=100;
    //mAxisValue=0;
}

void CMouseButtonInputAction::update() {
    if( mInputManager->isMouseButtonPressed( mButton )) {
        mPressed=true;
        //increment();
    }
    else {
        //decrement();
        mPressed=false;
    }

}

CJoystickButtonInputAction::CJoystickButtonInputAction( int joynum, int button )
    :CAxisInputAction( false,0,1,1 ) {
    mButton=button;
    mJoyNum=joynum;
    //mAxisMin=0;
    //mAxisMax=100;
}


void CJoystickButtonInputAction::update() {
    if( mInputManager->isJoystickButtonPressed( mJoyNum, mButton ) ) {
        mPressed=true;
        //increment();
    }
    else {
        //decrement();
        mPressed=false;
    }

}



