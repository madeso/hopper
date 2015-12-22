

/// configuration for controls


#ifndef _CINPUTCONFIG_
#define _CINPUTCONFIG_

//#include "inputmanager.h" // don't. really.

#include <string>
#include <vector>


class CInputConfiguration {

public:

    typedef enum IC_TYPE {
        PRESSED, RELEASED, AXIS
    };

    typedef enum IC_METHOD {
        KEY, MOUSE_BUTTON, JOYSTICK_BUTTON, MOUSE_AXIS, JOYSTICK_AXIS, JOYSTICK_AXIS_RELATIVE
    };


    struct SInputActionConfig {
        std::string name; // action name
        std::string method_name; // keyname, axis or button name
        bool configurable;    // whether it can be changed by the configuration interface
        int id;
        IC_TYPE type;   // pressed, released or axis
        IC_METHOD method; // buttons ( keys, mouse, joystick ) or axes ( mouse, joystick )
        int periph_num;  // for joystick : 0 for keyboard or mouse
        int periph_value; // keycode, button number
        int periph_value2; // secondary keycode for axis controlled with keys
        bool reversed;  // reversed control
        int minvalue; // minimum axis value
        int maxvalue;
        int step;
    };


    CInputConfiguration( const int confnum, int aNumberOfActions );
    ~CInputConfiguration();

    void save();
    void apply(bool onlyConfigurables );

    int getNumberOfActions();
    SInputActionConfig& getInputActionConfig( int action_id );
    void setInputActionConfig( SInputActionConfig& config );

private:


    void load();
    std::vector<SInputActionConfig> mActionConfiguration;
    int mConfigNum;
    std::string mName;
    int mNumberOfActions;

};

/*
    bool setInputAction(
        int action_id,
        IC_TYPE type,
        IC_METHOD method,
        int periph_num,
        int periph_value,
        int periph_value2,
        bool reversed,
        int parameter );

    bool getInputAction(
        int action_id,
        IC_TYPE& type,
        IC_METHOD& method,
        int& periph_num,
        int& periph_value,
        int& periph_value2,
        bool& reversed,
        int& parameter );
*/
#endif

