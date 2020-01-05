//
//  LPTPlayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 12/09/18.
//

#include "LPTPlayer.hpp"

#define kKey_App42_Session_ID           "kKeyApp42SessionID"
#define kKey_Player_Login_State         "kKeyPlayerLoginState"
#define kKey_Player_Email_ID            "kKeyPlayerEmailID"
#define kKey_Player_Unique_Name         "kKeyPlayerUniqueName"
#define kKey_Player_Auth_Token          "kKeyPlayerAuthToken"
#define kKey_Player_Game_Level          "kKeyPlayerGameLevel"
#define kKey_Player_Game_Won            "kKeyPlayerGameWon"
#define kKey_Player_Total_Game          "kKeyPlayerTotalGame"
#define kKey_Player_Total_Coin          "kKeyPlayerTotalCoins"
#define kKey_Player_Game_XP             "kKeyPlayerGameXP"
#define kKey_Player_Age                 "kKeyPlayerAge"

static LPTPlayer *_sharedInstance = NULL;

LPTPlayer* LPTPlayer::getInstance() {
    if (_sharedInstance == NULL) {
        _sharedInstance = new LPTPlayer();
        _sharedInstance->init();
    }
    return _sharedInstance;
}

void LPTPlayer::init() {
}

void LPTPlayer::populatePlayerData(picojson::object playerData) {
    picojson::object object = playerData;    
    setPlayerUniqueName(object[FIELD_UNAME].get<string>());
    setPlayerGameWon(object[FIELD_TOTAL_WIN].get<double>());
    setPlayerGamePlayed(object[FIELD_TOTAL_GAMES].get<double>());
    setTotalCoins(object[FIELD_TOTAL_COINS].get<double>());
//    setPlayerEmailID(object[FIELD_EMAIL].get<string>());
    savePlayerAuthToken(object[FIELD_ACCESS_TOKEN].get<string>());
    setPlayerGameLevel(object[FIELD_GAME_LEVEL].get<double>());
    setPlayerGameXP(object[FIELD_GAME_XP].get<double>());
    setPlayerLoginState((LOGIN_STATE)object[FIELD_LOGIN_TYPE].get<double>());
    setPlayerSessionID(object[FIELD_SESSION_ID].get<string>().c_str());
    
    if(object[FIELD_PROFILE_PICTURE].get<string>() == "" || object[FIELD_PROFILE_PICTURE].get<string>().length() <= 0) {
        PLAYER->setProfilePic(kPlayerProfileImageName);
    }else {
        PLAYER->setProfilePic(object[FIELD_PROFILE_PICTURE].get<string>());
    }
    PLAYER->setfacebookID(object[FIELD_fbId].get<string>());
}

void LPTPlayer::updateUserProfile(picojson::object playerData) {
    picojson::object object = playerData;
    
    setPlayerUniqueName(object[FIELD_UNAME].get<string>());
    setPlayerGameWon(object[FIELD_TOTAL_WIN].get<double>());
    setPlayerGamePlayed(object[FIELD_TOTAL_GAMES].get<double>());
    setTotalCoins(object[FIELD_TOTAL_COINS].get<double>());
    setPlayerGameLevel(object[FIELD_GAME_LEVEL].get<double>());
    setPlayerGameXP(object[FIELD_GAME_XP].get<double>());
    PLAYER->setProfilePic(kPlayerProfileImageName);
    
    if(object[FIELD_PROFILE_PICTURE].get<string>() == "" || object[FIELD_PROFILE_PICTURE].get<string>().length() <= 0) {
        PLAYER->setProfilePic(kPlayerProfileImageName);
    }else {
        PLAYER->setProfilePic(object[FIELD_PROFILE_PICTURE].get<string>());
    }
}

LOGIN_STATE LPTPlayer::getLoginState() {
    if(PLAYER->getPlayerLoginState() == APP42_USER) {
        return APP42_USER;
    } else if(PLAYER->getPlayerLoginState() == FB_USER) {
        return FB_USER;
    } else if(PLAYER->getPlayerLoginState() == GUEST_USER) {
        return GUEST_USER;
    } else {
        return NONE;
    }
}

