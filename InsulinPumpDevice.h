#ifndef INSULINPUMPDEVICE_H
#define INSULINPUMPDEVICE_H

#include "UserProfileManager.h"
#include "Battery.h"
#include "Insulincartridge.h"
#include <unordered_map>
#include <vector>


class InsulinPumpDevice
{
public:
    InsulinPumpDevice(double currentBloodGlucose);
    ~InsulinPumpDevice();

    void deliverInsulin(double amount);
    void deliverBolusDefault(int time, double amount);
    void deliverBolusExtended(int time, double amount, int immediateBolusPercentage, int distributionDuration);
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

    struct insulinDeliveredDuration {
        double insulinDelivered;
        int bolusInsulinDuration;
    };

    UserProfileManager* userProfileManager;
    Battery* battery;
    InsulinCartridge* insulinCartridge;
    std::unordered_map<int, vector<insulinDeliveredDuration>> insulinOnBoardMap;
};

#endif // INSULINPUMPDEVICE_H
