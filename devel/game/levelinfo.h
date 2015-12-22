

#ifndef _LEVELINFO_
#define _LEVELINFO_

/**
  * CLevelInfo : get information or retrieve a level ID based on the game type ( LANDING, RACING, ... )
  *
  */

// these should match the leveltypes table
#define UNUSED_LEVEL 0
#define LANDING_GAME 1
#define RACING_GAME 2
#define TUTORIAL_GAME 4
#define MISSION_GAME 8
#define ANY_GAME 255

class CLevelInfo {

public:

//    enum EGameType { LANDING = 1, RACING = 2, TUTORIAL = 4, MISSION = 8, ANY = 255 );

    /// returns the number of levels for the given game type
    static int getMaxLevels( int gameType );

    /// returns the level ID based on the given game type and level order
    static int getLevelID( int gameType, int levelOrder );

    /// returns a new level ID
    static int getNewLevelID();

    /// returns a random level ID for the specified game type
    static int getRandomLevel( int gametype );

};


#endif


