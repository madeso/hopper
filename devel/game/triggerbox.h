

#ifndef _TRIGGERBOX_
#define _TRIGGERBOX_

#include "actor.h"


/** TODO:
  * Use that in the game
  * Need a manager ?
  * Find a way to retrieve the actor instance from the MovableObject instance : name ?
  *
*/

class ITriggerOwner {
    public:
    virtual ~ITriggerOwner() {};
    virtual void notifyEffect( Ogre::MovableObject* obj )=0;

};

class ILevelChanger {
    public:
    virtual ~ILevelChanger() {};
    virtual void notifyLevelChange( unsigned int levelnum )=0;
};

/**
 * Zone affecting gameplay (trigger, wind, etc... )
 *
 * The goal is to have this created by some gameplay objects or as standalone objects in the editor.
 *
 */
class CTriggerBox {


public:
    CTriggerBox( Ogre::Vector3 position, Ogre::Vector3 dimensions );
    virtual ~CTriggerBox();

    void update();
    virtual void action( Ogre::MovableObject* obj )=0;

    void setActive( bool active );

protected:
    //std::vector<CActor*> mActors;
    // ITriggerOwner* mEffectOwner;
    Ogre::AxisAlignedBox mAAB;
    Ogre::AxisAlignedBoxSceneQuery* mQuery;

private:
    bool mActive;

};



class CWindZone: public CTriggerBox {

public:
    CWindZone( Ogre::Vector3 pos, Ogre::Vector3 dimensions, Ogre::Vector3 velocity );
    ~CWindZone();

    void action( Ogre::MovableObject* obj );

private:
    Ogre::Vector3 mVelocity;

};



class CBlast: public CTriggerBox {
public:
    CBlast( Ogre::Vector3 pos, Ogre::Vector3 dimensions, float power );
    ~CBlast();

    void action( Ogre::MovableObject* obj );

private:
    Ogre::Vector3 mPosition;
    float mPower;

};



class CLevelChanger: public CTriggerBox {
public:
    CLevelChanger( ILevelChanger* changer, Ogre::Vector3 pos, Ogre::Vector3 dimensions, unsigned int levelNum );
    ~CLevelChanger();

    void action( Ogre::MovableObject* obj );

private:
    unsigned int mLevelNum;
    ILevelChanger* mChanger;
};





#endif


