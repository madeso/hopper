

#ifndef _CPROJECTILE_
#define _CPROJECTILE_

#include "physicobject.h"
#include "delay.h"
#include "light.h"

class CProjectile: public CDynamicPhysSphere {

        public:

            // enum PROJECTILE_TYPE { PULSE, ROCKET };

            CProjectile( Ogre::Vector3 pos, float scale, float mass,
                         Ogre::String name, Ogre::String model );
            ~CProjectile();

            virtual void update() = 0;
            bool isDead();
            void onCollision( float impactForce );

        protected:
            CDelay* mTTL;
            bool mDead;
            CPointLight* mLight;


    };



class CPulse: public CProjectile {

        public:
            CPulse( Ogre::String name, Ogre::Vector3 pos, Ogre::Vector3 dir, Ogre::Vector3 velocity  );
            // ~CPulse();

            void update();

    };


class CMissile: public CProjectile {

        public:
            CMissile( Ogre::String name, Ogre::Vector3 pos, Ogre::Vector3 dir );
            // ~CRocket();

            void update();

    };

/*
template<class TProjectile>
class CProjectileFactory {

        public:
            CProjectileFactory() {
                mProjectileCount = 0;
            }

            // ~CProjectileFactory();

            TProjectile* create( Ogre::Vector3 pos, Ogre::Vector3 dir );
            // CPulse* createPulse(Ogre::Vector3 pos, Ogre::Vector3 dir);
            // CRocket* createRocket(Ogre::Vector3 pos, Ogre::Vector3 dir);

        private:
            int mProjectileCount;

    };



template<class TProjectile>
TProjectile* CProjectileFactory<TProjectile>::create( Ogre::Vector3 pos, Ogre::Vector3 dir ) {
    return( new TProjectile(
                "projectile" + Ogre::StringConverter::toString( mProjectileCount ++ ),
                pos, dir ) );
}

*/

#endif

