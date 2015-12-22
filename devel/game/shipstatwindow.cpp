
#include "shipstatwindow.h"

CShipStatWindow::CShipStatWindow() {

    mGUI=DynaGUI::getInstance();
    mWindow=mGUI->createWindow( DWindow::MIN, DWindow::MAX );

    mHullText=mWindow->addText( "Hull : 100%", 80 );
    mHullBar=mWindow->addProgressBar( "Hoper/HullProgressBar", 100 );
    mWindow->newLine();
    mFuelText=mWindow->addText( "Fuel : 100%", 80 );
    mFuelBar=mWindow->addProgressBar( "Hoper/FuelProgressBar", 100 );

}

CShipStatWindow::~CShipStatWindow() {

    mGUI->destroyWindow( mWindow );

}

void CShipStatWindow::setHull(  int hull ) {

    if( hull <0 ) hull=0;
    mHullText->setText("Hull : " + Ogre::StringConverter::toString( hull ) + "%" );
    mHullBar->setPercent(hull);

}

void CShipStatWindow::setFuel(  int fuel ) {

    if (fuel<0 ) fuel=0;
    mFuelText->setText("Fuel : " + Ogre::StringConverter::toString( fuel ) + "%" );
    mFuelBar->setPercent(fuel);

}
