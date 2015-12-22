#include "optionmanager.h"
#include "Ogre.h"

#include "logger.h"



COptionManager::COptionManager() {
       mDBconn=new sqlite3x::sqlite3_connection("../data/config.db");
}


COptionManager::~COptionManager() {
    mDBconn->close();
    delete(mDBconn);
}


bool COptionManager::getOption(std::string aOptionName, int &value ) {

    std::string val;

    if ( getValue( aOptionName, val )) {

        value=Ogre::StringConverter::parseInt( val );

        return(true);

    }

    return(false);

}


bool COptionManager::getOption(std::string aOptionName, float &value ) {

    std::string val;

    if ( getValue( aOptionName, val )) {
        value=Ogre::StringConverter::parseReal( val );
        return(true);
    }

    return(false);


}

bool COptionManager::getOption(std::string aOptionName, std::string &value ) {

    std::string val;

    if ( getValue( aOptionName, val )) {
        value=val;
        return(true);
    }

    return(false);
}


bool COptionManager::getValue( std::string name, std::string &value) {

    try {
        value=(*mDBconn).executestring( "select value from options where name='"+ name + "';" );
        return(true);
    }
    catch ( std::exception &e ) {
        LOG("\n**sqlite error : %s", e.what() );
        return(false );
    }
    /*
    std::string commandstring="select value from options where name='"+ name + "';";

        sqlite3x::sqlite3_command cmd ( *mDBconn, commandstring );

        sqlite3x::sqlite3_reader reader=cmd.executereader();
        if( reader.read() ) {
            value=reader.getstring(0);
            return(true);
        }

        return(false);
    */
}


bool COptionManager::setOption(std::string aOptionName, std::string value ) {
    return(setValue( aOptionName, value ) );
}

bool COptionManager::setOption(std::string aOptionName, int value ) {

    return(setValue( aOptionName, Ogre::StringConverter::toString( value)));
}

bool COptionManager::setOption(std::string aOptionName, float value ) {
    return(setValue( aOptionName, Ogre::StringConverter::toString( value)));
}


bool COptionManager::setValue( std::string name, std::string value) {

/*    sqlite3x::sqlite3_command cmd ( *mDBconn,
                "replace into options values ( " + name + "," + value + ");" );
*/
    try {

        sqlite3x::sqlite3_transaction trans(*mDBconn);

		{
			sqlite3x::sqlite3_command cmd ( *mDBconn,
                "replace into options values ( ?, ? );");
			cmd.bind( 1, name );
			cmd.bind( 2, value );
			cmd.executenonquery();

		}

        trans.commit();

        return(true);
    }

    catch(/*sqlite3x::database_error*/ std::exception &e) {
        LOG("\n** Error while setting option '%s' with value '%s' !", name.c_str(), value.c_str() );
        LOG("\n** %s", e.what() );
        return(false);

    }


}

