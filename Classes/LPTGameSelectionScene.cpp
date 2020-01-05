//
//  LPTGameSelectionScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 24/09/18.
//

#include "LPTGameSelectionScene.hpp"
#include "LPTTopBar.hpp"
#include "LPTBetSelectionScene.hpp"
#include "LPTDifficultySelection.hpp"
#include "LPTInteractiveTutorialScene.hpp"
#include "LPTEvents.hpp"

#define kMaxNumberOfTicket                      3
#define kTopBarTag                              101
#define kTagUserAgelayer                        10101
#define EDITBOX_AGE_TAG                         10102
#define SUBMIT_AGE_BUTTON_TAG                   10103

#pragma mark - Layer Life Cycle
BaseScene* LPTGameSelectionScene::createScene() {
    APP_MANAGER->gameState = GAME_SELECTION;
    auto scene = BaseScene::create();
    auto layer = LPTGameSelectionScene::create();
    scene->addChild(layer);
    return scene;
}

bool LPTGameSelectionScene::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    //--
    ValueMap valueMap;
    valueMap["LPTuserName"] = PLAYER->getPlayerUniqueName();
    Value parameters = Value(valueMap);
    sendMessageWithParams("setUserIdFIRAnalytics", parameters);
    //--
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setConnectionRequestListener(this);
    warpClientRef->setNotificationListener(this);
    warpClientRef->setRoomRequestListener(this);
    warpClientRef->setZoneRequestListener(this);
    
    /*register current device with current user name*/
    APP_MANAGER->registerDeviceToken();
    //===Delete Room
    if (PLAYER->isAdmin) {
        warpClientRef->deleteRoom(PLAYER->getRoomId());
    }else {
        warpClientRef->leaveRoom(PLAYER->getRoomId());
    }
    //--log user location
    LPTEvents::logUserLocation();
    return true;
}

void LPTGameSelectionScene::onEnter() {
    BaseLayer::onEnter();
    LPTEvents::logAppOpenTime();
    this->removeAllChildrenWithCleanup(true);
    this->setBackgroundSprite(kGameSelectionBGImageName);
    setupUI();
    
//    if(APP_MANAGER->isInternetAvilable || UserDefault::getInstance()->getBoolForKey(kShowLoaderForFirstTime)) {
////        checkForValidSession();
//    } else {
//        auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
//        popup->setPopUpDelegate(this);
//        this->addChild(popup, 5000);
//    }
    //
    //SYNC DATA
    if(APP_MANAGER->isInternetAvilable && APP_MANAGER->getDataSyncStatus()) {
        LPTActivityIndicator::startAnimating(this);
        LPTServerListener::getInstance()->setLPTServerDelegate(this);
        LPTServerListener::getInstance()->setLPTServerDelegate(this);
        LPTServerListener::getInstance()->fetchPlayerData(PLAYER->getPlayerUniqueName().c_str(), PLAYER->getPlayerEmailID().c_str(), "", PLAYER->getPlayerAuthToken().c_str(), kFetchUserDetails);
    }
    this->addTopBarInGameSelectionScene();
    if (!UserDefault::getInstance()->getBoolForKey(KEY_FIRST_TIME_RUN,false)) {
        addUserAgePanel();
    }
}

void LPTGameSelectionScene::onExit() {
    LPTServerListener::getInstance()->setLPTServerDelegate(NULL);
    BaseLayer::onExit();
}

#pragma mark - TOPBAR
void LPTGameSelectionScene::addTopBarInGameSelectionScene() {
    LPTTopBar *layer = GameUtils::createTopBar(this, kGameTopBarImageName);
    layer->setTag(kTopBarTag);
    log("profile pic - %s", PLAYER->getPlayerProfilePicture().c_str());
    layer->addGameSelectionSceneTopBarUI(PLAYER->getPlayerProfilePicture(), 2000);
    layer->addProgressBar(PLAYER->getPlayerGameXP(), GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel()));
}

