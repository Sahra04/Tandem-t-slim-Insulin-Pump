#include "userprofilemanager.h"


//SB: Still in PROGRESS!

UserProfileManager::UserProfileManager()
{

}


//when profile is created, it is added to the vector
void UserProfileManager::createProfile(string profileName, double basalRate, int carbRatio, int correctionFactor, double targetBGLevel,
                                       double quickBolusUnits, bool isActivated){

   UserProfile* newUserProfile = new UserProfile(profileName, basalRate, carbRatio, correctionFactor, targetBGLevel,
                                         quickBolusUnits, isActivated);

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
                                       double quickBolusUnits, bool isActivated){

    for(int i = 0; i < userProfiles.size(); i++){
        if(userProfiles[i]->getProfileName().compare(profileName) == 0){
                    userProfiles[i]->setProfile(profileName, basalRate, carbRatio, correctionFactor, targetBGLevel,
                                                quickBolusUnits, isActivated);

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
