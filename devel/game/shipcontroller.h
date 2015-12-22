


#ifndef _CSHIPCONTROLLER_
#define _CSHIPCONTROLLER_

#include "ship.h"

/**
 *   Base controller class for the ship
 *
 */

class CShipController {

    public:
    CShipController( CShip* ship ) { mShip=ship; }
    virtual ~CShipController() {};
    virtual void run()=0;

    protected:
    CShip* mShip;

    private:
    CShipController();


};


/**
 * ship controller that does nothing ( ship dead or out of fuel )
 *
 */
class CEmptyShipController: public CShipController {

    public:
    CEmptyShipController( CShip* ship ): CShipController( ship ) {};

    void run() {};

};



#endif



