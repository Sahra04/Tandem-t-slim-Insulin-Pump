#ifndef INSULINPUMPDEVICE_H
#define INSULINPUMPDEVICE_H

#include "UserProfileManager.h"
class InsulinPumpDevice
{
public:
    InsulinPumpDevice(double currentBloodGlucose);
    ~InsulinPumpDevice();
    double getCurrentBG();
    void setCurrentBG(double currentBloodGlucose);
    UserProfileManager* getUserProfileManager();

private:
    double currentBG;
    UserProfileManager* userProfileManager;
};

#endif // INSULINPUMPDEVICE_H
