#ifndef INSULINCARTRIDGE_H
#define INSULINCARTRIDGE_H

#include <iostream>

class InsulinCartridge
{
    private:
        double insulinLevel;
    public:
        InsulinCartridge();
        int getInsulinLevel();
        void refillInsulin();
        void depleteInsulin(double amountToDeplete);

};

#endif // INSULINCARTRIDGE_H
