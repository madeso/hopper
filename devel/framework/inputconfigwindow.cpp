

#include "inputconfigwindow.h"
#include "logger.h"
#include "optionmanager.h"

CWaitForInputWindow::CWaitForInputWindow( CInputConfiguration::SInputActionConfig& config ) {

    mGUI=DynaGUI::getInstance();
    mInputManager=CInputManager::getInstance();

    mWindow = mGUI->createWindow( DWindow::CENTERED, DWindow::CENTERED, "Waiting for input" );
    mWindow->newLine();

    mLine1 = mWindow->addText( "Press a key or a button", 300 );
    mWindow->newLine();
    mLine2 = mWindow->addText( "or move the mouse or the joystick", 300 );
    mWindow->newLine();
    mLine3 = mWindow->addText( "(Press [Esc] to cancel )", 300 );

    mInputActionConfig=config;

    mInputDelay=new CDelay(1000);

    mDone=false;
    mCancelled=false;

    mStatus=CHECK_PRIMARY;

}

CWaitForInputWindow::~CWaitForInputWindow() {
    mGUI->destroyWindow( mWindow );
}


bool CWaitForInputWindow::done() {

    std::string method;

    switch( mStatus ) {
        case CHECK_PRIMARY:
            checkInput();
        break;
        case CHECK_SECONDARY:
            mInputDelay->restart();
            mLine1->setText("");
            mLine2->setText( methodName( false ) );
            mLine3->setText("");
            mStatus=WAIT_BEFORE_SECONDARY;
        break;

        case WAIT_BEFORE_SECONDARY:
            if( mInputDelay->isOver() ) {
                mLine1->setText( "Please press the secondary button" );
                mLine2->setText( "" );
                mLine3->setText( "(Press [Esc] to cancel )" );
                mStatus=DO_CHECK_SECONDARY;
            }
        break;

        case DO_CHECK_SECONDARY:
            if( mInputDelay->isOver() )  {
                checkForSecondaryInput();
            }
        break;

        case DONE:
            mInputDelay->restart();
            mLine1->setText( "" );
            mLine2->setText( methodName( true ) );
            mLine3->setText( "" );
            mStatus=FINISHED;
        break;

        case CANCELLED:
            mInputDelay->restart();
            mLine1->setText( "" );
            mLine2->setText( "Cancelled" );
            mLine3->setText( "" );
            mStatus=FINISHED;
        break;

        case FINISHED:
            if( mInputDelay->isOver() )
                mDone=true;
        break;

    }

    return( mDone );
}

bool CWaitForInputWindow::cancelled() {
    return( mCancelled );
}

CInputConfiguration::SInputActionConfig& CWaitForInputWindow::getInputActionConfig() {
    return( mInputActionConfig );
}


void CWaitForInputWindow::checkInput() {

    OIS::KeyCode key;
    IM_MOUSE_AXIS mouseaxis;
    OIS::MouseButtonID mousebutton;
    int joynum, joybutton, joyaxis;


    if( mInputManager->keyPressedDetected( key ) ) {

        LOG("\n CWaitForInputWindow : Key Pressed : %d", (int)key );

        if( key==OIS::KC_ESCAPE ) {
            mStatus=CANCELLED;
            mCancelled=true;
            return;
        }
        else {
            mInputActionConfig.method=CInputConfiguration::KEY;
            mInputActionConfig.periph_value=key;
            if( mInputActionConfig.type==CInputConfiguration::AXIS )
                mStatus=CHECK_SECONDARY;
            else
                mStatus=DONE;
            return;
        }
    }


    if( mInputManager->mouseButtonDetected( mousebutton ) ) {
        mInputActionConfig.method=CInputConfiguration::MOUSE_BUTTON;
        mInputActionConfig.periph_value=mousebutton;

        if( mInputActionConfig.type==CInputConfiguration::AXIS )
            mStatus=CHECK_SECONDARY;  // do a call to checkForSecondaryInput()
        else
        mStatus=DONE;
        return;
    }


    if( mInputManager->joystickButtonDetected( joynum, joybutton ) ) {
        mInputActionConfig.method=CInputConfiguration::JOYSTICK_BUTTON;
        mInputActionConfig.periph_num=joynum;
        mInputActionConfig.periph_value=joybutton;
        if( mInputActionConfig.type==CInputConfiguration::AXIS )
            mStatus=CHECK_SECONDARY;  // do a call to checkForSecondaryInput()
        else
            mStatus=DONE;

        return;
    }

    //if( mInputActionConfig.type==CInputConfiguration::AXIS ) {
        if( mInputManager->mouseAxisDetected( mouseaxis ) ) {
            mInputActionConfig.method=CInputConfiguration::MOUSE_AXIS;
            mInputActionConfig.periph_value=mouseaxis;
            mStatus=DONE;
            return;
        }

        if( mInputManager->joystickAxisDetected( joynum, joyaxis )) {
            mInputActionConfig.method=CInputConfiguration::JOYSTICK_AXIS;
            mInputActionConfig.periph_num=joynum;
            mInputActionConfig.periph_value=joyaxis;
            mStatus=DONE;
        }
    //}

}


