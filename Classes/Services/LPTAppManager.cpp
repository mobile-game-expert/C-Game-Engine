//
//  LPTAppManager.cpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 11/09/18.
//

#include "LPTAppManager.hpp"
#include "LPTPlayer.hpp"
#include "LPTOpponent.hpp"
#include "LPTNotificationPopup.hpp"
#include "LPTServerListener.hpp"


static LPTAppManager *_sharedInstance = NULL;

LPTAppManager* LPTAppManager::sharedInstance(){
    if (!_sharedInstance) {
        _sharedInstance = new LPTAppManager();
        _sharedInstance->init();
    }
    return _sharedInstance;
}

void LPTAppManager::init(){
    _sharedInstance->gameState              = UNKNOWN;
    _sharedInstance->isConnectedToAppWarp   = false;
}

void LPTAppManager::resetAppManager(){
    if(_sharedInstance){
        _sharedInstance = NULL;
    }
    LPTAppManager::sharedInstance();
}

void* ThreadFunctionToCheckInternet(void* arg)
{
    if (GameUtils::isInternetConnected()) {
        _sharedInstance->isInternetAvilable = true;
    }else{
        _sharedInstance->isInternetAvilable = false;
    }
    return NULL;
}

void LPTAppManager::checkInternetConnection(){
    pthread_t thread;
    pthread_create(&thread, NULL, &ThreadFunctionToCheckInternet, NULL);
}

#pragma mark - Device token

void LPTAppManager::registerDeviceToken() {

//    App42API::setIsTraceEnabled(true);
//    App42PushHelper::setPushCallBack(CC_CALLBACK_1(LPTAppManager::didReceivePushNotification, this));
//
//    userName = LPTPlayer::getPlayerUniqueName();
//    deviceToken =  NULL;
//
////    if(PLAYER->getDeviceToken() != NULL){
//        App42PushHelper::registerForPush(CC_CALLBACK_1(LPTAppManager::deviceDidRegisteredWithDeviceToken, this));
////    }

}

void LPTAppManager::deviceDidRegisteredWithDeviceToken(const char* l_deviceToken)
{
//    deviceToken = l_deviceToken;
//    PLAYER->setDeviceToken(l_deviceToken);
//    /*set status*/
//    log("deviceToken = %s",deviceToken);
//
//    /*delete any device associated with the user name*/
//    PushNotificationService *pushService = App42API::BuildPushNotificationService();
//    pushService->DeleteAllDevices(userName.c_str(), app42callback(LPTAppManager::onDeviceTokenRemoved, this));
}

#pragma mark - Notification
void LPTAppManager::sendChallangeNotification(string userName) {
//    PushNotificationService *pushService = App42API::BuildPushNotificationService();
    map<string, string> messageMap;
    string alertMsg = StringUtils::format("%s has challenged you!!!",PLAYER->getPlayerUniqueName().c_str());
    string profilePic = PLAYER->getPlayerProfilePicture().compare("") == 0 ?  kPlayerProfileImageName : PLAYER->getPlayerProfilePicture();
    string betAmt = StringUtils::format("%.0f",GameUtils::getUserBetValueFromSelectedBet(PLAYER->getBetSelected()));
    string jsonData = "{\"receiverName\": \""+userName+"\", \"dataToSend\": {\"alert\": \""+alertMsg+"\",\"badge\": \"1\",\"notificationtype\": \""+kSendChallenge+"\",\"sendername\": \""+PLAYER->getPlayerUniqueName()+"\",\"betselected\": \""+betAmt+"\",\"profilepic\": \""+profilePic+"\"}}";
//    pushService->SendPushMessageToUser(userName.c_str(),messageMap ,app42callback(LPTAppManager::onPushRequestCompleted, this));
    log("url is %s",kPostNotification);
    log("Auth token %s",PLAYER->getPlayerAuthToken().c_str());
    LPTServerListener::getInstance()->postRequestToServer(1, jsonData, kPostNotification, CC_CALLBACK_2(LPTAppManager::onPushRequestCompleted, this));
    notificationState = k_SEND_CHALLENGE;
}

