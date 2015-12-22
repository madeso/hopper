
/// sun object

#ifndef _CSUN_
#define _CSUN_


#include "astrum.h"
/*
#include "Ogre.h"
#include "scenemanager.h"
#include "light.h"
*/

// #define SUN_DISTANCE_TO_CAMERA 2000.f

class CSun: public CAstrum {

    public:
    CSun( Ogre::Vector3 direction, Ogre::ColourValue colour, Ogre::String material, float size );
    ~CSun();

    void setDirection( Ogre::Vector3 direction );
//    void onCameraMoved( Ogre::Vector3 campos );

    private:
    CDirectionalLight* mLight;


};


#endif
