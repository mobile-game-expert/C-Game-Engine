//
//  LPTHUDLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 18/09/18.
//

#include "LPTHUDLayer.hpp"
#include "LPTLoginScene.hpp"
#include "LPTProfilePicture.hpp"
#include "LPTGameSelectionScene.hpp"
#include "LPTPlayerProfileLayer.hpp"
#include "LPTSideMenuLayer.hpp"
#include "LPTStoreLayer.hpp"
#include "LPTSettingsLayer.hpp"

#define kHUDBaseSpriteTag                           2000
#define BACK_BUTTON_TAG                             2001
#define kChipCoinBaseTag                            2002
#define kOpponentProfilePicNameBarTag               2007
#define kUserProfilePicNameBarTag                   2008
#define kUserNameLabelTag                           2009
#define kTimerBaseSpriteTag                         2010
#define kTimerLabelTag                              2011
#define kSideMenuButtonTag                          2012
#define kSideMenuLayerTag                           2013

LPTHUDLayer* LPTHUDLayer::createLayer() {
    auto layer = LPTHUDLayer::create();
    layer->init();
    return layer;
}
#pragma mark LayerMethod
bool LPTHUDLayer::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}

void LPTHUDLayer::onEnter() {
    BaseLayer::onEnter();
    
}

void LPTHUDLayer::onExit() {
    BaseLayer::onExit();
}

#pragma mark SetUpUI
/**
 setupUI: This method will setUp UI for HUDLayer. This method done following things
 1. This will add base sprite HUDLayer Sprite
 2. This will add back button
 3. This will add ChipCoin Base Sprite along with bet coins.
 4. This will add Player profile along with name bar for opposition as well as player.
 5. This will timer in game.
 -params:
    - string player1: Name of the player1.
    - string profileImage1: Image of the player1.
    - string player2: Name of the player2.
    - string profileImage2: Image of the player2.
 */
void LPTHUDLayer::setupUI(string player1, string profileImage1, string player2, string profileImage2, UserPlaySide selectedSide) {
    
    string player1Name = (selectedSide == PLAY_AREA_LEFT) ? player1 : player2;
    string player2Name = (selectedSide == PLAY_AREA_LEFT) ? player2 : player1;
    string player1Image = (selectedSide == PLAY_AREA_LEFT) ? profileImage1 : profileImage2;
    string player2Image = (selectedSide == PLAY_AREA_LEFT) ? profileImage2 : profileImage1;
    
    Sprite *baseSprite = Sprite::create(kHUDBarSpriteImageName);
    this->setContentSize(baseSprite->getContentSize());
    
    BaseSprite *background = BaseLayer::createSprite(kHUDBarSpriteImageName, kHUDBaseSpriteTag, Vec2(this->getContentSize().width * 0.0, this->getContentSize().height * 0.0), this);
    background->setAnchorPoint(Vec2(0, 1));
    background->setScale(this->getContentSize().width / background->getContentSize().width);
    
//    this->addBackButton();
    this->addChipCoin();
    
    LPTCoinComponent *coinBase = this->getCoinBaseSprite();
    
    this->addPlayerProfile(player1Image,Vec2(coinBase->getPosition().x - coinBase->getContentSize().width * (0.6 * coinBase->getScale()) - baseSprite->getContentSize().height * 0.75, baseSprite->getPositionY() + baseSprite->getContentSize().height * 0.125), Vec2(1, 0), (selectedSide == PLAY_AREA_LEFT) ? kUserProfilePicTag : kOpponentProfilePicTag);
    
    this->addPlayerProfile(player2Image,Vec2(coinBase->getPosition().x + coinBase->getContentSize().width * (0.6 * coinBase->getScale()), baseSprite->getPositionY() + baseSprite->getContentSize().height * 0.125), Vec2(0, 0), (selectedSide == PLAY_AREA_LEFT) ? kOpponentProfilePicTag : kUserProfilePicTag);
    
    this->addPlayerNameBar(kUserProfilePicNameBarTag, kUserProfilePicTag, (selectedSide == PLAY_AREA_LEFT) ? player1Name : player2Name, selectedSide);
    this->addPlayerNameBar(kOpponentProfilePicNameBarTag, kOpponentProfilePicTag, (selectedSide == PLAY_AREA_LEFT) ? player2Name : player1Name, selectedSide);
    
    this->addTimerBaseSprite();
    this->addSideMenuButton();
}

