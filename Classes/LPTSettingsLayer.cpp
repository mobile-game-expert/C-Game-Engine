//
//  LPTSettingsLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 08/10/18.
//

#include "LPTSettingsLayer.hpp"
#include "LPTTopBar.hpp"
#include "LPTLoginScene.hpp"
//#include "LPTTutorialScene.hpp"
//#include "LPTTutorialLayer.hpp"
#include "LPTInteractiveTutorialLayer.hpp"

#define     kTopLayerBarTag             652231
#define     kLogOutButtonTag            652232
#define     kSettingProfileBaseTag      652233
#define     kLogoutButtonTextTag        652234
#define     kMusicOnButtonTag           652235
#define     kMusicOffButtonTag          652236
#define     kSoundOnButtonTag           652237
#define     kSoundOffButtonTag          652238
#define     kTutorialPlayButtonTag      652239
#define     kMusicStatusLabelTag        652240
#define     kSoundStatusLabelTag        652241
#define     kEmailLoginButtonTag        652242
#define     kFBLoginButtonTag           652243

#define     kMinOpacity                 100
#define     kMaxOpacity                 255

LPTSettingsLayer* LPTSettingsLayer::createLayer(int backButtonTag, bool showTutorial) {
    auto layer = LPTSettingsLayer::create();
    layer->initWithTag(backButtonTag, showTutorial);
    return layer;
}

bool LPTSettingsLayer::initWithTag(int buttonTag, bool showTutorial) {
    if (!BaseLayer::init()) {
        return false;
    }
    
    backButtonTag       =   buttonTag;
    needToShowTutorial  =   showTutorial;
    
    return true;
}

void LPTSettingsLayer::onEnter() {
    BaseLayer::onEnter();
    isMusicPlaying  =   SoundManager::getMusicStatus();
    isSoundPlaying  =   SoundManager::getSoundStatus();
    //
    this->setBackgroundSprite(kGameSelectionBGImageName);
    LayerColor *layerColor = LayerColor::create(Color4B(16, 6, 8, 150), this->getContentSize().width, this->getContentSize().height);
    this->addChild(layerColor);
    auto touchRestrictButton = this->createButton(kSignupBG, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5), 0);
    touchRestrictButton->setOpacity(0);
    touchRestrictButton->setContentSize(Size(this->getContentSize().width, this->getContentSize().height));
    this->addChild(touchRestrictButton);
    //
    this->addTopBarInUserProfileScene(backButtonTag);
    this->addUI();
    
    //TEST  
    string buildVersionNumber = UserDefault::getInstance()->getStringForKey("iOSVersionKey");
    createTTFLabel(buildVersionNumber.c_str(),Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.03), Vec2::ANCHOR_MIDDLE, this, Color3B::WHITE, kNOUGAT_EXTRABLACK_1, (GameUtils::isIphoneX ? 35 : 20));
}

#pragma mark - TOPBAR
void LPTSettingsLayer::addTopBarInUserProfileScene(int backButtonTag) {
    LPTTopBar *layer = GameUtils::createTopBar(this, kUserProfileTopBarImageName);
    layer->setTag(kTopLayerBarTag);
    layer->addUserProfileTopBar(backButtonTag, kSettingScreenTitle, CC_CALLBACK_2(LPTSettingsLayer::buttonCallback, this));
}

