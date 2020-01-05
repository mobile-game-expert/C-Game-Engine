//
//  GameUtils.cpp
//  Max_Damage
//
//  Created by Davinder on 19/05/17.
//
//

#include "GameUtils.hpp"
#include <curl/curl.h>
#include "BaseLayer.hpp"
#include "LPTTopBar.hpp"

bool GameUtils::isIpad = false;
bool GameUtils::isIphone = false;
bool GameUtils::isIphoneX = false;
bool GameUtils::isAndroid = false;

#pragma mark string utility methods

struct tm * GameUtils::getCurrentTime() {
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    return timeinfo;
}

struct tm * GameUtils::getDiffrenceBetweenTimes(struct tm * t1,struct tm * t2) {
    time_t rawTime_1 = mktime(t1);
    time_t rawTime_2 = mktime(t2);
    time_t diffTime = rawTime_2 - rawTime_1;
    struct tm * timeinfo;
    time (&diffTime);
    timeinfo = localtime (&diffTime);
    return timeinfo;    
}

bool GameUtils::is_email(std::string address) {
    //== check if string contains @ and .
    size_t at_index = address.find_first_of('@', 0);
    return at_index != std::string::npos
    && address.find_first_of('.', at_index) != std::string::npos;
}

void GameUtils::removeSubstrs(string& s, string& p) {
    string::size_type n = p.length();
    for (string::size_type i = s.find(p);
         i != string::npos;
         i = s.find(p))
    {
        s.erase(i, n);
        
    }
}
string GameUtils::removeSubstrsValue(string str, string subStr) {
    string temp = str;
    string::size_type n = subStr.length();
    for (string::size_type i = temp.find(subStr); i != string::npos; i = temp.find(subStr)) {
        temp.erase(i, n);

    }
    return temp;
}

void GameUtils::replaceCharWithChar(string mainStr,char x,char y){
    std::string s = mainStr;
    std::replace( s.begin(), s.end(), 'x', 'y'); // replace all 'x' to 'y'
}

string GameUtils::random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

bool GameUtils::findSubstrInString(string str, string subStr) {
    int result = str.find(subStr.c_str());
    if(result < 0 ) {
        return false;
    } else {
        return true;
    }
}

int GameUtils::lengthOfData(unsigned char * str)
{
    int i = 0;
    while(*(str++)){
        i++;
        if(i == INT_MAX)
            return -1;
    }
    return i;
}

bool GameUtils::isInternetConnected(){
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);// Callback function to get Body of HTML page
        curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L);// To Disable Verbose Messages to default Standered Output
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK)
            return true;
        else
            return false;
    }
    else
        return false;
}
size_t GameUtils::write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

LPTTopBar* GameUtils::createTopBar(BaseLayer *baseLayer, string imageName) {
    LPTTopBar *layer = LPTTopBar::createLayer(imageName);
    layer->setPosition(Vec2(0, baseLayer->getContentSize().height));
    layer->setAnchorPoint(Vec2(0, 1));
    baseLayer->addChild(layer, 2);
    return layer;
}

DEVICE_TYPE GameUtils::getDeviceType() {
    DEVICE_TYPE dTpye = iPhone;
    if (isIphoneX) {
        dTpye = iPhoneX;
    }else if(isIphone) {
        dTpye = iPhone;
    }else if(isIpad) {
        dTpye = iPad;
    }else if(isAndroid) {
        dTpye = Android;
    }
    return dTpye;
}

string GameUtils::getBotName() {
    string arrayNames[12] = {"Lara", "Davinder", "John", "Jane", "Pragati", "Michael", "Peter", "Ryan", "Michael", "Deep", "Lisa",  "Vinod"};
    int randonIndex= arc4random() % 12;
    return arrayNames[randonIndex];
}

string GameUtils::getBotProfilePictureName() {
    int randonIndex= arc4random() % 5;
    return StringUtils::format("avatar_%d.png",randonIndex);
}

double GameUtils::getUserBetValueFromSelectedBet(BetSelected bet) {
    {
        switch (bet) {
            case BET_WITH_AI:
                return BET_0;
                break;
            case BET_1:
                return BET_50;
                break;
            case BET_2:
                return BET_100;
                break;
            case BET_3:
                return BET_500;
                break;
            case BET_4:
                return BET_750;
                break;
            case BET_5:
                return BET_1000;
                break;
            case BET_6:
                return BET_1500;
                break;
            case BET_7:
                return BET_2000;
                break;
            case BET_8:
                return BET_2500;
                break;
                
        }
    }
}

BetSelected GameUtils::getUserBetValueToSelectedBet(int bet) {
    switch (bet) {
        case BET_0:
            return BET_WITH_AI;
            break;
        case BET_50:
            return BET_1;
            break;
        case BET_100:
            return BET_2;
            break;
        case BET_500:
            return BET_3;
            break;
        case BET_750:
            return BET_4;
            break;
        case BET_1000:
            return BET_5;
            break;
        case BET_1500:
            return BET_6;
            break;
        case BET_2000:
            return BET_7;
            break;
        case BET_2500:
            return BET_8;
            break;
    }
}

