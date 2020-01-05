//
//  LPTPlayerProfileLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 01/10/18.
//

#include "LPTPlayerProfileLayer.hpp"
#include "LPTTopBar.hpp"
#include "LPTProfilePicture.hpp"
#include "LPTPlayer.hpp"
#include "LPTProgressBarLayer.hpp"

#define kTopLayerTag                    22222
#define kPlayerProfileBaseTag           22223
#define kPlayerCoinImageTag             22224
#define kPlayerCoinBaseTag              22225
#define kChipCoinLabelTag               22226
#define kPlayerNameBarImageTag          22227
#define kPlayerNameTag                  22228
#define kChangeImageTag                 22229

#define kChangeImageImageName           "changeImage.png"

//GameText
#define kPlayerLevelText                "Player Level:"
#define kGamesPlayedText                "Games Played:"
#define kGamesWonText                   "Games Won:"
#define kWinPercentageText              "Win Percentage:"
#define kUniqueIDText                   "Unique ID:"
#define kPlayerStatsText                "Player Stats"


//
LPTPlayerProfileLayer* LPTPlayerProfileLayer::createLayer(int backButtonTag, LPTPlayerProfileData *player) {
    auto layer = LPTPlayerProfileLayer::create();
    layer->initWithTag(backButtonTag, player);
    return layer;
}

bool LPTPlayerProfileLayer::initWithTag(int buttonTag, LPTPlayerProfileData *player) {
    if (!BaseLayer::init()) {
        return false;
    }
    backButtonTag = buttonTag;
    this->player = player;
    return true;
}

void LPTPlayerProfileLayer::onEnter() {
    BaseLayer::onEnter();
    
    //
    this->setBackgroundSprite(kGameSelectionBGImageName);
    LayerColor *layerColor = LayerColor::create(Color4B(16, 6, 8, 150), this->getContentSize().width, this->getContentSize().height);
    this->addChild(layerColor);
    
    //
    auto touchRestrictButton = this->createButton(kSignupBG, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5), 0);
    touchRestrictButton->setOpacity(0);
    touchRestrictButton->setContentSize(Size(this->getContentSize().width, this->getContentSize().height));
    this->addChild(touchRestrictButton);
    //
    this->addTopBarInUserProfileScene(backButtonTag);
    this->addUserProfileBase();
    //
}

#pragma mark - TOPBAR
void LPTPlayerProfileLayer::addTopBarInUserProfileScene(int backButtonTag) {
    LPTTopBar *layer = GameUtils::createTopBar(this, kUserProfileTopBarImageName);
    layer->setTag(kTopLayerTag);
    layer->addUserProfileTopBar(backButtonTag, kPlayerProfileScreenTitle, CC_CALLBACK_2(LPTPlayerProfileLayer::buttonCallback, this));
}