void CWaitForInputWindow::checkForSecondaryInput() {
    OIS::KeyCode key;
    OIS::MouseButtonID mousebutton;
    int joynum, joybutton;

    if( mInputActionConfig.method==CInputConfiguration::KEY ) {

        if( mInputManager->keyPressedDetected( key ) ) {
            if( key == OIS::KC_ESCAPE ) {
                mCancelled=true;
                mStatus=CANCELLED;
                return;
            }
            else {
                mInputActionConfig.periph_value2=key;
                mStatus=DONE;
                return;
            }

        }

    }


    if( mInputManager->mouseButtonDetected( mousebutton ) ) {
        mInputActionConfig.periph_value2=mousebutton;
    }

    if( mInputActionConfig.method==CInputConfiguration::JOYSTICK_BUTTON ) {
        if( mInputManager->joystickButtonDetected( joynum, joybutton ) ) {
            mInputActionConfig.method=CInputConfiguration::JOYSTICK_BUTTON;
            mInputActionConfig.periph_value2=joybutton;
            mStatus=DONE;
        }
    }

}


Ogre::String CWaitForInputWindow::methodName( bool secondary ) {

    Ogre::String method_name="?";

    int value=mInputActionConfig.periph_value;
    int value2=mInputActionConfig.periph_value2;
    int joynum=mInputActionConfig.periph_num;

    switch( mInputActionConfig.method ) {

        case CInputConfiguration::KEY:
            if( secondary )
                method_name=mInputManager->getKeyName( (OIS::KeyCode)value2 );
            else
                method_name=mInputManager->getKeyName( (OIS::KeyCode)value );
        break;

        case CInputConfiguration::MOUSE_BUTTON:
            if( secondary )
                method_name = mInputManager->getMouseButtonName( (OIS::MouseButtonID)value2 );
            else
                method_name = mInputManager->getMouseButtonName( (OIS::MouseButtonID)value );
        break;

        case CInputConfiguration::JOYSTICK_BUTTON:
            if( secondary )
                method_name = mInputManager->getJoystickButtonName( joynum, value2 );
            else
                method_name = mInputManager->getJoystickButtonName( joynum, value );
        break;

        case CInputConfiguration::MOUSE_AXIS:
            method_name = mInputManager->getMouseAxisName( (IM_MOUSE_AXIS)value );
        break;

        case CInputConfiguration::JOYSTICK_AXIS_RELATIVE:
        case CInputConfiguration::JOYSTICK_AXIS:
            method_name = mInputManager->getJoystickAxisName( joynum, value );
        break;

    }


    return( method_name );
}


// ===============================================================================



