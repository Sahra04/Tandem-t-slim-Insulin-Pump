#ifndef BATTERY_H
#define BATTERY_H

#include <iostream>
class Battery {
private:
    int batteryLevel;

public:
    Battery();
    int getBatteryLevel();
    void rechargeBattery();
    void depleteBattery();

};
#endif // BATTERY_H
