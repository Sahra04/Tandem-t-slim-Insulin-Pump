#include "Insulincartridge.h"

// Constructor
InsulinCartridge::InsulinCartridge(){
    insulinLevel = 300;
}

// Getter
double InsulinCartridge::getInsulinLevel(){
    return insulinLevel;
}

// Refills catridge to full insulin
void InsulinCartridge::refillInsulin(){
    insulinLevel = 300;
}

// Depletes catridge by specified amount
void InsulinCartridge::depleteInsulin(double amountToDeplete){
    insulinLevel -= amountToDeplete;
}
