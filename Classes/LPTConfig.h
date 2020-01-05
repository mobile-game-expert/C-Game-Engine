//
//  LPTConfig.h
//  LePasseTrappe
//
//  Created by Davinder Singh on 12/09/18.
//

#ifndef LPTConfig_h
#define LPTConfig_h

#define APPWARP_API_KEY         "686f55b53f2c1db740d156b87373fc0a108ac7e8b75a6527eb996a313a1dc323"
#define APPWARP_SECRET_KEY      "f2de3615567796b59596c03578bf765a74c5810f3e747bd6a51c6f2ac452a03d"
#define APPWARP_DB_NAME         "LPTDATABASE"
#define COLLECTION_USER         "USERINFO"
#define COLLECTION_FBUSERS      "FBUSERINFO"
#define COLLECTION_GUESTUSERCOUNT   "GUESTUSERCOUNT"

struct LPTFixtureData {
    // ...other properties
    int tag;
};


//FB users database fields
#define FIELD_FB_ID                 "fbId"
#define FIELD_FB_USERNAME           "fbUsername"
#define FIELD_FB_EMAIL_ID           "fbEmail"

//Guest user database fields
#define FIELD_GUEST_COUNT           "GuestCount"
#define FIELD_GUEST_UNIQUE_KEY      "UserType"
#define FIELD_GUEST_UNIQUE_VALUE    "Guest"
#define FIELD_CURRENT_GUEST_COUNT   "CurrentGuestCount"

//App42 error code
#define KEY_VALUE_NOT_FOUND         2601

//App42 Custom code Name
#define APP42_CustomCode_Guest_login    "App42GuestRoutine"//"App42GuestRoutine2"
//IAP KEY
#define BUY_COINS_200               "BUY_COINS_200"
#define BUY_COINS_350               "BUY_COINS_350"
#define BUY_COINS_500               "BUY_COINS_500"
#define BUY_COINS_700               "BUY_COINS_700"
#define BUY_COINS_900               "BUY_COINS_900"
#define BUY_COINS_1000              "BUY_COINS_1000"

#endif /* LPTConfig_h */
