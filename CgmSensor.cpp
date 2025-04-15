#include "CgmSensor.h"

CgmSensor::CgmSensor(){

    valueTracker = 0;

    simulatedBgValues = {4.7, 7.2, 5.1, 8.3, 6.6, 4.0, 9.4, 6.2, 4.5, 8.9,
                         5.8, 3.9, 9.1, 7.7, 5.2, 8.0, 6.8, 4.8, 9.0, 6.1,
                         7.4, 5.0, 9.7, 4.2, 7.1, 6.0, 8.4, 5.4, 7.9, 6.3,
                         10.1, 8.1, 4.9, 7.5, 5.6, 8.6, 6.4, 7.0, 5.7, 9.3,
                         4.3, 6.7, 9.5, 7.6, 5.3, 8.7, 4.6, 6.5, 7.3, 9.8,
                         8.2, 4.1, 5.5, 6.9, 9.9, 8.8, 7.8, 5.1, 4.4, 6.1,
                         7.2, 4.0, 5.2, 8.3, 6.2, 9.6, 7.4, 3.7, 8.5, 4.7};
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
