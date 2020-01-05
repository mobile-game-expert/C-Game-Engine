//
//  LPTPlayerProfileData.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 15/12/18.
//

#include "LPTPlayerProfileData.hpp"

static LPTPlayerProfileData *_sharedInstance = NULL;
LPTPlayerProfileData* LPTPlayerProfileData::getInstance() {
    if (_sharedInstance == NULL) {
        _sharedInstance = new LPTPlayerProfileData();
        _sharedInstance->init();
    }
    return _sharedInstance;
}

void LPTPlayerProfileData::init() {
}

string LPTPlayerProfileData::getPlayerUserName() {
    return playerName;
}
string LPTPlayerProfileData::getPlayerProfilePic() {
    return playerProfilePicture;
}
double LPTPlayerProfileData::getPlayerGameWon() {
    return playerGameWon;
}
double LPTPlayerProfileData::getPlayerCoins() {
    return playerCoin;
}
double LPTPlayerProfileData::getPlayerGameXP() {
    return playerGameXP;
}
double LPTPlayerProfileData::getPlayerGameLevel() {
    return playerGameLevel;
}
double LPTPlayerProfileData::getPlayerGamePlayed() {
    return playerGamePlayed;
}
void LPTPlayerProfileData::setPlayerUserName(string userName) {
    playerName = userName;
}
void LPTPlayerProfileData::setPlayerProfilePic(string profilePic) {
    playerProfilePicture = profilePic;
}
void LPTPlayerProfileData::setPlayerGameWon(double gamePlayed) {
    playerGameWon = gamePlayed;
}
void LPTPlayerProfileData::setPlayerCoin(double coin) {
    playerCoin = coin;
}
void LPTPlayerProfileData::setPlayerGameXP(double playerXP) {
    playerGameXP = playerXP;
}
void LPTPlayerProfileData::setPlayerGameLevel(double level) {
    playerGameLevel = level;
}
void LPTPlayerProfileData::setPlayerGamePlayed(double gamePlayed) {
    playerGamePlayed = gamePlayed;
}
