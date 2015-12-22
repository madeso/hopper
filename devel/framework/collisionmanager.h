
#ifndef _CCOLLISIONMANAGER_
#define _CCOLLISIONMANAGER_

#include "singleton.h"
#include "OgreOde_Core.h"

/*
class CCollisionHandler {
    public:
    CCollisionHandler() {};
    virtual bool collision(OgreOde::Contact* contact)=0;

};
*/

/// base collision manager : could be overloaded for sounds or other effect

class CCollisionManager: /*public SSingleton<CCollisionManager>,*/ public OgreOde::CollisionListener {

    public:
    CCollisionManager();
    virtual ~CCollisionManager();

//    void setCollisionHandler( CCollisionHandler* h );
   virtual bool collision(OgreOde::Contact* contact);  // callback on collision

//    private:
//    CCollisionHandler* mCollisionHandler;

};


#endif


