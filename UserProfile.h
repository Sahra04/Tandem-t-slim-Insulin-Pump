#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <iostream>
#include <string>

using namespace std;

class UserProfile
{
    public:
        UserProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
                    double quickBolusUnits, int insulinDuration, bool isActivated);

        //getters:
        string getProfileName(){return profileName;}
        double getBasalRate(){return basalRate;}
        int getCarbRatio(){return carbRatio; }
        int getCorrectionFactor(){return correctionFactor; }
        double gettargetBGlevel(){return targetBGLevel; }
        double getquickBolusUnits(){return quickBolusUnits; }
        bool getIsActivated(){return isActivated; }
        int getInsulinDuration() {return insulinDuration; }


        //setter:
        void setProfile(string nProfileName, double nBasalRate, int nCarbRatio, int nCorrectionFactor, double nTargetBGLevel,
                        double nQuickBolusUnits, int nInsulinDuration, bool nIsActivated){
          profileName = nProfileName;
          basalRate = nBasalRate;
          carbRatio = nCarbRatio;
          correctionFactor = nCorrectionFactor;
          targetBGLevel = nTargetBGLevel;
          quickBolusUnits = nQuickBolusUnits;
          isActivated = nIsActivated;
          insulinDuration = nInsulinDuration;
          std::cout << "UserProfile: setProfile" << std::endl;

        };




    private:
        string profileName;
        double basalRate;
        int carbRatio;
        int correctionFactor;
        double targetBGLevel;
        double quickBolusUnits;
        bool isActivated;
        int insulinDuration;

};

#endif // USERPROFILE_H
