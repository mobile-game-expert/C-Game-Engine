//
//  LPTServerListener.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 06/12/18.
//

#ifndef LPTServerListener_hpp
#define LPTServerListener_hpp

#define ktestConstantAPIUrl                     "192.168.2.56:2201/v1"
#define kConstantAPIUrl                         "100.24.187.148:2200/v1"
//"35.153.215.232:2200/v1"
#define kUrlUserRegistered                      "http://" kConstantAPIUrl "/register"
#define kUserLoginAPI                           "http://" kConstantAPIUrl "/signin"
#define kCheckUserExist                         "http://" kConstantAPIUrl "/check_fb_user"
#define kForgetPassword                         "http://" kConstantAPIUrl "/forget_password"
#define kFetchUserRecord                        "http://" kConstantAPIUrl "/user_profile"
#define kLogoutUserAPI                          "http://" kConstantAPIUrl "/logout"
#define kUpdateUserProfileAPI                   "http://" kConstantAPIUrl "/user_profile"
#define kUpdateUserDataAPI                      "http://" kConstantAPIUrl "/game_model_update"
#define kGameOverAPI                            "http://" kConstantAPIUrl "/game_over"
#define kFetchUserAPI                           "http://" kConstantAPIUrl "/users"
#define kUpdateOnlineStatus                     "http://" kConstantAPIUrl "/update_online_status"
#define kPostNotification                       "http://" kConstantAPIUrl "/game_request"

#define kGetLPTDebugValues                      "http://" kConstantAPIUrl "/get_game_default_values"

#include <stdio.h>
#include "cocos2d.h"
#include "../cocos2d/cocos/network/HttpRequest.h"
#include "../cocos2d/cocos/network/HttpResponse.h"
#include "LPTServerDelegate.hpp"
#include "LPTHeaders.h"
using namespace cocos2d;
using namespace cocos2d::network;
using namespace std;

class LPTServerListener : Node {
public:
    static LPTServerListener *instance;
    
    CC_SYNTHESIZE(LPTServerDelegate *, serverDelegate, LPTServerDelegate);
    static LPTServerListener *getInstance();

//    void reteriveDataFromServer(string fname,string lname,string email,string fbID,string fbToken, string deviceType, string deviceToken, string country, ServerRequest serverRequest);
    void postRequestToServer(int reqType,string jsonData, string url,const ccHttpRequestCallback& callback);
    void reteriveDataFromServerForGame(string uniqueName, string email, string password, string facebookId, string deviceToken, int loginType, ServerRequest serverRequest);
    void fetchPlayerData(string uniqueName, string email, string facebookId, string authToken, ServerRequest serverRequest);
    void updateUserProfilePic(string email, string profilePic, string authToken, ServerRequest serverRequest);
    void updateUserGameData(double gameXP, double totalCoins, double totalWin, double gamePlayed, double playerLevel, string authToken, string isUserWin, ServerRequest serverRequest);
    virtual void onHttpRetrieveRequestCompleted(HttpClient *sender,HttpResponse *response);
};
#endif /* LPTServerListener_hpp */
