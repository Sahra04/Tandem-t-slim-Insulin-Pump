#include "Userprofile.h"

UserProfile::UserProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
            double quickBolusUnits, bool isActivated )
{

    this->profileName = profileName;
    this->basalRate = basalRate;
    this->carbRatio = carbRatio;
    this->correctionFactor = correctionFactor;
    this->targetBGLevel = targetBGLevel;
    this->quickBolusUnits = quickBolusUnits;
    this->isActivated = isActivated;


};
