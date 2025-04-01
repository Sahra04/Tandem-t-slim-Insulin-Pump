#ifndef USERPROFILEMANAGER_H
#define USERPROFILEMANAGER_H

#include <Userprofile.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class UserProfileManager
{
    public:
        UserProfileManager();

        void createProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
                           double quickBolusUnits, bool isActivated);
        void deleteProfile(const string profileName);
        void updateProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
                           double quickBolusUnits, bool isActivated);
        UserProfile* getprofile(const string profileName);

        vector<UserProfile*> userProfiles;





};

#endif // USERPROFILEMANAGER_H
