//
//  GameConstants.h
//  Max_Damage
//
//  Created by Davinder on 19/05/17.
//
//

#ifndef GameConstants_h
#define GameConstants_h

#pragma mark PhysicsInitialisation
#define kPixelToMeterRatio                                                      32.0f
#define DEBUG_DRAW                                                              1

#define ROOM_USER_LIMIT                                                         2

#define BET_0                                                                   0
#define BET_50                                                                  50
#define BET_100                                                                 100
#define BET_500                                                                 500
#define BET_750                                                                 750
#define BET_1000                                                                1000
#define BET_1500                                                                1500
#define BET_2000                                                                2000
#define BET_2500                                                                2500

#pragma mark DeviceSize
#define kWidthiPad                                                              1024
#define kWidthiPhoneHD                                                          960
#define kWidthiPhone5                                                           1136
#define kWidthiPhoneX                                                           2436

#define kHeightiPad                                                             768
#define kHeightiPhoneHD                                                         640
#define kHeightiPhone5                                                          640
#define kHeightiPhoneX                                                          1125

#pragma mark Colors
#define kFontColorWhite                                                         Color3B(255,255,255)

#define APPWARP_SESSION                                                         "AppWarpSessionID"

#define KEY_iOS_DEVICE_TOKEN                                                    "kkeydevicetoken"

#define KEY_FIRST_TIME_RUN                                                      "kkeyFirstTimeRunLPT"

#define keyFIREventName                                                         "FIRAnalyticsEventNameKey"
#define keyFIREventData                                                         "FIRAnalyticsEventDataKey"
#define keyLPTPlayerCountryArea                                                 "LPTPlayerCountryArea"

#pragma mark Fonts
#define kArialFont                                                              "arial.fnt"
#define kOpenSans_Bold                                                          "OpenSans-Bold.ttf"
#define kNOUGAT_EXTRABLACK_1                                                    "NOUGAT-EXTRABLACK_1.TTF"
//Userdefaults keys
#define kAIMode                                                                 "AIMode"
#define kShowLoaderForFirstTime                                                 "ShowLoaderForFirstTime"

#pragma mark LoginScene
//ImageName
#define kMainMenuBG                                                             "mainMenuBG.png"
#define kSignupBG                                                               "signupBG.png"
#define kGamePlayBG                                                             "gamePlayBG.png"
#define kLoginStarsImageName                                                    "loginStars.png"
#define kFBLoginBtnImageName                                                    "fBLoginBtn.png"
#define kEmailLoginBtnImageName                                                 "emailLoginBtn.png"
#define kSignUpBtnImageName                                                     "signUpBtn.png"
#define kGuestBtnImageName                                                      "guestBtn.png"
#define kBackButtonImageName                                                    "backBtn.png"
#define kSubmitButtonImageName                                                  "submitBtn.png"
#define kTextFieldImageName                                                     "textFieldBG.png"
#define kTextFieldTextComponent                                                 "textFieldTextComponent.png"
#define kScreenTitleBaseImage                                                   "screenTitle.png"
#define kGameTitleImageName                                                     "gameTitle.png"

//Constant String



//LoginScene
#define kUserNameText                                                           "User Name"
#define kEMailText                                                              "E-Mail"
#define kEMailUsernameText                                                      "E-Mail / UserName"
#define kPasswordText                                                           "Password"
#define kForgotPasswordText                                                     "Forgot password?"
#define kEmailVerificationText                                                  "emailVerification"
#define kAlertErrorText                                                         "Error!"
#define kAlertMessageText                                                       "Message"
#define kAlertSucessText                                                        "Success!"
#define kPasswordResetText                                                      "A new Password has been sent to your email Id."
#define kGuestInitialText                                                       "Guest_"
#define kUsernameEmpty                                                          "Username cannot be empty."
#define kGameAlert                                                              "Game Alert!!"

#define kGameTitle1                                                             "Le Passe"

