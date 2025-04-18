#ifndef CGMSENSOR_H
#define CGMSENSOR_H

#include <iostream>
#include <vector>

using namespace std;

class CgmSensor {
private:
    int valueTracker;
    vector<double> simulatedBgValues;
    //vector<double> readBgValues; // to be plotted on graph

public:
    CgmSensor();
    double getNextReading();
    //void addToRead(double newBgValue);
    int getValueTracker();
    void setSimulatedBgValues(double newValue);    
};
#endif // CGMSENSOR_H
