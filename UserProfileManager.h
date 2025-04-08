#ifndef USERPROFILEMANAGER_H
#define USERPROFILEMANAGER_H

#include <UserProfile.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class UserProfileManager
{
    public:
        UserProfileManager();

        void createProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
                           double quickBolusUnits, bool isActivated, int insulinDuration);
        void deleteProfile(const string profileName);
        void updateProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
                           double quickBolusUnits, bool isActivated, int insulinDuration);
        UserProfile* getprofile(const string profileName);

        const std::vector<UserProfile*>& getAllProfiles() const;
        void setActiveProfile(const std::string& profileName);
        UserProfile* getActiveProfile();

        vector<UserProfile*> userProfiles;

};

#endif // USERPROFILEMANAGER_H