void LPTHUDLayer::addSideMenuButton() {
    BaseSprite *baseSprite = this->getHUDBaseSprite();
    Button *sideMenuButon = createButton(kSideMenuImageName, kSideMenuButtonTag, Vec2(baseSprite->getContentSize().width * 0.985, baseSprite->getContentSize().height * 0.5), baseSprite, CC_CALLBACK_2(LPTHUDLayer::buttonCallback, this), 1);
    sideMenuButon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sideMenuButon->setScale(baseSprite->getContentSize().height * 0.5 / sideMenuButon->getContentSize().height);
    sideMenuButon->setLocalZOrder(10000);
}

void LPTHUDLayer::setHUDDelegate(LPTHUDLayerDelegate *delegate) {
    this->delegate = delegate;
}

#pragma mark AddSideMenuPopup
void LPTHUDLayer::addSettingLayer() {
    BaseSprite *baseSprite = this->getHUDBaseSprite();
    Button *sideMenuButton = (Button *)baseSprite->getChildByTag(kSideMenuButtonTag);
    
    LPTSideMenuLayer *sideMenuLayer = (LPTSideMenuLayer *)baseSprite->getChildByTag(kSideMenuLayerTag);
    if(sideMenuLayer) {
        sideMenuLayer->removeFromParentAndCleanup(true);
    } else {
        sideMenuLayer = LPTSideMenuLayer::createLayer();
        
        sideMenuLayer->setupUI(Size(this->getContentSize().width * 0.25, this->getContentSize().height * 2.5), CC_CALLBACK_2(LPTHUDLayer::buttonCallback, this));
        
        sideMenuLayer->setPosition(Vec2(sideMenuButton->getPosition().x - sideMenuLayer->getContentSize().width, sideMenuButton->getPosition().y - sideMenuLayer->getContentSize().height + (sideMenuButton->getContentSize().height * 0.5 * sideMenuButton->getScale())));
        sideMenuLayer->setTag(kSideMenuLayerTag);
        baseSprite->addChild(sideMenuLayer, 100);
    }
}

#pragma mark GetHUDBaseSprite
/**
 getHUDBaseSprite: This method will return base HUDSprite.
 */
BaseSprite* LPTHUDLayer::getHUDBaseSprite() {
    BaseSprite *background = (BaseSprite *)this->getChildByTag(kHUDBaseSpriteTag);
    return background;
}

#pragma mark CoinDisplay
/**
 addChipCoin: This method will addChipCoin Component in HUDLayer.
 ChipCoin component comprise of the ChipCoinBase image, Coins Image and Bet Amount
 */
void LPTHUDLayer::addChipCoin() {
    BaseSprite *hudSprite = this->getHUDBaseSprite();
    LPTCoinComponent *coinComponent = LPTCoinComponent::createCoinComponent(kBetAmountBaseImageName, Vec2(hudSprite->getContentSize().width * 0.5, hudSprite->getContentSize().height * 0.925), kChipCoinBaseTag, Vec2(0.5, 1));
    coinComponent->setScale(hudSprite->getContentSize().height * 0.65 / coinComponent->getContentSize().height);
    hudSprite->addChild(coinComponent);
    coinComponent->setGameComponentUI(PLAYER->getTotalCoins());
}

LPTCoinComponent* LPTHUDLayer::getCoinBaseSprite() {
    BaseSprite *hudSprite = this->getHUDBaseSprite();
    LPTCoinComponent *coinComponent = (LPTCoinComponent *)hudSprite->getChildByTag(kChipCoinBaseTag);
    return coinComponent;
}
/**
 setCoinValue: This method will set bet amount coin base component.
 */