//#define kGameTitle2                                                             "Passe"
#define kGameTitle2                                                             "Trappe"

#define kUserExists                                                             "Username already exists."
#define kErrorFB                                                                "Error connecting to facebook."
#define kInvalidEmail                                                           "Please enter valid email id."
#define kSpacesInUsername                                                       "Spaces are not allowed in username."
#define kNoInternetConnection                                                   "\nPlease check your internet connection."
#define kComingSoon                                                             "Coming Soon!!!"
#define kInformation                                                            "Message"
#define kQuitGame                                                               "Do you want to quit current game?"
#define kMessageWin                                                             "You Win!!!"
#define kMessageLose                                                            "You Lose!!!"
#define kInsufficientCoins                                                      "Sorry you do not have\nenough coins for the bet."
#define kCancelButtonText                                                       "Cancel"
#define kShopNowButtonText                                                      "Shop Now"
#define kCoinsAddedToProfile                                                    "\t                 Thank you!!!\nCoins has been added to your profile."
#define kErrorWhilePurchasing                                                   "\t                    Sorry!!!\nThere is some problem while purchasing."
#define kChallengeSent                                                          "Challenge has been sent."
#define kChallengeAlreadySent                                                   "You have already sent\na challenge to this user."
#define kRejectChallengeText                                                    "Your Request has been sent."

#define kNoUserFound                                                            "No user found."
#define kLoadingData                                                            "Loading data..."
#define kNoAvatarSelected                                                       "Please select an avatar."

#define kDisconnectAlertText "You are no longer active player \n Please try again later"

#define kGameTutorialCompletionAlert "Great Job!!!"
#define kGameTutorialCompletionText "                You are all set \n You are ready to play tournament."
#define kGameLogoutErrorText    "You donot have active internet connection, \n         You will lose all your progress. \n\n          Do you still want to continue?"
typedef enum LOGOUT_TYPE {
    kLOGOUT_BUTTON,
    kEMAIL_BUTTON,
    kFACEBOOK_BUTTON,
    kNOT_LOGGED_IN
}LOGOUT_TYPE;

typedef enum STATUS_CODE{
    SUCCESS = 200
    
}STATUS_CODE;

typedef enum API_FAILURE_REASON_CODE{
    DATABASE_ERROR = 3002,
    BAD_REQUEST,
    RECORD_NOT_FOUND,
    USER_NOT_FOUND,
    UPDATION_FAILED,
    UNAUTHORIZE_ACCESS,
    AUTH_TOKEN_WRONG,
    PASSWORD_DO_NOT_MATCH,
    INVALID_CREDENTIAL,
    INVALID_DATA,
    EMAIL_ALREADY_EXIST,
    USER_NAME_ALREADY_EXIST,
    UNIQUE_NAME_LESS_THAN_4,
    FILE_FORMAT_NOT_SUPPORTED,
    SOMETHING_WENT_WRONG,
    USER_ALREADY_EXIST,
    PASSWORD_REQUIRED,
    FBID_REQUIRED,
    LOGOUT_FAILED,
    EMAIL_REQUIRED,
    NOTIFICATION_SENT_FAILED,
    LEVEL_NOT_FOUND,
    INVALID_OTP,
    NAME_REQUIRED,
    PASSWORD_RESET_FAILED,
    REGISTERED_SUCCESSFULLY,
    LOGGED_IN_SUCCESSFULLY,
    PASSWORD_CHANGED,
    PASSWORD_RESET_OTP,
    GAME_XP_UPDATED,
    NOTIFICATION_SENT,
    WORK_IN_PROGRESS,
    NETWORK_CONNECTION,
}API_FAILURE_REASON_CODE;

typedef enum NOTIFICATION_STATUS{
    k_SEND_CHALLENGE,
    k_ACCEPT_CHALLENGE,
    k_REJECT_CHALLENGE,
    k_NONE
}NOTIFICATION_STATUS;

