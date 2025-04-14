#ifndef INSULINCARTRIDGE_H
#define INSULINCARTRIDGE_H

#include <iostream>

class InsulinCartridge
{
    public:
        InsulinCartridge();
        double getInsulinLevel();
        void refillInsulin();
        void depleteInsulin(double amountToDeplete);
    private:
        double insulinLevel;
};

#endif // INSULINCARTRIDGE_H
