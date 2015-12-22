

#ifndef _GLOGGER_
#define _GLOGGER_

//#include <iostream.h>
//#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "singleton.h"

/// basic printf based logfile management.
class CLogger : public SSingleton<CLogger>  {
    public :

        CLogger();
         ~CLogger();

        void setLogFile( char *aFilename );
        void log(const char *fmt, ... );

    private:
        char* mFilename;
        FILE *mLogFile;
        bool mActive;
};

 // static CLogger *gLogger;



#define LOG CLogger::getInstance()->log

#endif
