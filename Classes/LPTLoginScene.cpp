//
//  LPTLoginScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 11/09/18.
//

#include "LPTLoginScene.hpp"
#include "LPTGameSelectionScene.hpp"


#pragma mark - Macros
#define FB_BUTTON_TAG                   100
#define EMAIL_BUTTON_TAG                101
#define SIGNUP_BUTTON_TAG               102
#define EDITBOX_USERNAME_TAG            103
#define EDITBOX_EMAIL_TAG               104
#define EDITBOX_PASSWORD_TAG            105
#define BACK_BUTTON_TAG                 106
#define SUBMIT_BUTTON_TAG               107
#define GUEST_BUTTON_TAG                108
#define FORGOT_PASSWORD_UI_BUTTON_TAG   109
#define kLoginStarsTag                  110

#define kGuestStartCount                0
#define kGuestCountPending              -1


LOGOUT_TYPE LPTLoginScene::logoutType  =   kNOT_LOGGED_IN;

#pragma mark - Layer life cycle
/**
 createScene : This methods will create scene with Login scene layer.
 */
BaseScene* LPTLoginScene::createScene(LOGOUT_TYPE type){
    auto scene = BaseScene::create();
    auto layer = LPTLoginScene::create();
    logoutType = type;
    scene->addChild(layer);
    return scene;
}

/**
 init : This methods will create layer, setup background its background and also setup login scene UI.
 - retruns : bool, returns true if initialised and false if it is not initialised.
 */
bool LPTLoginScene::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    this->setBackgroundSprite(kMainMenuBG);
    setUploginSceneUI();
    return true;
}

/**
 onEnter : This methods will set listener for Facebook ans sets the current game state.
 */
void LPTLoginScene::onEnter() {
    BaseLayer::onEnter();
    
    LPTServerListener::getInstance()->setLPTServerDelegate(this);
    
    sdkbox::PluginFacebook::setListener(this);
    APP_MANAGER->gameState = HOME_SCENE;
    totalCountOfGuestUsers = kGuestCountPending;
    
    if(logoutType == kFACEBOOK_BUTTON) {
        this->facebookLoginProcessStarted();
    }else if(logoutType == kEMAIL_BUTTON) {
        this->signupUI();
    }
    logoutType = kNOT_LOGGED_IN;
}

/*
 loginWithEmailUI:
 */
void LPTLoginScene::signupUI() {
    this->removeAllChildrenWithCleanup(true);
    this->setBackgroundSprite(kSignupBG);
    setUpLoginUI();
}
/**
 onExit : This methods will be called when user exits the scene.
 */
void LPTLoginScene::onExit() {
    LPTActivityIndicator::stopAnimating(this);
//    LPTServerListener::getInstance()->setLPTServerDelegate(NULL);
    BaseLayer::onExit();
}

#pragma mark - UI setup
/**
 setUploginSceneUI : This methods will set UI and animations on the Login scene.
 */
void LPTLoginScene::setUploginSceneUI() {
    
    APP_MANAGER->gameState = HOME_SCENE;
    Sprite *sprite = Sprite::create(kFBLoginBtnImageName);
    float scaleFactor = this->getContentSize().width * 0.3 / sprite->getContentSize().width;
    
    Button *guestLogin = createButton(kGuestBtnImageName, GUEST_BUTTON_TAG, Vec2(-winSize.width * 0.2, winSize.height * 0.125), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), scaleFactor);
    guestLogin->runAction(EaseIn::create(MoveTo::create(0.5, Vec2(winSize.width * 0.175, winSize.height * 0.125)), 0.5));
    
    
    Button *fbLogin = createButton(kFBLoginBtnImageName, FB_BUTTON_TAG, Vec2(winSize.width * 0.5, -winSize.height * 0.275), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), scaleFactor);
    fbLogin->runAction(EaseIn::create(MoveTo::create(0.5, Vec2(winSize.width * 0.5, winSize.height * 0.125)), 0.5));
    
    Button *signupButton = createButton(kSignUpBtnImageName, SIGNUP_BUTTON_TAG, Vec2(winSize.width * 1.2, winSize.height * 0.125), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), scaleFactor);
    signupButton->runAction(EaseIn::create(MoveTo::create(0.5, Vec2(winSize.width * 0.825, winSize.height * 0.125)), 0.5));
    
    Button *emailButton = createButton(kEmailLoginBtnImageName, EMAIL_BUTTON_TAG, Vec2(winSize.width * 0.5, winSize.height * 0.325), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), 0);
    emailButton->runAction(EaseBounceOut::create(ScaleTo::create(0.5, scaleFactor)));
    
    this->addStars();
    this->addGameTitle();
}

