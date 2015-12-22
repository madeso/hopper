




#include "logger.h"



CLogger::CLogger() {
}


CLogger::~CLogger() {

    if(mActive) {
        fclose(mLogFile);
    }

}

void CLogger::setLogFile( char *aFilename ) {

    mFilename=aFilename;
    mLogFile=NULL;
    mActive=false;

    if(mFilename!=NULL ) {
        mLogFile=fopen( mFilename, "w" );
    }

    if(mLogFile!=NULL) mActive=true;
}

#define BUFFER_SIZE 256

void CLogger::log( const char *fmt, ... )
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(mLogFile, fmt, ap );
    va_end(ap);
    fflush(mLogFile);
}



