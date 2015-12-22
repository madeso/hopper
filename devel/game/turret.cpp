

#include "turret.h"
#include "projectilemanager.h"
#include "explosionmanager.h"

CTurret::CTurret( Ogre::String name, Ogre::Vector3 pos ):
    CStaticPhysTrimesh( pos, 0, 1, name, "turret2.mesh" )   {

    //mTurretStand = new CStaticPhysTrimesh( pos, 0, 1, name + "stand", "turretstand.mesh" );
    //mTurretCannon = new CStaticPhysTrimesh( pos, 0, 1, name + "cannon", "turretcannon.mesh" );

    // mHeading = Ogre::Vector2( 0, 1 );
    mYaw = 0;
    mFireDelay = new CDelay( 300 );
    mFirePause = new CDelay( 2000 );
    mFireCount = 4;

}

CTurret::~CTurret() {

    //delete( mTurretStand );
    //delete( mTurretCannon );
    delete( mFireDelay );
    delete( mFirePause );

}


void CTurret::update( CActor* actor ) {

    if( isDead() ) return;

    Ogre::Vector3 ennemy_pos = actor->getNode()->getPosition();
    Ogre::Vector3 pos = mNode->getPosition();

    if( pos.distance( ennemy_pos ) < 50 ) {

        Ogre::Vector2 goal( ennemy_pos.x - pos.x, ennemy_pos.z - pos.z );
        goal.normalise();
        //mHeading = Ogre::Vector2( cos(mYaw.valueAngleUnits()), sin(mYaw.valueAngleUnits()) );
        Ogre::Vector2 v2( -cos( mYaw.valueAngleUnits() ), sin( mYaw.valueAngleUnits() ) );

        // rotate stand ( yaw )
        float dot = v2.dotProduct( goal );

        if( dot < -0.1 ) {
            mYaw += Ogre::Radian( 3.1415/180.f );
            // mNode->resetOrientation();
            mNode->yaw( mYaw );
        }
        else {
            if ( dot > 0.1 ) {
                mYaw -= Ogre::Radian( 3.1415/180.f );
                // mNode->resetOrientation();
                mNode->yaw( mYaw );
            }
            else  {
                // if actor in line, shoot !
                if( mFirePause->isOver() && mFireDelay->isOver() ) {
                    mFireDelay->restart();

                    Ogre::Vector3 v( ennemy_pos - pos );
                    v.normalise();

                    CProjectileManager::getInstance()->createProjectile( pos + v*2.0f, -v, v*3.f );
                    mFireCount--;
                    if( mFireCount == 0) {
                        mFirePause->restart();
                        mFireCount = 4;
                    }
                }

            }
        }




    }

}





// overloaded from CActor :
void CTurret::die() {

    CExplosionManager::getInstance()->createExplosion( getNode(), CExplosionManager::SMOKE );
    mEntity->setMaterialName( "destroyed" );

}



