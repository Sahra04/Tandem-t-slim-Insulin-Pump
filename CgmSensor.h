#ifndef CGMSENSOR_H
#define CGMSENSOR_H

#include <iostream>
#include <vector>

using namespace std;

class CgmSensor {
private:
    int valueTracker;
    vector<double> simulatedBgValues;

public:
    CgmSensor();
    double getNextReading();
    int getValueTracker();
    void setSimulatedBgValues(double newValue);    
};
#endif // CGMSENSOR_H
