

#include "soundmanager.h"
#include "optionmanager.h"
#include "logger.h"



CSoundManager::CSoundManager() {

    LOG("\nSoundManager: init" );

    if( ! alutInit(NULL,NULL) )
        throw( "Fatal : can't init ALut !" );

    // alListenerf( AL_GAIN, 0.5 );
    int volume;
    COptionManager::getInstance()->getOption( "volume", volume );
    setVolume( volume/10.f );

    // alDistanceModel( AL_LINEAR_DISTANCE );

    // LOG("\nSupported sound types (MIME) : %s", alutGetMIMETypes( ALUT_LOADER_BUFFER ) );

    if( alGetError() )
            LOG("\nSoundManager : Warning ALut error : %s");

    mMaxSystemSources=0;
    ALint err;

    for( int s=0; s<SOUND_MANAGER_MAX_SOURCES; s++ ) {

        alGenSources( 1, &mSource[s] );

        if( err=alGetError() ) {
            LOG("\nSoundManager : can't create source#%d ( ALut error#%d : %s ) !", s, err, alutGetErrorString( err )  );
            mSource[s]=0;
            break;
        }

        mMaxSystemSources++;
        mSourceReserved[s]=false;

    }

    LOG("\nSoundManager: This system will use a maximum of %d sound sources", mMaxSystemSources );

}

CSoundManager::~CSoundManager() {

    // Delete the sources and buffers
//    alDeleteSources( SOUND_MANAGER_MAX_SOURCES, mSource );
//    alDeleteBuffers( MAX_AUDIO_BUFFERS, mAudioBuffers );

/*
   // Destroy the sound context and device
   mSoundContext = alcGetCurrentContext();
   mSoundDevice = alcGetContextsDevice( mSoundContext );
   alcMakeContextCurrent( NULL );
   alcDestroyContext( mSoundContext );
   if ( mSoundDevice)
       alcCloseDevice( mSoundDevice );
*/


    for( unsigned int i = 0; i < mMaxSystemSources; i++ ) {
        delete( mSoundDelay[i] );
    }

    alutExit();

}

bool CSoundManager::load( unsigned int aSoundID, char *aFilename ) {

   int buffNum;
   buffNum=alutCreateBufferFromFile ( aFilename);

    mSoundDelay[aSoundID]=new CDelay(200); //can't play a sound more than once in 200ms

   if(buffNum != AL_NONE ) {
       mSoundBuffer[aSoundID]=buffNum;
   }
   else {
       mSoundBuffer[aSoundID]=0;
       LOG("\nSoundManager : Warning ALut error : %s while trying to load \"%s\" !",
            alutGetErrorString( alutGetError()) , aFilename );
   }

}

unsigned int CSoundManager::getSoundBuffer( unsigned int aSoundID ) {
     return( mSoundBuffer[aSoundID] );
}


void CSoundManager::setVolume( float volume ) {

    if( volume>=0 && volume <=10 )
        alListenerf ( AL_GAIN, volume );

}

void CSoundManager::set3DListener( Ogre::Vector3 pos, Ogre::Vector3 vel,  Ogre::Vector3 front, Ogre::Vector3 top ) {

    static float dir[6];

    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);

    alListener3f( AL_VELOCITY, vel.x, vel.y, vel.z );

    dir[0]=front.x;
    dir[1]=front.y;
    dir[2]=front.z;
    dir[3]=top.x;
    dir[4]=top.y;
    dir[5]=top.z;

    alListenerfv(AL_ORIENTATION, dir );

}


void CSoundManager::pauseAll() {

    for( int s=0; s<mMaxSystemSources; s++ ) {
        if( mSourceReserved[s] && isPlaying( mSource[s] )) {
            alSourcePause( mSource[s] );
            mPausedSources.push( mSource[s] ); // store only playing sources
        }
    }

}

void CSoundManager::resumeAll() {

    while( !mPausedSources.empty() ) {
        alSourcePlay( mPausedSources.front() );
        mPausedSources.pop();
    }

}


void CSoundManager::play3D( unsigned int aSoundID, Ogre::Vector3 pos, float volume ) {

    if( mSoundDelay[aSoundID]->isOver() )
        mSoundDelay[aSoundID]->restart();
    else {
        // LOG("CSoundManager:play3D() : sound delay for soundID#%d is NOT over !!!", aSoundID );
        return;
    }

    int sid = getUnreservedSourceID();
    if( sid >=0 ) {

        ALuint source = mSource[ sid ];
        if( source ) {

            alSourcei( source, AL_BUFFER, mSoundBuffer[aSoundID] );
            alSourcef( source, AL_REFERENCE_DISTANCE, 10.f );

            alSourcei( source, AL_SOURCE_RELATIVE, AL_FALSE ); // absolute position
            alSource3f( source, AL_POSITION, pos.x, pos.y, pos.z );
            alSourcef( source, AL_GAIN, volume );

            alSourcePlay ( source );

            //LOG("\nCSoundManager: playing soundID#%d as an unreserved 3D sound using source#%d", aSoundID, sid );

        }
    }
    /*
    else {
        LOG("\nCSoundManager:play3D() : NOT playing soundID#%d !!! (sourceID %d)", aSoundID, sid );
    }
    */

}


