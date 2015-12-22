

#ifndef _TRIGGERBOXMANAGER_
#define _TRIGGERBOXMANAGER_

#include "triggerbox.h"
#include "singleton.h"


class CTriggerBoxManager: public SSingleton<CTriggerBoxManager> {

public:
    CTriggerBoxManager();
    ~CTriggerBoxManager();

    void addTriggerBox( CTriggerBox* aTriggerBox );

    void removeTriggerBox( CTriggerBox* aTriggerBox );

    void update();


private:
    std::list<CTriggerBox*> mTriggerBoxes;

};




#endif