void LPTHUDLayer::setCoinValue(double coinValue) {
    LPTCoinComponent *coinComponent = this->getCoinBaseSprite();
    coinComponent->updateCoinText(coinValue);
}

#pragma mark AddBackButton
/**
 addBackButton: This method will add backbutton in HUDLayer. scale of the backbutton is depend upon the HUDlayer height.
 */
void LPTHUDLayer::addBackButton() {
    BaseSprite *baseSprite = this->getHUDBaseSprite();
    Button *btn = createButton(kBackButtonImageName, BACK_BUTTON_TAG, Vec2(baseSprite->getContentSize().width * 0.05, baseSprite->getContentSize().height * 0.5), baseSprite, CC_CALLBACK_2(LPTHUDLayer::buttonCallback, this), 1);
    btn->setScale(baseSprite->getContentSize().height * 0.6 / btn->getContentSize().height);
}

#pragma mark PlayerProfile
/**
 addPlayerProfile: This method will add Player profile in HUDLayer. This method will done following things
 1. This will add Player Profile picture in clipped way.
 2. This will add Player Name Bar along with player name.
 3. We added scale ease animation to add effect in layer.
 -params:
 - string imageName : Name of the player profile image.
 - Vec2 position    : position of the player profile image.
 - Vec2 anchorPoint : anchor point of the player profile image.
 - int tag          : tag of the player profile image.
 */
void LPTHUDLayer::addPlayerProfile(string imageName, Vec2 position, Vec2 anchorPoint, int tag) {
    BaseSprite *baseSprite = this->getHUDBaseSprite();
    
    LPTProfilePicture *profilePicture = new LPTProfilePicture();
    profilePicture->setContentSize(Size(baseSprite->getContentSize().height * 0.75, baseSprite->getContentSize().height * 0.75));
    profilePicture->setPosition(Vec2(position.x, position.y));
    profilePicture->setAnchorPoint(anchorPoint);
    profilePicture->setTag(tag);
    profilePicture->createProfilePictureView(imageName, kPlayerProfileImageName, Vec2(profilePicture->getContentSize().width*0.5, profilePicture->getContentSize().height*0.5), tag, true, CC_CALLBACK_2(LPTHUDLayer::buttonCallback, this));
    
    baseSprite->addChild(profilePicture, 10);
    profilePicture->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, 1)), NULL));
}


/**
 updatePlayerProfile: This method will update Player profile.
 -params:
 - string imageName : Name of the player profile image.
 - Vec2 anchorPoint : anchor point of the player profile image.
 - int tag          : tag of the player profile image.
 */
void LPTHUDLayer::updatePlayerProfile(string imageName, Vec2 anchorPoint, int tag) {
    BaseSprite *baseSprite = this->getHUDBaseSprite();
    LPTProfilePicture *userProfile =   (LPTProfilePicture *)baseSprite->getChildByTag(tag);
    userProfile->updatePicture(imageName, tag);
}

/**
 addPlayerNameBar: This method will add Player name bar.
 -params:
 - int tag              : tag of the player profile name bar.
 - int profilePicTag    : tag of the image according to which player name bar image will be displayed.
 -  string playerName   : name of the player.
 */
