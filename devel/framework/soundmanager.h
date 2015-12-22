
#ifndef _CSOUNDMANAGER_H_
#define _CSOUNDMANAGER_H_

#include "singleton.h"
#include "Ogre.h"

#include "AL/alut.h"

#include "delay.h"

#define SOUND_MANAGER_MAX_BUFFERS 128

#define SOUND_MANAGER_MAX_SOURCES 32


/// Basic sound management.


class CSoundManager:public SSingleton<CSoundManager> {

    friend class CSound;

    public:

    CSoundManager();

    ~CSoundManager();

    bool load( unsigned int aSoundID, char *aFilename );

    void play2D( unsigned int aSoundID, float volume=1.f );
    void play3D( unsigned int aSoundID, Ogre::Vector3 pos, float volume=1.f );

    void setVolume( float volume );


    void pauseAll();
    void resumeAll();


    void set3DListener( Ogre::Vector3 pos, Ogre::Vector3 vel, Ogre::Vector3 front, Ogre::Vector3 up );

    int getNumberOfReservedSources();



    protected:

    ALuint getSourceName( int aSourceID );

    int getSourceID();
    void releaseSource( int aSourceID );
    bool isPlaying( int aSourceID );
    unsigned int getSoundBuffer( unsigned int aSoundID );
    int reserveSource();

    private:

    unsigned int mSoundBuffer[SOUND_MANAGER_MAX_BUFFERS];
    ALuint mSource[SOUND_MANAGER_MAX_SOURCES];
    bool mSourceReserved[SOUND_MANAGER_MAX_SOURCES];
    CDelay* mSoundDelay[SOUND_MANAGER_MAX_BUFFERS];

    std::queue< ALuint > mPausedSources;

    int getUnreservedSourceID();

    int mMaxSystemSources;
};


#endif
