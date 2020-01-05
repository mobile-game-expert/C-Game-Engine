//
//  LPTServerListener.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 06/12/18.
//

#include "LPTServerListener.hpp"
#include "ExtensionMacros.h"
#include "../cocos2d/cocos/network/HttpClient.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

//KEY
#define kStatusCodeKey                          "statusCode"
#define kDataKey                                "data"
#define kRecordExistKey                         "recordExist"
#define kMessageKey                             "message"

LPTServerListener* LPTServerListener::instance = nullptr;
using namespace rapidjson;

LPTServerListener* LPTServerListener::getInstance() {
    if(instance == nullptr){
        instance = new LPTServerListener();
    }
    return instance;
}

void LPTServerListener::postRequestToServer(int reqType,string jsonData, string url,const ccHttpRequestCallback& callback ) {
    
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    string headerData = StringUtils::format("authorization: lpt_token %s", PLAYER->getPlayerAuthToken().c_str());
    headers.push_back(headerData);
    HttpRequest *httpRequest = new HttpRequest();
    HttpRequest::Type requestType = (reqType == 1) ? HttpRequest::Type::POST : HttpRequest::Type::GET;

    httpRequest->setRequestType(requestType);
    httpRequest->setResponseCallback(callback);
    httpRequest->setRequestData(jsonData.c_str(), jsonData.length());
    httpRequest->setHeaders(headers);
    httpRequest->setUrl(url);
    HttpClient::getInstance()->send(httpRequest);
    log("%s",jsonData.c_str());
    log("API Called %s", httpRequest->getUrl());
    httpRequest->release();

}

void LPTServerListener::fetchPlayerData(string uniqueName, string email, string facebookId, string authToken, ServerRequest serverRequest) {
    
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    
    HttpRequest * retrieveRequest = new HttpRequest();
    std::string json;
    
    HttpRequest::Type requestType = HttpRequest::Type::GET;
    
    switch (serverRequest) {
        case kFetchUserDetails:
        case kFetchOpponentDetails:
        {
            string updatedURL = StringUtils::format("%s/?search=%s", kFetchUserRecord, uniqueName.c_str());
            retrieveRequest->setUrl(updatedURL);
            json = "";
            retrieveRequest->setTag(StringUtils::format("%i",serverRequest).c_str());
            string headerData = StringUtils::format("authorization: lpt_token %s", authToken.c_str());
            headers.push_back(headerData);
          
        }
        break;
            
        case kFetchFriendsDataRequest: {
            string updatedURL = StringUtils::format("%s/?search=%s&limit=20&skip=0", kFetchUserAPI, uniqueName.c_str());
            retrieveRequest->setUrl(updatedURL);
            json = "";
            retrieveRequest->setTag(StringUtils::format("%i",kFetchFriendsDataRequest).c_str());
            string headerData = StringUtils::format("authorization: lpt_token %s", authToken.c_str());
            headers.push_back(headerData);
        }
            break;
        default:
            break;
    }
    
    retrieveRequest->setRequestType(requestType);
    retrieveRequest->setResponseCallback(CC_CALLBACK_2(LPTServerListener::onHttpRetrieveRequestCompleted,this));
    retrieveRequest->setRequestData(json.c_str(), json.length());
    retrieveRequest->setHeaders(headers);
    HttpClient::getInstance()->send(retrieveRequest);
    retrieveRequest->release();
}
void LPTServerListener::updateUserGameData(double gameXP, double totalCoins, double totalWin, double gamePlayed, double playerLevel, string authToken, string isUserWin, ServerRequest serverRequest) {
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    
    HttpRequest * retrieveRequest = new HttpRequest();
    std::string json;
    
    HttpRequest::Type requestType = HttpRequest::Type::PUT;
    
    string userGameXP = StringUtils::format("%f", gameXP);
    string userTotalCoins = StringUtils::format("%f", totalCoins);
    string userTotalWin = StringUtils::format("%f", totalWin);
    string userGamePlayed = StringUtils::format("%f", gamePlayed);
    string playerLevelInString  =   StringUtils::format("%f", playerLevel);
    switch (serverRequest) {
        case kGameModelUpdateRequest: {
            retrieveRequest->setUrl(kUpdateUserDataAPI);
            json = "{\"gameXP\":\""+userGameXP+"\",\"totalCoins\":\""+userTotalCoins+"\",\"totalWin\":\""+userTotalWin+"\",\"gamePlayed\":\""+userGamePlayed+"\",\"level\":\""+playerLevelInString+"\"}";
            
            retrieveRequest->setTag(StringUtils::format("%i",kGameModelUpdateRequest).c_str());
            string headerData = StringUtils::format("authorization: lpt_token %s", authToken.c_str());
            headers.push_back(headerData);
        }
            break;
        case kGameOverRequest: {
            retrieveRequest->setUrl(kGameOverAPI);
            json = "{\"gameXP\":\""+userGameXP+"\",\"coin\":\""+userTotalCoins+"\",\"totalWins\":\""+userTotalWin+"\",\"level\":\""+playerLevelInString+"\"}";
            retrieveRequest->setTag(StringUtils::format("%i",kGameOverRequest).c_str());
            string headerData = StringUtils::format("authorization: lpt_token %s", authToken.c_str());
            headers.push_back(headerData);
        
        }
            break;
        case kUpdateUserOnlineStatus: {
            retrieveRequest->setUrl(kUpdateOnlineStatus);
            json = "{\"online\":\""+isUserWin+"\"}";
            
            retrieveRequest->setTag(StringUtils::format("%i",kUpdateUserOnlineStatus).c_str());
            string headerData = StringUtils::format("authorization: lpt_token %s", authToken.c_str());
            headers.push_back(headerData);
        }
        break;
            
        default:
        break;
    }
    log("%s",json.c_str());
    log("API Called %s", retrieveRequest->getUrl());
    
    retrieveRequest->setRequestType(requestType);
    retrieveRequest->setResponseCallback(CC_CALLBACK_2(LPTServerListener::onHttpRetrieveRequestCompleted,this));
    retrieveRequest->setRequestData(json.c_str(), json.length());
    retrieveRequest->setHeaders(headers);
    HttpClient::getInstance()->send(retrieveRequest);
    retrieveRequest->release();
}

