

#ifndef _TUNNELS_
#define _TUNNELS_

#include "scenemanager.h"
#include <vector>
#include "light.h"


#define TUNNEL_MAX_WIDTH 20
#define TUNNEL_MAX_HEIGHT 10
#define TUNNEL_MAX_DEPTH 20

class CTunnels {

    public:
    CTunnels();
    ~CTunnels();
    void update( Ogre::Vector3 pos );

    private:
    unsigned char grid[TUNNEL_MAX_WIDTH+1][TUNNEL_MAX_HEIGHT+1][TUNNEL_MAX_DEPTH+1];

    std::vector<Ogre::ManualObject*> mManualObjList;
    std::vector<Ogre::SceneNode*> mNodeList;

    Ogre::SceneNode* mNode;

    std::vector<OgreOde::Geometry*> mGeomList;

    Ogre::Vector3 mPosition;
    Ogre::SceneManager* mSceneManager;

    std::vector<std::string> mMaterials;

    Ogre::Vector3 mVertices[24];
    unsigned int mNumVertices;
    unsigned int mIndices[24];
    unsigned int mNumIndices;

    Ogre::Vector3 mPoints[8];

    std::vector<CPointLight*> mLightList;


};



#endif