//TextField Length
#define UsernameMaxLength                                                       14


#pragma mark GameScene
//GameScene
#define kMsgLogout                                                              "You've been successfully logged out."
#define kPlayer2                                                                "Player2"
//Tag

#pragma mark GamePlay
//BoardImages
#define kPointHandImageName                                                     "pointHand.png"
#define kCenterTable_1_ImageName                                                "centerTable_1.png"
#define kBoard_1_CornerImageName                                                "board_1_Corner_"
#define kBoardDividerImageName                                                  "boardDivider_"
#define kCorner_1_Extension                                                     "1.png"
#define kCorner_2_Extension                                                     "2.png"
#define kCorner_3_Extension                                                     "3.png"
#define kCorner_4_Extension                                                     "4.png"


//ImageName
#define kHUDBarSpriteImageName                                                  "hudBarSprite.png"
#define kUserProfileTopBarImageName                                             "userProfileTopBar.png"
#define kSideMenuImageName                                                      "sideMenu.png"
#define kPlayerProfileImageName                                                 "playerProfile.png"
#define kStrikerImageName                                                       "striker.png"
#define kTimerBaseSpriteImageName                                               "timerBaseSprite.png"
#define kBetAmountBaseImageName                                                 "betAmountBase.png"
#define kGuestNameBaseImageName                                                 "guestNameBase.png"
//#define kGameBoard_3ImageName                                                   "gameBoard_3.png"
#define kGameCoinChipImageName                                                  "gameCoinChip.png"
#define kRopeSegmentImageName                                                   "ropeSegment.png"
#define kGameSensorImageName                                                    "sensor.png"

#pragma mark GameBoard
#define kGameBoard1                 "gameBoard_1.png"
#define kGameBoard2                 "gameBoard_2.png"
#define kGameBoard3                 "gameBoard_3.png"
#define kGameBoard4                 "gameBoard_4.png"
#define kGameBoard5                 "gameBoard_5.png"
#define kGameBoard6                 "gameBoard_6.png"
#define kGameBoard7                 "gameBoard_7.png"
#define kGameBoard8                 "gameBoard_8.png"
#pragma mark End

//Hud Layer Tags
#define kOpponentProfilePicNameBarTag               2007 // To update opponent name
#define kUserProfilePicNameBarTag                   2008 // To update player name
#define kOpponentProfilePicTag                      2005 // To update opponent profile image
#define kUserProfilePicTag                          2006 // To update player profile image
#define kUserProfilePicButtonTag                    2106 // To update player profile image
#define kOpponentProfilePicButtonTag                2107 // To update opponent profile image
#define kRopeLayerTag                               2009
#define kOppentRopeLayerTag                         2010

//Physics Property

#pragma mark Striker_Physics_Property
#define CategoryBit_Striker                                                     5
#define CollisionBit_Striker                                                    5
#define ContactBit_Striker                                                      5
#define GroupIndex_Striker                                                      1

#pragma mark RopeSegmentPhysicsProperty
#define CategoryBit_Rope                                                        10
#define CollisionBit_Rope                                                       6
#define ContactBit_Rope                                                         6

#define Wall_Category_Bit                                                       255
#define Wall_Collision_Bit                                                      255
#define Wall_Contact_Bit                                                        255

//GameSelection
#pragma mark GameSelectionScreen
#define kGameSelectionBGImageName                                               "gameSelectionBG.png"
#define kGameSelectionTicketName                                                "gameSelectionTicket_"
#define kGameSelectionBetTableName                                              "betSelectTable_"
#define kDifficultySelectionImageName                                           "difficultySelection_"
#define kBetAmountImageName                                                     "betAmount.png"
#define kActiveSelection                                                        "activeSelection.png"
#define kInActiveSelection                                                      "inActiveSelection.png"
#define kGameSelectionBackButtonImageName                                       "gameSelectionBackButton.png"
#define kTutorialImageName                                                      "Tutorial_"
//ValueMapKey

