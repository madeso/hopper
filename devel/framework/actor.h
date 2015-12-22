
#ifndef _CACTOR_
#define _CACTOR_


#include "Ogre.h"
#include "scenemanager.h"
#include "OgreUserDefinedObject.h"

/// Basic game object : owns a scenenode and a model ( entity ).

/* Refactoring :
    Should have a "static object" base class...
 */
class CActor:public Ogre::UserDefinedObject {



public:

    enum Flag { BASIC_OBJECT = 0x01, STATIC_OBJECT = 0x02, DYNAMIC_OBJECT = 0x04  };

     // CActor();
     CActor(Ogre::String aName, Ogre::String aModelname);
//     CActor( int aType);
    virtual ~CActor();

    Ogre::Vector3 getPosition();
    void setPosition(Ogre::Vector3 pos );
    void scale( Ogre::Vector3 scalefactor );
    void scale( float widthfactor, float depthfactor, float heightfactor );
    void setScale( Ogre::Vector3 scale );
    void setScale( float xscale, float yscale, float zscale );

    void setOrientation( Ogre::Quaternion o );
    Ogre::Quaternion getOrientation();

    // void yaw( float aYaw );

    void setYaw( float aYaw );
    void setPitch( float aPitch);
    void setRoll( float aRoll);

    void orient( float aYaw, float aPitch, float aRoll );

    float getYaw();
    float getPitch();
    float getRoll();


    void updateAABB();

    Ogre::AxisAlignedBox getAABB();

    Ogre::SceneNode* getNode();
    Ogre::Entity* getEntity();

    void showBoundingBox( bool show );

    void attachObject( Ogre::MovableObject* node );

    void setVisible( bool aVisible );

    Ogre::String getName();

    void setCastShadows( bool castshadow );

    void castShadows();

    void setQueryFlags( Ogre::uint32 flags );
    Ogre::uint32 getQueryFlags();


    bool isDestroyable();
    bool isDead();
    void setDestroyable( bool destroyable );
    void setLife( float life );



protected:
    Ogre::Entity* mEntity;
    Ogre::SceneNode* mNode;

    float mYaw, mPitch, mRoll;

    void damage( float aDamage );

    /// overload it to change the dying effect ( explosion type )
    virtual void die();


private:

    // Ogre::SceneManager* mSceneManager;
    CSceneManager* mSceneManager;
    Ogre::SceneNode* mRoot;
    Ogre::String mName;

    bool mDead;  /// actor life <= 0

    float mLife;
    bool mDestroyable;


};


#endif
