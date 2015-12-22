

#include "AIShipControllers.h"


#include "logger.h"




// ------------------------------------------------------------------------------------



CBreakShipController::CBreakShipController( CShip* ship ): CShipController(ship) {

    mDone = false;

}


CBreakShipController::~CBreakShipController() {
}

/*
void CBreakShipController::run() {


// if speed > threshold, break by thrusting in the opposite direction

    Ogre::Vector3 velocity3D=mShip->getLinearVelocity();
    Ogre::Vector2 velocity2D( velocity3D.x, velocity3D.z );
    if( velocity2D.length() > 1.f ) {  // if horizontal speed too high

        Ogre::Vector3 thrustDir = mShip->getOrientation() * Ogre::Vector3( 0, -1, 0 );
        Ogre::Quaternion arc = thrustDir.getRotationTo( velocity3D );

        float arcPitch = arc.getPitch().valueRadians();
        float arcRoll = arc.getRoll().valueRadians();

        mShip->setPitch( arcPitch );
        mShip->setRoll( arcRoll );

        mShip->thrust( 1.f );

    }
    else mDone = true;


    mShip->update();
}
*/

/*
void CBreakShipController::run() {


    Ogre::Vector3 velocity3D=mShip->getLinearVelocity();
    Ogre::Vector2 velocity2D( velocity3D.x, velocity3D.z );
    if( velocity2D.length() > 1.f ) {

        Ogre::Vector3 thrustDir = mShip->getOrientation() * Ogre::Vector3( 0, -1, 0 );

        float arcPitch;
        float arcRoll;
        if(  velocity3D.z > 1.f ) arcPitch = -0.01;
        else if(  velocity3D.z < 1.f ) arcPitch = +0.01;
        else arcPitch = 0;

        if( velocity3D.x > 1.f ) arcRoll = -0.01;
        else if( velocity3D.x < -1.f ) arcRoll = +0.01;
        else arcRoll = 0;


        mShip->setYaw( 0.f );
        mShip->pitch( arcPitch );
        mShip->roll( arcRoll );

        if( velocity3D.y > 2.f )
            mShip->thrust( 0.f );  // TRY: thrust power function of velocity
        else
            mShip->thrust( 1.f);
    }
    else mDone = true;


    mShip->update();
}

*/

void CBreakShipController::run() {

    Ogre::Vector3 velocity3D=mShip->getLinearVelocity();
    Ogre::Vector2 velocity2D( velocity3D.x, velocity3D.z );


    if( velocity2D.length() > 3.f ) {

        // =============== yaw

        Ogre::Vector2 a=velocity2D.normalisedCopy();
        Ogre::Vector2 b( -1, 0 );

        float yaw=acos(a.dotProduct(b));
        if( a.y < 0 ) yaw=2*3.1415928-yaw;  // we're working in [ 0 - 2PI ]
        yaw+=90 * M_PI / 180.f;            // adujstment because the ship is -z aligned
        fmodf(yaw, 2 * M_PI );

        mShip->setYaw( mShip->getYaw() + ( yaw - mShip->getYaw() ) / 10.f );

        // ============== pitch

        float roll = mShip->getRoll() - mShip->getRoll()/20.f;

        float pitch = tanhf( a.length() * 1.25f ) ;

        mShip->setPitch( pitch );
        mShip->setRoll( roll );

        mShip->thrust( 1.f);

    }
    else mDone = true;

    mShip->update();
}



bool CBreakShipController::done() {

    return( mDone );

}

// ------------------------------------------------------------------------------------





CAutoHoverShipController::CAutoHoverShipController( CShip* ship ):
    CShipController( ship ) {


    mHoverAltitude = mShip->getPosition().y + 1.f;

//    mDebugWindow = new CDebugWindow(5);

    mBreakShipController = new CBreakShipController( ship );

}


CAutoHoverShipController::~CAutoHoverShipController() {

//    delete( mDebugWindow );

    if( mBreakShipController ) delete( mBreakShipController );

}



void CAutoHoverShipController::run() {

    // break if needed then hover

    if( mBreakShipController ) {
        mBreakShipController->run();
        if( mBreakShipController->done() ) {
            delete( mBreakShipController );
            mBreakShipController = 0;
        }
    }
    else {
        hover();
    }


}



void CAutoHoverShipController::hover() {

    Ogre::Vector3 velocity3D=mShip->getLinearVelocity();

    mShip->recover();

    float shipAlt = mShip->getPosition().y;

    /*
    mDebugWindow->setText( 1, "ship pos : " + Ogre::StringConverter::toString( shipAlt ));
    // mDebugWindow->setText( 2, "ship vel : " + Ogre::StringConverter::toString( velocity3D ));
    mDebugWindow->setText( 3, "Hover alt : " + Ogre::StringConverter::toString( mHoverAltitude ));
    */


    // LOG("\n Ship Altitude = %f / mHoverAltitude = %f", shipAlt, mHoverAltitude );


    if( shipAlt > (mHoverAltitude + 0.2f) ) {
        mShip->thrust( false );  // TRY: thrust power function of velocity
    }
    else {
        if( shipAlt < (mHoverAltitude - 0.2f) ) {
            mShip->thrust( true );
        }
    }

    mShip->update();

}






