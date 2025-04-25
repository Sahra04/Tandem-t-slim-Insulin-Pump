#include "InsulinPumpDevice.h"

InsulinPumpDevice::InsulinPumpDevice(double currentBloodGlucose)
{
    currentBG = currentBloodGlucose;
    userProfileManager = new UserProfileManager();
    insulinCartridge = new InsulinCartridge();
    insulinOnBoard = 0;
    cgmSensor = new CgmSensor();
    battery = new Battery();
    cgmMode = true;
    controlIQMode = false;
    extendedBolusHour =  0;
    extendedBolusTime = 0;
    extendedBolusPhase = 0;
    isStopped = false; 
    bolusPerHour = 0;
    correctionBolusInitiated = false;
}

InsulinPumpDevice::~InsulinPumpDevice(){
    delete userProfileManager;
    delete insulinCartridge;
    delete battery;
    delete cgmSensor;
}

void InsulinPumpDevice::deliverBolusDefault(int time, double amount){

    // update currentBG
    currentBG -= (userProfileManager->getActiveProfile()->getCorrectionFactor() * amount);

    //updating stats
    updateInsulinDelivery(time, amount);

}

void InsulinPumpDevice::updateInsulinDelivery(int time, double amount){

    // deplete insulin from cartridge
    insulinCartridge->depleteInsulin(amount);

     // Deplete insulin from cartridge
    insulinCartridge->depleteInsulin(amount);

    // Check if time entry already exists
    auto it = insulinOnBoardMap.find(time);
    if (it != insulinOnBoardMap.end()) {
      // Key exists - add to existing amount
      it->second[0].insulinDelivered += amount;
    } else {
      // Key doesn't exist - create new entry
      insulinOnBoardMap.insert({time, {{amount, userProfileManager->getActiveProfile()->getInsulinDuration()}}});
    }

     calculateInsulinOnBoard(time);

}

//we set to 0 when extended bolus is completed
//extendedPhase == 1: immediate bolus portion -- think of it as phase 1 of extended bolus
//extendedPhase == 2: rest of duration boluses -- think of it as phase 2 of extended bolus
void InsulinPumpDevice::deliverBolusExtended(int time){

   // phase 1: immediate bolus portion
   if(extendedBolusPhase == 1){

        double immediateBolus = insulinAmountForExtended * (double(immediateBolusPercentage)/double(100));

        //updating stats
        updateInsulinDelivery(time, immediateBolus);

        currentBG -= (userProfileManager->getActiveProfile()->getCorrectionFactor() * insulinOnBoard);

        extendedBolusPhase = 2;

        return;

   }
   // phase 2: extended bolus portion
   else if (extendedBolusPhase == 2){

        extendedBolusTime += 5;

        double extendedBolusPercentage = 1 - (double(immediateBolusPercentage)/double(100));

        double extendedBolus = insulinAmountForExtended * extendedBolusPercentage;

        bolusPerHour = extendedBolus / distributionDuration;

       if (extendedBolusTime == 60){
           extendedBolusHour += 1;
           extendedBolusTime = 0;

           //updating stats
           updateInsulinDelivery(time, bolusPerHour);

           currentBG -= (userProfileManager->getActiveProfile()->getCorrectionFactor() * insulinOnBoard);

           //extended bolus is completed
           if(extendedBolusHour == distributionDuration){
               extendedBolusHour = 0;
               extendedBolusPhase = 0;
               extendedBolusTime = 0;
               bolusPerHour = 0;

           }
        }
    }

}


void InsulinPumpDevice::readInBGFromCGM(){
    currentBG = cgmSensor->getNextReading();
}

void InsulinPumpDevice::decreaseBatteryLevel(){
    battery->depleteBattery();

}

double InsulinPumpDevice::calculateBolus(int carbIntake, double currentBG, int currentTime){

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

    //check edge case when final Bolus is negative
    if(finalBolus <  0){
        finalBolus = 0;
    }

    return finalBolus;

}

double InsulinPumpDevice::calculatateCorrectionBolus(double currentBG){
    double correctionBolus = (currentBG - userProfileManager->getActiveProfile()->gettargetBGlevel()) /
                             userProfileManager->getActiveProfile()->getCorrectionFactor();
    return correctionBolus;
}


void InsulinPumpDevice::calculateInsulinOnBoard(int currentTime){

    insulinOnBoard = 0;

    // Loop through all key-value pairs in map
    // Key: time
    // Value: vector{struct{amount, duration}}
    for (auto it = insulinOnBoardMap.begin(); it != insulinOnBoardMap.end(); ) {
        const auto& [timeDelivered, deliveries] = *it;
        const auto& delivery = deliveries[0];

        double remainingIOB = delivery.insulinDelivered * (1 - (double(currentTime - timeDelivered) / (delivery.bolusInsulinDuration * 60.0)));

        if (remainingIOB > 0) {
            insulinOnBoard += remainingIOB;
            ++it;
        } else {
            it = insulinOnBoardMap.erase(it);
         }
      }
}

void InsulinPumpDevice::deliverBasal(int time){
    updateInsulinDelivery(time, currentBasalRate);
}


double InsulinPumpDevice::getInsulinOnBoard(){
    return insulinOnBoard;
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
}
bool InsulinPumpDevice::getControlIQMode(){
    return controlIQMode;
}
void InsulinPumpDevice::setControlIQMode(bool val){
    controlIQMode = val;
}

Battery* InsulinPumpDevice::getBattery(){
    return battery;
}
InsulinCartridge* InsulinPumpDevice::getInsulinCartridge(){
    return insulinCartridge;
}

CgmSensor* InsulinPumpDevice::getCgmSensor(){
    return cgmSensor;
}

int InsulinPumpDevice::getExtendedBolusPhase(){
    return extendedBolusPhase;
}

double InsulinPumpDevice::getInsulinAmountForExtended(){
    return insulinAmountForExtended;

}
int InsulinPumpDevice::getImmediateBolusPercentage(){
    return immediateBolusPercentage;
}

int InsulinPumpDevice::getDistributionDuration(){
    return distributionDuration;

}

void InsulinPumpDevice::setExtendedBolusPhase(int newExtendedBolusPhase){
    extendedBolusPhase = newExtendedBolusPhase;

}
void InsulinPumpDevice::setInsulinAmountForExtended(double newInsulinAmountForExtended){
    insulinAmountForExtended = newInsulinAmountForExtended;

}
void InsulinPumpDevice::setImmediateBolusPercentage(int newImmediateBolusPercentage){
    immediateBolusPercentage = newImmediateBolusPercentage;

}
void InsulinPumpDevice::setDistributionDuration(int newDistributionDuration){
    distributionDuration = newDistributionDuration;

}

void InsulinPumpDevice::setCurrentBasalRate(double newCurrentBasalRate) {
    currentBasalRate = newCurrentBasalRate;
    
}

double InsulinPumpDevice::getCurrentBasalRate(){
    return currentBasalRate;
}

bool InsulinPumpDevice::getIsStopped(){
    return isStopped;
}

void InsulinPumpDevice::setIsStopped(bool newIsStopped){
   isStopped = newIsStopped;
}

double InsulinPumpDevice::getBolusPerHour(){
    return bolusPerHour;
}

int InsulinPumpDevice::getExtendedBolusTime(){
    return extendedBolusTime;
}

void InsulinPumpDevice::setCorrectionBolusInitiated(bool newCorrectionBolusInitiated){
    correctionBolusInitiated = newCorrectionBolusInitiated;
}
bool InsulinPumpDevice::getCorrectionBolusInitiated(){
    return correctionBolusInitiated;
}
