#ifndef OPTIONMANAGER_H
#define OPTIONMANAGER_H

#include "singleton.h"

#include <string>
#include "sqlite3x.hpp"

/**
    stores and retrieves ints, floats or strings ( stored as strings ) in the 'options' table

    Usage exemple:

    COptionManager* opt=COptionManager::getInstance();
    opt->setOption( "test1", (float)1.23456 );
    opt->setOption( "test2", 123 );
    opt->setOption( "test3", "hello" );

    int test2;
    if ( opt->getOption( "test9", test2 ) ) {
        LOG("optionmanager test : test2=%d", test2 );
    }

    COptionManager::kill();
*/
class COptionManager : public SSingleton<COptionManager>
{
    public:
        COptionManager();
        ~COptionManager();

        bool getOption(std::string aOptionName, int &value );
        bool getOption(std::string aOptionName, float &value );
        bool getOption(std::string aOptionName, std::string &value );

        bool setOption(std::string aOptionName, int value);
        bool setOption(std::string aOptionName, float value);
        bool setOption(std::string aOptionName, std::string value);

    protected:

    private:

    sqlite3x::sqlite3_connection* mDBconn;

    bool getValue( std::string name, std::string &value );
    bool setValue( std::string name, std::string value );


};



#endif // OPTIONMANAGER_H
