//
//  LPTTopBar.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 28/09/18.
//

#include "LPTTopBar.hpp"
#include "LPTTopBar.hpp"
#include "LPTProfilePicture.hpp"
#include "LPTPlayerProfileScene.hpp"
#include "LPTCoinComponent.hpp"
#include "LPTStoreScene.hpp"
#include "LPTSettingsScene.hpp"
#include "LPTProgressBarLayer.hpp"
#include "LPTServerListener.hpp"

#define     kTopBarTag                              30000
#define     kSettingButtonTag                       30001
#define     kTopBarCoinBaseTag                      30002
#define     kTopBarCoinTag                          30003
#define     kBuyCoinButtonTag                       30004
#define     kChipCoinLabelTag                       30005
#define     kScreenDisplayTextBaseTag               30006
#define     kScreenDisplayTextTag                   30007
#define     kUserXP_ProgressTag                     30008

LPTTopBar* LPTTopBar::createLayer(string imageName) {
    auto layer = LPTTopBar::create();
    layer->initWithImageName(imageName);
    return layer;
}

#pragma mark LayerMethod
bool LPTTopBar::initWithImageName(string imageName) {
    if (!BaseLayer::init()) {
        return false;
    }
    BaseSprite *background = BaseLayer::createSprite(imageName, kTopBarTag, Vec2(this->getContentSize().width * 0.0, this->getContentSize().height * 0.0), this);
    background->setAnchorPoint(Vec2(0, 1));
    background->setScale(this->getContentSize().width / background->getContentSize().width);
    
    //Setting ContentSize
    this->setContentSize(background->getContentSize() * background->getScale());
    return true;
}
void LPTTopBar::addProgressBar(double currentXP, double totalXP) {
    BaseSprite *background =  (BaseSprite *)this->getChildByTag(kTopBarTag);
    LPTProgressBarLayer *layer = LPTProgressBarLayer::createLayer();
    layer->setPosition(Vec2(background->getContentSize().width * (GameUtils::isIpad ? 0.115 : 0.10), background->getContentSize().height * 0.25));
    layer->setupUI(currentXP, totalXP,  (GameUtils::isIphoneX ? 1.5 : 0.75));
//    layer->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, 1)), NULL));
    layer->setTag(kUserXP_ProgressTag);
    background->addChild(layer);

    log("Player name: %s", PLAYER->getPlayerUniqueName().c_str());
    
//    string coinsValue = StringUtils::format("%.0f", coinValue);   PLAYER->getPlayerUniqueName().c_str()
    createTTFLabel(PLAYER->getPlayerUniqueName(), Vec2(background->getContentSize().width * ((GameUtils::isIphoneX) ? 0.10 : (GameUtils::isIpad ? 0.1 : 0.095)), background->getContentSize().height * 0.765), Vec2::ANCHOR_MIDDLE_LEFT, background, Color3B::WHITE, kOpenSans_Bold, GameUtils::isIphoneX ? 50 : 27);
    
//    Label::createWithTTF(PLAYER->getPlayerUniqueName().c_str(), kOpenSans_Bold, 60);
//    chipCoinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    chipCoinLabel->setPosition(Vec2(coinComponent->getContentSize().width * 0.2, coinComponent->getContentSize().height * 0.5));
//    chipCoinLabel->setColor(Color3B(254, 200, 35));
//    chipCoinLabel->setTag(tag);
//    coinComponent->addChild(chipCoinLabel);
//
//
//    auto GuestNamelabel = createBMFLabel(PLAYER->getPlayerUniqueName().c_str(), Vec2(background->getContentSize().width * ((GameUtils::isIphoneX) ? 0.10 : (GameUtils::isIpad ? 0.1 : 0.085)), background->getContentSize().height * 0.8), Vec2::ANCHOR_MIDDLE_LEFT, Color3B::WHITE, kOpenSans_Bold, 60);
////    GuestNamelabel->setScale((GameUtils::isIphoneX) ? 1 : 0.5);
//    background->addChild(GuestNamelabel);
    
//    GuestNamelabel->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.3, (GameUtils::isIphoneX) ? 1 : 0.5)), NULL));
}
void LPTTopBar::removeUserXPBar() {
    
    BaseSprite *background =  (BaseSprite *)this->getChildByTag(kTopBarTag);
    background->removeChildByTag(kUserXP_ProgressTag);

}
void LPTTopBar::onEnter() {
    BaseLayer::onEnter();
}

void LPTTopBar::onExit() {
    BaseLayer::onExit();
}

#pragma mark AddUserProfileTop
/**
 addUserProfileTopBar: This method will add following component in TopBar:
 1. BackButton: To navigate to previous screen.
 2. ScreenTitleDisplay: To display the current screen name
 -params:
 - backButtonTag: To navigate to back screen.
 - screenText: Title of the screen.
 - callBack: In which class you want to take callback of back button.
 -void:
 */