/**
 addStars : This methods will add stars and its animation on login scene.
 */
void LPTLoginScene::addStars() {
    BaseSprite *loginBaseSprite = BaseLayer::createSprite(kLoginStarsImageName, kLoginStarsTag, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height), this, 1);
    loginBaseSprite->setAnchorPoint(Vec2(0.5, 1));
    loginBaseSprite->setScale((this->getContentSize().width * (GameUtils::isIphoneX ? 0.8 : 0.9)) / loginBaseSprite->getContentSize().width);
    loginBaseSprite->runAction(RepeatForever::create(Sequence::create(EaseIn::create(FadeOut::create(2.0), 1.5), EaseOut::create(FadeIn::create(1.7), 1.5), NULL)));
}

/**
 addGameTitle : This methods will add game title.
 */
void LPTLoginScene::addGameTitle() {
    BaseSprite *gameTitle = BaseLayer::createSprite(kGameTitleImageName, -1, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.6), this, 1);
    gameTitle->setScale(0);
    float titleScale = this->getContentSize().width * 0.75 / gameTitle->getContentSize().width;
    gameTitle->runAction(EaseInOut::create(ScaleTo::create(1.0, titleScale), 1.0));
}

/**
 setUpSignUpUI : This methods will set up signup screen UI.
 */
void LPTLoginScene::setUpSignUpUI() {
    float componentScale    =   GameUtils::isIphoneX ? 0.9 : 0.5;
    float backButtonScale   =   GameUtils::isIphoneX ? 1.0 : 0.75;
    
    APP_MANAGER->gameState = SIGNUP_SCENE;
    auto uNameSprite = createEditBox(kTextFieldImageName, Vec2(winSize.width * 0.5, winSize.height * 0.8), Vec2(0.0, 0.0), kUserNameText, EDITBOX_USERNAME_TAG, this, EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS, EditBox::InputMode::SINGLE_LINE, this, componentScale);
    EditBox *uNameTxtField = (EditBox*)uNameSprite->getChildByTag(EDITBOX_USERNAME_TAG);
    uNameTxtField->setMaxLength(UsernameMaxLength);
    createEditBox(kTextFieldImageName, Vec2(winSize.width * 0.5, winSize.height * 0.55), Vec2(0.0, 0.0), kEMailText, EDITBOX_EMAIL_TAG, this, EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS, EditBox::InputMode::SINGLE_LINE, this, componentScale);
    createEditBox(kTextFieldImageName, Vec2(winSize.width * 0.5, winSize.height * 0.3), Vec2(0.0, 0.0), kPasswordText, EDITBOX_PASSWORD_TAG, this, EditBox::InputFlag::PASSWORD, EditBox::InputMode::SINGLE_LINE, this, componentScale);
    
    createButton(kBackButtonImageName, BACK_BUTTON_TAG, Vec2(winSize.width * 0.08, winSize.height * 0.9), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), backButtonScale);
    
    createButton(kSubmitButtonImageName, SUBMIT_BUTTON_TAG, Vec2(winSize.width * 0.5, winSize.height * 0.125), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), componentScale);
}

/**
 setUpLoginUI : This methods will set up user login screen UI.
 */
void LPTLoginScene::setUpLoginUI() {
    float componentScale    =   GameUtils::isIphoneX ? 0.9 : 0.5;
    float backButtonScale   =   GameUtils::isIphoneX ? 1.0 : 0.75;
    
    APP_MANAGER->gameState = LOGIN_SCENE;
    createEditBox(kTextFieldImageName, Vec2(winSize.width * 0.5, winSize.height * 0.675), Vec2(0.0, 0.0), kEMailUsernameText, EDITBOX_EMAIL_TAG, this, EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS, EditBox::InputMode::SINGLE_LINE, this, componentScale);
    auto txtPassword = createEditBox(kTextFieldImageName, Vec2(winSize.width * 0.5, winSize.height * 0.425), Vec2(0.0, 0.0), kPasswordText, EDITBOX_PASSWORD_TAG, this, EditBox::InputFlag::PASSWORD, EditBox::InputMode::SINGLE_LINE, this, componentScale);
    createButtonWithText(kForgotPasswordText, FORGOT_PASSWORD_UI_BUTTON_TAG, Vec2(txtPassword->getPositionX() + txtPassword->getContentSize().width * 0.3 * txtPassword->getScale(), txtPassword->getPositionY() - txtPassword->getContentSize().height * 0.75 * txtPassword->getScale()), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this));
    
    createButton(kBackButtonImageName, BACK_BUTTON_TAG, Vec2(winSize.width * 0.08, winSize.height * 0.9), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), backButtonScale);
    
    createButton(kSubmitButtonImageName, SUBMIT_BUTTON_TAG, Vec2(winSize.width * 0.5, winSize.height * 0.15), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), componentScale);
}

