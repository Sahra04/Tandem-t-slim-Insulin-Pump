#include "CgmSensor.h"

CgmSensor::CgmSensor(){

    valueTracker = 0;

    simulatedBgValues = {4.7, 5.1, 5.7, 6.1, 6.6, 7.0, 7.2, 7.1, 6.8, 6.3, 5.8, 5.6, 5.3, 5.1, 5.3,
                         5.7, 6.2, 6.4, 6.7, 8.1, 8.3, 8.9, 9.4, 10.4, 10.8, 11.4, 10.8, 10.2, 9.8, 9.2, 8.4, 8.0, 7.9, 7.4,
                         7.1, 6.9, 6.4, 6.1, 5.7, 5.5, 5.1, 4.8, 4.6, 4.2, 3.6, 3.3, 2.8,
                         2.5, 2.0, 2.4, 2.6, 3.1, 3.6, 4.0, 4.5, 4.7, 5.0, 5.1, 5.6, 6.2, 6.7,
                         6.9, 7.4, 7.7, 8.3, 8.6, 8.4, 8.8, 9.1, 9.4, 9.3, 8.7, 8.4, 8.0, 7.6,
                         7.3, 6.9, 6.5, 6.2, 5.8, 5.4, 5.1, 4.7, 4.5, 5.0, 5.3, 5.8, 6.3, 6.4,
                         6.9, 7.3, 7.8, 8.4, 8.9, 8.6, 8.2, 7.9, 7.3, 7.0, 6.7, 6.2, 5.9, 5.8};

}

// reads in next simulated reading
double CgmSensor::getNextReading(){
    if (valueTracker >= simulatedBgValues.size()) {
        valueTracker = 0;
    }
    cout<<"#CgmSensor/getNextReading: "<< simulatedBgValues[valueTracker] << endl;
    //readBgValues.push_back(simulatedBgValues[valueTracker]);
    return simulatedBgValues[valueTracker++];
    
}

int CgmSensor::getValueTracker(){
    return valueTracker;
}

void CgmSensor::setSimulatedBgValues(double newValue){
    //initial check to make sure BG isn't negative
    if(newValue < 0){
        newValue = 0;
    }

    simulatedBgValues.insert(simulatedBgValues.begin() + valueTracker, newValue);
}
/*
VeryLow:    0.0 - 3.8 --> Hypoglycemia
Low:        3.9 - 6.2
Normal:     6.3 - 8.9
High:       9.0 - 10.0 
VeryHigh:   10.1 - 15.0 --> Hyperglycemia
*/
