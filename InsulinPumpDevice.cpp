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

    cout << "#InsulinPumpDevice/deliverBolusDefault" << endl;

    // update currentBG
    currentBG -= (userProfileManager->getActiveProfile()->getCorrectionFactor() * amount);
    // cgmSensor->getValueTracker();
    // simulatedBGValues.insert(simulatedBGValues.begin() + cgmSensor->getValueTracker(), currentBG );

    cout << "#InsulinPumpDevice/deliverBolusDefault  currentBG: "<< currentBG << endl;


    //cgmSensor->setSimulatedBgValues(currentBG);
    //updating stats
    updateInsulinDelivery(time, amount);

    //calculateInsulinOnBoard(time);

}

void InsulinPumpDevice::updateInsulinDelivery(int time, double amount){

    // Add to read vector
    //cgmSensor->addToRead(currentBG);
    //cgmSensor->setSimulatedBgValues(currentBG);

    // check if BG too low/high

    // deplete insulin from cartridge
    insulinCartridge->depleteInsulin(amount);

    cout << "#InsulinPumpDevice/updateInsulinDelivery| after depleting cartridge" << endl;



        // Deplete insulin from cartridge
       insulinCartridge->depleteInsulin(amount);

      // Check if time entry already exists
       auto it = insulinOnBoardMap.find(time);
       if (it != insulinOnBoardMap.end()) {
          // Key exists - add to existing amount (keep same duration)
          it->second[0].insulinDelivered += amount;
          cout<<"InsulinPumpDevice/updateInsulinDelivery|" << " KEY EXISTS"<<endl;
       } else {
          // Key doesn't exist - create new entry
          insulinOnBoardMap.insert({time, {{amount, userProfileManager->getActiveProfile()->getInsulinDuration()}}});
          cout<<"InsulinPumpDevice/updateInsulinDelivery|" << " KEY DOES NOT EXIST"<<endl;
       }

       calculateInsulinOnBoard(time);


     // add to timeInsulinMap
     // insulinOnBoardMap.insert({time,{{amount, userProfileManager->getActiveProfile()->getInsulinDuration()}}});
     //calculateInsulinOnBoard(time);

    cout << "#InsulinPumpDevice/updateInsulinDelivery| after adding to map" << endl;

}