/**
 setupForgotPasswordUI : This methods will set up forgot password screen UI.
 */
void LPTLoginScene::setupForgotPasswordUI() {
    
    float componentScale    =   GameUtils::isIphoneX ? 0.9 : 0.5;
    float backButtonScale   =   GameUtils::isIphoneX ? 1.0 : 0.75;
    
    APP_MANAGER->gameState = FORGOT_PASSWORD;
    createEditBox(kTextFieldImageName, Vec2(winSize.width*0.5, winSize.height * 0.6), Vec2(0.0, 0.0), kEMailText, EDITBOX_EMAIL_TAG, this, EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS, EditBox::InputMode::SINGLE_LINE, this, componentScale);
    
    createButton(kBackButtonImageName, BACK_BUTTON_TAG, Vec2(winSize.width * 0.08, winSize.height*0.9), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), backButtonScale);
    
    createButton(kSubmitButtonImageName, SUBMIT_BUTTON_TAG, Vec2(winSize.width * 0.5, winSize.height*0.45), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), componentScale);
}

/**
 setupCreateUserNameUI : This methods will set up UI of create username screen which comes when user successfully signup from facebook.
 */
void LPTLoginScene::setupCreateUserNameUI() {
    float componentScale    =   GameUtils::isIphoneX ? 0.9 : 0.5;
    float backButtonScale   =   GameUtils::isIphoneX ? 1.0 : 0.75;
    
    APP_MANAGER->gameState = SET_USERNAME;
    auto *uNameSprite = createEditBox(kTextFieldImageName, Vec2(winSize.width*0.5, winSize.height*0.6), Vec2(0.0, 0.0), kUserNameText, EDITBOX_USERNAME_TAG, this, EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS, EditBox::InputMode::SINGLE_LINE, this, componentScale);
    EditBox *uNameTxtField = (EditBox*)uNameSprite->getChildByTag(EDITBOX_USERNAME_TAG);
    uNameTxtField->setMaxLength(UsernameMaxLength);
    
    createButton(kBackButtonImageName, BACK_BUTTON_TAG, Vec2(winSize.width * 0.08, winSize.height*0.9), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), backButtonScale);
    
    createButton(kSubmitButtonImageName, SUBMIT_BUTTON_TAG, Vec2(winSize.width * 0.5, winSize.height*0.45), this, CC_CALLBACK_2(LPTLoginScene::buttonCallback, this), componentScale);
}
#pragma mark - AddingFaceBook
void LPTLoginScene::facebookLoginProcessStarted() {
    std::vector<std::string> permissions;
    permissions.push_back(FB_PERM_READ_EMAIL);
    permissions.push_back(FB_PERM_READ_PUBLIC_PROFILE);
    if(APP_MANAGER->isInternetAvilable) {
        LPTActivityIndicator::startAnimating(this);
        sdkbox::PluginFacebook::login();
    } else {
//        LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
        
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
//        MessageBox(kNoInternetConnection,kAlertErrorText);
    }
}



#pragma mark - Button callback
/**
 buttonCallback : This methods will process button callbacks.
 - param :
    - sender : Button whose calls this method.
    - type : Type of event that has occured.
 */