map<string,string> LPTPlayer::setTableProperties(string roomName,string password,int betAmount,int minUser,int maxuser) {
    map<string,string> table;
    table["LPTRoom_Name"]           = roomName;
    table["LPTRoom_Pass"]           = password;
    table["LPTRoom_Bet_Amount"]     = betAmount;
    table["LPTRoom_Min_User"]       = minUser;
    table["LPTRoom_Max_User"]       = maxuser;
    return table;
}

string LPTPlayer::moveDataToSend(int strikerTag, Vec2 positionOnRope, b2Vec2 impulse) {
    string dataFormat = "{\"StrikerTag\":%d,\"PositionXOnRope\":%f,\"PositionYOnRope\":%f,\"ImpulseX\":%f,\"ImpulseY\":%f}";
    __String *data = __String::createWithFormat(dataFormat.c_str(),strikerTag,positionOnRope.x,positionOnRope.y,impulse.x,impulse.y);
    return data->getCString();
}


string LPTPlayer::getPlayerProfilePicture() {
    string pic = PLAYER->getProfilePic();
    if(pic.compare("") != 0) {
        return pic;
    }
    return kPlayerProfileImageName;
}
#pragma mark ****************STATIC Functions*********************
void LPTPlayer::setIsAdmin(bool isAdmin) {
    if(PLAYER->getGameSelected() == GAME_WITH_FRIEND) {
        PLAYER->isAdmin = PLAYER->isAdmin;
    }else {
        PLAYER->isAdmin = isAdmin;
    }
}

void LPTPlayer::setPlayerSessionID(const char* sessionId) {
    UserDefault::getInstance()->setStringForKey(kKey_App42_Session_ID, sessionId);
}

string LPTPlayer::getPlayerSessionID() {
    return UserDefault::getInstance()->getStringForKey(kKey_App42_Session_ID,"");
}

void LPTPlayer::setPlayerLoginState(LOGIN_STATE loginState) {
    UserDefault::getInstance()->setIntegerForKey(kKey_Player_Login_State, loginState);
}

LOGIN_STATE LPTPlayer::getPlayerLoginState() {
    return (LOGIN_STATE)UserDefault::getInstance()->getIntegerForKey(kKey_Player_Login_State, NONE);
}

void LPTPlayer::setPlayerUniqueName(string uName) {
    UserDefault::getInstance()->setStringForKey(kKey_Player_Unique_Name, uName);
}

string LPTPlayer::getPlayerUniqueName() {
     return UserDefault::getInstance()->getStringForKey(kKey_Player_Unique_Name,"");
}

void LPTPlayer::setPlayerEmailID(string emailId) {
    string key = getPlayerUniqueName() + kKey_Player_Email_ID;
    UserDefault::getInstance()->setStringForKey(key.c_str(), emailId);
}

string LPTPlayer::getPlayerEmailID() {
    string key = getPlayerUniqueName() + kKey_Player_Email_ID;
    return UserDefault::getInstance()->getStringForKey(key.c_str(),"");
}

void LPTPlayer::savePlayerAuthToken(string authToken) {
    string key = getPlayerUniqueName() + kKey_Player_Auth_Token;
    UserDefault::getInstance()->setStringForKey(key.c_str(), authToken);
}
string LPTPlayer::getPlayerAuthToken() {
    string key = getPlayerUniqueName() + kKey_Player_Auth_Token;
    return UserDefault::getInstance()->getStringForKey(key.c_str(),"");
}

void LPTPlayer::setPlayerGamePlayed(double totalGamePlayed) {
    string key = getPlayerUniqueName() + kKey_Player_Total_Game;
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), totalGamePlayed);
}

double LPTPlayer::getPlayerGamePlayed() {
    string key = getPlayerUniqueName() + kKey_Player_Total_Game;
    return UserDefault::getInstance()->getDoubleForKey(key.c_str(), 0);
}

