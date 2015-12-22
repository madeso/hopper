

#include "sound.h"

#include "logger.h"


CSound::CSound( unsigned int aSoundID, bool looping )
    :mSoundID(aSoundID),
     mLooping( looping),
     mSoundManager( CSoundManager::getInstance()),
     mSourceID( mSoundManager->reserveSource() ),
     mSourceName ( mSoundManager->getSourceName( mSourceID )) {

    if( looping )
        alSourcei( mSourceName, AL_LOOPING, AL_TRUE );
    else
        alSourcei( mSourceName, AL_LOOPING, AL_FALSE );



    alSourcei( mSourceName, AL_BUFFER, mSoundManager->getSoundBuffer(aSoundID) );
    alSourcef( mSourceName, AL_REFERENCE_DISTANCE, 10.f );

    //LOG("\nCSound : sound %d got source id %d ", mSoundID, mSourceID );

}


CSound::~CSound() {


    stop();
    mSoundManager->releaseSource( mSourceID );

    //LOG("\nCSound : sound %d released source id %d ", mSoundID, mSourceID );
}



void CSound::stop() {

    // mSoundManager->sourceStop( mSourceID );

    if( mSourceName ) {
        // LOG("\nCSound : stopping source %d ( %d )", mSourceID, mSourceName );
        // alSourcei( mSourceName, AL_LOOPING, AL_FALSE );
        alSourceStop( mSourceName );
    }
}

void CSound::pause() {

//    mSoundManager->sourcePause( mSourceID );
    if( mSourceName )
        alSourcePause( mSourceName );
}


void CSound::setVolume( float gain ) {

// mSoundManager->setSourceVolume( mSourceID, gain );

    if( mSourceName )
        alSourcef( mSourceName, AL_GAIN, gain );
}


CSound2D::CSound2D( unsigned int aSoundID, bool looping ):CSound( aSoundID, looping ) {

//    mSoundManager->setSourceMode2D( mSourceID );
    alSourcei( mSourceName, AL_SOURCE_RELATIVE, AL_TRUE ); // relative to the listener
    alSource3f( mSourceName, AL_POSITION, 0,0,0  );

}


void CSound2D::play() {

    if( mSourceName ) { // && !mSoundManager->isPlaying( mSource ) ) {
        alSourcePlay( mSourceName );
    }
}



// constructor for an attached (to another scenenode ) 3D sound
CSound3D::CSound3D( unsigned int aSoundID, Ogre::SceneNode* parent, bool looping )
    :CSound( aSoundID, looping ),
     mParent( parent ) {


    alSourcei( mSourceName, AL_SOURCE_RELATIVE, AL_FALSE ); // absolute position

}

// constructor for an unattached 3D sound with no initial position
CSound3D::CSound3D( unsigned int aSoundID, bool looping )
    :CSound( aSoundID, looping ),
     mParent( 0),
     mPosition( Ogre::Vector3(0,0,0) ) {

    alSourcei( mSourceName, AL_SOURCE_RELATIVE, AL_FALSE ); // absolute position

}



void CSound3D::play() {

    if( mSourceName )  { // && !mSoundManager->isPlaying( mSource ) ) {

        if(mParent) {
            mPosition=mParent->getPosition();
            alSource3f( mSourceName, AL_POSITION, mPosition.x, mPosition.y, mPosition.z );
        }
        alSourcePlay( mSourceName );
    }

}


void CSound3D::setPosition( Ogre::Vector3 position ) {
    mPosition=position;
    alSource3f( mSourceName, AL_POSITION, position.x, position.y, position.z );
}






#if 0

CSound::CSound( int aSoundID, int aSoundType ) {

    mSoundManager=CSoundManager::getInstance();

    mSoundId=aSoundID;

    mSoundType=aSoundType;
/*

    alGenSources( 1, (ALuint*)&mSourceNum );

    alSourcei(mSourceNum, AL_BUFFER, mSoundManager->getSound(mSoundId) );

    switch(aSoundType) {
        case SOUNDTYPE_2D:
            alSourcei( mSourceNum, AL_SOURCE_RELATIVE, AL_TRUE );
            alSourcei( mSourceNum, AL_LOOPING, AL_FALSE );
            break;
        case SOUNDTYPE_2D_LOOPING:
            alSourcei( mSourceNum, AL_SOURCE_RELATIVE, AL_TRUE );
            alSourcei( mSourceNum, AL_LOOPING, AL_TRUE );
            break;
        case SOUNDTYPE_3D:
            alSourcei( mSourceNum, AL_SOURCE_RELATIVE, AL_FALSE );
            alSourcei( mSourceNum, AL_LOOPING, AL_FALSE );
            break;
        case SOUNDTYPE_3D_LOOPING:
            alSourcei( mSourceNum, AL_SOURCE_RELATIVE, AL_FALSE );
            alSourcei( mSourceNum, AL_LOOPING, AL_TRUE );
            break;

    }

    alSourcef( mSourceNum, AL_REFERENCE_DISTANCE, 20.f );
*/
}

CSound::~CSound() {

/*
    alDeleteSources( 1 , (ALuint*)&mSourceNum );
*/
}

/*
void CSound::setPosition( Ogre::Vector3 pos ) {
    mPosition=pos;
    alSource3f( mSourceName, AL_POSITION, pos.x, pos.y, pos.z );
}
*/

void CSound::setVolume( float gain ) {
    // alSourcef( mSourceNum, AL_GAIN, gain );
}

void CSound::play() {
    // alSourcePlay ( mSourceNum );
    if(
    mSoundManager->play( mSoundId );
}


void CSound::stop() {
    mSoundManager->stop( mSoundID );
    // alSourceStop ( mSourceNum );
}


void CSound::pause() {
    mSoundManager->pause( mSoundID );
    // alSourcePause ( mSourceNum );
}


/*
CSound3D::CSound3D( int aSoundID, Ogre::SceneNode* parent, bool looping )
    :CSound( aSoundID, SOUNDTYPE_3D, looping ) {

    mNode=parent->createChildSceneNode();

}
*/
#endif