void LPTLoginScene::buttonCallback(Ref* sender, Widget::TouchEventType type){
    Button *button = (Button*)sender;
    bool needToRunAnimation = false;
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::CANCELED:
            break;
        case Widget::TouchEventType::ENDED:
        {
            SoundManager::playSound(BUTTON_TAP_SOUND);
            switch (button->getTag()) {
                case FB_BUTTON_TAG:{
                    this->facebookLoginProcessStarted();
                }
                    break;
                case EMAIL_BUTTON_TAG:
                    signupUI();
                    needToRunAnimation = true;
                    break;
                case SIGNUP_BUTTON_TAG:
                    this->removeAllChildrenWithCleanup(true);
                    this->setBackgroundSprite(kSignupBG);
                    setUpSignUpUI();
                    needToRunAnimation = true;
                    break;
                case BACK_BUTTON_TAG:
                    PLAYER->setPlayerLoginState(NONE);
                    this->removeAllChildrenWithCleanup(true);
                    this->setBackgroundSprite(kMainMenuBG);
                    setUploginSceneUI();
                    break;
                case SUBMIT_BUTTON_TAG:
                    submitButtonFunctionality();
                    break;
                case GUEST_BUTTON_TAG: {
                    LPTActivityIndicator::startAnimating(this);
                    LPTServerListener::getInstance()->reteriveDataFromServerForGame("", "", "", "", "1", GUEST_USER, kRegisterRequest);
                }
                    break;
                case FORGOT_PASSWORD_UI_BUTTON_TAG:
                    this->removeAllChildrenWithCleanup(true);
                    this->setBackgroundSprite(kSignupBG);
                    setupForgotPasswordUI();
                    needToRunAnimation = true;
                    break;
                default:
                    break;
            }
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
    if(needToRunAnimation)
        this->runAction(Sequence::create(ScaleTo::create(0, 0), EaseBounceOut::create(ScaleTo::create(0.5, 1.0)), NULL));
}

/**
 submitButtonFunctionality : This methods check the game state and process user request accordong to that.
 */
void LPTLoginScene::submitButtonFunctionality() {
    switch (APP_MANAGER->gameState) {
        case SIGNUP_SCENE: {
            EditBox* uNameEditBox = (EditBox*)this->getChildByTag(EDITBOX_USERNAME_TAG)->getChildByTag(EDITBOX_USERNAME_TAG);
            EditBox* emailEditBox = (EditBox*)this->getChildByTag(EDITBOX_EMAIL_TAG)->getChildByTag(EDITBOX_EMAIL_TAG);
            EditBox* passwordEditBox = (EditBox*)this->getChildByTag(EDITBOX_PASSWORD_TAG)->getChildByTag(EDITBOX_PASSWORD_TAG);
            createNewEmailUser(uNameEditBox->getText(), passwordEditBox->getText(), emailEditBox->getText());
        }
            break;
        case LOGIN_SCENE: {
        
            EditBox* emailEditBox = (EditBox*)this->getChildByTag(EDITBOX_EMAIL_TAG)->getChildByTag(EDITBOX_EMAIL_TAG);
            EditBox* passwordEditBox = (EditBox*)this->getChildByTag(EDITBOX_PASSWORD_TAG)->getChildByTag(EDITBOX_PASSWORD_TAG);
            
            authenticateApp42User(emailEditBox->getText(), passwordEditBox->getText());
        }
            break;
        case FORGOT_PASSWORD : {
            EditBox* emailEditBox = (EditBox*)this->getChildByTag(EDITBOX_EMAIL_TAG)->getChildByTag(EDITBOX_EMAIL_TAG);
            LPTActivityIndicator::startAnimating(this);
            LPTServerListener::getInstance()->reteriveDataFromServerForGame("", emailEditBox->getText(), "", " ", "", APP42_USER, kForgetPasswordRequest);            
//            UserService *userServices = App42API::BuildUserService();
//            userServices->GetUserByEmailId(emailEditBox->getText(), app42callback(LPTLoginScene::getuserByEmailResponse, this));
        }
            break;
        case SET_USERNAME:{
            EditBox* userNameEditBox = (EditBox*)this->getChildByTag(EDITBOX_USERNAME_TAG)->getChildByTag(EDITBOX_USERNAME_TAG);
            createUserName(userNameEditBox->getText());
        }
            break;
        default:
            break;
    }
}

#pragma mark - Login and signup methods
/**
 authenticateApp42User : This methods authenticate the user to log in to the game.
 - param :
    - email : Email id of the user.
    - password : Password of the user.
 */
void LPTLoginScene::authenticateApp42User(const char* email,const char* password) {
    auto emailText = GameUtils::removeSubstrsValue(email, " ");
    if(APP_MANAGER->isInternetAvilable) {
        LPTActivityIndicator::startAnimating(this);
        
        LPTServerListener::getInstance()->reteriveDataFromServerForGame("", email, password, " ", "", APP42_USER, kLoginRequest);
        
//        map<string,string>otherMetaHeaders;
//        otherMetaHeaders["emailAuth"] = "true";
//        UserService *uServices = App42API::BuildUserService();
//        uServices->setOtherMetaHeaders(otherMetaHeaders);
//        uServices->Authenticate(emailText.c_str(), password, app42callback(LPTLoginScene::authenticateUserResponse, this));
    } else {
//        LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
        
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
//        MessageBox(kNoInternetConnection,kAlertErrorText);
    }
}

/**
 createNewEmailUser : This methods signs up user. It will take username, email id and password from the user and creates a new APP42 user.
 - param :
    - uName : Unique username of the user.
    - password : Password of the user.
    - emailID : Email id of the user.
 */
void LPTLoginScene::createNewEmailUser(const char* uName, const char* password, const char* emailID) {
    if(GameUtils::findSubstrInString(uName, " ")) {
//        LPTPopUp::showPopUpWith(kAlertErrorText, kSpacesInUsername, false, NORMAL_POPUP);
        
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kSpacesInUsername, false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
//        MessageBox(kSpacesInUsername,kAlertErrorText);
    } else {
        auto uNameText = GameUtils::removeSubstrsValue(uName, " ");
        auto emailText = GameUtils::removeSubstrsValue(emailID, " ");
        if(GameUtils::is_email(emailText)) {
            if(APP_MANAGER->isInternetAvilable) {
                LPTActivityIndicator::startAnimating(this);
                log("Password: %s", password);
                LPTServerListener::getInstance()->reteriveDataFromServerForGame(uName, emailID, password, "", "", APP42_USER, kRegisterRequest);
//                UserService *uServices = App42API::BuildUserService();
//                uServices->CreateUser(uName, password, emailID, app42callback(LPTLoginScene::createNewApp42UserResponse, this));
            } else {
//                LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
//                MessageBox(kNoInternetConnection,kAlertErrorText);
                
                LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
                this->addChild(gamePopup, 10000);
            }
            
        } else {
//            LPTPopUp::showPopUpWith(kAlertErrorText, kInvalidEmail, false, NORMAL_POPUP);
            
            LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kInvalidEmail, false, NORMAL_POPUP);
            this->addChild(gamePopup, 10000);
//            MessageBox(kInvalidEmail, kAlertErrorText);
        }
    }
}

