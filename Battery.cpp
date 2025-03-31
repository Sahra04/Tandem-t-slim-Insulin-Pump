#include "Battery.h"

#include <string>
#include <iostream>

Battery::Battery(){

    batteryLevel = 50;

}

//returns current battery level
int Battery::getBatteryLevel(){

    return batteryLevel;

}

//recharges battery to full
void Battery::rechargeBattery(){

    batteryLevel = 100;

}

//deplete battery to low
void Battery::depleteBattery(){

    batteryLevel = 30;

}

//for testing battery functions
/*
int main(){

    Battery test;

    std::cout<<test.getBatteryLevel();

    test.rechargeBattery();

    std::cout<<test.getBatteryLevel();

    test.depleteBattery();

    std::cout<<test.getBatteryLevel();

    return 0;
}
*/