// ------------------------------------------------------------------------------------




#include "math.h"
#define DEG *M_PI/180.0

CLandingShipController::CLandingShipController(CShip* aShip, CLevel* aLevel ): CShipController( aShip ) {

    // mShip=aShip;
    mLevel=aLevel;
    mTerrain=mLevel->getTerrain();


    float x,z,xs,zs,y;
    xs=mTerrain->getXSize();
    zs=mTerrain->getZSize();


    CLandingPad* lp;
    CLevel::GameObjectList::iterator i;

    for(i=mLevel->mGameObjectList.begin(); i!=mLevel->mGameObjectList.end(); ++i ) {

       if( typeid( *((*i).actor) ) == typeid( CLandingPad ) ) {

            lp=static_cast<CLandingPad*>((*i).actor);
            mGoalPos.push_back( lp->getPosition() + Ogre::Vector3( 0, 3, 0 ));

       }
    }


    mGoal=mGoalPos.begin();


//    mShip->setCollisionMode(false);

    mLandedDelay=new CDelay( 3000 );
    mLanded=false;


    mGoalCube=new CActor( "goal", "tutgoal.mesh");
//    mGoalCube->mNode->setPosition( *mGoal );
    mGoalCube->setPosition( *mGoal);

    // mGoalCube->mNode->scale( 3,3,3 );

    mYaw=0;
    mPitch=0;
    mRoll=0;


    mShip->setGodMode( true );

    /*

    mLineDir=new CLine( (*mGoal), mShip->getPosition(), Ogre::ColourValue::Red );

    mLineVelocity=new CLine( mShip->getPosition(), mShip->getPosition()+mShip->mPhysBody->getLinearVelocity(), Ogre::ColourValue::Green );

    mLineVelocity2D=new CLine( mShip->getPosition(), mShip->getPosition()+mShip->mPhysBody->getLinearVelocity(), Ogre::ColourValue::White );

    mLineVectorx=new CLine( (*mGoal), mShip->getPosition(), Ogre::ColourValue(1,1,0,1) );
    mLinePitchRoll=new CLine( (*mGoal), mShip->getPosition(), Ogre::ColourValue( 0,1,1,1)  );

    */


    mPrevDist2D=0;

}


CLandingShipController::~CLandingShipController() {

    delete(mGoalCube);
    delete(mLandedDelay);

}

