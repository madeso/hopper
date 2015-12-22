


#ifndef _HOPPERCOLLISIONMANAGER_
#define _HOPPERCOLLISIONMANAGER_

#include "collisionmanager.h"
#include "particleemittermanager.h"
// #include "sound.h"
#include "physicobject.h"

class CHopperCollisionManager: public SSingleton<CHopperCollisionManager>, public CCollisionManager {


    public:
    CHopperCollisionManager();
    ~CHopperCollisionManager();

    bool collision( OgreOde::Contact* contact );

    float relativeSpeed( CPhysActor* actor1, CPhysActor* actor2 );


    private:
    struct SCollisionPair {
        int materialA;
        int materialB;

        float min_impact;  // minimum penetration depth necessary to trigger the effect

        int soundID;
        int particleType;

        float friction;
        float bouncyness;

        unsigned int num_particles;  // number of particles to emit at each contact

        float damage;
        bool useVelocity;  // use velocity to compute damage ?

    };

    std::vector< SCollisionPair > mCollisionPairs;

    CParticleEmitterManager* mParticleEmitterManager;


    CParticleEmitterPool* mCollisionParticlePool;


//    CSound3D* mSoundBonk;
//    CSound3D* mSoundPlastic;

    void addCollisionPair( int materialA, int materialB,
        float min_impact,
        int soundID, int particleType,
        float friction, float bouncyness, unsigned int num_particles,
        float damage, bool useVelocity );

    bool handleCollision(
        OgreOde::Geometry* g1,
        OgreOde::Geometry* g2,
        OgreOde::Contact* contact );


    void debugCollision( OgreOde::Geometry* g1, OgreOde::Geometry* g2 );

};


#endif


