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
    bool getCgmMode();
    void setCgmMode(bool val);
    bool getControlIQMode();
    void setControlIQMode(bool val);


private:
    double currentBG;
    UserProfileManager* userProfileManager;
    bool cgmMode;
    bool controlIQMode;
};

#endif // INSULINPUMPDEVICE_H