/**
 resetPassword : This methods will send a reset password request to the server.
 - param :
    - uName : Unique username of the user.
 */
void LPTLoginScene::resetPassword(const char* uName) {
    if(APP_MANAGER->isInternetAvilable) {
        LPTActivityIndicator::startAnimating(this);
//        UserService *uServices = App42API::BuildUserService();
//        uServices->ResetUserPassword(uName, app42callback(LPTLoginScene::resetPasswordResponse, this));
    } else {
//        LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
        
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
//        MessageBox(kNoInternetConnection,kAlertErrorText);
    }
    
}

/**
 createUserName : This methods will check for username in the database.
 - param :
    - uName : Unique username of the user.
 */
void LPTLoginScene::createUserName(const char* uName) {
    if(GameUtils::findSubstrInString(uName, " ")) {
//        LPTPopUp::showPopUpWith(kAlertErrorText, kSpacesInUsername, false, NORMAL_POPUP);
        
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kSpacesInUsername, false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
//        MessageBox(kSpacesInUsername, kAlertErrorText);
    } else {
        if(APP_MANAGER->isInternetAvilable) {
            LPTActivityIndicator::startAnimating(this);
            LPTServerListener::getInstance()->reteriveDataFromServerForGame(uName, PLAYER->getPlayerEmailID(), "", PLAYER->getfacebookID(), "", FB_USER, kRegisterRequest);
        } else {
//            LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
            
            LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
            this->addChild(gamePopup, 10000);
            
//            MessageBox(kNoInternetConnection,kAlertErrorText);
        }
    }
}

/**
 saveIntialPlayerData : This methods will creates an entry in USERINFO collection.
 - param :
    - uName : Unique username of the user.
    - emailId : Email id of the user.
    - totalWins : Total wins of the user.
    - totalLose :Total lose of the user.
    - totalCoins : Total coins of the user.
    - isFBUser : True is user has logged in from facebook.
    - isApp42User : True is user is a new sign up.
    - isGuest : True is user is guest user.
 */
