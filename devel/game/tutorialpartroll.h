
/// gravity tutorial


#include "tutorialpartbox.h"
#include "Ogre.h"


class CTutorialpartRoll:public CTutorialpartBox {

    public:

    CTutorialpartRoll(CShip *aShip);
    ~CTutorialpartRoll();
    void init();


   private:
    bool getThrustEnabled();
    bool getYawEnabled();
    bool getPitchEnabled();
    bool getRollEnabled();

    std::vector<std::string> getMessage();

    std::vector<std::string> mMessage;

};