void LPTSettingsLayer::buttonCallback(Ref* sender, Widget::TouchEventType type){
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
                case kBackToGameSceneBackButtonTag:
                    this->removeFromParentAndCleanup(true);
                    break;
                    
                case kBackToGameSelectionSceneBackButtonTag:
                    Director::getInstance()->popScene();
                    break;
                    
                case  kEmailLoginButtonTag:
                    gameLogoutType = kEMAIL_BUTTON;
//                    logoutFromDevice();
                    this->syncDataWithServer();
                    break;
                    
                case  kFBLoginButtonTag:
                    gameLogoutType = kFACEBOOK_BUTTON;
//                    logoutFromDevice();
                    this->syncDataWithServer();
                    break;
                    
                case kLogOutButtonTag:
                    gameLogoutType = kNOT_LOGGED_IN;
//                    logoutFromDevice();
                    this->syncDataWithServer();
                    break;
                    
                case kMusicOnButtonTag: {
                    if (!isMusicPlaying) {
                        isMusicPlaying = true;
                        SoundManager::setMusicStatus(isMusicPlaying);
                        if(!SoundManager::isBackgroundMusicPlaying()) {
                            SoundManager::playMusic(GAME_BACKGROUND_MUSIC);
                        }
                        updateStatusOfMusicSoundMenu(isMusicPlaying, kMusicStatusLabelTag);
                        this->changeOpacityOfAlernativeButton(kMusicOffButtonTag, kMaxOpacity);
                        ((Button *)sender)->setOpacity(kMinOpacity);
                    }
                }
                    break;
                    
                case kMusicOffButtonTag:
                    if (isMusicPlaying) {
                        isMusicPlaying = false;
                        SoundManager::setMusicStatus(isMusicPlaying);
                        if(SoundManager::isBackgroundMusicPlaying()) {
                            SoundManager::stopMusic();
                        }
                        updateStatusOfMusicSoundMenu(isMusicPlaying, kMusicStatusLabelTag);
                        this->changeOpacityOfAlernativeButton(kMusicOnButtonTag, kMaxOpacity);
                        ((Button *)sender)->setOpacity(kMinOpacity);
                    }
                    break;
                    
                case kSoundOffButtonTag:
                    if (isSoundPlaying) {
                        isSoundPlaying = false;
                        SoundManager::setSoundStatus(isSoundPlaying);
                        updateStatusOfMusicSoundMenu(isSoundPlaying, kSoundStatusLabelTag);
                        this->changeOpacityOfAlernativeButton(kSoundOnButtonTag, kMaxOpacity);
                        ((Button *)sender)->setOpacity(kMinOpacity);
                    }
                    break;
                    
                case kSoundOnButtonTag:
                    if (!isSoundPlaying) {
                        isSoundPlaying = true;
                        SoundManager::setSoundStatus(isSoundPlaying);
                        updateStatusOfMusicSoundMenu(isSoundPlaying, kSoundStatusLabelTag);
                        this->changeOpacityOfAlernativeButton(kSoundOffButtonTag, kMaxOpacity);
                        ((Button *)sender)->setOpacity(kMinOpacity);
                    }
                    break;
                    
                case kTutorialPlayButtonTag: {
                    LPTInteractiveTutorialLayer *layer = LPTInteractiveTutorialLayer::createLayer(kBackToGameSceneFromTutorialButtonTag);
                    this->getParent()->addChild(layer, 100000);
//                    this->removeFromParentAndCleanup(true);
                    
//                    if(backButtonTag == kBackToGameSceneBackButtonTag) {
//                        LPTTutorialLayer *layer = LPTTutorialLayer::createLayer(kBackToGameSceneFromTutorialButtonTag);
//                        this->getParent()->addChild(layer,1001);
//                        layer->runAction(Sequence::create(ScaleTo::create(0, 0), EaseBackOut::create(ScaleTo::create(0.1, 1.0)), NULL));
//                        this->removeFromParentAndCleanup(true);
//
//                    } else if (backButtonTag == kBackToGameSelectionSceneBackButtonTag) {
//                        SoundManager::playSound(SCENE_CHANGE_SOUND);
//                        LPTTutorialScene *TutorialScene =  LPTTutorialScene::createScene();
//                        Director::getInstance()->pushScene(TransitionFade::create(0.25, TutorialScene));
//                    }
                }
                    break;
                }
                    
                default:
                    break;
            }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
}


#pragma mark ADDUI
void LPTSettingsLayer::addUI() {
    settingBase = BaseLayer::createSprite(kSettingPopupBaseImageName, kSettingProfileBaseTag, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.43), this);
    settingBase->setScale(this->getContentSize().width * (GameUtils::isIphoneX ? 0.55 : 0.65) / settingBase->getContentSize().width);
    this->addLogoutButton();
    this->addSignButton();
    this->addSeparaterLayer();
    this->addSoundMenu();
    this->addMusicMenu();
    if(needToShowTutorial)
        this->addTutorialScene();
}

#pragma mark ADD_SOUND_MENU
void LPTSettingsLayer::addSoundMenu() {
     Label *soundLabel = createTTFLabel(kSoundEffectsText, Vec2(settingBase->getContentSize().width * 0.1, settingBase->getContentSize().height * 0.775), Vec2::ANCHOR_MIDDLE_LEFT, settingBase, Color3B::WHITE, kOpenSans_Bold, 70, -1);

    this->addMusicAndEffectsButton(Vec2(settingBase->getContentSize().width * 0.65, soundLabel->getPosition().y), kSoundOffButtonTag, true, isSoundPlaying ? kMaxOpacity : kMinOpacity);
    
    createTTFLabel(isSoundPlaying ? kStatusOn : kStatusOff, Vec2(settingBase->getContentSize().width * 0.75, soundLabel->getPosition().y), Vec2::ANCHOR_MIDDLE, settingBase, Color3B::WHITE, kOpenSans_Bold, 70, kSoundStatusLabelTag);
    
    this->addMusicAndEffectsButton(Vec2(settingBase->getContentSize().width * 0.85, soundLabel->getPosition().y), kSoundOnButtonTag, false, isSoundPlaying ? kMinOpacity : kMaxOpacity);
}

