//
//  LPTOpponent.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 08/10/18.
//

#include "LPTOpponent.hpp"

static LPTOpponent *_sharedInstance = NULL;

LPTOpponent* LPTOpponent::getInstance() {
    if (_sharedInstance == NULL) {
        _sharedInstance = new LPTOpponent();
        _sharedInstance->init();
    }
    return _sharedInstance;
}

void LPTOpponent::init() {
}

void LPTOpponent::updateUserProfile(picojson::object playerData) {
    picojson::object object = playerData;
    OPPONENT->setgamePlayed(object[FIELD_TOTAL_GAMES].get<double>());
    OPPONENT->setTotalWins(object[FIELD_TOTAL_WIN].get<double>());
    OPPONENT->setCoins(object[FIELD_TOTAL_COINS].get<double>());
    OPPONENT->setlevel(object[FIELD_GAME_LEVEL].get<double>());
    OPPONENT->setgameXP(object[FIELD_GAME_XP].get<double>());
    
    if(object[FIELD_PROFILE_PICTURE].get<string>() == "" || object[FIELD_PROFILE_PICTURE].get<string>().length() <= 0) {
        OPPONENT->setProfilePic(kPlayerProfileImageName);
    }else {
        OPPONENT->setProfilePic(object[FIELD_PROFILE_PICTURE].get<string>());
    }
}

void LPTOpponent::createProfileWith(const char *jsonString) {
    picojson::value v;
    std::string error = picojson::parse(v, jsonString, jsonString + strlen(jsonString));
    picojson::object object = v.get<picojson::object>();
    OPPONENT->setUniqueName(object[FIELD_UNAME].get<string>());
    OPPONENT->setTotalWins(object[FIELD_TOTAL_WIN].get<double>());
    OPPONENT->setTotalLose(object[FIELD_TOTAL_LOSE].get<double>());
    OPPONENT->setCoins(object[FIELD_TOTAL_COINS].get<double>());
//    OPPONENT->setEmailId(object[FIELD_EMAIL].get<string>());
    OPPONENT->setProfilePic(object[FIELD_PROFILE_PICTURE].get<string>());
}

map<string,string> LPTOpponent::setTableProperties(string roomName,string password,int betAmount,int minUser,int maxuser) {
    map<string,string> table;
    table["LPTRoom_Name"]           = roomName;
    table["LPTRoom_Pass"]           = password;
    table["LPTRoom_Bet_Amount"]     = betAmount;
    table["LPTRoom_Min_User"]       = minUser;
    table["LPTRoom_Max_User"]       = maxuser;
    return table;
}

void LPTOpponent::createProfileForBotWith(string username , string profilePic) {
    
    int totalNumberOfMatches    =   GameUtils::getRandomIntegerNumber(10, 120);
    int totalWins               =   GameUtils::getRandomIntegerNumber(10, totalNumberOfMatches);
    int totalCoins              =   GameUtils::getRandomIntegerNumber(500, 50000);
    int gameLevel               =   GameUtils::getRandomIntegerNumber(1, 3);
    int gameXP                  =   GameUtils::getRandomIntegerNumber(0, 2100);
    
    
    OPPONENT->setUniqueName(username);
    OPPONENT->setgamePlayed(totalNumberOfMatches);
    OPPONENT->setTotalWins(totalWins);
    OPPONENT->setTotalLose(totalNumberOfMatches - totalWins);
    OPPONENT->setCoins(totalCoins);
    OPPONENT->setEmailId("");
    OPPONENT->setProfilePic(profilePic);
    OPPONENT->setlevel(gameLevel);
    OPPONENT->setgameXP(gameXP);
}


void LPTOpponent::removeSharedInstance() {
    if(_sharedInstance) {
        _sharedInstance = NULL;
    }
}
#pragma mark ProfileData
LPTPlayerProfileData* LPTOpponent::getPlayerProfileData() {
    LPTPlayerProfileData *profileData = LPTPlayerProfileData::getInstance();
    profileData->setPlayerUserName(this->getUniqueName().c_str());
    profileData->setPlayerGameWon(this->getTotalWins());
    profileData->setPlayerProfilePic(this->getProfilePic());
    profileData->setPlayerGameLevel(this->getlevel());
    profileData->setPlayerGameXP(this->getgameXP());
    profileData->setPlayerCoin(this->getCoins());
    profileData->setPlayerGamePlayed(this->getgamePlayed());
    return profileData;
}
