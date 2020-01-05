//
//  LPTPlayerMatchingLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 04/10/18.
//

#include "LPTPlayerMatchingLayer.hpp"
#include "LPTCoinComponent.hpp"
#include "LPTProfilePicture.hpp"

#define kMatchBaseSpriteTag                 4001
#define kUserProfileTag                     4002
#define kOpponentProfileTag                 4003
#define kVsSpriteTag                        4004
#define kCoinBaseTag                        4005
#define kChipCoinLabelTag                   4009
#define kUserProfileImageTag                4011
#define kOpponentProfileImageTag            4012
#define kOpponentUniqueNameTag              4013
#define kUserUniqueNameTag                  4014
#define kPlayerStatusTag                    4015

LPTPlayerMatchingLayer* LPTPlayerMatchingLayer::createLayer() {
    auto layer = LPTPlayerMatchingLayer::create();
    return layer;
}

bool LPTPlayerMatchingLayer::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}

void LPTPlayerMatchingLayer::onEnter() {
    BaseLayer::onEnter();
}

void LPTPlayerMatchingLayer::onExit() {
    BaseLayer::onExit();
}
void LPTPlayerMatchingLayer::setupUIForPlayerMatching(double coinValue, bool isGameWon, string opponentPlayerImageName, string playerImageName, string playerName, string opponentPlayerName) {
    
    matchBaseSprite = BaseLayer::createSprite(kPopupMatchMaking, kMatchBaseSpriteTag, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.43), this);
    matchBaseSprite->setScale(this->getContentSize().height * (GameUtils::isIphoneX ? 0.65 : 0.6) / matchBaseSprite->getContentSize().height);
    
    //Searching Label
    BaseLayer::createTTFLabel(kSearchingForOpponent, Vec2(this->getContentSize().width * 0.5, matchBaseSprite->getPositionY() + matchBaseSprite->getContentSize().height * matchBaseSprite->getScale() * 0.55), Vec2::ANCHOR_MIDDLE, this, Color3B::WHITE, kOpenSans_Bold, 36);
    
    //PlayerStatus
    BaseLayer::createTTFLabel("Connecting...", Vec2(matchBaseSprite->getContentSize().width * 0.5, matchBaseSprite->getContentSize().height * 0.125), Vec2::ANCHOR_MIDDLE, matchBaseSprite, Color3B::WHITE, kNOUGAT_EXTRABLACK_1, 36, kPlayerStatusTag);
    
    //CoinSection
    this->addCoinsSection(0); //Initially set 0
    this->addVSTitle();
    //
    this->addUserProfile(Vec2(matchBaseSprite->getContentSize().width * 0.05, matchBaseSprite->getContentSize().height * 0.57), Vec2(0, 0.5), kUserProfileTag, isGameWon, playerImageName, PLAYER->getPlayerUniqueName());
    this->addUserCoins(coinValue, Vec2(0.5, 0.5), false , kMatchingSceneChipCoinTagForUser);
    this->addUserProfile(Vec2(matchBaseSprite->getContentSize().width * 0.95, matchBaseSprite->getContentSize().height * 0.57), Vec2(1, 0.5), kOpponentProfileTag, isGameWon, opponentPlayerImageName, opponentPlayerName);
    this->addUserCoins(coinValue, Vec2(0.5, 0.5), true, kMatchingSceneChipCoinTagForOpponent);
    this->opponentSearchingAnimation();
}
#pragma mark updateUserStatus
void LPTPlayerMatchingLayer::updatePlayerAppwarpStatus(string text) {
    BaseSprite *baseSprite  =   (BaseSprite *)this->getChildByTag(kMatchBaseSpriteTag);
    Label *playerStatus     =   (Label *)baseSprite->getChildByTag(kPlayerStatusTag);
    playerStatus->setString(text);
}
#pragma mark AddUserProfile
void LPTPlayerMatchingLayer::addUserProfile(Vec2 position, Vec2 anchorPoint, int tag, bool isGameWon, string playerProfileName, string playerName) {
    
    BaseSprite *playerProfileBase = BaseLayer::createSprite(kPlayerProfileBoxImageName, tag, position, matchBaseSprite);
    playerProfileBase->setAnchorPoint(anchorPoint);
    playerProfileBase->setScale(matchBaseSprite->getContentSize().height * 0.70 / playerProfileBase->getContentSize().height);
    
    BaseSprite *userProfile = BaseLayer::createSprite(playerProfileName, -1, Vec2(playerProfileBase->getContentSize().width * 0.5, playerProfileBase->getContentSize().height * 0.6), playerProfileBase);
    userProfile->setScale(playerProfileBase->getContentSize().height * 0.6 / userProfile->getContentSize().height);
    userProfile->setOpacity((tag == kUserProfileTag ? 255 : 0));
    int tagImage = tag == kUserProfileTag ? kUserProfileImageTag : kOpponentProfileImageTag;
//    userProfile->setTag(tagImage);
    
    LPTProfilePicture *profilePicture = new LPTProfilePicture();
    profilePicture->setContentSize(userProfile->getContentSize());
    profilePicture->setPosition(Vec2(userProfile->getPositionX()- userProfile->getContentSize().width * 0.5,userProfile->getPositionY() - userProfile->getContentSize().height * 0.5));
    profilePicture->setTag(tagImage);
    profilePicture->setScale(userProfile->getScale());
    profilePicture->createProfilePictureView(playerProfileName, kPlayerProfileImageName, Vec2(profilePicture->getContentSize().width * 0.5, profilePicture->getContentSize().height * 0.5), tag, false);
    playerProfileBase->addChild(profilePicture, 10);
    
    //Player unique Label
    int labelTag = tag == kUserProfileTag ? kUserUniqueNameTag : kOpponentUniqueNameTag;
    createTTFLabel(playerName, Vec2(playerProfileBase->getContentSize().width * 0.5, playerProfileBase->getContentSize().height * 0.2), Vec2::ANCHOR_MIDDLE, playerProfileBase, Color3B::WHITE, kOpenSans_Bold, 30, labelTag);
}

