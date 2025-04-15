#include "Insulincartridge.h"

// Constructor
InsulinCartridge::InsulinCartridge(){
    insulinLevel = 300.0;
}

// Getter
double InsulinCartridge::getInsulinLevel(){
    return insulinLevel;
}

// Refills catridge to full insulin
void InsulinCartridge::refillInsulin(){
    insulinLevel = 300.0;
}

// Depletes catridge by specified amount
void InsulinCartridge::depleteInsulin(double amountToDeplete){

    insulinLevel -= amountToDeplete;
    if(insulinLevel <= 0){
        insulinLevel = 0;
    }

    std::cout << "#InsulinCartridge/depleteInsulin  after depleting insulin: "<< insulinLevel << std::endl;

}