#pragma mark - UI setup
void LPTGameSelectionScene::setupUI() {
    LPTHorizontalScrollView *tableLayer = LPTHorizontalScrollView::createLayer();
    
    //GameSelection
    vector<ValueMap> imagesName;
    for(int imageCounter = 1; imageCounter <= kMaxNumberOfTicket; imageCounter++) {
        string imageName = StringUtils::format("%s%d.png", kGameSelectionTicketName, imageCounter);
        ValueMap valueMap;
        valueMap[kImageName] = imageName;
        imagesName.push_back(valueMap);
    }
    
    //GameSelection
    tableLayer->setupUI(imagesName, Vec2::ZERO, Size(this->getContentSize().width, this->getContentSize().height * 0.85));
    tableLayer->setSelectionLayerDelegate(this);
    this->addChild(tableLayer);
    
    
 /*   //temp code for notification
    LPTOpponent *opponent = new LPTOpponent();
    opponent->setUniqueName(GameUtils::getBotName());
    opponent->setTotalWins(10);
    opponent->setTotalLose(10);
    opponent->setCoins(500);
    opponent->setEmailId("");
    opponent->setProfilePic(GameUtils::getBotProfilePictureName());
    opponent->setBetSelected(BET_3);
    LPTNotificationPopup *popup = LPTNotificationPopup::createLayer(opponent);
    popup->setPosition(Vec2(this->getContentSize().width * 0.1, this->getContentSize().height * 1.2));
    this->addChild(popup, 200);
    popup->runAction(MoveTo::create(1, Vec2(this->getContentSize().width * 0.1, this->getContentSize().height * 0.8))); */
}

#pragma mark - Button callback
void LPTGameSelectionScene::buttonCallback(Ref* sender, Widget::TouchEventType type) {
    Button *button = (Button*)sender;
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::CANCELED:
            break;
        case Widget::TouchEventType::ENDED:
        {
            SoundManager::playSound(BUTTON_TAP_SOUND);
            switch (button->getTag()) {
                case SUBMIT_AGE_BUTTON_TAG:
                {
                    LayerColor *userAgeLayer = (LayerColor*)this->getChildByTag(kTagUserAgelayer);
                    auto ageEditBoxSp = userAgeLayer->getChildByTag(EDITBOX_AGE_TAG);
                    EditBox *ageEditBox = (EditBox*)ageEditBoxSp->getChildByTag(EDITBOX_AGE_TAG);
                    string  ageValue = StringUtils::format("%s",ageEditBox->getText());
                    ageValue = GameUtils::removeSubstrsValue(ageValue.c_str()," ");
                    if(ageValue.length() && (atoi(ageValue.c_str()) > 0 && atoi(ageValue.c_str()) < 100)){
                        UserDefault::getInstance()->setBoolForKey(KEY_FIRST_TIME_RUN, true);
                        LPTEvents::logUserAge(ageValue);
                        PLAYER->setPlayerAge(ageValue);
                        this->removeUserAgePanel();
                    }else {
                        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertMessageText, "Please enter valid age number", false, NORMAL_POPUP);
                        this->addChild(gamePopup, 10000);
                    }
                    
                }
                    break;
                default:
                    break;
            }
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
}

#pragma mark - DelegateMethod
void LPTGameSelectionScene::tableCellSelectedWithIndex(int index) {
    SoundManager::playSound(BUTTON_TAP_SOUND);
    switch(index) {
        case GAME_ONE_ON_ONE : /*{
            auto popup = LPTPopUp::showPopUpWith(kInformation, kComingSoon, false, NORMAL_POPUP);
            popup->setPopUpDelegate(this);
            this->addChild(popup, 5000);
        }
            break;*/
            
        case GAME_WITH_FRIEND:
            SoundManager::playSound(SCENE_CHANGE_SOUND);
            PLAYER->setGameSelected((GameSelected)index);
            SoundManager::playSound(SCENE_CHANGE_SOUND);
            
            if(APP_MANAGER->getTutorialShownStatus()) {
                Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3,  LPTBetSelectionScene::createScene()));
            }else {
                Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, LPTInteractiveTutorialScene::createScene()));
            }
            
         
            break;
            
        case GAME_WITH_COMPUTER:
            PLAYER->setGameSelected((GameSelected)index);
            PLAYER->setBetSelected(BET_WITH_AI);
            OPPONENT->createProfileForBotWith(GameUtils::getBotName(), GameUtils::getBotProfilePictureName());
            SoundManager::playSound(SCENE_CHANGE_SOUND);
            
            if(APP_MANAGER->getTutorialShownStatus()) {
                Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3,  LPTDifficultySelection::createScene()));
            }else {
                Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, LPTInteractiveTutorialScene::createScene()));
            }
            
            break;
    }
}


