//
//  LPTPlayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 12/09/18.
//

#ifndef LPTPlayer_hpp
#define LPTPlayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "App42Object.h"
#include "picojson.h"
#include <Box2D/Box2D.h>
#include "LPTPlayerProfileData.hpp"

#define PLAYER              LPTPlayer::getInstance()

#define FIELD_UNAME             "UniqueName"
#define FIELD_EMAIL             "EMailId"
#define FIELD_TOTAL_WIN         "TotalWin"
#define FIELD_TOTAL_LOSE        "TotalLose"
#define FIELD_TOTAL_COINS       "TotalCoins"
#define FIELD_FB_USER           "FBUser"
#define FIELD_GUEST_USER        "GuestUser"
#define FIELD_APP42_USER        "App42User"
#define FIELD_PROFILE_PICTURE   "profileImage"
#define FIELD_GAME_XP           "gameXP"
#define FIELD_GAME_LEVEL        "level"
#define FIELD_ACCESS_TOKEN      "accessToken"
#define FIELD_TOTAL_GAMES       "gamePlayed"
#define FIELD_LOGIN_TYPE        "loginType"
#define FIELD_SESSION_ID        "_id"
#define FIELD_fbId              "fbId"
#define FIELD_Record_Status     "recordExist"
#define FIELD_NextLevelXP       "nextLevelXP"
#define FIELD_Online_Status     "online"

typedef enum LOGIN_STATE{
    GUEST_USER = 100,
    APP42_USER,
    FB_USER,
    NONE
    
}LOGIN_STATE;

class LPTPlayer : public Ref {
public:
    CC_SYNTHESIZE(string, profilePic, ProfilePic);
    CC_SYNTHESIZE(GameSelected, gameSelected, GameSelected);
    CC_SYNTHESIZE(BetSelected, betSelected, BetSelected);
    CC_SYNTHESIZE(string, roomId, RoomId);
    CC_SYNTHESIZE(string, deviceToken, DeviceToken);
    CC_SYNTHESIZE(string, facebookID, facebookID);
    CC_SYNTHESIZE(bool, recordExist, recordExist);
    void init();
    static LPTPlayer* getInstance();
    void updateUserProfile(picojson::object playerData);
    LOGIN_STATE getLoginState();
    bool isAdmin = false;
    UserPlaySide playerSide = PLAY_AREA_LEFT;
    //
    string roomName         = "";
    string roompassword     = "";
    //
    map<string,string> setTableProperties(string roomName,string password,int betAmount,int minUser,int maxuser);
    string moveDataToSend(int strikerTag, Vec2 positionOnRope, b2Vec2 impulse);
    
    //====Static Functions
    void setIsAdmin(bool isAdmin);
    
    void setPlayerSessionID(const char* sessionId);
    string getPlayerSessionID();
    
    void setPlayerAge(string age);
    string getPlayerAge();
    
    void setPlayerUniqueName(string uName);
    string getPlayerUniqueName();
    
    void setPlayerEmailID(string emailId);
    string getPlayerEmailID();
    
    void savePlayerAuthToken(string authToken);
    string getPlayerAuthToken();
    
    void setPlayerLoginState(LOGIN_STATE loginState);
    LOGIN_STATE getPlayerLoginState();
    
    void setPlayerGamePlayed(double totalGamePlayed);
    double getPlayerGamePlayed();
    
    void setPlayerGameWon(double totalGameWon);
    double getPlayerGameWon();
    
    void setPlayerGameLevel(double playerGameLevel);
    double getPlayerGameLevel();
    
    void setTotalCoins(double coins);
    double getTotalCoins();
    
    void setPlayerGameXP(double xp);
    double getPlayerGameXP();
    
    void logoutUser();
    void populatePlayerData(picojson::object playerData);
    
    LPTPlayerProfileData* getPlayerProfileData();
    string getPlayerProfilePicture();
private:
protected:
};

#endif /* LPTPlayer_hpp */
