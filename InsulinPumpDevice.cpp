#include "InsulinPumpDevice.h"

InsulinPumpDevice::InsulinPumpDevice(double currentBloodGlucose)
{
    currentBG = currentBloodGlucose;
    userProfileManager = new UserProfileManager();
    insulinCartridge = new InsulinCartridge();
    battery = new Battery();
    cgmMode = true;
    controlIQMode = false;
}

InsulinPumpDevice::~InsulinPumpDevice(){
    delete userProfileManager;
    delete insulinCartridge;
    delete battery;
}

void InsulinPumpDevice::deliverInsulin(double amount){


    //THOUGHT (confirm with team): instead of amount, have an attrribute that is insulinTodeliever
    //this can change depending on the case; basal, bolus, quick


    //keep track of insulin taken
    totalInsulinTaken += amount;

    //check if insulin level is low
    if(insulinCartridge->getInsulinLevel() - amount <= 0){
        cout<<"#InsulinPumpDevice/deliverInsulin; low insulin level alert"<<endl;
    }

    //deplete insulin Cartidge
    insulinCartridge->depleteInsulin(amount);

}

void InsulinPumpDevice::deliverBolusDefault(int time, double amount){

    cout << "#InsulinPumpDevice/deliverBolusDefault" << endl;

    // update currentBG
    currentBG -= (userProfileManager->getActiveProfile()->getCorrectionFactor() * amount);

    cout << "#InsulinPumpDevice/deliverBolusDefault  currentBG: "<< currentBG << endl;

    // check if BG too low/high

    // deplete insulin from cartridge
    insulinCartridge->depleteInsulin(amount);

    cout << "#InsulinPumpDevice/deliverBolusDefault  after depleting cartridge" << endl;

    // add to timeInsulinMap
    insulinOnBoardMap.insert({time,{{amount, userProfileManager->getActiveProfile()->getInsulinDuration()}}});
    calculateInsulinOnBoard(time);

    cout << "#InsulinPumpDevice/deliverBolusDefault  after adding to map" << endl;


}

void InsulinPumpDevice::deliverBolusExtended(int time, double amount, int immediateBolusPercentage, int distributionDuration){

    cout << "#InsulinPumpDevice/deliverBolusExtended" << endl;
    // Give immediate bolus
    double immediateBolus = amount * (immediateBolusPercentage/100);
    currentBG -= (userProfileManager->getActiveProfile()->getCorrectionFactor() * immediateBolus);

    // Extended:
    int extendedBolusPercentage = 1 - (immediateBolusPercentage/100);

    double extendedBolus = amount * extendedBolusPercentage;

    double bolusPerHour = extendedBolus / distributionDuration;

    // USE bolusPerHour AND distributionDuration TO DELIVER OVER TIME


}

double InsulinPumpDevice::calculateBolus(int carbIntake, double currentBG, int currentTime){

     cout<<"#InsulinPD/calculatedBolus; carbIntake: "<<carbIntake<<endl;
     cout<<"#InsulinPD/calculatedBolus; currentBG: "<<currentBG<<endl;
     cout<<"#InsulinPD/calculatedBolus; currentTime: "<<currentTime<<endl;

    double foodBolus;
    double correctionBolus;
    double totalBolus;
    double finalBolus;

    //food bolus
    foodBolus = carbIntake/userProfileManager->getActiveProfile()->getCarbRatio();
    //correction bolus
    correctionBolus = (currentBG - userProfileManager->getActiveProfile()->gettargetBGlevel())/
                        userProfileManager->getActiveProfile()->getCorrectionFactor();
    //total required bolus
    totalBolus = foodBolus + correctionBolus;

    //final bolus calculated based IOB
    calculateInsulinOnBoard(currentTime);
    finalBolus = totalBolus - insulinOnBoard;

    cout<<"#InsulinPD/calculatedBolus; finalBolus: "<<finalBolus<<endl;

    return finalBolus;

}


void InsulinPumpDevice:: calculateInsulin(){

}

void InsulinPumpDevice::calculateInsulinOnBoard(int currentTime){
    insulinOnBoard = 0;

    // Loop through all key-value pairs in map
    // Key: time
    // Value: vector{struct{amount, duration}}

    for (const auto& [time, insulinDeliveredDuration] : insulinOnBoardMap) {
        //Getting struct:
        const auto& insulinDeliveredDurationStruct = insulinDeliveredDuration[0];

        std::cout << "Key (Time) : " << time << "Value (Insulin Delivered) : " << insulinDeliveredDurationStruct.insulinDelivered << ", Value (Duration) : " << insulinDeliveredDurationStruct.bolusInsulinDuration << std::endl;

        insulinOnBoard +=  insulinDeliveredDurationStruct.insulinDelivered * (1 - ((currentTime - time )/insulinDeliveredDurationStruct.bolusInsulinDuration));
    }


    //insulinOnBoard = totalInsulinTaken*(1 - (timepassed/userProfileManager->getActiveProfile()->getInsulinDuration()));
    //insulinOnBoard = totalInsulinTake*(1 - timepassed/insulin duration)

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