void LPTTopBar::addUserProfileTopBar(int backButtonTag, string screenText, std::function<void(Ref*, Widget::TouchEventType)> callback) {
    Button * backButton = createButton(kBackButtonImageName, backButtonTag, Vec2(this->getContentSize().width * (GameUtils::isIphoneX ? 0.03 : 0.01), -this->getContentSize().height * 0.5), this, callback, 1);
    backButton->setScale(this->getContentSize().height * 0.7 / backButton->getContentSize().height);
    backButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->sceneStatusBar(screenText);
}

void LPTTopBar::sceneStatusBar(string screenName) {
    BaseSprite *screenTitleBase = BaseLayer::createSprite(kScreenTitleBaseImage, kScreenDisplayTextBaseTag, Vec2(this->getContentSize().width * 0.5, -this->getContentSize().height * 0.5), this);
    if(GameUtils::isIphoneX)
        screenTitleBase->setScale(this->getContentSize().height * 0.7 / screenTitleBase->getContentSize().height);
    else
        screenTitleBase->setScale(this->getContentSize().width * 0.3 / screenTitleBase->getContentSize().width);
    
    createTTFLabel(screenName.c_str(), Vec2(screenTitleBase->getContentSize().width * 0.5, screenTitleBase->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE, screenTitleBase, Color3B::WHITE, kOpenSans_Bold, 70, kScreenDisplayTextTag);
}



#pragma mark AddGameSelectionSceneTopBarUI
/**
 addGameSelectionSceneTopBarUI: This method will add following component in TopBar:
 1. player profile: picture of player
 2. Setting Button: To control game setting like music etc
 3. Coin Display: This will display the number of coins user have.
 -params:
 - profileImage: image name of player
 - coinValue: Total number of coins user have.
 -void:
 */
void LPTTopBar::addGameSelectionSceneTopBarUI(string profileImage, double coinValue) {
    profileImage = profileImage.compare("") == 0 ? kPlayerProfileImageName : profileImage;
    this->addPlayerProfile(profileImage, Vec2(this->getContentSize().width * (GameUtils::isIphoneX ? 0.03 : 0.01), -this->getContentSize().height * 0.5), Vec2(0, 0), kUserProfilePicTag);
    this->addSettingButton(Vec2(this->getContentSize().width * (GameUtils::isIphoneX ? 0.97 : 0.99), -this->getContentSize().height * 0.5), kSettingButtonTag, Vec2(1, 0.5));
    this->addCoinChipBase(coinValue);
}

void LPTTopBar::addPlayerProfile(string imageName, Vec2 position, Vec2 anchorPoint, int tag) {
    BaseSprite *baseSprite = this->getTopBarBaseSprite();
    LPTProfilePicture *profilePicture = new LPTProfilePicture();
    profilePicture->setContentSize(Size(baseSprite->getContentSize().height * 0.75, baseSprite->getContentSize().height * 0.75));
    profilePicture->setPosition(Vec2(position.x, position.y));
    profilePicture->setAnchorPoint(anchorPoint);
    profilePicture->setTag(tag);
    profilePicture->createProfilePictureView(imageName, kPlayerProfileImageName, Vec2(profilePicture->getContentSize().width * 0.5, profilePicture->getContentSize().height * 0.5), tag, true, CC_CALLBACK_2(LPTTopBar::buttonCallback, this));
    baseSprite->addChild(profilePicture, 10);
    
    float  positionInY =  baseSprite->getContentSize().height - profilePicture->getContentSize().height;
    profilePicture->setPositionY(positionInY / 2);
    
    
    profilePicture->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, 1)), NULL));
    //Username label
//    BaseLayer::createTTFLabel(LPTPlayer::getPlayerUniqueName().c_str(), Vec2(profilePicture->getPositionX()+ profilePicture->getContentSize().width * 1.5,  profilePicture->getPositionY() + profilePicture->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE, baseSprite, Color3B::WHITE, kOpenSans_Bold, 10);
}

BaseSprite* LPTTopBar::getTopBarBaseSprite() {
    BaseSprite *background = (BaseSprite *)this->getChildByTag(kTopBarTag);
    return background;
}

void LPTTopBar::addSettingButton(Vec2 position, int tag, Vec2 anchorPoint) {
    Button *settingButton = createButton(kGameTopBarSettingButtonImageName, tag, position, this, CC_CALLBACK_2(LPTTopBar::buttonCallback, this), 1);
    settingButton->setAnchorPoint(anchorPoint);
    settingButton->setScale(this->getContentSize().height * 0.7 / settingButton->getContentSize().height);
    settingButton->setPositionX(position.x);
    settingButton->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, settingButton->getScale())), NULL));
}

void LPTTopBar::addCoinChipBase(double coinValue) {
    Button *settingButton = (Button *)this->getChildByTag(kSettingButtonTag);
    
    LPTCoinComponent *coinComponent = LPTCoinComponent::createCoinComponent(kGameSelectionTopBarCoinDisplayImageName, Vec2(settingButton->getPosition().x - (settingButton->getContentSize().width * 1.5 * settingButton->getScale()), -this->getContentSize().height * 0.5), kTopBarCoinBaseTag, Vec2(1, 0.5));
    coinComponent->setScale(this->getContentSize().height * 0.6 / coinComponent->getContentSize().height);
    this->addChild(coinComponent);
    coinComponent->setGameSelectionUI(CC_CALLBACK_2(LPTTopBar::buttonCallback, this), kBuyCoinButtonTag);
}

