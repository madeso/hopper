


#include "hoppercollisionmanager.h"
#include "hoppercollisionmaterials.h"

#include "hopperparticles.h"

#include "physicsmanager.h"


#include "hoppersounds.h"
#include "soundmanager.h"

#include "logger.h"

CHopperCollisionManager::CHopperCollisionManager() {

    CPhysicsManager::getInstance()->setCollisionManager( this );

    mParticleEmitterManager=CParticleEmitterManager::getInstance();



    // these should be read from the database
    //  matA, matB, valid, min impact, soundID, particlesID, friction, bouncyness, num_particles, damage, use_velocity_for_damage

    addCollisionPair( MAT_SHIP,       MAT_SHIP,       0.01, SOUND_METAL_HIT,      PART_COLLISION_SPARKS, 20, 0,   10,  0.5f, true  );
    addCollisionPair( MAT_SHIP,       MAT_METAL,      0.01, SOUND_METAL_HIT,      PART_COLLISION_SPARKS, 20, 0,   10,  0.5f, true  );
    addCollisionPair( MAT_SHIP,       MAT_PLASTIC,    0.01, SOUND_PLASTIC,        -1,                    50, 0.5, 0,   0.1f, true  );
    addCollisionPair( MAT_SHIP,       MAT_STONE,      0.01, SOUND_GROUND_HIT,     PART_COLLISION_SPARKS, 40, 0,   10,  0.5f, true  );
    addCollisionPair( MAT_SHIP,       MAT_TERRAIN,    0.01, SOUND_GROUND_HIT,     PART_TERRAIN_SMOKE,    60, 0,   5,   0.3f,  true  );
    addCollisionPair( MAT_SHIP,       MAT_PROJECTILE, 0,    SOUND_PROJECTILE_HIT, PART_PROJECTILE_HIT,   0,  0,   10,  5.f, false );
    addCollisionPair( MAT_TERRAIN,    MAT_METAL,      0.01, SOUND_METAL_HIT,      PART_TERRAIN_SMOKE,    50, 0,   10,  0.f,  true  );
    addCollisionPair( MAT_TERRAIN,    MAT_PLASTIC,    0.01, SOUND_PLASTIC,        PART_TERRAIN_SMOKE,    40, 0.5, 10,  0.f,  true  );
    addCollisionPair( MAT_TERRAIN,    MAT_STONE,      0.01, SOUND_GROUND_HIT,     PART_TERRAIN_SMOKE,    0,  0,   20,  0.f,  true  );
    //addCollisionPair( MAT_TERRAIN,    MAT_TERRAIN,  0.01, -1,                   -1,                    0,  0,   0,   0    );
    addCollisionPair( MAT_TERRAIN,    MAT_PROJECTILE, 0,    SOUND_PROJECTILE_HIT, PART_TERRAIN_SMOKE,    0,  0,   10,  0.f,  false );
    addCollisionPair( MAT_PROJECTILE, MAT_METAL,      0,    SOUND_PROJECTILE_HIT, PART_PROJECTILE_HIT,   0,  0,   10,  50.f, false );
    addCollisionPair( MAT_PROJECTILE, MAT_PLASTIC,    0,    SOUND_PLASTIC,        PART_PROJECTILE_HIT,   0,  0,   10,  50.f, false );
    addCollisionPair( MAT_PROJECTILE, MAT_STONE,      0,    SOUND_PROJECTILE_HIT, PART_PROJECTILE_HIT,   0,  0,   10,  0.f,  false );
    //addCollisionPair( MAT_PROJECTILE, MAT_PROJECTILE, 0,    -1,                   -1,                    0,  0,   0.f, 0.f  );
    addCollisionPair( MAT_METAL,      MAT_METAL,      0.01, SOUND_METAL_HIT,      PART_COLLISION_SPARKS, 0,  0,   10,  20.f, true  );
    addCollisionPair( MAT_METAL,      MAT_PLASTIC,    0.01, SOUND_PLASTIC,        -1,                    40, 0.5, 0,   10.f, true  );
    addCollisionPair( MAT_METAL,      MAT_STONE,      0.01, SOUND_METAL_HIT,      PART_COLLISION_SPARKS, 20, 0,   10,  30.f, true  );
    addCollisionPair( MAT_PLASTIC,    MAT_PLASTIC,    0.01, SOUND_PLASTIC,        -1,                    20, 0.5, 0,   1.0f, true  );
    addCollisionPair( MAT_PLASTIC,    MAT_STONE,      0.01, SOUND_PLASTIC,        PART_TERRAIN_SMOKE,    40, 0.5, 5,   10.f, true  );
    addCollisionPair( MAT_STONE,      MAT_STONE,      0.01, SOUND_GROUND_HIT,     PART_COLLISION_SPARKS, 0,  0,   10,  10.f, true  );

}



CHopperCollisionManager::~CHopperCollisionManager() {

LOG("\n Destroying Hopper Collision Manager");
    //delete( mSoundBonk);
    //delete( mSoundPlastic);

    //mParticleEmitterManager->destroyEmitter( mSparks );
    //mParticleEmitterManager->destroyEmitter( mSmoke );
    //mParticleEmitterManager->destroyPool( "collision_sparks" );
    //mParticleEmitterManager->destroyPool( "collision_smoke" );

    mCollisionPairs.clear();


LOG(" done." );

}


