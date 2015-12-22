

#include <stdlib.h>
#include "screen.h"
#include "timemanager.h"
#include "inputmanager.h"
#include "inputconfigurationmanager.h"
#include "gamemanager.h"
#include "scenemanager.h"
#include "logger.h"
#include "splash.h"
#include "Ogre.h"
#include "resourcemanager.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) {
    MSG msg;

#else
    int main(int argc, char *argv[]) {
#endif

        int mStatus=APP_RUN;

        try  {

            CLogger *mLogger=CLogger::getInstance();

            mLogger->setLogFile("hopper.log");
            mLogger->log("Hopper.log\n");
            mLogger->log("==========\n\n");

            //mLogger->log("\nScreen init..." );
            CScreen *mScreen=CScreen::getInstance();  // opens a window
            //mLogger->log("\tdone.\n" );

            mLogger->log("\nTimemanager init..." );
            CTimeManager *mTimeManager=CTimeManager::getInstance();
            mLogger->log("\tdone.\n" );

            mLogger->log("\nInputmanager init..." );
            CInputManager* mInputManager=CInputManager::getInstance();
            mLogger->log("\tdone.\n" );

            mLogger->log("\nInputconfig manager init..." );
            CInputConfigurationManager::getInstance();
            mLogger->log("\tdone.\n" );

            mLogger->log("\nResourcemanager init..." );
            CResourceManager *theResourceManager=CResourceManager::getInstance();
            mLogger->log("\tdone.\n" );

            LOG("\nLoading splash screen resources...");
            theResourceManager->loadSplash();
            LOG("\t displaying splash... \t");
            Splash* splash=new Splash;
            LOG("\tdone.");

            mLogger->log("\nSoundmanager init...");
            /* CSoundManager* theSoundManager= */ CSoundManager::getInstance();
            mLogger->log("\tdone.\n" );

            mLogger->log("\nLoading resources..." );
            theResourceManager->loadAll();
            mLogger->log("\tdone." );


            mLogger->log("\nScenemanager init...");
            /* CSceneManager* theSceneManager= */ CSceneManager::getInstance();
            mLogger->log("\tdone." );

            delete splash;
            theResourceManager->unloadSplash();

            mLogger->log("\nGamemanager init..." );
            CGameManager* mGameManager=0;
            mGameManager = new CGameManager;
            mLogger->log("\tdone." );

            mLogger->log("\n\nEntering mainloop...\n" );


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

            while( GetMessage(&msg, NULL, 0, 0) != 0  && mStatus==APP_RUN ) {
                mTimeManager->update();
                mInputManager->update();
                mStatus=mGameManager->run();

                // TODO: manage lost of focus => the game should be paused
                // see Ogre::RenderWindow::isActive();

                switch( msg.message) {

                    default:
                    case WM_PAINT:
                       mScreen->render();
                    break;

                    case WM_QUIT:
                    case WM_DESTROY:
                    case WM_CLOSE:
                        mStatus=APP_QUIT;
                    break;


                }
            }
#else

            while( mStatus == APP_RUN ) {
                    mTimeManager->update();
                    mInputManager->update();
                    mStatus=mGameManager->run();
                    //Ogre::WindowEventUtilities::messagePump(); // something to try if problems under linux
                    mScreen->render();
            }

#endif



            delete(mGameManager);
            CSceneManager::kill();
            CSoundManager::kill();
            CResourceManager::kill();
            CInputConfigurationManager::kill();
            CInputManager::kill();
            CScreen::kill();

            mLogger->log("\n...Out of mainloop\n" );
            mLogger->log("deleting logger...\n");
            CLogger::kill();
        }
        catch (Ogre::Exception e) {
            #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            //std::cerr << "exception : " << e.getFullDescription().c_str() << std::endl;

            MessageBox(NULL,e.getFullDescription().c_str(),
                       "Fatal",MB_OK|MB_ICONERROR|MB_TASKMODAL);

            #else
            std::cerr << "exception : " << e.getFullDescription().c_str() << std::endl;
            #endif
        }


        return(0);
    }


