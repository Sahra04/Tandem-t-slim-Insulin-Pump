#ifndef INSULINPUMPDEVICE_H
#define INSULINPUMPDEVICE_H

#include "UserProfileManager.h"
#include "Battery.h"
#include "Insulincartridge.h"
#include "CgmSensor.h"
#include <unordered_map>
#include <vector>


class InsulinPumpDevice
{
public:
    InsulinPumpDevice(double currentBloodGlucose);
    ~InsulinPumpDevice();

    void deliverBolusDefault(int time, double amount);
    void deliverBolusExtended(int time);
    void calculateInsulinOnBoard(int timepassed);
    double calculateBolus(int carbIntake, double currentBG, int currentTime);
    void readInBGFromCGM();
    void decreaseBatteryLevel();
    void updateInsulinDelivery(int time, double amount);
    void deliverBasal(int time);
    double calculatateCorrectionBolus(double currentBG);


    //setters
    void setControlIQMode(bool val);
    void setCurrentBG(double currentBloodGlucose);
    void setCgmMode(bool val);
    void setExtendedBolusPhase(int newExtendedBolusPhase);
    void setInsulinAmountForExtended(double newInsulinAmountForExtended);
    void setImmediateBolusPercentage(int newImmediateBolusPercentage);
    void setDistributionDuration(int newDistributionDuration);
    void setIsStopped(bool newIsStopped);
    void setCurrentBasalRate(double currentBasalRate);
    void setCorrectionBolusInitiated(bool newCorrectionBolusInitiated);
    //getters
    UserProfileManager* getUserProfileManager();
    Battery* getBattery();
    InsulinCartridge* getInsulinCartridge();
    CgmSensor* getCgmSensor();
    bool getCgmMode();
    double getCurrentBG();
    bool getControlIQMode();
    double getInsulinOnBoard();
    int getExtendedBolusPhase();
    double getInsulinAmountForExtended();
    int getImmediateBolusPercentage();
    int getDistributionDuration();
    bool getIsStopped(); 
    double getBolusPerHour();
    int getExtendedBolusTime();
    double getCurrentBasalRate();
    bool getCorrectionBolusInitiated();
    




private:
    double currentBG;
    bool cgmMode;
    bool controlIQMode;
    double insulinOnBoard;
    double totalInsulinTaken;
    int extendedBolusHour;
    int extendedBolusTime;
    int extendedBolusPhase;
    double insulinAmountForExtended;
    int immediateBolusPercentage;
    int distributionDuration;
    bool isStopped; 
    double bolusPerHour; 
    double currentBasalRate;
    bool correctionBolusInitiated; 


    struct insulinDeliveredDuration {
        double insulinDelivered;
        int bolusInsulinDuration;
    };

    UserProfileManager* userProfileManager;
    Battery* battery;
    InsulinCartridge* insulinCartridge;
    CgmSensor* cgmSensor;
    std::unordered_map<int, vector<insulinDeliveredDuration>> insulinOnBoardMap;
};

#endif // INSULINPUMPDEVICE_H
