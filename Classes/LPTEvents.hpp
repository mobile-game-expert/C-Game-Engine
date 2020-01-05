//
//  LPTEvents.hpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 28/02/19.
//

#ifndef LPTEvents_hpp
#define LPTEvents_hpp

#include <stdio.h>
#include <cocos2d.h>
#include "NDKHelper.h"
#include "GameUtils.hpp"
#include "LPTAppManager.hpp"

using namespace cocos2d;
using namespace std;

//Player
#define kUserCountry                    "UserCountry"
#define kUserEnteryTime                 "User_Entery_Time"
#define kUserLocation                   "User_Location"
#define kUserAge                        "User_Age"
//Session
#define kUserSessionLength              "User_Session_Length"
#define kUserGameLength                 "User_Game_Length"
#define kUserTotal_SPG_In_Session       "Total_SPG_In_Session"     /* SPG - Single Player Games */
#define kUserTotal_MPG_In_Session       "Total_MPG_In_Session"     /* MPG - Multi Player Games */
#define kUserLevel                      "User_Level"
#define kUserXP                         "User_XP"
#define kUserSpentsPerSession           "User_Spents_Per_Session"
#define kUserPurchaseIAP                "IAP_Pack_Purchased"
#define kUserTotalSpentsIAP             "User_Total_Spents_In_IAP"
//Game
#define kUserTableSelected              "User_Table_Selected"
#define kUserTableWin                   "User_Table_Win"
#define kUserTableLoose                 "User_Table_Loose"
#define kUserTimeTakenToPlayNextShot    "User_Time_Taken_To_Play_Next_Shot" // Not Needed
#define kUserShotSource                 "User_Shot_Source"
#define kUserShotGotStruckInJaw         "User_Shot_Got_Struck_In_Jaw"


class LPTEvents {
public:
    // User Events
    static void logAppOpenTime();
    static void logUserLocation();
    static void logUserAge(string age);
    // user Session Events
    static void logUserSessionLength();
    static void logUserGameLength(bool isWon, string timeTaken);
    static void logNumberOfSPG(int noOfgames);
    static void logNumberOfMPG(int noOfgames);
    static void logUserLevel(int level);
    static void logUserXP(int userXP);
    static void logUserTotalSpentsPerSession(float totalSpentsInSession);
    static void logUserTotalSpents(float totalSpents);
    static void logIAPPackPurchased(float packPrice);
    // User Game events
    static void logTableSelection(int tableNo);
    static void logTableWonByUser(int tableNo);
    static void logTableLooseByUser(int tableNo);
    static void logUserShotTime(float time);
    static void logUserShotSource();
    static void logNumberOfTimeStrikerStruckInJaw(int occurrences);
};

#endif /* LPTEvents_hpp */
