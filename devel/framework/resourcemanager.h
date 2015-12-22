

#ifndef _CRESOURCEMANAGER_
#define _CRESOURCEMANAGER_


#include "Ogre.h"
#include "soundmanager.h"
#include "singleton.h"

/// loads the ressources.

class CResourceManager: public SSingleton<CResourceManager> {
    public:
    void loadAll();

    void loadSplash();
    void unloadSplash();

    void printResources(Ogre::String aResourceGroup );
    void printAllResources();
   // void printResourceInfo(Ogre::String aResourceName );

   void loadSounds();
   void initParticleSystems();


};

/*
#include "resource.h"
#include <map>
#include <string>


class CRessourceManager: public Singleton<CRessourceManager> {

    public:
    CRessourceManager();

    IResource* getRessource( string aRessourceName );

    private:
    map<string, IResource*> mRessourceMap;

};
*/

#endif