void LPTHUDLayer::addPlayerNameBar(int tag, int profilePicTag, string playerName, UserPlaySide selectedSide) {
    BaseSprite *baseSprite  =   this->getHUDBaseSprite();
    LPTProfilePicture *userProfile =   (LPTProfilePicture *)baseSprite->getChildByTag(profilePicTag);
    
    
    float difference        =   userProfile->getContentSize().width * userProfile->getScale() * 1.15;
    Vec2 anchorPoint        =   Vec2(0, 0.5);
    float labelPositionFact =   0.1;
    
    int profileTag = (selectedSide == PLAY_AREA_LEFT) ? kUserProfilePicTag : kOpponentProfilePicTag;
    
    if(profilePicTag == profileTag) {
        anchorPoint         =   Vec2(1, 0.5);
        difference          =   -(userProfile->getContentSize().width * userProfile->getScale() * 0.15);
        labelPositionFact   =   0.9;
    }
    
    BaseSprite *userProfileNameBar = BaseLayer::createSprite(kGuestNameBaseImageName, tag, Vec2(userProfile->getPosition().x + difference, userProfile->getPosition().y + (userProfile->getContentSize().height * 0.5 * userProfile->getScale())), baseSprite);
    userProfileNameBar->setAnchorPoint(anchorPoint);
    float maxScale = baseSprite->getContentSize().width * 0.2 / userProfileNameBar->getContentSize().width;
    userProfileNameBar->setScale(maxScale);
    
    createTTFLabel(playerName, Vec2(userProfileNameBar->getContentSize().width * labelPositionFact, userProfileNameBar->getContentSize().height * 0.5), anchorPoint, userProfileNameBar, Color3B(255, 255, 255), kOpenSans_Bold, 30, kUserNameLabelTag);
    
    userProfileNameBar->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, maxScale)), NULL));
}

void LPTHUDLayer::setPlayerName(string name, int tag) {
    BaseSprite *baseSprite  =   this->getHUDBaseSprite();
    BaseSprite *userNameBar =   (BaseSprite *)baseSprite->getChildByTag(tag);
    Label *userNameLabel =   (Label *)userNameBar->getChildByTag(kUserNameLabelTag);
    userNameLabel->setString(name);
    
}

#pragma mark addTimerBaseSprite
/**
 addTimerBaseSprite: This method will add timer base component i.e base sprite and timer label.
 */
void LPTHUDLayer::addTimerBaseSprite() {
    BaseSprite *baseSprite = this->getHUDBaseSprite();
    BaseSprite *timerBase = BaseLayer::createSprite(kTimerBaseSpriteImageName, kTimerBaseSpriteTag, Vec2(baseSprite->getContentSize().width * 0.5, -baseSprite->getContentSize().height * 0.05), baseSprite);
    float maxScale = baseSprite->getContentSize().height * 0.5 / timerBase->getContentSize().height;
    timerBase->setScale(maxScale);
    timerBase->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, maxScale)), NULL));
    
    createTTFLabel("00 : 00", Vec2(timerBase->getContentSize().width * 0.5, timerBase->getContentSize().height * 0.5), Vec2(0.5, 0.5), timerBase, Color3B(255, 255, 255), kOpenSans_Bold, 40, kTimerLabelTag);
}

/**
 getTimerLabel: This method will return timer label to update timer value.
 */
Label* LPTHUDLayer::getTimerLabel() {
    BaseSprite *baseSprite      =   this->getHUDBaseSprite();
    BaseSprite *timerSprite     =   (BaseSprite *)baseSprite->getChildByTag(kTimerBaseSpriteTag);
    Label *timerLabel           =   (Label *)timerSprite->getChildByTag(kTimerLabelTag);
    return timerLabel;
}

void LPTHUDLayer::updateTimerLabel(string timerValue) {
    Label *timerLabel = this->getTimerLabel();
    timerLabel->setString(timerValue);
}