void LPTAppManager::acceptChallangeNotification(string userName) {
    
    string alertMsg = StringUtils::format("%s has accepeted your challenge!!!",PLAYER->getPlayerUniqueName().c_str());
    string profilePic = PLAYER->getPlayerProfilePicture().compare("") == 0 ?  kPlayerProfileImageName : PLAYER->getPlayerProfilePicture();
    string betAmt = StringUtils::format("%.0f",GameUtils::getUserBetValueFromSelectedBet(PLAYER->getBetSelected()));
    string jsonData = "{\"receiverName\": \""+userName+"\", \"dataToSend\": {\"alert\": \""+alertMsg+"\",\"badge\": \"1\",\"notificationtype\": \""+kAcceptChallenge+"\",\"sendername\": \""+PLAYER->getPlayerUniqueName()+"\",\"betselected\": \""+betAmt+"\",\"profilepic\": \""+profilePic+"\"}}";
    LPTServerListener::getInstance()->postRequestToServer(1, jsonData, kPostNotification, CC_CALLBACK_2(LPTAppManager::onPushRequestCompleted, this));
    notificationState = k_ACCEPT_CHALLENGE;
}


void LPTAppManager::rejectChallangeNotification(string userName) {
    
    string alertMsg = StringUtils::format("Sorry, can't play right now");
    string profilePic = PLAYER->getPlayerProfilePicture().compare("") == 0 ?  kPlayerProfileImageName : PLAYER->getPlayerProfilePicture();
    string betAmt = StringUtils::format("%.0f",GameUtils::getUserBetValueFromSelectedBet(PLAYER->getBetSelected()));
    string jsonData = "{\"receiverName\": \""+userName+"\", \"dataToSend\": {\"alert\": \""+alertMsg+"\",\"badge\": \"1\",\"notificationtype\": \""+kRejectChallenge+"\",\"sendername\": \""+PLAYER->getPlayerUniqueName()+"\",\"betselected\": \""+betAmt+"\",\"profilepic\": \""+profilePic+"\"}}";
    LPTServerListener::getInstance()->postRequestToServer(1, jsonData, kPostNotification, CC_CALLBACK_2(LPTAppManager::onPushRequestCompleted, this));
    notificationState = k_REJECT_CHALLENGE;
    
}
#pragma mark - Notification call response
void LPTAppManager::didReceivePushNotification(const char* pushMsg)
{
    log("Message = %s",pushMsg);
    
    picojson::value v;
    std::string error = picojson::parse(v, pushMsg, pushMsg + strlen(pushMsg));
    picojson::object object = v.get<picojson::object>();
    picojson::object notificationMessgae = object[kAps].get<picojson::object>();
    picojson::object alert = notificationMessgae["alert"].get<picojson::object>();
    string body = alert["body"].get<string>();
    string msgTitle = alert["title"].get<string>();
    
    string senderName = object[kSenderName].get<string>();
    string profilePicture = object[kProfilePicture].get<string>();
    string betSelected = object[kBetSelected].get<string>();
    int betAmount = GameUtils::convertStringToInt(betSelected);
    string notificationType = object[kNotificationType].get<string>();
    
    //Notification popup
    LPTOpponent *opponent = new LPTOpponent();
    opponent->setUniqueName(senderName); //senderName
    opponent->setTotalWins(0);
    opponent->setTotalLose(0);
    opponent->setCoins(0);
    opponent->setEmailId("");
    opponent->setProfilePic(profilePicture); //profilePicture
    opponent->setBetSelected(GameUtils::getUserBetValueToSelectedBet(betAmount));//betAmount
    
    LPTNotificationPopup *popup = LPTNotificationPopup::createLayer(opponent, notificationType);
    popup->setPosition(Vec2(GET_WIDTH * 0.15, GET_HEIGHT * 1.2));
    float heightFactor = GameUtils::isIpad ? 0.85 : 0.8;
    popup->runAction(MoveTo::create(1, Vec2(GET_WIDTH * 0.15, GET_HEIGHT * heightFactor)));
    
    Director::getInstance()->getRunningScene()->addChild(popup);
}


void LPTAppManager::onPushRequestCompleted(HttpClient *sender,HttpResponse *response) {
    log("\ncode=%ld",response->getResponseCode());
    if(response->getResponseCode() == SUCCESS){
        string message = kChallengeSent;
        switch (notificationState) {
            case k_ACCEPT_CHALLENGE:
                message = kRejectChallengeText;
                break;
                
            case k_REJECT_CHALLENGE:
                message = kRejectChallengeText;
                break;
                
            case k_SEND_CHALLENGE:
                message = kChallengeSent;
                break;
                
            default:
                break;
        }
        
        auto popup = LPTPopUp::showPopUpWith(kInformation, message, false, NORMAL_POPUP);
        Director::getInstance()->getRunningScene()->addChild(popup,5000);
        notificationState = k_NONE;
    }else {
        auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, "Request can't be completed this time.\nPlease try after some time", false, NORMAL_POPUP);
        Director::getInstance()->getRunningScene()->addChild(popup,5000);
    }
}