void LPTLoginScene::saveIntialPlayerData(const char* uName,const char* emailId, int totalWins, int totalLose, int totalCoins, bool isFBUser, bool isApp42User,bool isGuest) {
    App42Object *obj = new App42Object();
    obj->setObject(FIELD_UNAME, uName);
//    obj->setObject(FIELD_EMAIL, emailId);
    obj->setObject(FIELD_TOTAL_WIN, totalWins);
    obj->setObject(FIELD_TOTAL_LOSE, totalLose);
    obj->setObject(FIELD_TOTAL_COINS, totalCoins);
    obj->setObject(FIELD_GUEST_USER, isGuest);
    obj->setObject(FIELD_FB_USER, isFBUser);
    obj->setObject(FIELD_APP42_USER, isApp42User);
    obj->setObject(FIELD_PROFILE_PICTURE, kPlayerProfileImageName);
//    LPTActivityIndicator::startAnimating(this);
//    StorageService *sServices = App42API::BuildStorageService();
//    sServices->InsertJsonDocument(APPWARP_DB_NAME, COLLECTION_USER, obj, app42callback(LPTLoginScene::intialPlayerSetupResponse, this));
}

/**
 saveFBUserDetails : This methods will creates an entry of facebook user in USERINFO collection.
 */
void LPTLoginScene::saveFBUserDetails() {
    tempFBObj->setObject(FIELD_UNAME, PLAYER->getPlayerUniqueName().c_str());
    createUserLocalSession(fbUserId.c_str(), PLAYER->getPlayerUniqueName().c_str(),PLAYER->getPlayerEmailID().c_str(), FB_USER);
    LPTActivityIndicator::startAnimating(this);
//    StorageService *sServices = App42API::BuildStorageService();
//    sServices->InsertJsonDocument(APPWARP_DB_NAME, COLLECTION_USER, tempFBObj, app42callback(LPTLoginScene::intialPlayerSetupResponse, this));
}

void LPTLoginScene::createGuestCollection() {
    LPTActivityIndicator::startAnimating(this);
    const char* key = FIELD_GUEST_UNIQUE_KEY;
    const char* value = FIELD_GUEST_UNIQUE_VALUE;
    App42Object *obj = new App42Object();
    obj->setObject(FIELD_GUEST_UNIQUE_KEY, FIELD_GUEST_UNIQUE_VALUE);
    obj->setObject(FIELD_GUEST_COUNT, 0);
}


#pragma mark - Saving user session to local storage
/**
 createUserLocalSession : This methods will update the userdefaults.
 - param :
    - sessionId : Session Id of the user. In case of facebook user,it will store its facebook id.
    - userName : Unique username.
    - email : Email id of the user.
    - state : Login state of the user.
 */
void LPTLoginScene::createUserLocalSession(const char* sessionId, const char* userName, const char* email, LOGIN_STATE state) {
    PLAYER->setPlayerSessionID(sessionId); //In case of Fabcebook session id will be facebook id
    PLAYER->setPlayerUniqueName(userName);
    PLAYER->setPlayerEmailID(email);
    PLAYER->setPlayerLoginState(state);
    log("value of the auth token : %s", PLAYER->getPlayerAuthToken().c_str());
    PLAYER->savePlayerAuthToken(PLAYER->getPlayerAuthToken());
}

#pragma mark - App42 callbacks
/**
 createUserNameResponse : This methods will checks if the username entered by facebook user is available or not. If the username already exists, it will asks user to enter new username. If username is unique, it will save the user in the USERINFO collection.
 - param :
    - response : Response of App42 query.
 */
void LPTLoginScene::createUserNameResponse(void *response) {
    LPTActivityIndicator::stopAnimating(this);
    App42StorageResponse *res = (App42StorageResponse*)response;
    log("%d",res->isSuccess);
    log("%s",res->getBody().c_str());
    if (res->isSuccess) {
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kGameAlert, kUserExists, false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
//        MessageBox(kUserExists, "");
    } else {
        
        PLAYER->setPlayerGameWon(0);
        PLAYER->setTotalCoins(500);
        PLAYER->setPlayerEmailID(fbEmailID);
        PLAYER->setPlayerLoginState(NONE);
        PLAYER->setProfilePic(kPlayerProfileImageName);
        saveFBUserDetails();
    }
}

/**
 authenticateUserResponse : This methods will authenticate the user and requests for its corresponding data in the USERINFO collection.
 - param :
    - response : Response of App42 query.
 */
