

/// handle the tutorial game logic

#include "gameplaytutorial.h"

#include "gameconstants.h"
#include "logger.h"

#include "tutorialpartgravity.h"
#include "tutorialpartorientation.h"
#include "tutorialpartroll.h"
#include "tutorialpartpitch.h"

#include "shipcontroller.h"



// =========================================================================



/**
    Tutorial parts manager
*/
CGameplayTutorial::CGameplayTutorial(int aLevelNum):CGame(aLevelNum) {
    LOG("\n******************************");
    LOG("\n*** Gameplay Tutorial init ***");
    LOG("\n******************************");

    // mShipOrientationWindow=new CShipOrientationWindow();

   /*
    mMessageBox=new CInterfaceTutorialmessage;
    */
    mMessageComplete.push_back("Well done !");
  //  mMessageComplete.push_back("");
  //  mMessageComplete.push_back("");
  //  mMessageComplete.push_back("");
    mMessageComplete.push_back("Tutorial complete");
  //  mMessageComplete.push_back("");
  //  mMessageComplete.push_back("");
  //  mMessageComplete.push_back("");
  //  mMessageComplete.push_back("");
  //  mMessageComplete.push_back(" *** press <space> ***");



    mGUI=DynaGUI::getInstance();
/*
    mDialogBox=new CDialogBox( "Tutorial message" );
    mDialogBox->addLine( "Well done !" );
    mDialogBox->addOKButton( "Cool" );
*/

    mPartNum=0;

    mPart[0]=new CTutorialpartGravity(mShip);
    mPart[1]=new CTutorialpartOrientation(mShip);
    mPart[2]=new CTutorialpartRoll(mShip);
    mPart[3]=new CTutorialpartPitch(mShip);
    mPart[4]=NULL;


    mPart[mPartNum]->init();


    switchShipController( new CEmptyShipController(mShip ) );

    messageInit( mPart[mPartNum]->getMessage() );

    mTutState=TUT_MESSAGE;


    mSequenceManager->addSequence( "tutorial", new CTutorialGameplaySequence( this ) );

}


CGameplayTutorial::~CGameplayTutorial() {

    if( mMessageBox) delete(mMessageBox);

    int i=mPartNum;
    while( mPart[i] != NULL ) delete(mPart[i++] );

}


void CGameplayTutorial::gameplay() {

    switch( mTutState ) {

        case TUT_MESSAGE:
            mGUI->injectMouse();

            if( mMessageBox->done() ) {
                messageEnd();
            }

        break;



        case TUT_RUNNING:

            coreGameplay();

            if( mPart[mPartNum]->run() == TUTPART_DONE ) {
                nextPart();
            }

        break;


        case TUT_COMPLETE:

            mGUI->injectMouse();
            if( mMessageBox->done() ) {
                mGUI->hideMousePointer();
                delete(mMessageBox);
                mMessageBox=0;
                mGameState=GS_END;
            }
        break;

    }



}



void CGameplayTutorial::messageInit( std::vector<std::string> lines ) {

    std::vector<std::string>::iterator i;
    int n;


    i=lines.begin();
    mMessageBox=new CDialogBox((*i)); // the first line is the window title
    i++;

    for( ; i!=lines.end(); ++i ) {
       mMessageBox->addLine( *i );

    }

    mMessageBox->addOKButton( "OK" );

    mGUI->showMousePointer();
    mShip->setAffectedByGravity( false );
    mTutState=TUT_MESSAGE;

}



void CGameplayTutorial::messageEnd() {

    LOG("\n...Tupart message box done.");
    mGUI->hideMousePointer();
    delete(mMessageBox); mMessageBox=0;

    mTutState=TUT_RUNNING;
    mGameState=GS_GAMEPLAY;
    mShip->setAffectedByGravity(true);
    switchShipController( mPart[mPartNum]->getShipController() );

}


void CGameplayTutorial::nextPart() {

    LOG("\n  Deleting mPart[%d] !!",mPartNum);

    delete(mPart[mPartNum]);
    switchShipController( new CEmptyShipController(mShip ) );

    mPartNum++;
    if(mPart[mPartNum] == NULL) {
        messageInit( mMessageComplete );
        mTutState=TUT_COMPLETE;
    }
    else
    {
        mPart[mPartNum]->init();
        messageInit( mPart[mPartNum]->getMessage() );
        mTutState=TUT_MESSAGE;
    }

}



