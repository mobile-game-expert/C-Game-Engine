//
//  LPTPlayerProfileData.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 15/12/18.
//

#ifndef LPTPlayerProfileData_hpp
#define LPTPlayerProfileData_hpp

#include <stdio.h>
#include <cocos2d.h>

using namespace cocos2d;
using namespace std;

class LPTPlayerProfileData : public Ref {
private:
    string playerName;
    string playerProfilePicture;
    double playerGameWon;
    double playerGamePlayed;
    double playerCoin;
    double playerGameXP;
    double playerGameLevel;
    
public:
    static LPTPlayerProfileData* getInstance();
    void init();
    
    void setPlayerUserName(string userName);
    void setPlayerProfilePic(string profilePic);
    void setPlayerGameWon(double gameWon);
    void setPlayerGamePlayed(double gamePlayed);
    void setPlayerCoin(double coin);
    void setPlayerGameXP(double playerXP);
    void setPlayerGameLevel(double playerGameLevel);
    
    //Getters
    string getPlayerUserName();
    string getPlayerProfilePic();
    double getPlayerGameWon();
    double getPlayerCoins();
    double getPlayerGameXP();
    double getPlayerGameLevel();
    double getPlayerGamePlayed();
};
#endif /* LPTPlayerProfileData_hpp */