//we set to 0 when extended bolus is completed
//extendedPhase == 1: immediate bolus portion -- think of it as phase 1 of extended bolus
//extendedPhase == 2: rest of duration boluses -- think of it as phase 2 of extended bolus
void InsulinPumpDevice::deliverBolusExtended(int time){

    cout << "#InsulinPumpDevice/deliverBolusExtended| insulinAmountForExtended: " <<insulinAmountForExtended<<endl;
    cout << "#InsulinPumpDevice/deliverBolusExtended| immediateBolusPercentage: " <<immediateBolusPercentage<<endl;
    cout << "#InsulinPumpDevice/deliverBolusExtended| distributionDuration: " <<distributionDuration<<endl;
    cout << "#InsulinPumpDevice/deliverBolusExtended| extendedBolusPhase: " <<extendedBolusPhase<<endl;

   // phase 1: immediate bolus portion
   if(extendedBolusPhase == 1){

        cout << "#InsulinPumpDevice/deliverBolusExtended| PHASE 1 IN EFFECT "<<endl;

        double immediateBolus = insulinAmountForExtended * (double(immediateBolusPercentage)/double(100));

        cout << "#InsulinPumpDevice/deliverBolusExtended| immediateBolus: " <<immediateBolus<<endl;

        cout << "#InsulinPumpDevice/deliverBolusExtended|currentBG BEFORE initial extendedbolus: " <<currentBG<<endl;

        //updating stats
        updateInsulinDelivery(time, immediateBolus);

        currentBG -= (userProfileManager->getActiveProfile()->getCorrectionFactor() * insulinOnBoard);

        cout << "#InsulinPumpDevice/deliverBolusExtended|currentBG AFTER initial extendedbolus: " <<currentBG<<endl;

        extendedBolusPhase = 2;

        return;

   }
   // phase 2: extended bolus portion
   else if (extendedBolusPhase == 2){
        cout << "#InsulinPumpDevice/deliverBolusExtended| PHASE 2 IN EFFECT "<<endl;

        extendedBolusTime += 5;

        cout << "#InsulinPumpDevice/deliverBolusExtended| extendedBolusTime: " <<extendedBolusTime<<endl;

        double extendedBolusPercentage = 1 - (double(immediateBolusPercentage)/double(100));

        cout << "#InsulinPumpDevice/deliverBolusExtended| extendedBolusPercentage: " <<extendedBolusPercentage<<endl;

        double extendedBolus = insulinAmountForExtended * extendedBolusPercentage;

        cout << "#InsulinPumpDevice/deliverBolusExtended| extendedBolus: " <<extendedBolus<<endl;

        bolusPerHour = extendedBolus / distributionDuration;

        cout << "#InsulinPumpDevice/deliverBolusExtended| bolusPerHour: " <<bolusPerHour<<endl;

       if (extendedBolusTime == 60){
           cout << "#InsulinPumpDevice/deliverBolusExtended| currentBG BEFORE extendedBolus: " <<currentBG<<endl;
           extendedBolusHour += 1;
           extendedBolusTime = 0;

           //updating stats
           updateInsulinDelivery(time, bolusPerHour);

           currentBG -= (userProfileManager->getActiveProfile()->getCorrectionFactor() * insulinOnBoard);

           cout << "#InsulinPumpDevice/deliverBolusExtended| currentBG AFTER extendedBolus: " <<currentBG<<endl;
           cout << "#InsulinPumpDevice/deliverBolusExtended| extendedBolusHour: " <<extendedBolusHour<<endl;


           //extended bolus is completed
           if(extendedBolusHour == distributionDuration){
               extendedBolusHour = 0;
               extendedBolusPhase = 0;
               extendedBolusTime = 0;
               bolusPerHour = 0;
               cout << "#InsulinPumpDevice/deliverBolusExtended| RESET "<<endl;
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

    //check edge case when final Bolus is negative
    if(finalBolus <  0){
        finalBolus = 0;
    }

    cout<<"#InsulinPD/calculatedBolus; finalBolus: "<<finalBolus<<endl;

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

        cout<<"InsulinPumpDevice/calculateInsulinOnBoard| time: " <<timeDelivered<<endl;
        cout<<"InsulinPumpDevice/calculateInsulinOnBoard| duration: " <<delivery.bolusInsulinDuration<<endl;
        cout<<"InsulinPumpDevice/calculateInsulinOnBoard| amount: " <<delivery.insulinDelivered<<endl;


        double remainingIOB = delivery.insulinDelivered * (1 - (double(currentTime - timeDelivered) / (delivery.bolusInsulinDuration * 60.0)));
       cout<<"InsulinPumpDevice/calculateInsulinOnBoard| CURRENTTIME: " <<currentTime<<endl;
       cout<<"InsulinPumpDevice/calculateInsulinOnBoard| TIMEDELIVERED: " <<timeDelivered<<endl;


        if (remainingIOB > 0) {
            insulinOnBoard += remainingIOB;
            cout<<"InsulinPumpDevice/calculateInsulinOnBoard| each insulinOnBoard: " <<insulinOnBoard<<endl;
            ++it;
        } else {
            it = insulinOnBoardMap.erase(it);
         }
      }

    cout<<"InsulinPumpDevice/calculateInsulinOnBoard| SUM insulinOnBoard: " <<insulinOnBoard<<endl;
}

void InsulinPumpDevice::deliverBasal(int time){
    // double unitsToDeliver = userProfileManager->getActiveProfile()->getBasalRate();
    //currentBG -= (userProfileManager->getActiveProfile()->getCorrectionFactor() * unitsToDeliver);
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
    std::cout << "CGM Mode: " << cgmMode << std::endl;
}
bool InsulinPumpDevice::getControlIQMode(){
    return controlIQMode;
}
void InsulinPumpDevice::setControlIQMode(bool val){
    controlIQMode = val;
    std::cout << "Control IQ Mode: " << controlIQMode << std::endl;
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