int GameUtils::convertStringToInt(string strValue) {
    return atoi(strValue.c_str());
}

double GameUtils::convertStringToDouble(string strValue) {
    return stod(strValue);
}

string GameUtils::upperCase(string input) {
    for (std::string::iterator it = input.begin(); it != input.end(); ++ it)
        *it = toupper(*it);
    return input;
}

string GameUtils::lowerCase(string input) {
    for (std::string::iterator it = input.begin(); it != input.end(); ++ it)
        *it = tolower(*it);
    return input;
}

string GameUtils::getBackgroundSoundNameForScene() {
    switch (APP_MANAGER->gameState) {
        case GAME_SCENE:
        case SIGNUP_SCENE:
        case LOGIN_SCENE:
        case FB_LOGIN_SCENE:
        case HOME_SCENE:
        case STORE_SCENE:
        case FORGOT_PASSWORD:
        case SET_USERNAME:
        case GAME_SELECTION:
        case BET_SELECTION:
        case DIFFICULTY_SELECTION:
        case PLAYER_MATCHING:
        case TUTORIAL_SCENE:
        default:
            return GAME_BACKGROUND_MUSIC;
            break;
    }
}

string GameUtils::getIAPPurchaseName(int index) {
    switch (index) {
        case 0:
            return BUY_COINS_200;
            break;
        case 1:
            return BUY_COINS_350;
            break;
        case 2:
            return BUY_COINS_500;
            break;
        case 3:
            return BUY_COINS_700;
            break;
        case 4:
            return BUY_COINS_900;
            break;
        case 5:
            return BUY_COINS_1000;
            break;
        default:
            return "";
            break;
    }
}

double GameUtils::getPurchasedCoinsFromIAPName(string iapName) {
    
    if(iapName.compare(BUY_COINS_200)) {
        return 200.0;
    } else if(iapName.compare(BUY_COINS_350)) {
        return 350.0;
    } else if(iapName.compare(BUY_COINS_500)) {
        return 500.0;
    } else if(iapName.compare(BUY_COINS_700)) {
        return 700.0;
    } else if(iapName.compare(BUY_COINS_900)) {
        return 900.0;
    } else if(iapName.compare(BUY_COINS_1000)) {
        return 1000.0;
    }
    return 0.0;
}
#pragma mark RandomNumber
int GameUtils::getRandomIntegerNumber(int minNumber, int maxNumber) {
    int range = maxNumber - minNumber + 1;
    int num = rand() % range + minNumber;
    return num;
}

#pragma mark PlayerMaxXP
double GameUtils::getPlayerMaxXP(int level) {
    double maxXPForLevel =  APP_MANAGER->levelUpUpperBound;//2100;
    maxXPForLevel = maxXPForLevel + (level * APP_MANAGER->levelUpDelta);//2100 * level * 0.1 + 2100;
    return maxXPForLevel;
}

#pragma mark MessageCode
string GameUtils::getMessageCode(int messageCode) {
    string message = kNoInternetConnection;
    API_FAILURE_REASON_CODE code = (API_FAILURE_REASON_CODE)messageCode;

    switch (code) {
        case INVALID_OTP:
            message = "";
            break;
            
        case LEVEL_NOT_FOUND:
        case FBID_REQUIRED:
        case EMAIL_REQUIRED:
        case NAME_REQUIRED:
        case FILE_FORMAT_NOT_SUPPORTED:
        case INVALID_DATA:
        case AUTH_TOKEN_WRONG:
        case UNAUTHORIZE_ACCESS:
        case UPDATION_FAILED:
            message = "Please login again to sync data from server.";
            break;
            
        case REGISTERED_SUCCESSFULLY:
        case LOGGED_IN_SUCCESSFULLY:
        case PASSWORD_CHANGED:
        case PASSWORD_RESET_OTP:
        case GAME_XP_UPDATED:
        case NOTIFICATION_SENT:
            message = "Data saved successfully.";
            break;
            
        case DATABASE_ERROR:
        case WORK_IN_PROGRESS:
        case PASSWORD_RESET_FAILED:
        case LOGOUT_FAILED:
        case NOTIFICATION_SENT_FAILED:
        case SOMETHING_WENT_WRONG:
        case BAD_REQUEST:
            
            message = "We are unable to conenct Database. \n\n       Please try again later.";
            break;
            
        case RECORD_NOT_FOUND:
        case USER_NOT_FOUND:
            message = "Please enter valid name and email.";
            break;
            
        case USER_NAME_ALREADY_EXIST:
        case USER_ALREADY_EXIST:
            message = "This username is already taken \n   Please enter new username";
            break;
            
        case EMAIL_ALREADY_EXIST:
            message = "This email-ID is already taken \n   Please enter new email-ID";
            break;
            
        case PASSWORD_DO_NOT_MATCH:
        case PASSWORD_REQUIRED:
            message = "Password you enter does not matched \n         Try again.";
            break;
        case INVALID_CREDENTIAL:
            message = "Email-ID and Password does not matched";
            break;
            
        case NETWORK_CONNECTION:
            message = kNoInternetConnection;
            break;
            
        default:
            message = kNoInternetConnection;
            break;
    }
    return message;
}
