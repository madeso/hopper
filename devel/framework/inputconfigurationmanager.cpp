

#include "inputconfigurationmanager.h"

#include "sqlite3x.hpp"
#include "inputmanager.h"
#include "logger.h"
#include "optionmanager.h"

CInputConfigurationManager::CInputConfigurationManager() {

    // mInputConfigs.push_back( new CInputConfiguration( mInputConfigs.size(), "Default Mouse" );


    // read configurations from the database
    sqlite3x::sqlite3_connection con("../data/config.db");
    {
        mNumberOfConfigs=con.executeint("select max( confnum )+1 from input_action_conf;" );
    }
    {
        mNumberOfActions=con.executeint("select count(*) from input_action;" );
    }

    con.close();

    LOG( "\n\tfound %d configurations and %d input actions !", mNumberOfConfigs, mNumberOfActions );

    CInputManager::getInstance()->setNumberOfInputActions( mNumberOfActions );

    for( int i=0; i<mNumberOfConfigs; i++ ) {
        mInputConfigs.push_back( new CInputConfiguration( mInputConfigs.size(), mNumberOfActions) );
    }

    LOG("\n\tapplying the default configuration... " );
    if( !COptionManager::getInstance()->getOption( "last_input_config", mLastInputConfig ) )
        mLastInputConfig=0;

    mInputConfigs[mLastInputConfig]->apply(false); // false = apply the whole input actions configuration
    LOG("\ndone.");

}

CInputConfigurationManager::~CInputConfigurationManager() {
    mInputConfigs.clear();
}



/** @brief returns the number of input configurations
  */
int CInputConfigurationManager::getNumberOfConfigs()
{
    return( mNumberOfConfigs );
}


CInputConfiguration* CInputConfigurationManager::getInputConfiguration( int num ) {
    if( num < mNumberOfConfigs )
        return( mInputConfigs[num] );
    else
        return( NULL );
}

int CInputConfigurationManager::getLastInputConfig() {
    return(mLastInputConfig );
}

void CInputConfigurationManager::setLastInputConfig( int lastConfig ) {
    mLastInputConfig = lastConfig;
    COptionManager::getInstance()->setOption( "last_input_config", mLastInputConfig );
}