#pragma mark ADD_MUSIC_MENU
void LPTSettingsLayer::addMusicMenu() {
    Label *musicLabel = createTTFLabel(kMusicText, Vec2(settingBase->getContentSize().width * 0.1, settingBase->getContentSize().height * 0.915), Vec2::ANCHOR_MIDDLE_LEFT, settingBase, Color3B::WHITE, kOpenSans_Bold, 70, -1);
    
    this->addMusicAndEffectsButton(Vec2(settingBase->getContentSize().width * 0.65, musicLabel->getPosition().y), kMusicOffButtonTag, true, isMusicPlaying ? kMaxOpacity: kMinOpacity);

    createTTFLabel(isMusicPlaying ? kStatusOn : kStatusOff, Vec2(settingBase->getContentSize().width * 0.75, musicLabel->getPosition().y), Vec2::ANCHOR_MIDDLE, settingBase, Color3B::WHITE, kOpenSans_Bold, 70, kMusicStatusLabelTag);
    
    this->addMusicAndEffectsButton(Vec2(settingBase->getContentSize().width * 0.85, musicLabel->getPosition().y), kMusicOnButtonTag, false, isMusicPlaying ? kMinOpacity: kMaxOpacity);
}

void LPTSettingsLayer::addMusicAndEffectsButton(Vec2 position, int tag, bool rotationRequired, int opacity) {
    Button *menuButton = createButton(kMusicOnOffButtonImageName, tag, position, settingBase, CC_CALLBACK_2(LPTSettingsLayer::buttonCallback, this), 1);
    if (rotationRequired) {
        menuButton->setFlippedX(true);
    }
    menuButton->setOpacity(opacity);
}

void LPTSettingsLayer::changeOpacityOfAlernativeButton(int tag, int opacity) {
    Button *settingButton = (Button *)settingBase->getChildByTag(tag);
    settingButton->setOpacity(opacity);
}