#define kImageName                                                              "imageName"
#define kPriceImageName                                                         "priceImageName"
#define kPriceLabel                                                             "priceLabel"
#define kTableName      "Table"


//TOP BAR
#define kGameTopBarImageName                                                    "gameTopBar.png"
#define kGameTopBarSettingButtonImageName                                       "setting.png"
#define kGameSelectionTopBarCoinDisplayImageName                                "gameSelectionTopBarCoinDisplay.png"
#define kGameSelectionTopBarCoinImageName                                       "topBarCoin.png"
#define kGameSelectionBuyCoinButtonImageName                                    "buyCoinButton.png"

//BackButtonTag
#define kBetSelectionBackButtonTag                                              500000
#define kDifficultySelectionBackButtonTag                                       500001
#define kBackToGameSceneBackButtonTag                                           500002
#define kBackToGameSelectionSceneBackButtonTag                                  500003
#define kBackToSettingSceneBackButtonTag                                        500004
#define kBackToGameSceneFromTutorialButtonTag                                   500005

//UserProfile
#define kPlayerProfileBaseImageName                                             "playerProfileBase.png"
#define kPlayerUniqueNameImageName                                              "playerUniqueName.png"

//GameResult
#define kHomeSceneImageName                                                     "homeScene.png"
#define kRematchImageName                                                       "rematch.png"
#define kResultScreenBaseImageName                                              "resultScreenBase.png"
#define kWinnerBackgroundGlowImageName                                          "winnerBackgroundGlow.png"
#define kWinnerRatingImageName                                                  "winnerRating.png"
#define kYouLooseImageName                                                      "you_lose.png"
#define kYouWinImageName                                                        "you_win.png"
#define kPlayerProfileBoxImageName                                              "playerProfileBox.png"

//Player match making
#define kSearchingForOpponent                                                   "Searching for Opponent"
#define kChooseYourSide                                                         "CHOOSE YOUR BOARD"
#define kSelectSideYouWantToPlay                                                "Select on which side ypu want to play"

#define kMatchingSceneChipCoinTagForUser                                        4007
#define kMatchingSceneChipCoinTagForOpponent                                    4008

#define kBetAmountBoxImageName                                                  "bet_amount_box.png"
#define kBetCoinImageName                                                       "bet_coin.png"
#define kCoinsImageName                                                         "coins.png"
#define kCoinGlowImageName                                                      "coinGlow.png"
#define kVSImageName                                                            "VS.png"
#define kPopupMatchMaking                                                       "popupMatchMaking.png"

//SETTING
#pragma mark GAMESETTING
#define kSideMenuPopUpImageName                                                 "sideMenuPopUp.png"
#define kShopButtonSettingSceneImageName                                        "shopButton.png"
#define kLeaveGameSettingSceneImageName                                         "leaveGameButton.png"
#define kSettingButtonInSettingSceneImageName                                   "settingButton.png"
#define kSettingPopupBaseImageName                                              "settingPopupBase.png"
#define kLogoutButtonImageName                                                  "logoutButton.png"
#define kTutorialPlayButtonImageName                                            "tutorialPlay.png"
#define kSettingSeparaterImageName                                              "settingSeparater.png"
#define kMusicOnOffButtonImageName                                              "musicOnOff.png"

//Store
#define kStorePopupImageName                        "popupStore.png"
#define kStoreItemBoxImageName                      "storeItembox.png"
#define kBuyButtonImageName                         "buy_btn.png"

#define kCancelImageName                "cancelPic.png"