#pragma mark ResultStatus
void LPTPlayerMatchingLayer::addVSTitle() {
    string imageName = kVSImageName;
    BaseSprite *vsSprite = BaseLayer::createSprite(imageName, kVsSpriteTag, Vec2(matchBaseSprite->getContentSize().width * 0.5, matchBaseSprite->getContentSize().height * 0.75), matchBaseSprite);
    vsSprite->setScale(matchBaseSprite->getContentSize().height * 0.25 / vsSprite->getContentSize().height);
}

#pragma mark CoinSection
void LPTPlayerMatchingLayer::addCoinsSection(double coinValue) {
    
    LPTCoinComponent *coinComponent = LPTCoinComponent::createCoinComponent(kBetAmountBaseImageName, Vec2(matchBaseSprite->getContentSize().width * 0.5, matchBaseSprite->getContentSize().height * 0.2), kCoinBaseTag, Vec2(0.5, 0));
    coinComponent->setScale((matchBaseSprite->getContentSize().height * 0.35) / coinComponent->getContentSize().height);
    matchBaseSprite->addChild(coinComponent);
    
    BaseSprite *backgroundGlow = BaseLayer::createSprite(kWinnerBackgroundGlowImageName, kMatchBaseSpriteTag, Vec2(coinComponent->getContentSize().width*0.5, coinComponent->getContentSize().height*0.5), coinComponent);
    backgroundGlow->setOpacity(150);
    backgroundGlow->setScale(0.7);
    backgroundGlow->runAction(RepeatForever::create(RotateBy::create(3.0, 180)));
    coinComponent->setGameComponentUI(coinValue);
}

#pragma mark CoinSection
void LPTPlayerMatchingLayer::addUserCoins(double coinValue, Vec2 anchorPoint, bool isOpponent, int tag) {
    int tagBase = isOpponent == true ? kOpponentProfileTag : kUserProfileTag;
    BaseSprite *playerProfileBase = (BaseSprite*)matchBaseSprite->getChildByTag(tagBase);
    float xFactor  = isOpponent == true ? -playerProfileBase->getContentSize().width * 0.7 : playerProfileBase->getContentSize().width * 0.8;
    Vec2 position = Vec2(playerProfileBase->getPositionX() + xFactor, playerProfileBase->getPositionY() - playerProfileBase->getContentSize().height * 0.9);
    LPTCoinComponent *coinComponent = LPTCoinComponent::createCoinComponent(kGameSelectionTopBarCoinDisplayImageName, position, tag, anchorPoint);
    coinComponent->setScale(playerProfileBase->getContentSize().width * 0.9 / coinComponent->getContentSize().width);
    matchBaseSprite->addChild(coinComponent);
    
    BaseSprite *chipCoin = BaseSprite::createWithFileName(kGameSelectionTopBarCoinImageName);
    chipCoin->setPosition(Vec2(coinComponent->getContentSize().width * 0.0, coinComponent->getContentSize().height * 0.5));
    coinComponent->addChild(chipCoin);
    chipCoin->setScale(coinComponent->getContentSize().height * 1.1 / chipCoin->getContentSize().height);
    
    string coinsValue = StringUtils::format("%.0f", coinValue);
    Label *chipCoinLabel = Label::createWithTTF(coinsValue.c_str(), kOpenSans_Bold, 60);
    chipCoinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    chipCoinLabel->setPosition(Vec2(coinComponent->getContentSize().width * 0.2, coinComponent->getContentSize().height * 0.5));
    chipCoinLabel->setColor(Color3B(254, 200, 35));
    chipCoinLabel->setTag(tag);
    coinComponent->addChild(chipCoinLabel);
}
#pragma mark - Coins Animation
void LPTPlayerMatchingLayer::runCoinAnimationForUser(bool fromUser) {
    for(int coinCounter = 0; coinCounter < 10; coinCounter++) {
        float sign = fromUser ? 1 : -1;
        
        BaseSprite *coinSprite = BaseSprite::createWithFileName(kGameSelectionTopBarCoinImageName);
        coinSprite->setPosition(Vec2(matchBaseSprite->getContentSize().width * (fromUser ? 0.2 : 0.8), matchBaseSprite->getContentSize().height * 0.22));
        coinSprite->setScale(0.6);
        matchBaseSprite->addChild(coinSprite);
        manageSoundEffects();
        coinSprite->runAction(Sequence::create(DelayTime::create(coinCounter * 0.15),
                                               Spawn::create(FadeTo::create(0.1, 255),
                                                             MoveBy::create(0.5, Vec2(matchBaseSprite->getContentSize().width * 0.1 * sign, -matchBaseSprite->getContentSize().height * 0.05)),
                                                             NULL),
                                               Spawn::create(FadeTo::create(0.1, 100),
                                                             MoveBy::create(0.5, Vec2(matchBaseSprite->getContentSize().width * 0.125 * sign, matchBaseSprite->getContentSize().height * 0.05)),
                                                             NULL),
                                               RemoveSelf::create(),
                                               NULL));
    }
}