void LPTLoginScene::authenticateUserResponse(void* response) {
//    LPTActivityIndicator::stopAnimating(this);
//    App42UserResponse *res = (App42UserResponse*)response;
//    log("%d",res->isSuccess);
//    log("%s",res->getBody().c_str());
//    if (res->isSuccess) {
//        App42User user = res->users.at(0);
//        fbUserId = user.sessionId.c_str(); //In case of login we are saving sessionid in fbUserId variable
//        LPTActivityIndicator::startAnimating(this);
//        StorageService *sServices = App42API::BuildStorageService();
//        sServices->FindDocumentByKeyValue(APPWARP_DB_NAME, COLLECTION_USER, FIELD_UNAME, user.userName.c_str(), app42callback(LPTLoginScene::fetchExistingUserData, this));
//    }else {
//
//        MessageBox(res->errorDetails.c_str(), kAlertErrorText);
//    }
}

void LPTLoginScene::fetchExistingUserData(void* response) {
//    LPTActivityIndicator::stopAnimating(this);
//    App42StorageResponse *res = (App42StorageResponse*)response;
//    log("%d",res->isSuccess);
//    log("%s",res->getBody().c_str());
//    if (res->isSuccess) {
//        App42Storage storage = res->storages.at(0);
//        JSONDocument doc = storage.jsonDocArray.at(0);
//        PLAYER->updateUserProfile(doc.getJsonDoc().c_str());
//        createUserLocalSession(fbUserId.c_str(), PLAYER->getPlayerUniqueName().c_str(), PLAYER->getEmailId().c_str(), PLAYER->getLoginState());
//        this->moveGameSelectionScene();        
//    }else {
//        MessageBox(res->errorDetails.c_str(), kAlertErrorText);
//    }
}



void LPTLoginScene::facebookUserInfoResposne(void* response) {
    LPTActivityIndicator::stopAnimating(this);
    App42Response *res = (App42Response*)response;
    log("%d",res->isSuccess);
    if (res->isSuccess) {
        this->moveGameSelectionScene();
//        Director::getInstance()->replaceScene(LPTGameSelectionScene::createScene());
    }else {
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, res->errorMessage.c_str(), false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
//        MessageBox(res->errorMessage.c_str(), kAlertErrorText);
    }
}

void LPTLoginScene::getuserByEmailResponse(void* response) {
    App42UserResponse *userResponse = (App42UserResponse*)response;
    LPTActivityIndicator::stopAnimating(this);
    if (userResponse->isSuccess) {
        App42User user = userResponse->users.at(0);
        resetPassword(user.userName.c_str());
    } else {
        
//        MessageBox(userResponse->errorDetails.c_str(), kAlertErrorText);

        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, userResponse->errorDetails.c_str(), false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
    }
}

void LPTLoginScene::resetPasswordResponse(void* response) {
    LPTActivityIndicator::stopAnimating(this);
    App42UserResponse *userResponse = (App42UserResponse*)response;
    printf("\ncode=%d...=%d",userResponse->getCode(),userResponse->isSuccess);
    printf("\nResponse Body=%s",userResponse->getBody().c_str());
    if (userResponse->isSuccess) {
        App42User user = userResponse->users.at(0);
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertSucessText, kPasswordResetText, false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
//        MessageBox(kPasswordResetText, kAlertSucessText);
    } else {
//        LPTPopUp::showPopUpWith(kAlertErrorText, userResponse->errorDetails.c_str(), false, NORMAL_POPUP);
        
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, userResponse->errorDetails.c_str(), false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
//        MessageBox(userResponse->errorDetails.c_str(), kAlertErrorText);
    }
}

void LPTLoginScene::checkForFBLogin(void* response) {
    LPTActivityIndicator::stopAnimating(this);
    App42StorageResponse *res = (App42StorageResponse*)response;
    log("%d",res->isSuccess);
    log("%s",res->getBody().c_str());
    if (res->isSuccess) {
        App42Storage storage = res->storages.at(0);
        JSONDocument doc = storage.jsonDocArray.at(0);
        picojson::value v;
        picojson::parse(v, doc.getJsonDoc().c_str());
//        const char *jsonString = doc.getJsonDoc().c_str();
        picojson::object object = v.get<picojson::object>();
        string username = object[FIELD_FB_USERNAME].get<string>();
        createUserLocalSession(fbUserId.c_str(), username.c_str(), fbEmailID.c_str(), FB_USER);
        this->moveGameSelectionScene();
//        Director::getInstance()->replaceScene(LPTGameSelectionScene::createScene());
    } else {
        this->removeAllChildrenWithCleanup(true);
        this->setBackgroundSprite(kMainMenuBG);
        setupCreateUserNameUI();
    }
}

