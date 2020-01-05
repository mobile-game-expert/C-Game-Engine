//
//  LPTAppManager.hpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 11/09/18.
//

#ifndef LPTAppManager_hpp
#define LPTAppManager_hpp

#include <stdio.h>
#include "GameUtils.hpp"
#include "App42API.h"
#include "App42PushHelper.h"
#include "App42API.h"
#include "App42Service.h"
#include "../cocos2d/cocos/network/HttpClient.h"
#include "../cocos2d/cocos/network/HttpRequest.h"
#include "../cocos2d/cocos/network/HttpResponse.h"
using namespace cocos2d;
using namespace cocos2d::network;
using namespace std;

#define APP_MANAGER LPTAppManager::sharedInstance()

#define GET_WIDTH  cocos2d::Director::getInstance()->getVisibleSize().width
#define GET_HEIGHT cocos2d::Director::getInstance()->getVisibleSize().height

#define FIT_WH(__node__,__width__,__height__) __node__->setScale(__width__/__node__->getContentSize().width,__height__/__node__->getContentSize().height)
#define FIT_W(__node__,__width__) __node__->setScale(__width__/__node__->getContentSize().width)
#define FIT_H(__node__,__height__) __node__->setScale(__height__/__node__->getContentSize().height)

typedef enum GAME_STATE{
    HOME_SCENE = 0,
    SIGNUP_SCENE,
    LOGIN_SCENE,
    FB_LOGIN_SCENE,
    GAME_SCENE,
    STORE_SCENE,
    FORGOT_PASSWORD,
    SET_USERNAME,
    GAME_SELECTION,
    BET_SELECTION,
    DIFFICULTY_SELECTION,
    PLAYER_MATCHING,
    TUTORIAL_SCENE,
    INTERACTIVE_TUTORIAL_SCENE,
    UNKNOWN
    
}GAME_STATE;


//Keys
#define     kNeedToShowTutorial         "needToShowTutorial"


//
class LPTAppManager{
public:
    bool isInternetAvilable = false;
    bool isPlayingWithFriend = false;
    int numberOfSPGPerSession = 0;
    int numberOfMPGPerSession = 0;
    float totalSpendsPerSession = 0;
    float totalSpendsByUser = 0;
    static LPTAppManager* sharedInstance();
    void resetAppManager();
    GAME_STATE gameState;
    void checkInternetConnection();
    bool isConnectedToAppWarp = false;
    struct tm *appStartTime;
    struct tm *appEndTime;
    //Push notification methods
    NOTIFICATION_STATUS notificationState = k_NONE;
    void registerDeviceToken();
    const char* deviceToken;
    vector<string> responseArray;
    string userName, message;
    
    void didReceivePushNotification(const char* pushMsg);
    void deviceDidRegisteredWithDeviceToken(const char* l_deviceToken);
    void sendChallangeNotification(string userName);
    void acceptChallangeNotification(string userName);
    void rejectChallangeNotification(string userName);
    void onPushRequestCompleted(HttpClient *sender,HttpResponse *response);
    void onDeviceTokenRemoved(void *response);
    
    void setIsTutorialShown(bool status);
    bool getTutorialShownStatus();
    //---
    void setDataSyncStatus(bool status);
    bool getDataSyncStatus();
    //--
    void setTotalSpendsByUser(float spents);
    float getTotalSpendsByUser();
    //-- Debug Values
    long levelUpUpperBound = 2100;
    long levelUpDelta = 210;
    void getDebugValues();
    void setDebugValues(HttpClient *sender,HttpResponse *response);
private:
    void init();
protected:
};

#endif /* LPTAppManager_hpp */
