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

void InsulinPumpDevice::deliverInsulin(double amount){


    //THOUGHT (confirm with team): instead of amount, have an attrribute that is insulinTodeliever
    //this can change depending on the case; basal, bolus, quick


    //keep track of insulin taken
    totalInsulinTaken += amount;

    //check if insulin level is low
    if(insulinCartidge->getInsulinLevel() - amount <= 0){
        cout<<"#InsulinPumpDevice/deliverInsulin; low insulin level alert"<<endl;
    }

    //deplete insulin Cartidge
    insulinCartidge->depleteInsulin(amount);

}

void InsulinPumpDevice::deliverBolus(){



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
    finalBolus = totalBolus - insulinOnBoard;

    cout<<"#InsulinPD/calculatedBolus; finalBolus: "<<finalBolus<<endl;

    return finalBolus;

}


void InsulinPumpDevice:: calculateInsulin(){

}

void InsulinPumpDevice::calculateInsulinOnBoard(int timepassed){




    insulinOnBoard = totalInsulinTaken*(1 - (timepassed/userProfileManager->getActiveProfile()->getInsulinDuration()));
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
