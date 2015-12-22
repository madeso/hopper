

#ifndef _CSGTUNNELS_
#define _CSGTUNNELS_


#include "cvCSGOgreWrapper.h"


class CSGTunnels {

public:

    CSGTunnels( Ogre::Vector3 pos );
    ~CSGTunnels();

private:

    Ogre::Entity* mRoom1;
    Ogre::Entity* mCube;
    Ogre::Entity* mResult;

    Ogre::SceneNode* mResultNode;
    Ogre::SceneNode* mRoom1Node;
    Ogre::SceneNode* mCubeNode;

};



#endif