void LPTServerListener::updateUserProfilePic(string email, string profilePic, string authToken, ServerRequest serverRequest) {
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    
    HttpRequest * retrieveRequest = new HttpRequest();
    std::string json;
    
    HttpRequest::Type requestType = HttpRequest::Type::PUT;
    
    switch (serverRequest) {
        case kUpdateUserProfileRequest: {
            retrieveRequest->setUrl(kUpdateUserProfileAPI);
            json = "{\"EMailId\":\""+email+"\",\"profileImage\":\""+profilePic+"\"}";
            retrieveRequest->setTag(StringUtils::format("%i",kUpdateUserProfileRequest).c_str());
            string headerData = StringUtils::format("authorization: lpt_token %s", authToken.c_str());
            headers.push_back(headerData);
        }
            break;
            
        default:
            break;
    }
    log("%s",json.c_str());
    log("API Called %s", retrieveRequest->getUrl());
    retrieveRequest->setRequestType(requestType);
    retrieveRequest->setResponseCallback(CC_CALLBACK_2(LPTServerListener::onHttpRetrieveRequestCompleted,this));
    retrieveRequest->setRequestData(json.c_str(), json.length());
    retrieveRequest->setHeaders(headers);
    HttpClient::getInstance()->send(retrieveRequest);
    retrieveRequest->release();
}

void LPTServerListener::reteriveDataFromServerForGame(string uniqueName, string email, string password, string facebookId, string deviceToken, int loginType, ServerRequest serverRequest) {
    
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    
    HttpRequest * retrieveRequest = new HttpRequest();
    std::string json;
    
    HttpRequest::Type requestType = HttpRequest::Type::POST;
    
    string loginTypeInString = StringUtils::format("%d", loginType);
    deviceToken = UserDefault::getInstance()->getStringForKey(KEY_iOS_DEVICE_TOKEN);
    switch (serverRequest) {
        case kRegisterRequest:
            retrieveRequest->setUrl(kUrlUserRegistered);
            json = "{\"UniqueName\":\""+uniqueName+"\",\"EMailId\":\""+email+"\",\"password\":\""+password+"\",\"fbId\":\""+facebookId+"\",\"deviceToken\":\""+deviceToken+"\",\"loginType\": \""+loginTypeInString+"\"}";
            retrieveRequest->setTag(StringUtils::format("%i",kRegisterRequest).c_str());
            break;
            
        case kLoginRequest:
            retrieveRequest->setUrl(kUserLoginAPI);
            json = "{\"EMailId\":\""+email+"\",\"password\":\""+password+"\",\"deviceToken\":\""+deviceToken+"\"}";
            retrieveRequest->setTag(StringUtils::format("%i",kLoginRequest).c_str());
            break;
            
        case kRecordExist: {
            string updatedURL = StringUtils::format("%s/?fbId=%s", kCheckUserExist, facebookId.c_str());
            retrieveRequest->setUrl(updatedURL);
            json = "";
            retrieveRequest->setTag(StringUtils::format("%i",kRecordExist).c_str());
            requestType =   HttpRequest::Type::GET;
        }
            break;
            
        case kForgetPasswordRequest: {
            retrieveRequest->setUrl(kForgetPassword);
            json = "{\"EMailId\":\""+email+"\"}";
            retrieveRequest->setTag(StringUtils::format("%i",kForgetPasswordRequest).c_str());
        }
            break;
            
        case kLogoutRequest: {
            retrieveRequest->setUrl(kLogoutUserAPI);
            string headerData = StringUtils::format("authorization: lpt_token %s", PLAYER->getPlayerAuthToken().c_str());
            headers.push_back(headerData);
            retrieveRequest->setTag(StringUtils::format("%i",kLogoutRequest).c_str());
        }
            break;
        default:
            break;
            
    }
    
    log("%s",json.c_str());
    log("API Called %s", retrieveRequest->getUrl());
    
    retrieveRequest->setRequestType(requestType);
    retrieveRequest->setResponseCallback(CC_CALLBACK_2(LPTServerListener::onHttpRetrieveRequestCompleted,this));
    retrieveRequest->setRequestData(json.c_str(), json.length());
    retrieveRequest->setHeaders(headers);
    HttpClient::getInstance()->send(retrieveRequest);
    retrieveRequest->release();
}

