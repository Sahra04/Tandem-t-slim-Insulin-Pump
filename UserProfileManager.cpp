#include "UserProfileManager.h"


//SB: Still in PROGRESS!

UserProfileManager::UserProfileManager()
{

}


//when profile is created, it is added to the vector
void UserProfileManager::createProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
                                       double quickBolusUnits, bool isActivated, int insulinDuration){

   UserProfile* newUserProfile = new UserProfile(profileName, basalRate, carbRatio, correctionFactor, targetBGLevel,
                                         quickBolusUnits, isActivated, insulinDuration);

   userProfiles.push_back(newUserProfile);
}


//profile deleted
void UserProfileManager::deleteProfile(const string profileName){

    for(int i = 0; i < userProfiles.size(); i++){
        if(userProfiles[i]->getProfileName().compare(profileName) == 0){
                    userProfiles.erase(userProfiles.begin() + i);

        }
    }


}

//update profile
void UserProfileManager::updateProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
                                       double quickBolusUnits, bool isActivated, int insulinDuration){

    for(int i = 0; i < userProfiles.size(); i++){
        if(userProfiles[i]->getProfileName().compare(profileName) == 0){
                    userProfiles[i]->setProfile(profileName, basalRate, carbRatio, correctionFactor, targetBGLevel,
                                                quickBolusUnits, isActivated, insulinDuration);

        }
    }

}

//return profile
UserProfile* UserProfileManager::getprofile(const string profileName){


    UserProfile* returnUserProfile = NULL;
    for(int i = 0; i < userProfiles.size(); i++){
        if(userProfiles[i]->getProfileName().compare(profileName) == 0){
                    returnUserProfile = userProfiles[i];

        }
    }


    return returnUserProfile;

}
const std::vector<UserProfile*>& UserProfileManager::getAllProfiles() const {
    return userProfiles;
}

void UserProfileManager::setActiveProfile(const std::string& profileName) {
    for (UserProfile* profile : userProfiles) {

        profile->setProfile(profile->getProfileName(), profile->getBasalRate(), profile->getCarbRatio(),
                             profile->getCorrectionFactor(), profile->gettargetBGlevel(),
                             profile->getquickBolusUnits(), profile->getInsulinDuration(), false);
    }
    for (UserProfile* profile : userProfiles) {
        if (profile->getProfileName() == profileName) {
            profile->setProfile(profile->getProfileName(), profile->getBasalRate(), profile->getCarbRatio(),
                                 profile->getCorrectionFactor(), profile->gettargetBGlevel(),
                                 profile->getquickBolusUnits(),profile->getInsulinDuration(), true);
            break;
        }
    }
}
