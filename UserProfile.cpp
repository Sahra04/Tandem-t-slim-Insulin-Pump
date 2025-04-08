#include "UserProfile.h"

UserProfile::UserProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
            double quickBolusUnits, int insulinDuration, bool isActivated)
{

    this->profileName = profileName;
    this->basalRate = basalRate;
    this->carbRatio = carbRatio;
    this->correctionFactor = correctionFactor;
    this->targetBGLevel = targetBGLevel;
    this->quickBolusUnits = quickBolusUnits;
    this->isActivated = isActivated;
    this->insulinDuration = insulinDuration;


};
