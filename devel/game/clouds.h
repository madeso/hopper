

#ifndef _CCLOUDS_
#define _CCLOUDS_

#include "Ogre.h"

class CClouds {

 public:
    CClouds( Ogre::Vector3 pos, Ogre::Vector3 size, int aMax , Ogre::String material=Ogre::StringUtil::BLANK );
    // CClouds();
    ~CClouds();

//    void update( Ogre::Vector3 pos);
    void setPos( Ogre::Vector3 pos);
    void setHeight( float aHeight );

 private:
    Ogre::SceneNode* mNode;
    float mHeight;
    Ogre::BillboardSet* mBillboardSet;
    float mX,mZ;


};



#endif
