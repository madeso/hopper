

#include "triggerboxmanager.h"

CTriggerBoxManager::CTriggerBoxManager() {
}

CTriggerBoxManager::~CTriggerBoxManager() {

    mTriggerBoxes.clear();
}


void CTriggerBoxManager::addTriggerBox( CTriggerBox* aTriggerBox ) {

    mTriggerBoxes.push_back( aTriggerBox);

}



void CTriggerBoxManager::removeTriggerBox( CTriggerBox* aTriggerBox ) {

    mTriggerBoxes.remove( aTriggerBox );

}



void CTriggerBoxManager::update() {

    std::list< CTriggerBox* >::iterator it;
    for( it = mTriggerBoxes.begin(); it != mTriggerBoxes.end(); ++it ) {

        (*it)->update();

    }

}