bool CHopperCollisionManager::collision( OgreOde::Contact* contact ) {


	OgreOde::Geometry * const g1 = contact->getFirstGeometry();
	OgreOde::Geometry * const g2 = contact->getSecondGeometry();

    // collisions between 2 static objects are not interesting !
    if( ( !g1->getBody() ) && ( !g2->getBody() ) )
        return( false);

    // debugCollision( g1, g2 );


    return( handleCollision( g1, g2, contact ));

  // return( true );


}





bool CHopperCollisionManager::handleCollision(
    OgreOde::Geometry* g1,
    OgreOde::Geometry* g2,
    OgreOde::Contact* contact ) {

    int mat1 = g1->getUserData();
    int mat2 = g2->getUserData();

    CPhysActor* actor1=static_cast<CPhysActor*>(g1->getUserObject());
    CPhysActor* actor2=static_cast<CPhysActor*>(g2->getUserObject());

    std::vector<SCollisionPair>::const_iterator it;

    for( it = mCollisionPairs.begin(); it != mCollisionPairs.end(); ++it ) {

        if( ( (*it).materialA == mat1  && (*it).materialB == mat2 ) ||
         ((*it).materialA == mat2  && (*it).materialB == mat1 ) ) {

            contact->setCoulombFriction( (*it).friction );
            contact->setBouncyness( (*it).bouncyness, -0.5 );


            float impactForce=contact->getPenetrationDepth();


            if( impactForce > (*it).min_impact /*0.01*/ ) {  // prevent particle and sound emission on any contact

                if( (*it).soundID >= 0 ) {
                    // play the sound 'soundID' at the collision position
                    CSoundManager::getInstance()->play3D( (*it).soundID, contact->getPosition(), impactForce*10.f );
                }

                if( (*it).particleType >=0  ) {
                    mParticleEmitterManager->emit( (*it).particleType, (*it).num_particles, contact->getPosition() );
                }


                float damage = (*it).damage;

                if( (*it).useVelocity ) {
                    damage = relativeSpeed( actor1, actor2 ) * damage / 10000.f;
                }

                if( actor1 ) {
                    actor1->onCollision( (*it).damage );
                }

                if( actor2 ) {
                    actor2->onCollision( (*it).damage );
                }

                break;

            }
            else {
                // notify ship about being in contact with something ( no damage ) !
                if( mat1 == MAT_SHIP && actor1 ) actor1->onCollision( 0 );
                if( mat2 == MAT_SHIP && actor2 ) actor2->onCollision( 0 );
            }


        }

    }

    return( true ); // collision are valid even if no sound or particle are emitted
}



inline float CHopperCollisionManager::relativeSpeed( CPhysActor* actor1, CPhysActor* actor2 ) {

    Ogre::Vector3 velocity1(0,0,0);
    Ogre::Vector3 velocity2(0,0,0);

    if( actor1 ) velocity1=actor1->getVelocity();
    if( actor2 ) velocity2=actor2->getVelocity();

    return( (velocity1 + velocity2 ).length() );

}


void CHopperCollisionManager::addCollisionPair(
    int materialA, int materialB,
    float min_impact,
    int soundID, int particleType,
    float friction, float bouncyness, unsigned int num_particles,
    float damage, bool useVelocity ) {

    SCollisionPair pair;
    pair.materialA    = materialA;
    pair.materialB    = materialB;
    pair.min_impact   = min_impact;
    pair.soundID      = soundID;
    pair.particleType = particleType;
    pair.friction     = friction;
    pair.bouncyness   = bouncyness;
    pair.num_particles= num_particles;
    pair.damage       = damage;
    pair.useVelocity  = useVelocity;


    mCollisionPairs.push_back( pair );

}



void CHopperCollisionManager::debugCollision( OgreOde::Geometry* g1, OgreOde::Geometry* g2 ) {

    LOG("\nCollision between ");
        switch( g1->getUserData() ) {
            case MAT_SHIP: LOG("ship" ); break;
            case MAT_METAL: LOG("metallic object" ); break;
            case MAT_STONE: LOG("stone object" ); break;
//            case UNDEFINED: LOG("undefined object" ); break;
            case MAT_PLASTIC: LOG("plastic object" ); break;
            case MAT_SAND: LOG("sand object" ); break;
            case MAT_ICE: LOG("ice object" ); break;
            // case CONCRETE:
            default: LOG( "mat %d object", g1->getUserData() ); break;
        }

    //LOG(" (%s) ", g1->getTypeName().c_str() );
    LOG(" and " );


        switch( g2->getUserData() ) {
            case MAT_SHIP: LOG("ship" ); break;
            case MAT_METAL: LOG("metallic object" ); break;
            case MAT_STONE: LOG("stone object" ); break;
//            case UNDEFINED: LOG("undefined object" ); break;
            case MAT_PLASTIC: LOG("plastic object" ); break;
            case MAT_SAND: LOG("sand object" ); break;
            case MAT_ICE: LOG("ice object" ); break;
            default: LOG( "mat %d object", g2->getUserData() ); break;
        }

    //LOG(" (%s) ", g2->getTypeName().c_str() );


}