CInputConfigWindow::CInputConfigWindow() {

    LOG("\nCInputConfigWindow()" );

/*
    if( ! COptionManager::getInstance()->getOption( "mousesensitivity", mMouseSensitivity ) ) {
       mMouseSensitivity=5;
    }
*/
    mGUI = DynaGUI::getInstance();
    mInputManager=CInputManager::getInstance();

/*
    mMouseSensitivityText=mGUI->addText( mWindow, 100, "Mouse sensitivity : " + Ogre::StringConverter::toString( mMouseSensitivity ) );
    mGUI->newLine( mWindow );
    mMouseSensitivityDown=mGUI->addButton( mWindow, 30, "<", BetaGUI::Callback(this) );
    mMouseSensitivityBar=mGUI->addProgressbar( mWindow, 100, "Hoper/BasicProgressBar" );
    mMouseSensitivityUp=mGUI->addButton( mWindow, 30, ">", BetaGUI::Callback(this) );
*/


    mConfigMax=CInputConfigurationManager::getInstance()->getNumberOfConfigs()-1;
    mCurrentConfig=CInputConfigurationManager::getInstance()->getLastInputConfig();
    mConfig=CInputConfigurationManager::getInstance()->getInputConfiguration(mCurrentConfig);
    mNumberOfActions=mConfig->getNumberOfActions();

    mInputActionConfig.resize(mNumberOfActions);
    mRedefineButton.resize(mNumberOfActions);
    mAxisModeButton.resize(mNumberOfActions );
    mAxisReversedButton.resize(mNumberOfActions);
    mMethodName.resize( mNumberOfActions );

    for( int i=0; i<mNumberOfActions; i++ ) {
        mInputActionConfig[i]=mConfig->getInputActionConfig(i);
    }

    buildWindow();
    updateWindow();
//    mGUI->debugOverlays();


    mWaitForInput=false;
    mWaitForInputAction=0;
    mWaitForInputWindow=0;

    mDone=false;
}

CInputConfigWindow::~CInputConfigWindow() {

    LOG("\n~CInputConfigWindow() ...");
    mInputActionConfig.clear();
    mGUI->destroyWindow(mWindow);

    LOG(" done." );
}



void CInputConfigWindow::applyConfig() {

    for( int i=0; i<mNumberOfActions; i++ ) {


        if( mInputActionConfig[i].configurable ) {


            if( mInputActionConfig[i].type == CInputConfiguration::AXIS ) {

                mInputActionConfig[i].reversed = mAxisReversedButton[i]->checked();


                if( mInputActionConfig[i].method == CInputConfiguration::JOYSTICK_AXIS
                    && mAxisModeButton[i]->checked() )
                        mInputActionConfig[i].method = CInputConfiguration::JOYSTICK_AXIS_RELATIVE;

                if( mInputActionConfig[i].method == CInputConfiguration::JOYSTICK_AXIS_RELATIVE
                    && !mAxisModeButton[i]->checked() )
                    mInputActionConfig[i].method = CInputConfiguration::JOYSTICK_AXIS;

            }

            mConfig->setInputActionConfig( mInputActionConfig[i] );
        }

    }

    mConfig->apply(true); // applies only configurables actions
    mConfig->save();
    CInputConfigurationManager::getInstance()->setLastInputConfig( mCurrentConfig );
}




void CInputConfigWindow::buildWindow() {

    //std::string method_name;
    int value=0;
    int value2=0;
    int joynum=0;
    std::string opt;


    mWindow=mGUI->createWindow( DWindow::CENTERED, DWindow::CENTERED, "Input configuration");
    mWindow->newLine();

    //if( mCurrentConfig>0 )
        mPreviousConfigButton = mWindow->addButton( "<",  30, this );
    //else {
    //    mPreviousConfigButton=0;
    //    mWindow->addSpace( 30 );
    //}



    //std::stringstream configname;
    //configname << "Configuration#" << mCurrentConfig;
    //mWindow->addText( configname.str(), 150 );
    mConfignameText = mWindow->addText( "Configuration#" + Ogre::StringConverter::toString( mCurrentConfig ), 150 );


    //if( mCurrentConfig<mConfigMax )
        mNextConfigButton = mWindow->addButton( ">", 30, this );
    //else {
    //    mNextConfigButton=0;
    //    mWindow->addSpace( 30 );
    //}

    mWindow->newLine();

    mWindow->addText( "Action", 150 );
    mWindow->addText( "Input method", 300 );
    mWindow->addText( "Rev.", 60 );
    mWindow->addText( "Rel.", 30 );
    mWindow->newLine();

    for( int i=0; i<mNumberOfActions; i++ ) {

            if( mInputActionConfig[i].configurable ) {

                mRedefineButton[i] = mWindow->addButton( mInputActionConfig[i].name,  150, this );
                mMethodName[i] = mWindow->addText( "input method description", 300 );


                if( mInputActionConfig[i].type == CInputConfiguration::AXIS ) {

                    mAxisReversedButton[i] = mWindow->addCheckbox( false );

                    //LOG("\n axis reversed button #%d = %p", i, (void*)mAxisReversedButton[i] );
                    /*
                    if( mInputActionConfig[i].reversed )
                        opt="X";
                    else opt=" ";
                    mAxisReversedButton[i] = mWindow->addButton( opt, 20, this );
                    */

                    mWindow->addSpace( 50 );

                    /*
                    if( mInputActionConfig[i].method == CInputConfiguration::JOYSTICK_AXIS_RELATIVE )
                        opt="X";
                    else opt=" ";
                    mAxisModeButton[i] = mWindow->addButton( opt, 20, this );
                    */
                    mAxisModeButton[i] = mWindow->addCheckbox( false );

                }

                mWindow->newLine();

            }

    }

    mWindow->addText( "Note: Relative mode is recomended for joypad axes", 500, 15 );
    mWindow->addSpace( 20 );

    mOKButton=mWindow->addButton( "Use that", 80, this );

//    mGUI->debugOverlays();
//    mWindow->debugElements();

}