#pragma ButtonCallBack
void LPTHUDLayer::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case BACK_BUTTON_TAG:
                    if(APP_MANAGER->isInternetAvilable) {
                        Director::getInstance()->pause();
                        auto popup = LPTPopUp::showPopUpWith(kInformation, kQuitGame, true, GAMEQUIT_POPUP);
                        popup->setPopUpDelegate(this);
                        this->getParent()->addChild(popup, 5000);
                    } else {
                        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
                        this->addChild(gamePopup, 10000);
                    }
                    break;
                    
                case kUserProfilePicButtonTag: {
                     LPTPlayerProfileData *player = createProfilePlayerObjectFromUser(true);
                    LPTPlayerProfileLayer *playerProfileLayer = LPTPlayerProfileLayer::createLayer(kBackToGameSceneBackButtonTag, player);
                    
                    playerProfileLayer->setPosition(Vec2(0, this->getParent()->getContentSize().height));
                    this->getParent()->addChild(playerProfileLayer, 1000);
                    playerProfileLayer->runAction(MoveTo::create(0.25, Vec2::ZERO));
                    playerProfileLayer->addPlayerXP(PLAYER->getPlayerGameXP(), GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel()));
                }
                break;
                    
                case kOpponentProfilePicButtonTag: {
                    LPTPlayerProfileData *opponent = createProfilePlayerObjectFromUser(false);
                    log("opponent player : %s", opponent->getPlayerUserName().c_str());
                    LPTPlayerProfileLayer *playerProfileLayer = LPTPlayerProfileLayer::createLayer(kBackToGameSceneBackButtonTag, opponent);
                    playerProfileLayer->setPosition(Vec2(0, this->getParent()->getContentSize().height));
                    this->getParent()->addChild(playerProfileLayer, 1000);
                    playerProfileLayer->runAction(MoveTo::create(0.25, Vec2::ZERO));
                    playerProfileLayer->disableEditButtton();
                    playerProfileLayer->addPlayerXP(opponent->getPlayerGameXP(), GameUtils::getPlayerMaxXP(OPPONENT->getlevel()));
                }
                break;
                    
                case kSideMenuButtonTag:
                    this->addSettingLayer();
                    break;
                case kLeaveGameButtonTagInSettingScene:
                    this->addSettingLayer();
                    if(APP_MANAGER->isInternetAvilable) {
                        auto popup = LPTPopUp::showPopUpWith(kInformation, kQuitGame, true, GAMEQUIT_POPUP);
                        popup->setPopUpDelegate(this);
                        this->getParent()->addChild(popup, 5000);
                    } else {
                        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoInternetConnection, false, NORMAL_POPUP);
                        this->addChild(gamePopup, 10000);
                    }
                    break;
                    
                case kShopGameButtonTagInSettingScene: {
                    this->addSettingLayer();
                    LPTStoreLayer *layer = LPTStoreLayer::createLayer(kBackToGameSceneBackButtonTag);
                    layer->setPosition(Vec2(0,this->getParent()->getContentSize().height));
                    this->getParent()->addChild(layer, 1000);
                    layer->runAction(MoveTo::create(0.25, Vec2::ZERO));
                }
                break;
                case kSettingGameButtonTagInSettingScene: {
                    this->addSettingLayer();
                    LPTSettingsLayer *layer = LPTSettingsLayer::createLayer(kBackToGameSceneBackButtonTag, false);
                    layer->setPosition(Vec2(0,this->getParent()->getContentSize().height));
                    this->getParent()->addChild(layer, 1000);
                    layer->runAction(MoveTo::create(0.25, Vec2::ZERO));
                    
                }
                default:
                    break;
            }
        }
            break;
            
        case Widget::TouchEventType::MOVED:
            break;
    }
}

LPTPlayerProfileData* LPTHUDLayer::createProfilePlayerObjectFromUser(bool fromUser) {
    if(fromUser) {
        return PLAYER->getPlayerProfileData();
    } else {
        return OPPONENT->getPlayerProfileData();
    }
}

#pragma mark - Game popup callabck
void LPTHUDLayer::okButtonAction(Popup_Type type) {
    switch (type) {
        case LOGOUT_POPUP:
            break;
        case GAMEQUIT_POPUP:{
            AppWarp::Client *appWarpClient = AppWarp::Client::getInstance();
            appWarpClient->leaveRoom(PLAYER->getRoomId());
            SoundManager::playSound(SCENE_CHANGE_SOUND);
//            Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, LPTGameSelectionScene::createScene()));
            
            if(PLAYER->getGameSelected() == GAME_WITH_COMPUTER) {
                Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, LPTGameSelectionScene::createScene()));
            } else {
                this->delegate->gameQuitByUser(); // TODO : Appwarp room leave calls and check if opponent has left
            }
        }
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

void LPTHUDLayer::cancelButtonAction(Popup_Type type) {
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