#pragma mark - checkForValidSession
void LPTGameSelectionScene::checkForValidSession() {
    switch (PLAYER->getPlayerLoginState()) {
        case APP42_USER:
        case GUEST_USER: {
            if(UserDefault::getInstance()->getBoolForKey(kShowLoaderForFirstTime)) {
                LPTActivityIndicator::startAnimating(this);
            }
            LPTServerListener::getInstance()->setLPTServerDelegate(this);
            LPTServerListener::getInstance()->fetchPlayerData(PLAYER->getPlayerUniqueName().c_str(), PLAYER->getPlayerEmailID().c_str(), "", PLAYER->getPlayerAuthToken().c_str(), kFetchUserDetails);
            
//            StorageService *sServices = App42API::BuildStorageService();
//            sServices->FindDocumentByKeyValue(APPWARP_DB_NAME, COLLECTION_USER, FIELD_UNAME, LPTPlayer::getPlayerUniqueName().c_str(), app42callback(LPTGameSelectionScene::fetchExistingUserData, this));
        }
            
            break;
        case FB_USER:
            if (sdkbox::PluginFacebook::isLoggedIn()) {
                if(UserDefault::getInstance()->getBoolForKey(kShowLoaderForFirstTime)) {
                    LPTActivityIndicator::startAnimating(this);
                }
//                StorageService *sServices = App42API::BuildStorageService();
//                sServices->FindDocumentByKeyValue(APPWARP_DB_NAME, COLLECTION_USER, FIELD_UNAME, LPTPlayer::getPlayerUniqueName().c_str(), app42callback(LPTGameSelectionScene::fetchExistingUserData, this));
                LPTServerListener::getInstance()->setLPTServerDelegate(this);
                LPTServerListener::getInstance()->fetchPlayerData(PLAYER->getPlayerUniqueName().c_str(), PLAYER->getPlayerEmailID().c_str(), "", PLAYER->getPlayerAuthToken().c_str(), kFetchUserDetails);
            }  else {
                sdkbox::PluginFacebook::logout();
//                LPTPlayer::logoutUser();
                UserDefault::getInstance()->setBoolForKey(kShowLoaderForFirstTime, true);
                this->moveToLoginScene(kNOT_LOGGED_IN);
                //        Director::getInstance()->replaceScene(LPTLoginScene::createScene());
            }
            break;
        default:
            this->moveToLoginScene(kNOT_LOGGED_IN);
//            Director::getInstance()->replaceScene(LPTLoginScene::createScene());
            break;
    }
}
#pragma mark logoutUser
//void LPTGameSelectionScene::logoutUser() {
//    switch (PLAYER->getPlayerLoginState()) {
//        case APP42_USER: {
//            LPTActivityIndicator::startAnimating(this);
//            SessionService *sessionService = App42API::BuildSessionService();
//            sessionService->Invalidate(PLAYER->getPlayerSessionID().c_str(), app42callback(LPTGameSelectionScene::invalidateSessionResponse, this));
//        }
//
//            break;
//        case FB_USER:
//            LPTActivityIndicator::startAnimating(this);
//            if (sdkbox::PluginFacebook::isLoggedIn()) {
//                sdkbox::PluginFacebook::logout();
////                LPTPlayer::logoutUser();
//                LPTActivityIndicator::stopAnimating(this);
//                auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kMsgLogout, false, NORMAL_POPUP);
//                popup->setPopUpDelegate(this);
//                this->addChild(popup, 5000);
//                this->moveToLoginScene(kNOT_LOGGED_IN);
////                Director::getInstance()->replaceScene(LPTLoginScene::createScene());
//            }
//            break;
//        case GUEST_USER : {
////            LPTPlayer::logoutUser();
//            auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kMsgLogout, false, NORMAL_POPUP);
//            popup->setPopUpDelegate(this);
//            this->addChild(popup, 5000);
//            this->moveToLoginScene(kNOT_LOGGED_IN);
////            Director::getInstance()->replaceScene(LPTLoginScene::createScene());
//        }
//            break;
//        default:
//            this->moveToLoginScene(kNOT_LOGGED_IN);
////            Director::getInstance()->replaceScene(LPTLoginScene::createScene());
//            break;
//    }
//}