void CInputConfigWindow::updateWindow() {

    Ogre::String method_name;

    mConfignameText->setText( "Configuration#" + Ogre::StringConverter::toString( mCurrentConfig ) );

    if( mCurrentConfig == mConfigMax )
        mNextConfigButton->hide();
    else
        mNextConfigButton->show();

    if( mCurrentConfig == 0 )
        mPreviousConfigButton->hide();
    else
        mPreviousConfigButton->show();



    for( int i=0; i<mNumberOfActions; i++ ) {

        if( mInputActionConfig[i].configurable ) {

            mMethodName[i]->setText( methodName(i) );


            if( mInputActionConfig[i].type == CInputConfiguration::AXIS ) {


                if( mInputActionConfig[i].reversed )
                    mAxisReversedButton[i]->check();
                else
                    mAxisReversedButton[i]->uncheck();

                if( mInputActionConfig[i].method == CInputConfiguration::JOYSTICK_AXIS_RELATIVE )
                    mAxisModeButton[i]->check();
                else
                    mAxisModeButton[i]->uncheck();

            }


        }

    }

}


Ogre::String CInputConfigWindow::methodName( int action ) { // , std::string& method_name ) {

    Ogre::String method_name="?";

    int value=mInputActionConfig[action].periph_value;
    int value2=mInputActionConfig[action].periph_value2;
    int joynum=mInputActionConfig[action].periph_num;

    switch( mInputActionConfig[action].method ) {

        case CInputConfiguration::KEY:
            method_name=mInputManager->getKeyName( (OIS::KeyCode)value );
            if( mInputActionConfig[action].type == CInputConfiguration::AXIS ) {
                method_name = method_name + " / " +
                    mInputManager->getKeyName( (OIS::KeyCode)value2 );
            }
        break;

        case CInputConfiguration::MOUSE_BUTTON:
            method_name = mInputManager->getMouseButtonName( (OIS::MouseButtonID)value );
            if( mInputActionConfig[action].type == CInputConfiguration::AXIS ) {
                method_name = method_name + " / " +
                    mInputManager->getMouseButtonName( (OIS::MouseButtonID)value2 );
            }
        break;

        case CInputConfiguration::JOYSTICK_BUTTON:
            method_name = mInputManager->getJoystickButtonName( joynum, value );
            if( mInputActionConfig[action].type == CInputConfiguration::AXIS ) {
                method_name = method_name + " / " +
                    mInputManager->getJoystickButtonName( joynum, value2 );
            }
            method_name = method_name + " (" + mInputManager->getJoystickName( joynum ) + ")";
        break;

        case CInputConfiguration::MOUSE_AXIS:
            method_name = mInputManager->getMouseAxisName( (IM_MOUSE_AXIS)value );
        break;

        case CInputConfiguration::JOYSTICK_AXIS_RELATIVE:
        case CInputConfiguration::JOYSTICK_AXIS:
            method_name = mInputManager->getJoystickAxisName( joynum, value );
            method_name = method_name + " (" + mInputManager->getJoystickName( joynum ) + ")";
        break;
        /*
        case CInputConfiguration::JOYSTICK_AXIS_RELATIVE:
            method_name = mInputManager->getJoystickAxisName( joynum, value );
            method_name += "  [Rel.]";
        break;
        */
    }

    //LOG("\nCInputConfigWindow::methodName(): action %d => %s", action, method_name.c_str() );

    return( method_name );
}


