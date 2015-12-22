

#include "inputconfiguration.h"

#include "inputmanager.h"
#include "sqlite3x.hpp"
#include "logger.h"

CInputConfiguration::CInputConfiguration( int confignum, int aNumberOfActions ) {

    mConfigNum=confignum;

    mNumberOfActions=aNumberOfActions;
    mActionConfiguration.resize( mNumberOfActions );

    LOG("\nLoading input actions...");
    load();
    LOG("done.");

}


CInputConfiguration::~CInputConfiguration() {
    mActionConfiguration.clear();
}


int CInputConfiguration::getNumberOfActions() {
    return( mNumberOfActions );
}

/** @brief returns an input configuration reference (for copy purpose)
  *
  */
CInputConfiguration::SInputActionConfig& CInputConfiguration::getInputActionConfig(int action_id)
{
    return( mActionConfiguration[action_id] );
}

/** @brief copies the input configuration
  *
  */
void CInputConfiguration::setInputActionConfig(SInputActionConfig& config) {

    mActionConfiguration[ config.id ] = config;

}



/** @brief applies the configuration to the inputmanager
  *
  */
void CInputConfiguration::apply( bool onlyConfigurables )
{
    std::vector<SInputActionConfig>::const_iterator it;
    for( it=mActionConfiguration.begin(); it!=mActionConfiguration.end(); ++it ) {

        if( ( onlyConfigurables && (*it).configurable ) || !onlyConfigurables ) {

            //LOG("\nSetting action#%d : %s ...", (*it).id, (*it).name.c_str() );

            CInputManager::getInstance()->setAction(
                (*it).id,
                (*it).type,
                (*it).method,
                (*it).periph_num,
                (*it).periph_value,
                (*it).periph_value2,
                (*it).reversed,
                (*it).minvalue,
                (*it).maxvalue,
                (*it).step
            );
            //LOG(" done.");
        }

    }
}





/** @brief saves the configuration to the database
  *
  */
void CInputConfiguration::save()
{
    try {

        sqlite3x::sqlite3_connection con("../data/config.db");
        //CLogger::getInstance()->log("\nDatabse opened.");

		sqlite3x::sqlite3_transaction trans(con);
        //CLogger::getInstance()->log("\nTransaction begun.");

        std::vector<SInputActionConfig>::const_iterator it;
        for( it=mActionConfiguration.begin(); it!=mActionConfiguration.end(); ++it ) {

            {
                // level parameters
                sqlite3x::sqlite3_command cmd ( con,
                    "update input_action_conf set \
                    method=?, \
                    periph_id=?, \
                    periph_value=?, \
                    periph_value2=?, \
                    reversed=?, \
                    minvalue=?,\
                    maxvalue=?,\
                    step=? \
                    where confnum=? and action_id=?;");
                cmd.bind( 1, (*it).method );
                cmd.bind( 2, (*it).periph_num );
                cmd.bind( 3, (*it).periph_value );
                cmd.bind( 4, (*it).periph_value2 );
                cmd.bind( 5, (*it).reversed );
                cmd.bind( 6, (*it).minvalue );
                cmd.bind( 7, (*it).maxvalue );
                cmd.bind( 8, (*it).step );
                cmd.bind( 9, mConfigNum );
                cmd.bind( 10, (*it).id );
                cmd.executenonquery();
            }

        }

        trans.commit();
        // CLogger::getInstance()->log("\ncommitted\n");

        con.close();
    }


    catch( std::exception &e) {
        CLogger::getInstance()->log("\nException while saving input configuration : %s", e.what());
        throw "Exception while saving input configuration";
    }

}


/** @brief loads an input configuration from the database
  *
  */
void CInputConfiguration::load()
{
    sqlite3x::sqlite3_connection con("../data/config.db");

    {
        // level parameters
        sqlite3x::sqlite3_command cmd ( con,
        "select \
        action_id, \
        action_name, \
        configurable, \
        action_type, \
        method, \
        periph_id, \
        periph_value, \
        periph_value2, \
        reversed, \
        minvalue, \
        maxvalue, \
        step \
        from input_action natural join input_action_conf where confnum=?;" );

        cmd.bind( 1, mConfigNum );
        sqlite3x::sqlite3_reader reader=cmd.executereader();


        while (reader.read() ) {
            int action_id=reader.getint(0);
            mActionConfiguration[action_id].id=action_id;
            mActionConfiguration[action_id].name=reader.getstring(1);
            mActionConfiguration[action_id].configurable=(bool)reader.getint(2);
            mActionConfiguration[action_id].type=(IC_TYPE)reader.getint(3);
            mActionConfiguration[action_id].method=(IC_METHOD)reader.getint(4);
            mActionConfiguration[action_id].periph_num=reader.getint(5);
            mActionConfiguration[action_id].periph_value=reader.getint(6);
            mActionConfiguration[action_id].periph_value2=reader.getint(7);
            mActionConfiguration[action_id].reversed=(bool)reader.getint(8);
            mActionConfiguration[action_id].minvalue=reader.getint(9);
            mActionConfiguration[action_id].maxvalue=reader.getint(10);
            mActionConfiguration[action_id].step=reader.getint(11);
        }

    }

    con.close();

}