#pragma mark App42
void LPTGameSelectionScene::fetchExistingUserData(void* response) {
    
    if(UserDefault::getInstance()->getBoolForKey(kShowLoaderForFirstTime)) {
        LPTActivityIndicator::stopAnimating(this);
    }
    
    App42StorageResponse *res = (App42StorageResponse*)response;
    if (res->isSuccess) {
        App42Storage storage = res->storages.at(0);
        JSONDocument doc = storage.jsonDocArray.at(0);
//        PLAYER->updateUserProfile(doc.getJsonDoc().c_str());
        if(UserDefault::getInstance()->getBoolForKey(kShowLoaderForFirstTime)) {
            LPTTopBar *topBar = (LPTTopBar*)this->getChildByTag(kTopBarTag);
            topBar->updateCoinText(PLAYER->getTotalCoins());
            topBar->updatePlayerProfile(PLAYER->getPlayerProfilePicture());
        }
        UserDefault::getInstance()->setBoolForKey(kShowLoaderForFirstTime, false);
        
    } else {
//        LPTPlayer::logoutUser();
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, res->errorDetails.c_str(), false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
//        MessageBox(res->errorDetails.c_str(), kAlertErrorText);
        UserDefault::getInstance()->setBoolForKey(kShowLoaderForFirstTime, true);
//        this->moveToLoginScene();
//        Director::getInstance()->replaceScene(LPTLoginScene::createScene());
    }
}

