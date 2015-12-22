
/// gravity tutorial

// #include "tutpart.h"
#include "tutorialpartbox.h"
#include "Ogre.h"

// #define MAX_DATA 5

class CTutorialpartGravity:public CTutorialpartBox {

    public:

    CTutorialpartGravity(CShip *aShip);
    ~CTutorialpartGravity();
    void init();
    //CShipController* getShipController();

   private:
/*
    bool getThrustEnabled();
    bool getYawEnabled();
    bool getPitchEnabled();
    bool getRollEnabled();
*/
    std::vector<std::string> getMessage();

    std::vector<std::string> mMessage;

};
