#ifndef INSULINPUMPDEVICE_H
#define INSULINPUMPDEVICE_H

#include "UserProfileManager.h"
#include "Battery.h"
#include "Insulincartridge.h"


class InsulinPumpDevice
{
public:
    InsulinPumpDevice(double currentBloodGlucose);
    ~InsulinPumpDevice();

    void deliverInsulin(double amount);
    void deliverBolus();
    void calculateInsulin();
    void calculateInsulinOnBoard(int timepassed);
    double calculateBolus(int carbIntake, double currentBG, int currentTime);

    //getters
    void setControlIQMode(bool val);
    void setCurrentBG(double currentBloodGlucose);
    void setCgmMode(bool val);

    //setters
    UserProfileManager* getUserProfileManager();
    bool getCgmMode();
    double getCurrentBG();
    bool getControlIQMode();



private:
    double currentBG;
    bool cgmMode;
    bool controlIQMode;
    double insulinOnBoard;
    double totalInsulinTaken;


    UserProfileManager* userProfileManager;
    Battery* battery;
    InsulinCartridge* insulinCartidge;
};

#endif // INSULINPUMPDEVICE_H