void LPTSettingsLayer::updateStatusOfMusicSoundMenu(bool status, int tag) {
    Label *statusLabel = (Label *)settingBase->getChildByTag(tag);
    status == true ? statusLabel->setString(kStatusOn) : statusLabel->setString(kStatusOff);
}
#pragma mark ADD_TUTORIAL_SCENE
void LPTSettingsLayer::addTutorialScene() {
    Label *tutorialLabel = createTTFLabel(kTutorialText, Vec2(settingBase->getContentSize().width * 0.1, settingBase->getContentSize().height * 0.6), Vec2::ANCHOR_MIDDLE_LEFT, settingBase, Color3B::WHITE, kOpenSans_Bold, 70, -1);
    
    Button *playButton = createButton(kTutorialPlayButtonImageName, kTutorialPlayButtonTag, Vec2(settingBase->getContentSize().width * 0.75, tutorialLabel->getPosition().y), settingBase, CC_CALLBACK_2(LPTSettingsLayer::buttonCallback, this), 1);
    createTTFLabel(kPlayText, Vec2(playButton->getContentSize().width * 0.5, playButton->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE, playButton, Color3B::WHITE, kOpenSans_Bold, 70, -1);
}

#pragma mark SEPARATOR_BETWEEN_LAYER
void LPTSettingsLayer::addSeparaterLayer() {
    Vec2 initialPosition = Vec2(settingBase->getContentSize().width * 0.5, settingBase->getContentSize().height * 0.175);
    for(int separaterCounter = 0; separaterCounter < 5; separaterCounter++) {
        BaseSprite *separater = BaseLayer::createSprite(kSettingSeparaterImageName, -1, initialPosition, settingBase);
        separater->setScale(settingBase->getContentSize().width * 0.9 / separater->getContentSize().width);
        initialPosition.y = initialPosition.y + settingBase->getContentSize().height * 0.17;
    }
}
#pragma mark SIGN_BUTTON
void LPTSettingsLayer::addSignButton() {
    
    Vec2 fbButtonPosition       =   Vec2(settingBase->getContentSize().width * 0.5, settingBase->getContentSize().height * 0.43);
    Vec2 emailButtonPosition    =   Vec2(settingBase->getContentSize().width * 0.5, settingBase->getContentSize().height * 0.26);
    bool isFbButtonVisible      =   true;
    bool isEmailButtonVisible   =   true;
    
    
    if(PLAYER->getPlayerLoginState() == FB_USER) {
        isFbButtonVisible = false;
    }else if(PLAYER->getPlayerLoginState() == APP42_USER) {
        isEmailButtonVisible = false;
        fbButtonPosition = emailButtonPosition;
    }
    Button *emailLoginButton = createButton(kEmailLoginBtnImageName, kEmailLoginButtonTag, emailButtonPosition, settingBase, CC_CALLBACK_2(LPTSettingsLayer::buttonCallback, this), 1);
    emailLoginButton->setVisible(isEmailButtonVisible);
    emailLoginButton->setScale(0.75);
    
    Button *fbLoginButton = createButton(kFBLoginBtnImageName, kFBLoginButtonTag, fbButtonPosition, settingBase, CC_CALLBACK_2(LPTSettingsLayer::buttonCallback, this), 1);
    fbLoginButton->setVisible(isFbButtonVisible);
    fbLoginButton->setScale(0.75);
}
#pragma mark LOG_OUT_BUTTON
void LPTSettingsLayer::addLogoutButton() {
    Button *logoutButton = createButton(kLogoutButtonImageName, kLogOutButtonTag, Vec2(settingBase->getContentSize().width * 0.5, settingBase->getContentSize().height * 0.1), settingBase, CC_CALLBACK_2(LPTSettingsLayer::buttonCallback, this), 1);
    logoutButton->setScale(0.75);
     createTTFLabel(kLogOutButtonText, Vec2(logoutButton->getContentSize().width * 0.5, logoutButton->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE, logoutButton, Color3B::WHITE, kOpenSans_Bold, 70, kLogoutButtonTextTag);
}

void LPTSettingsLayer::onExit() {
    BaseLayer::onExit();
}

#pragma mark - Game popup callabck
void LPTSettingsLayer::okButtonAction(Popup_Type type) {
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

void LPTSettingsLayer::cancelButtonAction(Popup_Type type) {
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

#pragma mark UpdateDataWithLogout
void LPTSettingsLayer::syncDataWithServer() {
    if(APP_MANAGER->isInternetAvilable) {
        LPTActivityIndicator::startAnimating(this);
        LPTServerListener::getInstance()->setLPTServerDelegate(this);
        LPTServerListener::getInstance()->updateUserGameData(PLAYER->getPlayerGameXP(), PLAYER->getTotalCoins(), PLAYER->getPlayerGameWon(), PLAYER->getPlayerGamePlayed(), PLAYER->getPlayerGameLevel(), PLAYER->getPlayerAuthToken(), "", kGameModelUpdateRequest);
    }else {
        auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
        popup->setPopUpDelegate(this);
        this->addChild(popup, 5000);
    }
}

#pragma mark LogoutCallBack
void LPTSettingsLayer::logoutFromDevice() {
    PLAYER->logoutUser();
    
    LPTPopUp *popup = LPTPopUp::showPopUpWith(kAlertMessageText, kMsgLogout, false, NORMAL_POPUP);
    this->addChild(popup, 10000);
    
//    MessageBox(kMsgLogout, "");
    UserDefault::getInstance()->setBoolForKey(kShowLoaderForFirstTime, true);
    this->moveToLoginScene(gameLogoutType);
}

#pragma mark ServerResponseCallback
void LPTSettingsLayer::serverResponseComplete(int tag) {
    if(tag == kGameModelUpdateRequest) {
        log("token %s", PLAYER->getPlayerAuthToken().c_str());
        LPTServerListener::getInstance()->reteriveDataFromServerForGame("", "", "", "", PLAYER->getPlayerAuthToken(), GUEST_USER, kLogoutRequest);
    }else if(tag == kLogoutRequest){
        LPTActivityIndicator::stopAnimating(this);
        logoutFromDevice();
    }
}
void LPTSettingsLayer::serverListenerResponseFaliure(int tag, string message) {
    LPTActivityIndicator::stopAnimating(this);
    auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, message, false, NORMAL_POPUP);
    popup->setPopUpDelegate(this); 
    this->addChild(popup, 5000);
}
void LPTSettingsLayer::serverResponseCompletedWithData(picojson::array data) {
    LPTActivityIndicator::stopAnimating(this);
}