void CSoundManager::play2D( unsigned int aSoundID, float volume ) {

    if( mSoundDelay[aSoundID]->isOver() )
        mSoundDelay[aSoundID]->restart();
    else {
        // LOG("CSoundManager:play2D() : sound delay for soundID#%d is NOT over !!!", aSoundID );
        return;
    }

    int sid = getUnreservedSourceID();
    if( sid >=0 ) {
        ALuint source = mSource[ sid ];
        if( source ) {

            alSourcei( source, AL_BUFFER, mSoundBuffer[aSoundID] );
            alSourcef( source, AL_REFERENCE_DISTANCE, 10.f );

            alSourcei( source, AL_SOURCE_RELATIVE, AL_TRUE ); // absolute position
            alSource3f( source, AL_POSITION, 0, 0, 0 );
            alSourcef( source, AL_GAIN, volume );
            alSourcePlay ( source );

            //LOG("\nCSoundManager: playing soundID#%d as an unreserved 2D sound using source#%d", aSoundID, sid );

        }
    }
    /*
    else {
        LOG("\nCSoundManager:play2D() : NOT playing soundID#%d !!! (sourceID %d)", aSoundID, sid );
    }
    */
}

ALuint CSoundManager::getSourceName( int aSourceID ) {
    if( aSourceID >=0 )
        return( mSource[aSourceID] );
    else
        return( 0 );

}



int CSoundManager::getUnreservedSourceID() {

    for( int sid=0; sid<mMaxSystemSources; sid++ ) {

        if( (!mSourceReserved[sid]) && (!isPlaying( sid )) ) {
            return( sid );
        }

    }
    LOG("\nCSoundManager: warning : no free unreserved source !");
    return(-1);
}


int CSoundManager::reserveSource() {

    for( int sid=0; sid<mMaxSystemSources; sid++ ) {

        if( (!mSourceReserved[sid]) && (!isPlaying( sid )) ) {
            mSourceReserved[sid]=true;

            return( sid );
        }
    }

    // no free source ! :(
    LOG("\nCSoundManager: warning : no free source !");
    return( -1 );

}



void CSoundManager::releaseSource( int aSourceID ) {
    if( aSourceID >= 0 ) {
        // BUG: looping sounds are not stopping with alSourceStop !
        // we have to delete the source, then recreate it
        alDeleteSources( 1, &mSource[ aSourceID ] );
        alGenSources( 1, &mSource[ aSourceID ] );
        mSourceReserved[ aSourceID ]=false;
    }
}


bool CSoundManager::isPlaying( int aSourceID ) {

    ALint state;

    if( aSourceID >=0 ) {
        alGetSourcei( mSource[aSourceID], AL_SOURCE_STATE, &state );
        return( state == AL_PLAYING );
    }
    else
        return( true ); // returns true because the main usage is !isPlaying()
}



int CSoundManager::getNumberOfReservedSources() {

    int nb=0;

    for( int sid=0; sid<mMaxSystemSources; sid++ ) {
        if( mSourceReserved[sid] ) nb++;
    }
    return( nb);
}





/*

// This function loads a .ogg file into a memory buffer and returns
// the format and frequency.
//
// Usage:
//
// Create sound buffer and source
//    alGenBuffers(1, &bufferID);
////
//  Load the OGG file into memory
//    LoadOGG(argv[1], bufferData, format, freq);
//  Upload sound data to buffer
//    alBufferData(bufferID, format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);
//
//
#define OGG_LOADER_BUFFER_SIZE     32768       // 32 KB buffers
bool CSoundManager::loadOGG(char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq) {

    int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[OGG_LOADER_BUFFER_SIZE];     // Local fixed size array
    FILE *f;

    // Open for binary reading
    f = fopen(fileName, "rb");

    if (f == NULL) {
        cerr << "Cannot open " << fileName << " for reading..." << endl;
        exit(-1);
    }

    vorbis_info *pInfo;
    OggVorbis_File oggFile;

    // Try opening the given file
    if (ov_open(f, &oggFile, NULL, 0) != 0) {
        LOG( "\nOgg Loader : Error opening \"%s\" for decoding...", filename );
        return( false );
    }


    // Get some information about the OGG file
    pInfo = ov_info(&oggFile, -1);

    // Check the number of channels... always use 16-bit samples
    if (pInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;


    // The frequency of the sampling rate
    freq = pInfo->rate;

    // Keep reading until all is read
    do {
        // Read up to a buffer's worth of decoded sound data
        bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

        if (bytes < 0)
            {
            ov_clear(&oggFile);
            cerr << "Error decoding " << fileName << "..." << endl;
            exit(-1);
            }
        // end if

        // Append to end of buffer
        buffer.insert(buffer.end(), array, array + bytes);

    } while (bytes > 0);

    // Clean up!
    ov_clear(&oggFile);
}

*/