bool CInputConfigWindow::done() {


    if( mWaitForInput ) {
        if( mWaitForInputWindow->done() ) {
            mWaitForInput=false;

            if( ! mWaitForInputWindow->cancelled() ) {
                mInputActionConfig[mWaitForInputAction]=mWaitForInputWindow->getInputActionConfig();
            }
            else
                LOG("\n ==========> Input Definition cancelled !");

            delete( mWaitForInputWindow );


            /*
            mGUI->destroyWindow( mWindow );
            buildWindow();
            */
            updateWindow();
            mWindow->show();

        }
    }

    return( mDone );
}



void CInputConfigWindow::onButtonPress( DButton* b) {

    std::string opt;
/*
    if( b==mMouseSensitivityDown && mMouseSensitivity>0 ) mMouseSensitivity--;
    if( b==mMouseSensitivityUp && mMouseSensitivity<10) mMouseSensitivity++;
    mMouseSensitivityText->setCaption( "Mouse Sensitivity : " + Ogre::StringConverter::toString( mMouseSensitivity ));
    mMouseSensitivityBar->setPercent( mMouseSensitivity*10 );
*/
    if( b==mOKButton ) {
        COptionManager::getInstance()->setOption( "last_input_config", mCurrentConfig );
        applyConfig();
        mDone=true;
    }

    for( int i=0; i<mNumberOfActions; i++ ) {

        if( b==mRedefineButton[i] ) {
            mWindow->hide();
            mWaitForInput=true;
            mWaitForInputAction=i;
            mWaitForInputWindow=new CWaitForInputWindow( mInputActionConfig[i] );
        }

        // checkbox result moved in applyConfig()

        /*
        if( b==mAxisReversedButton[i] ) {
            mInputActionConfig[i].reversed=!mInputActionConfig[i].reversed;
            if( mInputActionConfig[i].reversed )
                opt="X";
            else opt=" ";
            mAxisReversedButton[i]->setCaption(opt);
        }

        if( b==mAxisModeButton[i] ) {
            if( mInputActionConfig[i].method == CInputConfiguration::JOYSTICK_AXIS_RELATIVE )
                mInputActionConfig[i].method=CInputConfiguration::JOYSTICK_AXIS;
            else
                mInputActionConfig[i].method=CInputConfiguration::JOYSTICK_AXIS_RELATIVE;

            if( mInputActionConfig[i].method== CInputConfiguration::JOYSTICK_AXIS_RELATIVE )
                opt="X";
            else opt=" ";
            mAxisModeButton[i]->setCaption( opt );
        }
        */
    }

    if( b==mPreviousConfigButton ) {

        if( mCurrentConfig > 0 ) {
            mCurrentConfig--;

            mConfig=CInputConfigurationManager::getInstance()->getInputConfiguration(mCurrentConfig);
            for( int i=0; i<mNumberOfActions; i++ ) {
                mInputActionConfig[i]=mConfig->getInputActionConfig(i);
            }

            updateWindow();

        }
        /*
        LOG("\n InputConfigWindow destroying..." );
        mGUI->destroyWindow( mWindow );
        mGUI->debugOverlays();
        LOG(" done." );

        LOG("\n InputConfigWindow destroyed : rebuilding..." );
        buildWindow();
        mGUI->debugOverlays();
        LOG(" done." );
        */

    }

    if( b==mNextConfigButton ) {
        if( mCurrentConfig < mConfigMax ) {
            mCurrentConfig++;
            mConfig=CInputConfigurationManager::getInstance()->getInputConfiguration(mCurrentConfig);
            for( int i=0; i<mNumberOfActions; i++ ) {
                mInputActionConfig[i]=mConfig->getInputActionConfig(i);
            }

            updateWindow();
        }
        /*
        LOG("\n InputConfigWindow destroying..." );
        mGUI->destroyWindow( mWindow );
        mGUI->debugOverlays();
        LOG(" done." );


        LOG("\n InputConfigWindow destroyed : rebuilding..." );
        buildWindow();
        mGUI->debugOverlays();
        LOG(" done." );
        */
    }

}