#pragma mark MOVE_TO_GAMESELECTION
void LPTLoginScene::moveGameSelectionScene() {
    SoundManager::playSound(SCENE_CHANGE_SOUND);
    Director::getInstance()->replaceScene(TransitionFlipX::create(0.25, LPTGameSelectionScene::createScene()));
}

#pragma mark *********Facebook Login Listener**************************
void LPTLoginScene::onLogin(bool isLogin, const std::string& msg) {
    if(!isLogin) {
        PLAYER->setPlayerLoginState(NONE);
        LPTActivityIndicator::stopAnimating(this);
//        LPTPopUp::showPopUpWith(kAlertErrorText, msg.c_str(), false, NORMAL_POPUP);
        
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, msg.c_str(), false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
        
//        MessageBox(msg.c_str(), kAlertErrorText);
    } else {
        FBAPIParam params;
        params["fields"] = "email, first_name, picture, id";
        /*fetch these on login*/
        PluginFacebook::api("me", "GET", params, "me");
    }
}


void LPTLoginScene::onAPI(const std::string& key, const std::string& jsonData) {
    log("jsonData = %s",jsonData.c_str());
    picojson::value v;
    picojson::parse(v, jsonData);
    picojson::object object = v.get<picojson::object>();
    string email = "";
    
    if(GameUtils::findSubstrInString(jsonData, "\"email\"")) {
        email = object["email"].get<string>();
    }
    
    fbUserId = object["id"].get<string>();
    fbEmailID = email;
    
    PLAYER->setPlayerEmailID(fbEmailID);
    PLAYER->setfacebookID(fbUserId.c_str());
    
    LPTServerListener::getInstance()->reteriveDataFromServerForGame("1", fbEmailID, "test", fbUserId, "1", FB_USER, kRecordExist);
}

void LPTLoginScene::onPermission(bool isLogin, const std::string& msg) {
    
}

void LPTLoginScene::onFetchFriends(bool ok, const std::string& msg) {
    
}

void LPTLoginScene::onRequestInvitableFriends( const FBInvitableFriendsInfo& friends ) {
    
}

void LPTLoginScene::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg ) {
    
}

void LPTLoginScene::onInviteFriendsResult( bool result, const std::string& msg ) {
    
}

void LPTLoginScene::onGetUserInfo( const FBGraphUser& userInfo ) {
    LPTActivityIndicator::stopAnimating(this);
}

#pragma mark Delegate Method
void LPTLoginScene::serverResponseComplete(int tag) {
    LPTActivityIndicator::stopAnimating(this);
    if(tag == kRegisterRequest || tag == kLoginRequest) {        
        createUserLocalSession(PLAYER->getPlayerSessionID().c_str(), PLAYER->getPlayerUniqueName().c_str(), PLAYER->getPlayerEmailID().c_str(), (LOGIN_STATE)PLAYER->getPlayerLoginState());
        this->moveGameSelectionScene();
    }else if(tag == kRecordExist) {
        if(PLAYER->getrecordExist()) {
            LPTActivityIndicator::startAnimating(this);
            LPTServerListener::getInstance()->reteriveDataFromServerForGame("", fbEmailID, "", PLAYER->getfacebookID(), "", FB_USER, kRegisterRequest);
        }else {
            this->removeAllChildrenWithCleanup(true);
            this->setBackgroundSprite(kMainMenuBG);
            setupCreateUserNameUI();
        }
    }else if(tag == kForgetPasswordRequest) {
        
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kForgotPasswordText, kPasswordResetText, false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
//        LPTPopUp::showPopUpWith(kForgotPasswordText, kPasswordResetText, false, NORMAL_POPUP);
//        MessageBox(kPasswordResetText, kForgotPasswordText);
    }
}

void LPTLoginScene::serverListenerResponseFaliure(int tag, string message){
    LPTActivityIndicator::stopAnimating(this);
    LPTPopUp *pop =  LPTPopUp::showPopUpWith(kAlertErrorText, message, false, NORMAL_POPUP);
    this->addChild(pop, 50000000);
    PLAYER->setPlayerLoginState(NONE);
}

void LPTLoginScene::serverResponseCompletedWithData(picojson::array data) {
}