void LPTPlayer::setPlayerGameWon(double totalGameWon){
    string key = getPlayerUniqueName() + kKey_Player_Game_Won;
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), totalGameWon);
}
double LPTPlayer::getPlayerGameWon() {
    string key = getPlayerUniqueName() + kKey_Player_Game_Won;
    return UserDefault::getInstance()->getDoubleForKey(key.c_str(), 0);
}

void LPTPlayer::setPlayerGameLevel(double playerGameLevel) {
    string key = getPlayerUniqueName() + kKey_Player_Game_Level;
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), playerGameLevel);
}
double LPTPlayer::getPlayerGameLevel() {
    string key = getPlayerUniqueName() + kKey_Player_Game_Level;
    return UserDefault::getInstance()->getDoubleForKey(key.c_str(), 0);
}

void LPTPlayer::setTotalCoins(double coins) {
    string key = getPlayerUniqueName() + kKey_Player_Total_Coin;
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), coins);
}
double LPTPlayer::getTotalCoins() {
    string key = getPlayerUniqueName() + kKey_Player_Total_Coin;
    return UserDefault::getInstance()->getDoubleForKey(key.c_str(), 0);
}

void LPTPlayer::setPlayerGameXP(double xp) {
    string key = getPlayerUniqueName() + kKey_Player_Game_XP;
    log("SetPlayerGameXP:  %s", key.c_str());
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), xp);
}

double LPTPlayer::getPlayerGameXP() {
    string key = getPlayerUniqueName() + kKey_Player_Game_XP;
    return UserDefault::getInstance()->getDoubleForKey(key.c_str(), 0);
}

void LPTPlayer::setPlayerAge(string age) {
    UserDefault::getInstance()->setStringForKey(kKey_Player_Age, age);
}

string LPTPlayer::getPlayerAge() {
    return UserDefault::getInstance()->getStringForKey(kKey_Player_Age," ");
}
#pragma mark Logot
void LPTPlayer::logoutUser() {
//    string key = getPlayerUniqueName() + kKey_App42_Session_ID;
    UserDefault::getInstance()->setStringForKey(kKey_App42_Session_ID, "");
    
//    key = getPlayerUniqueName() + kKey_Player_Login_State;
    UserDefault::getInstance()->setIntegerForKey(kKey_Player_Login_State, NONE);
    UserDefault::getInstance()->setStringForKey(kKey_Player_Unique_Name, "");
    
    string key = getPlayerUniqueName() + kKey_Player_Email_ID;
    UserDefault::getInstance()->setStringForKey(key.c_str(), "");
    
    key = getPlayerUniqueName() + kKey_Player_Game_XP;
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), 0);
    
    key = getPlayerUniqueName() + kKey_Player_Game_Level;
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), 0);
    
    key = getPlayerUniqueName() + kKey_Player_Total_Coin;
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), 0);
    
    key = getPlayerUniqueName() + kKey_Player_Game_Won;
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), 0);
    
    key = getPlayerUniqueName() + kKey_Player_Total_Game;
    UserDefault::getInstance()->setDoubleForKey(key.c_str(), 0);
    
    key = getPlayerUniqueName() + kKey_Player_Auth_Token;
    UserDefault::getInstance()->setStringForKey(key.c_str(), "");
    _sharedInstance = NULL;
}

#pragma mark ProfileData
LPTPlayerProfileData* LPTPlayer::getPlayerProfileData() {
    LPTPlayerProfileData *profileData = LPTPlayerProfileData::getInstance();
    string userName = getPlayerUniqueName().c_str();
    log("Player gamewon: %f", getPlayerGameWon());
    profileData->setPlayerUserName(userName);
    profileData->setPlayerGameWon(getPlayerGameWon());
    profileData->setPlayerProfilePic(this->getPlayerProfilePicture());
    profileData->setPlayerGameLevel(getPlayerGameLevel());
    profileData->setPlayerGameXP(getPlayerGameXP());
    profileData->setPlayerCoin(getTotalCoins());
    profileData->setPlayerGamePlayed(getPlayerGamePlayed());
    
    return profileData;
}