void LPTServerListener::onHttpRetrieveRequestCompleted(HttpClient *sender,HttpResponse *response) {
    
   
    
    long int statusCode = response->getResponseCode();
    char statusString[64] = {};
    
    sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
    
    std::vector<char> * buffer = response->getResponseData();
    std::string backStr = std::string(&(*buffer->begin()), buffer->size());
    
    if (backStr.length() > 0) {
        picojson::value data;
        picojson::parse(data, backStr.c_str());
        picojson::object object = data.get<picojson::object>();
        log("DATA RECEIVED: %s", backStr.c_str());
        if(object[kStatusCodeKey].get<double>() == SUCCESS) {
            if(atoi(response->getHttpRequest()->getTag()) == kRegisterRequest || atoi(response->getHttpRequest()->getTag()) == kLoginRequest) {
                picojson::object userProfileData = object[kDataKey].get<picojson::object>();
                PLAYER->populatePlayerData(userProfileData);
            }else if(atoi(response->getHttpRequest()->getTag()) == kRecordExist) {
                picojson::object userProfileData = object[kDataKey].get<picojson::object>();
                log(userProfileData[kRecordExistKey].get<bool>());
                PLAYER->setrecordExist(userProfileData[kRecordExistKey].get<bool>());
            }else if (atoi(response->getHttpRequest()->getTag()) == kFetchUserDetails) {
                picojson::object userProfileData = object[kDataKey].get<picojson::object>();
                double totalGamePlayed = userProfileData[FIELD_TOTAL_GAMES].get<double>();
                double totalCoins = userProfileData[FIELD_TOTAL_COINS].get<double>();
                if((totalGamePlayed > PLAYER->getPlayerGamePlayed()) || (totalGamePlayed == PLAYER->getPlayerGamePlayed() && totalCoins >= PLAYER->getTotalCoins()))
                    PLAYER->updateUserProfile(userProfileData);
                else {
                    this->getLPTServerDelegate()->serverResponseComplete(kUpdateGameDataToServer);
                }
            }else if(atoi(response->getHttpRequest()->getTag()) == kUpdateUserProfileRequest) {
                log("DATA RECEIVED");
            }else if (atoi(response->getHttpRequest()->getTag()) == kFetchOpponentDetails) {
                picojson::object userProfileData = object[kDataKey].get<picojson::object>();
                OPPONENT->updateUserProfile(userProfileData);
            }else if(atoi(response->getHttpRequest()->getTag()) == kFetchFriendsDataRequest) {
                picojson::array userProfileData = object[kDataKey].get<picojson::array>();
                if (this->getLPTServerDelegate() != NULL) {
                    this->getLPTServerDelegate()->serverResponseCompletedWithData(userProfileData);
                }
                log("%lu", userProfileData.size());
            }
            
            if (this->getLPTServerDelegate() != NULL) {
                this->getLPTServerDelegate()->serverResponseComplete(atoi(response->getHttpRequest()->getTag()));
            }
        }else {
            if (this->getLPTServerDelegate() != NULL) {
                int responseCode = object["code"].get<double>();
                this->getLPTServerDelegate()->serverListenerResponseFaliure(atoi(response->getHttpRequest()->getTag()), GameUtils::getMessageCode(responseCode));
            }
        }
    }else {
        if (this->getLPTServerDelegate() != NULL) {
           
            this->getLPTServerDelegate()->serverListenerResponseFaliure(atoi(response->getHttpRequest()->getTag()), GameUtils::getMessageCode(NETWORK_CONNECTION));
        }
    }
}

/*
 */
