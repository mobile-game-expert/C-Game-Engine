//
//  LPTOpponent.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 08/10/18.
//

#ifndef LPTOpponent_hpp
#define LPTOpponent_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "picojson.h"
#include "LPTPlayerProfileData.hpp"

#define OPPONENT                LPTOpponent::getInstance()

class LPTOpponent : public Ref {
public:
    CC_SYNTHESIZE(string, uniqueName, UniqueName);
    CC_SYNTHESIZE(string, emailId, EmailId);
    CC_SYNTHESIZE(int, totalWins, TotalWins);
    CC_SYNTHESIZE(int, totalLose, TotalLose);
    CC_SYNTHESIZE(double, coins, Coins);
    CC_SYNTHESIZE(string, profilePic, ProfilePic);
    CC_SYNTHESIZE(BetSelected, betSelected, BetSelected);
    CC_SYNTHESIZE(int, gamePlayed, gamePlayed);
    CC_SYNTHESIZE(int, level, level);
    CC_SYNTHESIZE(double, gameXP, gameXP);
    
    void init();
    static LPTOpponent* getInstance();
    void createProfileWith(const char *jsonString);
    map<string,string> setTableProperties(string roomName,string password,int betAmount,int minUser,int maxuser);
    void removeSharedInstance();
    
    // Temp method to create profile With Bot
    void createProfileForBotWith(string username , string profilePic);
    void updateUserProfile(picojson::object playerData);
    
    LPTPlayerProfileData* getPlayerProfileData();
private:
protected:
};


#endif /* LPTOpponent_hpp */
