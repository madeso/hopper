
/// tutorial part interface

#ifndef _ITUTPART_
#define _ITUTPART_

#include "delay.h"
#include "ship.h"
#include "PlayerControlledShipControllers.h"

    enum { TUTPART_NOTHING, TUTPART_DONE };

 class ITutPart {
    public:
        virtual ~ITutPart() {};
        virtual int run()=0;
        virtual void init()=0;
        virtual void nextPart()=0;
        virtual bool tutPartOk()=0;

        //virtual bool getThrustEnabled()=0;
        //virtual bool getYawEnabled()=0;
        //virtual bool getPitchEnabled()=0;
        //virtual bool getRollEnabled()=0;

        virtual CShipController* getShipController()=0;

        virtual std::vector<std::string> getMessage()=0;

/*
    public:
        virtual ~ITutPart() {};
        virtual int run() {};
        virtual void init() {};
        virtual void nextPart() {};
        virtual bool tutPartOk() {};

        virtual bool getThrustEnabled() {};
        virtual bool getYawEnabled() {};
        virtual bool getPitchEnabled() {};
        virtual bool getRollEnabled() {};
*/
 };



#endif

