//
//  GameUtils.hpp
//  Max_Damage
//
//  Created by Davinder on 19/05/17.
//
//

#ifndef GameUtils_hpp
#define GameUtils_hpp

#include <stdio.h>
#include <cocos2d.h>
#include "GameConstants.h"

using namespace std;
using namespace cocos2d;

struct GameSceneUIData {
    int RopeTag;
    UserPlaySide playingArea;
    int indexOfStriker;
};

enum PLAYER_STATE{
    IS_PLAYING,
    HAS_WON,
    HAS_LOST
};

enum DEVICE_TYPE{
    iPhoneX = 1010,
    iPhone = 1011,
    iPad = 1012,
    Android = 1013
};

using namespace std;
using namespace cocos2d;

class LPTTopBar;
class BaseLayer;
class GameUtils
{
public:
    
    static bool isIphone;
    static bool isIpad;
    static bool isIphoneX;
    static bool isAndroid;
    static struct tm * getCurrentTime();
    static struct tm * getDiffrenceBetweenTimes(struct tm * t1,struct tm * t2);
    static bool is_email(std::string address);
    static DEVICE_TYPE getDeviceType();
    static void removeSubstrs(string& s, string& p);
    static string removeSubstrsValue(string str, string subStr);
    static void replaceCharWithChar(string mainStr,char x,char y);
    static string random_string( size_t length );
    static bool findSubstrInString(string str, string subStr);
    static bool isInternetConnected();
    static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
    static LPTTopBar* createTopBar(BaseLayer *layer, string imageName);
    static string getBotName();
    static string getBotProfilePictureName();
    static double getUserBetValueFromSelectedBet(BetSelected bet);
    static BetSelected getUserBetValueToSelectedBet(int bet);
    static int convertStringToInt(string strValue);
    static double convertStringToDouble(string strValue);
    static string upperCase(string input);
    static string lowerCase(string input);
    static string getBackgroundSoundNameForScene();
    static string getIAPPurchaseName(int index);
    static double getPurchasedCoinsFromIAPName(string iapName);
    static int lengthOfData(unsigned char * str);
    static int getRandomIntegerNumber(int minNumber, int maxNumber);
    static double getPlayerMaxXP(int level);
    static string getMessageCode(int messageCode);
};

#endif /* GameUtils_hpp */
