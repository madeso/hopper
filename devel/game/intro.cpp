
#include "intro.h"
#include "hopperinputactions.h"

CIntro::CIntro() {

//    LOG("\nIntro start");

    mTimeManager=CTimeManager::getInstance();
    mInputManager=CInputManager::getInstance();
    mSceneManager=CSceneManager::getInstance();

    //mSceneManager->setAmbientColor( 0,0,0 );
    mSceneManager->setAmbientColor( 0.1,0.1,0.1 );


    mCam=CCamera::getInstance();

    mSceneManager->setFogOn( 0,0,0 );

/*
    mLight=new CSpotLight( "spot", mSceneManager->getRootSceneNode() );
    mLight->setAngle( 90 );
    mLight->setPosition( 0,20,20 );
    mLight->setDirection( 0, -1, -1 );
    mLight->setCastShadows(true );
    mLight->setColor( 0.9,0.9,0.9 );
*/
    mLight=mSceneManager->createLight( "light" );
    mLight->setType( Ogre::Light::LT_DIRECTIONAL );
    mLight->setDiffuseColour( 0.9,0.9,0.9 );
    // mLight->setPosition( 10,20,20 );
    // mLight->setDirection( (Ogre::Vector3(0,0,0)-mLight->getPosition()).normalise() );
    mLight->setDirection( Ogre::Vector3(0,-1,-1) );

    mPhysicsManager=CPhysicsManager::getInstance();
    // CCollisionManager::getInstance()->setCollisionHandler(this);

    mPhysicsManager->setGravity( -5.f);

    // mSlab=new CStaticPhysCube( Ogre::Vector3( 0,-3,0 ), 0, 2, "slab", "slab.mesh" );
    mPlane=new OgreOde::InfinitePlaneGeometry(
            Ogre::Plane( Ogre::Vector3( 0, 1, 0), 0 ),
            mPhysicsManager->getPhysicsWorld(),
            mPhysicsManager->getPhysicsSpace() );


    mBol=new CActor( "bol", "bol.mesh" );
    mBol->setScale( 8,8,8 );
    mBol->setPosition( Ogre::Vector3(0,2,0) );
    mCollBol=new CStaticPhysTrimesh( Ogre::Vector3( 0,2,0 ), 0, 8, "collision_bol", "coll_bol.mesh" );

    mCollBol->setVisible(false);

    mPolygon.push_back( new CDynamicPhysTrimesh( Ogre::Vector3( 0,20, 0), 1, 3, 2, "triangle", "triangle.mesh" ));
    mPolygon[0]->setAngularVelocity( Ogre::Vector3( rand()%100/50.f, rand()%100/50.f, rand()%100/50.f ));

    mPolygonCount=1;

    // mCube=new CDynamicPhysCube( Ogre::Vector3( 0,10, 0), 1, 1, 1, "cube", "testcube.mesh" );

    mCam->setPosition( 10,15,10 );
    mCam->lookAt( 0,5,0 );


    mGUI=DynaGUI::getInstance();
    mGUI->hideMousePointer();

    mLogo = mGUI->createImageWindow( Ogre::Vector4( 34,50,559,91 ), "PSLogo2" );

    mLogo->hide();

    mDelay=new CDelay( 1 * 1000 );

    mIntroDelay=new CDelay( 30 * 1000 );
}


CIntro::~CIntro() {

    // delete(mLight);
    CSceneManager::getInstance()->destroyLight( mLight );
    delete( mBol );
    delete( mCollBol );
    delete( mPlane);

    mGUI->destroyWindow( mLogo );

    for( std::vector<CDynamicPhysTrimesh*>::iterator i=mPolygon.begin(); i!=mPolygon.end(); ++i ) {
        delete( (*i) );
    }

    CPhysicsManager::kill();
}

int CIntro::run() {

    CDynamicPhysTrimesh* p;


    if( mDelay->isOver() && mPolygonCount<20 ) {
        mDelay->restart();

        mPolygonCount++;

        Ogre::Vector3 pos( rand()%2-1, 15, rand()%2-1 );

        switch( rand()%4 ) {
            case 0:
                p=new CDynamicPhysTrimesh( pos, 1, 3, 2,
                    "triangle"+Ogre::StringConverter::toString( mPolygonCount),
                     "triangle.mesh" );

                p->setAngularVelocity( Ogre::Vector3( rand()%100/50.f, rand()%100/50.f, rand()%100/50.f ));
                mPolygon.push_back(p);
            break;

            case 1:
                p=new CDynamicPhysTrimesh( pos, 1, 3, 2,
                    "square"+Ogre::StringConverter::toString( mPolygonCount),
                    "square.mesh" );

                p->setAngularVelocity( Ogre::Vector3( rand()%100/50.f, rand()%100/50.f, rand()%100/50.f ));
                mPolygon.push_back(p);
            break;

            case 2:
                p=new CDynamicPhysTrimesh( pos, 1, 3, 2,
                    "disc"+Ogre::StringConverter::toString( mPolygonCount),
                    "disc.mesh" );

                p->setAngularVelocity( Ogre::Vector3( rand()%100/50.f, rand()%100/50.f, rand()%100/50.f ));
                mPolygon.push_back(p);
            break;

            case 3:
                p=new CDynamicPhysTrimesh( pos, 1, 3, 2,
                    "pentagon"+Ogre::StringConverter::toString( mPolygonCount),
                    "pentagon.mesh" );

                p->setAngularVelocity( Ogre::Vector3( rand()%100/50.f, rand()%100/50.f, rand()%100/50.f ));
                mPolygon.push_back(p);
            break;
        }

    }

    // fake buyoyancy !
    for( std::vector<CDynamicPhysTrimesh*>::iterator i=mPolygon.begin(); i!=mPolygon.end(); ++i ) {
        if( (*i)->getPosition().y<4 ) {
            (*i)->setAffectedByGravity(false);
            // (*i)->setAngularDamping( 10 );
        }
        if( (*i)->getPosition().y>3.5 ) {
            (*i)->setAffectedByGravity(true);
            // (*i)->setAngularDamping( 0 );
        }
    }

    if( mPolygonCount==20 ) mLogo->show();

    // mCam->lookAt( mPolygon[0]->getPosition() );

    mPhysicsManager->update(16);

    if( mIntroDelay->isOver() || mInputManager->isReleased( IMR_ESCAPE )) {
        return( GM_MENU );
    }

    return( GM_GAMEPLAY );
}



bool CIntro::collision( OgreOde::Contact* contact ) {
    contact->setBouncyness( 0,0.1 );
    contact->setCoulombFriction( 20 );
    return( true );
}



