

#include "levelinfo.h"
#include "sqlite3x.hpp"

#include "logger.h"

int CLevelInfo::getLevelID( int gameType, int levelOrder ) {

    int lvl=1;

    LOG("\nCLevelInfo::getLevelID()" );

    sqlite3x::sqlite3_connection con("../data/levels.db");
    try
    {
        //CLogger::getInstance()->log("\nDatabse opened.");
        sqlite3x::sqlite3_command cmd ( con, "select id from level where gametype = ? and gameorder = ?;" );
        cmd.bind( 1, gameType );
        cmd.bind( 2, levelOrder );

        lvl=cmd.executeint();

    }
    catch( std::exception e ) {
        LOG("\nFatal : %s", e.what() );
    }
    con.close();

    LOG(": got level %d for gameType = %d and gameOrder = %d", lvl, gameType, levelOrder );

    return( lvl );


}



int CLevelInfo::getMaxLevels( int gameType ) {

    int maxlvl=0;

    LOG("\nCLevelInfo::getMaxLevels()" );

    sqlite3x::sqlite3_connection con("../data/levels.db");
    //CLogger::getInstance()->log("\nDatabse opened.");
    {
        sqlite3x::sqlite3_command cmd ( con, "select count(*) from level where gametype = ?;" );
        cmd.bind( 1, gameType );

        maxlvl=cmd.executeint();
    }

    con.close();

    LOG(" => got %d", maxlvl );

    return(maxlvl);
}


int CLevelInfo::getNewLevelID() {

    int levelid;

    LOG("\nCLevelInfo::getNewLevelID() ... ");

    sqlite3x::sqlite3_connection con("../data/levels.db");
    try
    {
        sqlite3x::sqlite3_command cmd ( con, "select max(id)+1 from level;" );
        levelid=cmd.executeint();
    }
    catch( std::exception e ) {
        LOG("\nFatal : %s", e.what() );
    }
    con.close();

    LOG(" done.");

    return( levelid );

}



int CLevelInfo::getRandomLevel( int gameType ) {

    int max = getMaxLevels( gameType );

    return( getLevelID( gameType, rand()%max + 1 ) );
}