void CLandingShipController::run() {

    mShipPos=mShip->getPosition();

    // orientation
    Ogre::Vector3 goaldir3D( (*mGoal).x-mShipPos.x, (*mGoal).y-mShipPos.y, (*mGoal).z-mShipPos.z );
    Ogre::Vector2 goaldir2D( (*mGoal).x-mShipPos.x, (*mGoal).z-mShipPos.z );
    float dist2D= goaldir2D.length();
    float dist3D= goaldir3D.length();

    Ogre::Vector2 a=goaldir2D.normalisedCopy();
    Ogre::Vector2 b( -1, 0 );

    float yaw=acos(a.dotProduct(b));
    if( a.y < 0 ) yaw=2*3.1415928-yaw;  // we're working in [ 0 - 2PI ]
    yaw+=90 DEG;            // adujstment because the ship is -z aligned
    fmodf(yaw, 2 * M_PI );


    mShip->setYaw( mShip->getYaw() + ( yaw - mShip->getYaw() ) / 10.f );



    Ogre::Vector3 velocity3D=mShip->getLinearVelocity();
    Ogre::Vector2 velocity2D=Ogre::Vector2( velocity3D.x, velocity3D.z );
    float speed2D=velocity2D.length();
    float speed3D=velocity3D.length();


    Ogre::Vector2 v=goaldir2D.normalisedCopy();
    Ogre::Vector2 w=velocity2D.normalisedCopy();

    Ogre::Vector2 u( v.y, -v.x);
    u.normalise();
    Ogre::Vector2 goalPerp( goaldir2D.y, -goaldir2D.x);
    float ps=v.dotProduct(w);

    float roll;
    roll= -tanh( pow(w.dotProduct( u ), 3 ) );


    float pitch;
//    pitch= -tanhf( dist2D/30.f );


    bool dobreak=false;

            float vspeed_max=-3.f;
            // if( dist2D < 15 ) vspeed_max=1.f;

            float height_max= (*mGoal).y + 0.5 + dist2D/2.f;
            if( height_max > 250.f ) height_max=250.f;

            float speed_max= dist2D/(speed2D+1); // speed_max; // /
            // float speed_max= dist2D;
            // if( dist2D < 200 ) speed_max=speed_max/(speed2D+1); // /2.f;

            // if( speed_max > 20.f ) speed_max=20.f;
            if( ps>0 && dist2D<50 ) pitch=-tanh( ( speed_max-speed2D )/10.f ); // pitch=-pitch/2.f;
            else  pitch= -tanh( (dist2D/(speed2D+1.f))/20.f );
            /*
            if( ps>0 && dist2D<300 && speed2D>speed_max ) pitch=tanhf( (speed2D-speed_max)/25.f ); // pitch=-pitch/2.f;
            else  pitch= -tanhf( dist2D/25.f );
            */
            //


    // float terrain_height=mTerrain->getHeightAt( mShipPos.x, mShipPos.z);
    float terrain_height=mTerrain->getHeightAt( mShipPos.x+velocity3D.x/4.f, mShipPos.z+velocity3D.z/4.f );
    if (dist2D > 6 ) terrain_height+=8.f;

    if( mShipPos.y < /* (*mGoal).y */ terrain_height  ) {
        if( ! ( adjustValue( mPitch, 0, 10 DEG , 5 DEG )
                && adjustValue( mRoll, 0, 10 DEG , 5 DEG )
        ) )
        doThrust=true;
        else
        doThrust=false;
    } else {


        if( ! (  adjustValue( mPitch, 80*pitch DEG, 3 DEG, 2 DEG )
                  &&
                  adjustValue( mRoll, 80*roll DEG, 1 DEG, 1 DEG )
                  ) ) {

            if(  /* speed2D>speed_max && */ mShipPos.y< height_max && velocity3D.y < vspeed_max  ) // || dobreak  )
                 doThrust=true;
            else
                // if( mShipPos.y>( (*mGoal).y+dist2D/10.f ) || velocity3D.y>7.f ) //  || speed2D>((dist2D+1.f)/2.f) )
                if( /* speed2D<speed_max+speed_max*0.1f || */ mShipPos.y> height_max+0.1f || velocity3D.y>vspeed_max+6.0f )
                    doThrust=false;
        }
    }




// =================================================================================================


    if ( mShip->getOrientation().yAxis().directionEquals( Ogre::Vector3( 0,-1,0),
                                                    Ogre::Radian(120.f*3.1415f/180.0f) ) ) {
        //mShip->setRecoverMode(true);
        mShip->recover();
        mShip->thrust(false);
    }
    else {
//        mShip->setRecoverMode(false);

        if( dist3D>3) {
            mShip->thrust(doThrust);
            mShip->setPitch( mPitch);
            mShip->setRoll( mRoll);

        }
        else
            mShip->recover();
            //mShip->setRecoverMode(true);

    }


    if (!mLanded) {
        if( dist3D<3 && mShip->isIdle() && mShip->isUp() ) {
            mGoalCube->getEntity()->setMaterialName("tutgoal/translucent_green");
            mLandedDelay->restart();
            mLanded=true;
        }
    } else {

        if( mLandedDelay->isOver() ) {

            mGoal++;
            if( mGoal == mGoalPos.end() ) mGoal=mGoalPos.begin();
            // mGoalCube->mNode->setPosition( *mGoal );
            mGoalCube->setPosition( *mGoal );

            mGoalCube->getEntity()->setMaterialName("tutgoal/translucent");
            mLanded=false;
        }

    }

    /*
    if(dist3D < 2 ) {
        mGoal++;
        if( mGoal == mGoalPos.end() ) mGoal=mGoalPos.begin();

        mGoalCube->mNode->setPosition( *mGoal );
    }
    */


    mShip->update();


/*
    mLineDir->update( (*mGoal), mShipPos );
    mLineVelocity->update( mShipPos, mShipPos + mShip->mPhysBody->getLinearVelocity()/5.0 );


    mLinePitchRoll->update( mShipPos, mShipPos+ Ogre::Vector3(  goaldir2D.x, 0, goaldir2D.y ));
   // mLineVectorx->update( mShipPos+velocity3D/5.f, (*mGoal) );
    mLineVectorx->update( mShipPos+ Ogre::Vector3(  goaldir2D.x, 0, goaldir2D.y ), (*mGoal) );
    mLineVelocity2D->update( mShipPos, (mShipPos+ Ogre::Vector3(  velocity3D.x, 0, velocity3D.z )) );
*/




}


bool CLandingShipController::adjustValue( float &value, float goal, float threshold, float step ) {

    if( value > goal+threshold ) { value -= step; return(true); }
    else if( value < goal-threshold ) { value +=step; return(true); }

    return(false);

}

Ogre::Vector3 CLandingShipController::getGoalPosition() {

    return( (*mGoal) );

}


// ------------------------------------------------------------------------------------