void LPTAppManager::onDeviceTokenRemoved(void *response){
//    App42PushNotificationResponse *pushResponse = (App42PushNotificationResponse*)response;
//    log("\ncode=%d",pushResponse->getCode());
//    log("\nResponse Body=%s",pushResponse->getBody().c_str());
//
//    string code = StringUtils::format("Code = %d",pushResponse->getCode());
//    responseArray.push_back(code);
//    if (pushResponse->isSuccess)
//    {
//        for(std::vector<App42PushNotification>::iterator it = pushResponse->notifications.begin(); it != pushResponse->notifications.end(); ++it)
//        {
//            log("\n UserName=%s",it->userName.c_str());
//            log("\n Message=%s\n",it->message.c_str());
//            log("\n Type=%s\n",it->type.c_str());
//            log("\n DeviceToken=%s\n",it->deviceToken.c_str());
//            log("\n Expiry=%s\n",it->expiry.c_str());
//        }
//
//
//    }
//    else
//    {
//        log("\nerrordetails:%s",pushResponse->errorDetails.c_str());
//        log("\nerrorMessage:%s",pushResponse->errorMessage.c_str());
//        log("\nappErrorCode:%d",pushResponse->appErrorCode);
//        log("\nhttpErrorCode:%d",pushResponse->httpErrorCode);
//        string error = StringUtils::format("Error = %s",pushResponse->errorDetails.c_str());
//        responseArray.push_back(error);
//        string errorMsg = StringUtils::format("ErrorMessage = %s",pushResponse->errorMessage.c_str());
//        responseArray.push_back(errorMsg);
//    }
//
//    DeviceType deviceType = APP42_IOS;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    deviceType = APP42_ANDROID;
//#endif
//
//    PushNotificationService *pushService = App42API::BuildPushNotificationService();
//    pushService->RegisterDeviceToken(PLAYER->getDeviceToken().c_str(), userName.c_str(), deviceType, app42callback(LPTAppManager::onPushRequestCompleted, this));
}
void LPTAppManager::setIsTutorialShown(bool status) {
    UserDefault::getInstance()->setBoolForKey(kNeedToShowTutorial, status);
}
bool LPTAppManager::getTutorialShownStatus() {
    return UserDefault::getInstance()->getBoolForKey(kNeedToShowTutorial);
}

#pragma mark UpdateGameDataStatus
void LPTAppManager::setDataSyncStatus(bool status) {
    string key = "DataSyncStatus";
    UserDefault::getInstance()->setBoolForKey(key.c_str(), status);
}

bool LPTAppManager::getDataSyncStatus() {
    string key = "DataSyncStatus";
    return UserDefault::getInstance()->getBoolForKey(key.c_str());
}

#pragma mark Total Spends by user
void LPTAppManager::setTotalSpendsByUser(float spents) {
    string key = "keyLPTTotalSpentByUser";
    UserDefault::getInstance()->setBoolForKey(key.c_str(), spents);
}

float LPTAppManager::getTotalSpendsByUser() {
    string key = "keyLPTTotalSpentByUser";
    return UserDefault::getInstance()->getBoolForKey(key.c_str());
}

#pragma mark GET and Intilaize debug Values

void LPTAppManager::getDebugValues() {
    LPTServerListener::getInstance()->postRequestToServer(0, "{}", kGetLPTDebugValues, CC_CALLBACK_2(LPTAppManager::setDebugValues, this));
}

void LPTAppManager::setDebugValues(HttpClient *sender,HttpResponse *response) {
//    log("\ncode=%ld",response->getResponseCode());
    if(response->getResponseCode() == SUCCESS){
        std::vector<char> * buffer = response->getResponseData();
        std::string jsonStr = std::string(&(*buffer->begin()), buffer->size());
        picojson::value jsonData;
        picojson::parse(jsonData, jsonStr.c_str());
        picojson::object jsonObject = jsonData.get<picojson::object>();
        picojson::object dataObject = jsonObject["data"].get<picojson::object>();
        this->levelUpUpperBound = dataObject["initialLevelUpperBound"].get<double>();
        this->levelUpDelta = dataObject["levelUpDelta"].get<double>();
    }else {
        log("=============== Unable to fetch debug values right now ===============");
    }
}