void LPTTopBar::updateCoinText(double coinCount) {
    LPTCoinComponent *coinComponent = (LPTCoinComponent *)this->getChildByTag(kTopBarCoinBaseTag);
    coinComponent->updateCoinText(coinCount);
}

void LPTTopBar::updatePlayerProfile(string imageName) {
    BaseSprite *baseSprite = this->getTopBarBaseSprite();
    LPTProfilePicture *profilePicture = (LPTProfilePicture *)baseSprite->getChildByTag(kUserProfilePicTag);
    profilePicture->updatePicture(imageName, kUserProfilePicTag);
}

#pragma mark ButtonCallBack
void LPTTopBar::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case kSettingButtonTag: {
                    LPTSettingsScene *settingScene =  LPTSettingsScene::createScene();
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    Director::getInstance()->pushScene(TransitionSlideInT::create(0.25, settingScene));
                }
                break;
                    
                case kBuyCoinButtonTag: {
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    Director::getInstance()->pushScene(TransitionSlideInT::create(0.25, LPTStoreScene::createScene()));
                }
                break;
                    
                case kUserProfilePicButtonTag:
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    Director::getInstance()->pushScene(TransitionSlideInT::create(0.25, LPTPlayerProfileScene::createScene()));
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

//void LPTTopBar::logoutUser() {
//    switch (PLAYER->getPlayerLoginState()) {
//        case APP42_USER: {
//            LPTActivityIndicator::startAnimating(this);
////            SessionService *sessionService = App42API::BuildSessionService();
//            LPTServerListener::getInstance()->reteriveDataFromServerForGame("", "", "", "", PLAYER->getPlayerAuthToken(), FB_USER, kLogoutRequest);
//            this->invalidateSessionResponse();
////            sessionService->Invalidate(LPTPlayer::getPlayerApp42session().c_str(), app42callback(LPTTopBar::invalidateSessionResponse, this));
//        }
//
//            break;
//        case FB_USER:
//            LPTActivityIndicator::startAnimating(this);
//            if (sdkbox::PluginFacebook::isLoggedIn()) {
//                LPTServerListener::getInstance()->reteriveDataFromServerForGame("", "", "", "", PLAYER->getPlayerAuthToken(), FB_USER, kLogoutRequest);
//                sdkbox::PluginFacebook::logout();
////                LPTPlayer::logoutUser();
//                LPTActivityIndicator::stopAnimating(this);
//                MessageBox(kMsgLogout, "");
////                Director::getInstance()->replaceScene(LPTLoginScene::createScene());
//                this->moveToLoginScene(kNOT_LOGGED_IN);
//            }
//            break;
//        case GUEST_USER : {
//            LPTServerListener::getInstance()->reteriveDataFromServerForGame("", "", "", "", PLAYER->getPlayerAuthToken(), FB_USER, kLogoutRequest);
////            LPTPlayer::logoutUser();
//            MessageBox(kMsgLogout, "");
////            Director::getInstance()->replaceScene(LPTLoginScene::createScene());
//            this->moveToLoginScene(kNOT_LOGGED_IN);
//        }
//            break;
//        default:
////            Director::getInstance()->replaceScene(LPTLoginScene::createScene());
//            this->moveToLoginScene(kNOT_LOGGED_IN);
//            break;
//    }
//}
//void LPTTopBar::invalidateSessionResponse(void *response) {
//    LPTActivityIndicator::stopAnimating(this);
//    App42SessionResponse *res = (App42SessionResponse*)response;
//    if(res->isSuccess) {
//        LPTPlayer::logoutUser();
//        MessageBox(kMsgLogout, "");
////        Director::getInstance()->replaceScene(LPTLoginScene::createScene());
//        this->moveToLoginScene(kNOT_LOGGED_IN);
//    } else {
//        printf("\nerrordetails:%s",res->errorDetails.c_str());
//        MessageBox(res->errorDetails.c_str(), kAlertErrorText);
//    }
//}

void LPTTopBar::invalidateSessionResponse() {
    LPTActivityIndicator::stopAnimating(this);
////    LPTPlayer::logoutUser();
//    MessageBox(kMsgLogout, "");
    LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kGameAlert, kMsgLogout, false, NORMAL_POPUP);
    this->addChild(gamePopup, 10000);
    this->moveToLoginScene(kNOT_LOGGED_IN);
}
#pragma mark - Game popup callabck
void LPTTopBar::okButtonAction(Popup_Type type) {
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
    LPTPopUp * popUp = (LPTPopUp *)this->getParent()->getChildByTag((int)type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

void LPTTopBar::cancelButtonAction(Popup_Type type) {
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
    LPTPopUp * popUp = (LPTPopUp *)this->getParent()->getChildByTag(type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