void LPTPlayerProfileLayer::buttonCallback(Ref* sender, Widget::TouchEventType type){
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
                case kChangeImageTag : {
                    LPTAvatarSelectionLayer *layer = LPTAvatarSelectionLayer::createLayer();
                    layer->setAvatarDelegate(this);
                    layer->setPosition(Vec2(0,0));
                    this->addChild(layer, 1000);
//                    layer->runAction(MoveTo::create(0.25, Vec2::ZERO));
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


void LPTPlayerProfileLayer::addUserProfileBase() {
    
    LPTTopBar *topBar = (LPTTopBar *)this->getChildByTag(kTopLayerTag);
    float topBaseHeight = topBar->getScale() * topBar->getContentSize().height;
    
    BaseSprite *playerProfileBase = BaseLayer::createSprite(kPlayerProfileBaseImageName, kPlayerProfileBaseTag, Vec2(this->getContentSize().width * 0.5, (this->getContentSize().height - topBaseHeight) * 0.5), this);
    
    playerProfileBase->setScale(this->getContentSize().width * (GameUtils::isIphoneX ? 0.55 : 0.7) / playerProfileBase->getContentSize().width);
    
    //AddName Bar
    this->addPlayerProfileNameBar(playerProfileBase);
    this->addPlayerStats(playerProfileBase);
    
    //Add Profile Pic
    string profilePic = player->getPlayerProfilePic();
    string imageName = profilePic.compare("") == 0 ?  kPlayerProfileImageName: player->getPlayerProfilePic();
    
    //
    this->addPlayerProfile(imageName, Vec2(playerProfileBase->getContentSize().width * 0.5, playerProfileBase->getContentSize().height * 0.85), Vec2(0, 0), kUserProfilePicTag, playerProfileBase);
    
    //Animation
//    playerProfileBase->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, playerProfileBase->getScale())), NULL));
}

#pragma mark AddProfile
void LPTPlayerProfileLayer::addPlayerProfile(string imageName, Vec2 position, Vec2 anchorPoint, int tag, BaseSprite *parent) {
    LPTProfilePicture *profilePicture = new LPTProfilePicture();
    profilePicture->setContentSize(Size(parent->getContentSize().height * 0.2, parent->getContentSize().height * 0.2));
     profilePicture->setPosition(Vec2(position.x - profilePicture->getContentSize().width * 0.5, position.y - profilePicture->getContentSize().height * 0.5));
    profilePicture->setAnchorPoint(anchorPoint);
    profilePicture->setTag(tag);
    profilePicture->createProfilePictureView(imageName, kPlayerProfileImageName, Vec2(profilePicture->getContentSize().width * 0.5, profilePicture->getContentSize().height * 0.5), tag, false, NULL);
    parent->addChild(profilePicture, 10);
    
    //Edit button
    Button *editSprite = createButton(kChangeImageImageName, kChangeImageTag, Vec2(profilePicture->getContentSize().width * 0.95, profilePicture->getContentSize().height * 0.95), profilePicture, CC_CALLBACK_2(LPTPlayerProfileLayer::buttonCallback, this), 1);
    editSprite->setLocalZOrder(100);
    
    //Coins
    this->addCoinsDescription(parent, Vec2(profilePicture->getPosition().x + profilePicture->getContentSize().width * 0.5, profilePicture->getPosition().y - profilePicture->getContentSize().height * 0.35));
}
#pragma mark AddPlayerXP
void LPTPlayerProfileLayer::addPlayerXP(double playerCurrentXP, double playerTotalXP) {
    BaseSprite *baseProfile = (BaseSprite*)this->getChildByTag(kPlayerProfileBaseTag);
    LPTProgressBarLayer *layer = LPTProgressBarLayer::createLayer();
    layer->setPosition(Vec2(baseProfile->getContentSize().width * 0.60, baseProfile->getContentSize().height * 0.8));
    layer->setupUI(playerCurrentXP, playerTotalXP, 1.5);
    baseProfile->addChild(layer);
}

#pragma mark CoinDisplay
void LPTPlayerProfileLayer::addCoinsDescription(BaseSprite *parent, Vec2 position) {
    
    //Coins
    BaseSprite *chipCoinBase = BaseLayer::createSprite(kGameSelectionTopBarCoinDisplayImageName, kPlayerCoinBaseTag, Vec2(position.x, position.y), parent);
    chipCoinBase->setScale(parent->getContentSize().width * 0.2 / chipCoinBase->getContentSize().width);
    
    //COIN BUTTON
    BaseSprite *coinImage = BaseLayer::createSprite(kGameSelectionTopBarCoinImageName, kPlayerCoinImageTag, Vec2(-chipCoinBase->getContentSize().width * 0.2, chipCoinBase->getContentSize().height * 0.5), chipCoinBase);
    coinImage->setScale(chipCoinBase->getContentSize().height * 1.05 / coinImage->getContentSize().height
                        );
    string coinsValue = StringUtils::format("%.0f", player->getPlayerCoins());
    createTTFLabel(coinsValue.c_str(), Vec2(chipCoinBase->getContentSize().width * 0.5, chipCoinBase->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE, chipCoinBase, Color3B(254, 200, 35), kOpenSans_Bold, 60, kChipCoinLabelTag);
}

#pragma mark PlayerUnique Name
void LPTPlayerProfileLayer::addPlayerProfileNameBar(BaseSprite *parent) {
    
    //Coins
    BaseSprite *playerNameBase = BaseLayer::createSprite(kPlayerUniqueNameImageName, kPlayerNameBarImageTag, Vec2(parent->getContentSize().width * 0.5, parent->getContentSize().height * 0.09), parent);
    playerNameBase->setScale(parent->getContentSize().width * 0.375 / playerNameBase->getContentSize().width);
    
    log("Player Name: %s", player->getPlayerUserName().c_str());
    string userName = player->getPlayerUserName().c_str();
//    if(player->getUserName().length() > 0) {
//        userName = player->getPlayerUserName().c_str();
//    }
    
    createTTFLabel(userName, Vec2(playerNameBase->getContentSize().width * 0.5, playerNameBase->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE, playerNameBase, Color3B::WHITE, kOpenSans_Bold, 50, kPlayerNameTag);
}

void LPTPlayerProfileLayer::addPlayerStats(BaseSprite *parent) {
    int fontSize        =   50;
    Color3B statsColor              =   Color3B(15, 97, 157);
    Color3B statsDescriptionColor   =   Color3B::WHITE;
    
    createTTFLabel(kPlayerStatsText, Vec2(parent->getContentSize().width * 0.5,  parent->getContentSize().height * 0.55), Vec2::ANCHOR_MIDDLE, parent, statsDescriptionColor, kOpenSans_Bold, fontSize, -1);
    
    
    Label *playerLevelText = createTTFLabel(kPlayerLevelText, Vec2(parent->getContentSize().width * 0.2, parent->getContentSize().height * 0.46), Vec2::ANCHOR_MIDDLE_LEFT, parent, statsColor, kOpenSans_Bold, fontSize, -1);
    
    Label *gamePlayedLabel = createTTFLabel(kGamesPlayedText, Vec2(parent->getContentSize().width * 0.2, parent->getContentSize().height * 0.39), Vec2::ANCHOR_MIDDLE_LEFT, parent, statsColor, kOpenSans_Bold, fontSize, -1);
    
    Label *gamesWonLabel = createTTFLabel(kGamesWonText, Vec2(parent->getContentSize().width * 0.2, parent->getContentSize().height * 0.32), Vec2::ANCHOR_MIDDLE_LEFT, parent, statsColor, kOpenSans_Bold, fontSize, -1);

    Label *gamesWinPercentage = createTTFLabel(kWinPercentageText, Vec2(parent->getContentSize().width * 0.2, parent->getContentSize().height * 0.25), Vec2::ANCHOR_MIDDLE_LEFT, parent, statsColor, kOpenSans_Bold, fontSize, -1);
    
    string totalGame = StringUtils::format("%.0f", player->getPlayerGamePlayed());
    createTTFLabel(totalGame.c_str(), Vec2(parent->getContentSize().width * 0.65, gamePlayedLabel->getPosition().y), Vec2::ANCHOR_MIDDLE_LEFT, parent, statsDescriptionColor, kOpenSans_Bold, fontSize, -1);
    
    string gameWon = StringUtils::format("%.0f", player->getPlayerGameWon());
    createTTFLabel(gameWon.c_str(), Vec2(parent->getContentSize().width * 0.65, gamesWonLabel->getPosition().y), Vec2::ANCHOR_MIDDLE_LEFT, parent, statsDescriptionColor, kOpenSans_Bold, fontSize, -1);
    
    float winPercentage = 0.0;
    if(player->getPlayerGameWon() != 0) {
         winPercentage = (player->getPlayerGameWon() * 1.0 / (player->getPlayerGamePlayed()) * 1.0) * 100.0;
    }
    string winPercentageInString = StringUtils::format("%.0f%%", winPercentage);
    createTTFLabel(winPercentageInString, Vec2(parent->getContentSize().width * 0.65,  gamesWinPercentage->getPosition().y), Vec2::ANCHOR_MIDDLE_LEFT, parent, statsDescriptionColor, kOpenSans_Bold, fontSize, -1);
    
    //LEVEL
    string playerLevelValue = StringUtils::format("%.0f", player->getPlayerGameLevel());
    createTTFLabel(playerLevelValue, Vec2(parent->getContentSize().width * 0.65,  playerLevelText->getPosition().y), Vec2::ANCHOR_MIDDLE_LEFT, parent, statsDescriptionColor, kOpenSans_Bold, fontSize, -1);
    
    createTTFLabel(kUniqueIDText, Vec2(parent->getContentSize().width * 0.5,  parent->getContentSize().height * 0.18), Vec2::ANCHOR_MIDDLE, parent, statsColor, kOpenSans_Bold, fontSize, -1);
}

void LPTPlayerProfileLayer::onExit() {
    BaseLayer::onExit();
}

void LPTPlayerProfileLayer::disableEditButtton(){
    BaseSprite *baseProfile = (BaseSprite*)this->getChildByTag(kPlayerProfileBaseTag);
    LPTProfilePicture *profilePicture = (LPTProfilePicture*)baseProfile->getChildByTag(kUserProfilePicTag);
    Button *editButton = (Button*)profilePicture->getChildByTag(kChangeImageTag);
    editButton->setVisible(false);
}

#pragma mark - Avatar Delegate Method
void LPTPlayerProfileLayer::profilePictureUpdated(string imageName) {
    BaseSprite *profileBase = (BaseSprite*)this->getChildByTag(kPlayerProfileBaseTag);
    LPTProfilePicture *profilePicture = (LPTProfilePicture *)profileBase->getChildByTag(kUserProfilePicTag);
    profilePicture->updatePicture(imageName, kUserProfilePicTag);
}
