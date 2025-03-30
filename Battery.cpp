#include "Battery.h"

#include <string>
#include <iostream>

Battery::Battery(){

    batteryLevel = 50;

}

int Battery::getBatteryLevel(){

    return batteryLevel;

}

void Battery::rechargeBattery(){

    batteryLevel = 100;

}

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