#pragma mark - Manage sound
void LPTPlayerMatchingLayer::manageSoundEffects() {
    if(SoundManager::isBackgroundMusicPlaying()) {
        SoundManager::stopMusic();
    }
    SoundManager::playSound(COINS_SOUND);
}

#pragma mark - Opponent Searching animation
void LPTPlayerMatchingLayer::opponentSearchingAnimation() {
    BaseSprite *opponentProfileBase = (BaseSprite*)matchBaseSprite->getChildByTag(kOpponentProfileTag);
    LPTProfilePicture *opponentImage = (LPTProfilePicture*)opponentProfileBase->getChildByTag(kOpponentProfileImageTag);
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("AvatarsSheet.plist");
    auto changeTexture = CallFunc::create([opponentImage, cache](){
        opponentImage->updatePictureWithSpriteFrame(cache->getSpriteFrameByName(GameUtils::getBotProfilePictureName()), kOpponentProfileTag);
    });
    opponentImage->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.1), changeTexture,NULL)));
}


void LPTPlayerMatchingLayer::stopSearchingAnimation() {
    BaseSprite *opponentProfileBase = (BaseSprite*)matchBaseSprite->getChildByTag(kOpponentProfileTag);
    LPTProfilePicture *opponentImage = (LPTProfilePicture*)opponentProfileBase->getChildByTag(kOpponentProfileImageTag);
    opponentImage->stopAllActions();
}

#pragma mark - After search UI update
void LPTPlayerMatchingLayer:: updateOpponentProfile(string imageName){
    BaseSprite *opponentProfileBase = (BaseSprite*)matchBaseSprite->getChildByTag(kOpponentProfileTag);
    LPTProfilePicture *opponentImage = (LPTProfilePicture*)opponentProfileBase->getChildByTag(kOpponentProfileImageTag);
    opponentImage->updatePicture(imageName, kOpponentProfileTag);
}

void LPTPlayerMatchingLayer::updateCoinsAfterBet(double coins, int tag) {
    LPTCoinComponent *coinComponent = (LPTCoinComponent*)matchBaseSprite->getChildByTag(tag);
//    BaseSprite *chipCoin = (BaseSprite*)coinComponent->getChildByTag(tag);
    string coinStr = StringUtils::format("%.0f", coins);
    Label *chipCoinLabel = (Label *)coinComponent->getChildByTag(tag);
    chipCoinLabel->setString(coinStr);
}

void LPTPlayerMatchingLayer::updateOpponentUniqueName(string name) {
    BaseSprite *baseProfile = (BaseSprite*)matchBaseSprite->getChildByTag(kOpponentProfileTag);
    Label* opponentLabel = (Label*)baseProfile->getChildByTag(kOpponentUniqueNameTag);
    opponentLabel->setString(name);
}

void LPTPlayerMatchingLayer::updateBetAmount(double amount) {
    LPTCoinComponent *coinSection = (LPTCoinComponent*)matchBaseSprite->getChildByTag(kCoinBaseTag);
    coinSection->updateCoinText(amount);
}
