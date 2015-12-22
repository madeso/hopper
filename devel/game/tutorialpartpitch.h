
/// pitch tutorial


#include "tutorialpartbox.h"
#include "Ogre.h"


class CTutorialpartPitch:public CTutorialpartBox {

    public:

    CTutorialpartPitch(CShip *aShip);
    ~CTutorialpartPitch();
    void init();


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


