

#ifndef _CSOUND_
#define _CSOUND_

#include "Ogre.h"
#include "soundmanager.h"

enum { SOUNDTYPE_2D, SOUNDTYPE_3D, SOUNDTYPE_2D_LOOPING, SOUNDTYPE_3D_LOOPING };


/*
class CSound {

    public:
        CSound( int aSoundID, int aSoundType=SOUNDTYPE_2D );
        ~CSound();

        void setPosition(Ogre::Vector3 aPos );

        void setVolume( float gain );

        void play();
        void stop();
        void pause();

    private:
        int mSourceNum;  // openAL source "name"
        int mSoundId;
        CSoundManager* mSoundManager;
};
*/


class CSound {

    public:
    void notifyInvalidSource();
    void pause();
    void stop();
    void setVolume( float gain );

    protected:
    CSound( unsigned int aSoundID, bool looping );
    ~CSound();


    unsigned int mSoundID;
    bool mLooping;
    CSoundManager* mSoundManager;
    int mSourceID;
    ALuint mSourceName;



};


class CSound2D: public CSound {

    public:
    CSound2D( unsigned int aSoundID, bool looping=false );
    void play();

};



class CSound3D: public CSound {
    public:
    CSound3D( unsigned int aSoundID, Ogre::SceneNode* parent, bool looping=false );
    CSound3D( unsigned int aSoundID, bool looping=false );

    void play();

    void setPosition( Ogre::Vector3 position );

    private:
    Ogre::SceneNode* mParent;
    Ogre::Vector3 mPosition;

};

#endif