void LPTGameSelectionScene::invalidateSessionResponse(void *response) {
    
    if(UserDefault::getInstance()->getBoolForKey(kShowLoaderForFirstTime)) {
        LPTActivityIndicator::stopAnimating(this);
    }
    App42SessionResponse *res = (App42SessionResponse*)response;
    if(res->isSuccess) {
//        LPTPlayer::logoutUser();
        auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kMsgLogout, false, NORMAL_POPUP);
        popup->setPopUpDelegate(this);
        this->addChild(popup, 5000);
        if(UserDefault::getInstance()->getBoolForKey(kShowLoaderForFirstTime)) {
            this->moveToLoginScene(kNOT_LOGGED_IN);
//            Director::getInstance()->replaceScene(LPTLoginScene::createScene());
            UserDefault::getInstance()->setBoolForKey(kShowLoaderForFirstTime, false);
        }
        
    } else {
        printf("\nerrordetails:%s",res->errorDetails.c_str());
        UserDefault::getInstance()->setBoolForKey(kShowLoaderForFirstTime, true);
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, res->errorDetails.c_str(), false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
//        MessageBox(res->errorDetails.c_str(), kAlertErrorText);
    }
}
#pragma mark - Game popup callabck
void LPTGameSelectionScene::okButtonAction(Popup_Type type) {
    switch (type) {
        case LOGOUT_POPUP:
            
            break;
        case GAMEQUIT_POPUP:
            
            break;
        case NORMAL_POPUP:
            break;
        default:
            
            break;
    }
    LPTPopUp * popUp = (LPTPopUp *)this->getChildByTag((int)type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

void LPTGameSelectionScene::cancelButtonAction(Popup_Type type) {
    switch (type) {
        case LOGOUT_POPUP:
            
            break;
        case GAMEQUIT_POPUP:
            
            break;
        case NORMAL_POPUP:
            break;
        default:
            
            break;
    }
    LPTPopUp * popUp = (LPTPopUp *)this->getChildByTag(type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

void LPTGameSelectionScene::onLeaveRoomDone (AppWarp::room revent){
    log("onLeaveRoomDone");
}
void LPTGameSelectionScene::onDeleteRoomDone (AppWarp::room revent){
    log("onDeleteRoomDone");
}

#pragma mark Delegate Method
void LPTGameSelectionScene::serverResponseComplete(int tag) {
    LPTActivityIndicator::stopAnimating(this);
    APP_MANAGER->setDataSyncStatus(false);
    
    if(tag == kFetchUserDetails) {
        this->updateTopBarUI();
    }else if(tag == kUpdateGameDataToServer) {
        //GameChanges...
        LPTActivityIndicator::startAnimating(this);
        LPTServerListener::getInstance()->setLPTServerDelegate(this);
        LPTServerListener::getInstance()->updateUserGameData(PLAYER->getPlayerGameXP(), PLAYER->getTotalCoins(), PLAYER->getPlayerGameWon(), PLAYER->getPlayerGamePlayed(), PLAYER->getPlayerGameLevel(), PLAYER->getPlayerAuthToken(), "", kGameModelUpdateRequest);
    }
}
void LPTGameSelectionScene::serverListenerResponseFaliure(int tag, string message){
    LPTActivityIndicator::stopAnimating(this);
    auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, message, false, NORMAL_POPUP);
    popup->setPopUpDelegate(this);
    this->addChild(popup, 5000);
    UserDefault::getInstance()->setBoolForKey(kShowLoaderForFirstTime, true);
}

void LPTGameSelectionScene::serverResponseCompletedWithData(picojson::array data) {
    LPTActivityIndicator::stopAnimating(this);
}

void LPTGameSelectionScene::updateTopBarUI() {
    LPTActivityIndicator::stopAnimating(this);
    LPTTopBar *topBar = (LPTTopBar*)this->getChildByTag(kTopBarTag);
    topBar->updateCoinText(PLAYER->getTotalCoins());
    topBar->updatePlayerProfile(PLAYER->getPlayerProfilePicture());
    topBar->removeUserXPBar();
    topBar->addProgressBar(PLAYER->getPlayerGameXP(), GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel()));
    UserDefault::getInstance()->setBoolForKey(kShowLoaderForFirstTime, false);
}

//--ask user Age
void LPTGameSelectionScene::addUserAgePanel() {
    LayerColor *layer = LayerColor::create(Color4B::BLACK, this->getContentSize().width, this->getContentSize().height);
    layer->setTag(kTagUserAgelayer);
    this->addChild(layer,1000);
    
    auto bg = Sprite::create(kSignupBG);
    bg->setPosition(Vec2(layer->getContentSize().width*0.5, layer->getContentSize().height*0.5));
    layer->addChild(bg);
    auto touchRestrict = createButton(kSignupBG, Vec2(layer->getContentSize().width * 0.5, layer->getContentSize().height * 0.5), 0);
    touchRestrict->setOpacity(0.0);
    layer->addChild(touchRestrict);

    
    float componentScale    =   GameUtils::isIphoneX ? 0.9 : 0.5;
    auto ageEditBoxSp = createEditBox(kTextFieldImageName, Vec2(layer->getContentSize().width*0.5, layer->getContentSize().height * 0.5), Vec2(0.0, 0.0), "Enter User Age", EDITBOX_AGE_TAG, layer, EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS, EditBox::InputMode::NUMERIC, layer, componentScale);
    EditBox *ageEditBox = (EditBox*)ageEditBoxSp->getChildByTag(EDITBOX_AGE_TAG);
    ageEditBox->setMaxLength(3);
    createButton(kSubmitButtonImageName, SUBMIT_AGE_BUTTON_TAG, Vec2(layer->getContentSize().width*0.5, layer->getContentSize().height * 0.3), layer, CC_CALLBACK_2(LPTGameSelectionScene::buttonCallback, this), componentScale);
    
}

void LPTGameSelectionScene::removeUserAgePanel() {
    LayerColor *userAgeLayer = (LayerColor*)this->getChildByTag(kTagUserAgelayer);
    if(userAgeLayer){
        userAgeLayer->removeFromParentAndCleanup(true);
    }
}
