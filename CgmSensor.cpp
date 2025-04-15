#include "CgmSensor.h"

CgmSensor::CgmSensor(){

    valueTracker = 0;
    simulatedBgValues = {6.5, 6.8, 7.0, 7.2, 7.5, 11.0, 8.0, 8.2, 8.5, 8.8,
                         9.0, 8.9, 6.8, 3.3, 3.0, 2.9, 7.5, 7.8, 8.0, 8.2,
                         8.5, 8.8, 6.0, 6.7, 6.9, 10.0, 7.0, 11.0, 12.0, 6.0,
                         6.5, 7.0, 7.2, 7.5, 11.0, 8.0, 8.2, 8.5, 8.8, 3.0,
                         2.9, 7.5, 7.8, 8.0, 8.2, 8.5, 8.8, 6.0, 6.7, 6.9,
                         9.0, 8.9, 6.8, 3.3, 3.0, 2.9, 7.5, 7.8, 7.8, 8.0,
                         6.0, 6.7, 6.9, 10.0, 7.0, 11.0, 12.0, 6.0, 2.8, 8.0}; 
    readBgValues = {};
}

// reads in next simulated reading
double CgmSensor::getNextReading(){
    if (valueTracker >= simulatedBgValues.size()) {
        valueTracker = 0;
    }
    cout<<"#CgmSensor/getNextReading: "<< simulatedBgValues[valueTracker] << endl;
    readBgValues.push_back(simulatedBgValues[valueTracker]);
    return simulatedBgValues[valueTracker++];
    
}

// adds BG value to the read vector
void CgmSensor::addToRead(double newBgValue){
    cout<<"#CgmSensor/addToRead newBgValue: "<< newBgValue << endl;
    readBgValues.push_back(newBgValue);
}
    
/*
VeryLow:    0.0 - 3.8 --> Hypoglycemia
Low:        3.9 - 6.2
Normal:     6.3 - 8.9
High:       9.0 - 10.0 
VeryHigh:   10.1 - 15.0 --> Hyperglycemia
*/
