

#ifndef _CINPUTCONFIGURATION_MANAGER_
#define _CINPUTCONFIGURATION_MANAGER_

#include "singleton.h"
#include "inputconfiguration.h"

class CInputConfigurationManager:public SSingleton<CInputConfigurationManager> {

    public:
    CInputConfigurationManager();
    ~CInputConfigurationManager();
    int getNumberOfConfigs();
    CInputConfiguration* getInputConfiguration( int num );
    int getLastInputConfig();
    void setLastInputConfig( int lastConfig );

    private:
    std::vector<CInputConfiguration*> mInputConfigs;

    int mNumberOfConfigs;
    int mNumberOfActions;
    int mLastInputConfig;

};

#endif


