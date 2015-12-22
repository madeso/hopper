
#ifndef _SHIPSTATWINDOW_
#define _SHIPSTATWINDOW_

#include "dynagui.h"

class CShipStatWindow {

    public:
    CShipStatWindow();
    ~CShipStatWindow();

    void setHull( int hull );
    void setFuel( int fuel );

    private:
    DynaGUI* mGUI;
    DWindow* mWindow;
    DText* mHullText;
    DProgressBar* mHullBar;
    DText* mFuelText;
    DProgressBar* mFuelBar;

};



#endif