//TAG
#define kLeaveGameButtonTagInSettingScene                                       333334
#define kShopGameButtonTagInSettingScene                                        333335
#define kSettingGameButtonTagInSettingScene                                     333336
//TEXT
#define kLogOutButtonText                                                       "Log Out"
#define kMusicText                                                              "Music"
#define kSoundEffectsText                                                       "Sound Effects"
#define kTutorialText                                                           "Tutorial"
#define kStatusOff                                                              "Off"
#define kStatusOn                                                               "On"
#define kPlayText                                                               "Play"
//GAME
#pragma mark Physics
typedef enum physicsBodyType {
    kUserDataTypeStriker    = 1,
    kUserDataTypeCenterTable,
    kUserDataTypeWall,
    kUserDataTypeDeflector,
    kUserDataTypeRope,
    kUserDataTypeOpponentRope,
    kUserDataTypeSensor
}ballUserDataType;

typedef enum UserPlaySide {
    PLAY_AREA_LEFT,
    PLAY_AREA_RIGHT,
}UserPlaySide;

typedef enum GAME_MODE {
    BEGINNER = 1,
    INTERMEDIATE,
    EXPERT
} GAME_MODE;

typedef enum Popup_Type{
    NORMAL_POPUP = 1111,
    LOGOUT_POPUP,
    GAMEQUIT_POPUP,
    SHOPCOIN_POPUP,
    PLAYER_SIDE
}Popup_Type;

typedef enum GameSelected {
    GAME_WITH_COMPUTER = 1,
    GAME_ONE_ON_ONE,
    GAME_WITH_FRIEND
    
} GameSelected;


enum BetSelected {
    BET_WITH_AI = 0,
    BET_1,
    BET_2,
    BET_3,
    BET_4,
    BET_5,
    BET_6,
    BET_7,
    BET_8
};

//enum PlayerGameStatus {
//    ONLINE = 0,
//    OFFLINE
//};

//Structure
struct StoreItem {
    std::string title;
    std::string buysCoins;
    std::string forPrice;
    std::string coinImageName;
};

struct FriendData {
    std::string userName;
    double coins;
    bool status;
    bool challengeSent;
    std::string profilePic;
};
//NETWORK CALL
typedef enum ServerRequest {
    kRegisterRequest = 0,
    kLoginRequest,
    kFetchUserDetails,
    kFetchOpponentDetails,
    kRecordExist,
    kForgetPasswordRequest,
    kLogoutRequest,
    kUpdateUserProfileRequest,
    kGameModelUpdateRequest,
    kGameOverRequest,
    kFetchFriendsDataRequest,
    kUpdateUserOnlineStatus,
    kUpdateGameDataToServer
}ServerRequest;


//ScreenName
#define kPlayerProfileScreenTitle                                              "Player Profile"
#define kStoreSceneTitle                                                       "Shop"
#define kSettingScreenTitle                                                    "Settings"
#define kFriendsSceneTitle                                                     "Friends"
#define kTutorialSceneTitle                                                    "Tutorial"

//Sounds and Music Name
#define GAME_BACKGROUND_MUSIC                                                   "BackGroundMusic.mp3"
#define COINS_SOUND                                                             "CoinsLoop.mp3"
#define BUTTON_TAP_SOUND                                                        "ButtonTap.mp3"
#define POPUP_SOUND                                                             "PopupAppear.wav"
#define SCENE_CHANGE_SOUND                                                      ""//"PopupAppear.wav"
#define WIN_SOUND                                                               "WinSound.mp3"
#define LOOSE_SOUND                                                             "LooseSound.mp3"
#define STRIKER_WOODEN_SOUND                                                    "StrikerFastCollision.wav"
#define STRIKER_STRIKER_SOUND                                                   "StrikerVsStriker.mp3"


//Notification keys
#define kProfilePicture             "profilepic"
#define kBetSelected                "betselected"
#define kSenderName                 "sendername"
#define kAps                        "aps"
#define kBadge                      "badge"
#define kSound                      "sound"
#define kAlert                      "alert"
#define kNotificationType           "notificationtype"
#define kAcceptChallenge            "AcceptChallenge"
#define kSendChallenge              "SentChallenge"
#define kRejectChallenge            "RejectChallenge"
#endif

/* GameConstants_h */


