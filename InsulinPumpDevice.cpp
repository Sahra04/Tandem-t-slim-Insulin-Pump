#include "InsulinPumpDevice.h"

InsulinPumpDevice::InsulinPumpDevice(double currentBloodGlucose)
{
    currentBG = currentBloodGlucose;
    userProfileManager = new UserProfileManager();
    cgmMode = true;
    controlIQMode = false;
}

InsulinPumpDevice::~InsulinPumpDevice(){
    delete userProfileManager;
}

double InsulinPumpDevice::getCurrentBG(){
    return currentBG;
}

void InsulinPumpDevice::setCurrentBG(double currentBloodGlucose){
    currentBG = currentBloodGlucose;
}

UserProfileManager* InsulinPumpDevice::getUserProfileManager(){
    return userProfileManager;
}

bool InsulinPumpDevice::getCgmMode(){
    return cgmMode;
}
void InsulinPumpDevice::setCgmMode(bool val){
    cgmMode = val;
    std::cout << "CGM Mode: " << cgmMode << std::endl;
}
bool InsulinPumpDevice::getControlIQMode(){
    return controlIQMode;
}
void InsulinPumpDevice::setControlIQMode(bool val){
    controlIQMode = val;
    std::cout << "Control IQ Mode: " << controlIQMode << std::endl;
}
