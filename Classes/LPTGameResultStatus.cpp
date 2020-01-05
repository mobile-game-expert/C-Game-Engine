//
//  LPTGameResultStatus.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 01/10/18.
//

#include "LPTGameResultStatus.hpp"
#include "LPTCoinComponent.hpp"
#include "LPTGameSelectionScene.hpp"
#include "LPTGameScene.hpp"
#include "LPTGameSceneRW.hpp"

#define kResultBaseSpriteTag            11111
#define kCoinBaseTag                    11112
#define kResultTag                      11113
#define kUserProfileTag                 11114
#define kOpponentProfileTag             11115
#define kHomeButtonTag                  11116
#define kRematchButtonTag               11117

LPTGameResultStatus* LPTGameResultStatus::createLayer() {
    auto layer = LPTGameResultStatus::create();
    return layer;
}

bool LPTGameResultStatus::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}

void LPTGameResultStatus::onEnter() {
    BaseLayer::onEnter();
}

void LPTGameResultStatus::setupUI(double coinValue, bool isGameWon, string playerImageName, string opponentPlayerImageName, string playerName, string opponentPlayerName) {
    
    auto touchRestrictButton = this->createButton(kSignupBG, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5), 0);
    touchRestrictButton->setOpacity(0);
    touchRestrictButton->setContentSize(Size(this->getContentSize().width, this->getContentSize().height));
    this->addChild(touchRestrictButton);
    
    LayerColor *base = LayerColor::create(Color4B(0, 0, 0, 0), this->getContentSize().width, this->getContentSize().height);
    this->addChild(base);
    
    resultBaseSprite = BaseLayer::createSprite(kPopupMatchMaking, kResultBaseSpriteTag, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5), this);
    resultBaseSprite->setScale(this->getContentSize().height * 0.6 / resultBaseSprite->getContentSize().height);
    
    //CoinSection
    this->addCoinsSection(coinValue);
    this->addResultStatus(isGameWon);
//    this->addUserProfile(Vec2(resultBaseSprite->getContentSize().width * 0.1, resultBaseSprite->getContentSize().height * 0.6), Vec2(0, 0.5), kUserProfileTag, isGameWon, playerImageName, playerName);
     this->addUserProfile(Vec2(resultBaseSprite->getContentSize().width * 0.05, resultBaseSprite->getContentSize().height * 0.57), Vec2(0, 0.5), kUserProfileTag, isGameWon, playerImageName, PLAYER->getPlayerUniqueName());
    
    this->addUserProfile(Vec2(resultBaseSprite->getContentSize().width * 0.95, resultBaseSprite->getContentSize().height * 0.57), Vec2(1, 0.5), kOpponentProfileTag, isGameWon, opponentPlayerImageName, opponentPlayerName);
   
    
    //AddGameButton
    this->addGameButton(kRematchImageName, kRematchButtonTag, Vec2(this->getContentSize().width * 0.5, resultBaseSprite->getPositionY() - resultBaseSprite->getContentSize().height * resultBaseSprite->getScale() * 0.55));
    this->addGameButton(kHomeSceneImageName, kHomeButtonTag, Vec2(this->getContentSize().width * 0.5, resultBaseSprite->getPositionY() - resultBaseSprite->getContentSize().height * resultBaseSprite->getScale() * 0.725));
    this->runCoinAnimation(isGameWon);
}

void LPTGameResultStatus::hideRematchButton() {
    Button *rematchButton = (Button *)this->getChildByTag(kRematchButtonTag);
    rematchButton->setVisible(false);
}

#pragma mark AddUserProfile
void LPTGameResultStatus::addUserProfile(Vec2 position, Vec2 anchorPoint, int tag, bool isGameWon, string playerProfileName, string playerName) {
    
    BaseSprite *backgroundGlow = BaseLayer::createSprite(kWinnerBackgroundGlowImageName, kResultBaseSpriteTag, position, resultBaseSprite);
    backgroundGlow->runAction(RepeatForever::create(Sequence::create(Spawn::create(RotateBy::create(3.0, 180), FadeOut::create(3.0), NULL), Spawn::create(RotateBy::create(1.0, 120), FadeIn::create(0.3), NULL), NULL)));
    
    BaseSprite *playerProfileBase = BaseLayer::createSprite(kPlayerProfileBoxImageName, tag, position, resultBaseSprite);
    playerProfileBase->setAnchorPoint(anchorPoint);
    playerProfileBase->setScale(resultBaseSprite->getContentSize().height * 0.65 / playerProfileBase->getContentSize().height);
    
    BaseSprite *userProfile = BaseLayer::createSprite(playerProfileName, -1, Vec2(playerProfileBase->getContentSize().width * 0.5, playerProfileBase->getContentSize().height * 0.6), playerProfileBase);
    userProfile->setScale(playerProfileBase->getContentSize().height * 0.6 / userProfile->getContentSize().height);
    
    //PlayerImage
    createTTFLabel(playerName, Vec2(playerProfileBase->getContentSize().width * 0.5, playerProfileBase->getContentSize().height * 0.2), Vec2::ANCHOR_MIDDLE, playerProfileBase, Color3B::WHITE, kOpenSans_Bold, 30);
    
    //WinnerRating
    BaseSprite *winnerRating = BaseLayer::createSprite(kWinnerRatingImageName, -1, Vec2(playerProfileBase->getContentSize().width * 0.5, playerProfileBase->getContentSize().height * 1.1), playerProfileBase);
    winnerRating->setScale(playerProfileBase->getContentSize().width / winnerRating->getContentSize().width);
    winnerRating->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.0), FadeIn::create(0.2),NULL)));
    
    //WinnerRating
    int sign = 1;
    if(tag == kUserProfileTag) {
        backgroundGlow->setVisible(isGameWon);
        winnerRating->setVisible(isGameWon);
    }else if(tag == kOpponentProfileTag) {
        sign = -1;
        backgroundGlow->setVisible(!isGameWon);
        winnerRating->setVisible(!isGameWon);
    }
    //PlayerProfile Image
    backgroundGlow->setPositionX(backgroundGlow->getPosition().x + playerProfileBase->getContentSize().width * 0.5 * sign * playerProfileBase->getScale());
}

#pragma mark ResultStatus
void LPTGameResultStatus::addResultStatus(bool isGameWon) {
    string imageName = kYouLooseImageName;
    if(isGameWon)
        imageName = kYouWinImageName;
    
    BaseSprite *gameResult = BaseLayer::createSprite(imageName, kResultTag, Vec2(resultBaseSprite->getContentSize().width * 0.5, resultBaseSprite->getContentSize().height * 0.7), resultBaseSprite);
    gameResult->setScale(resultBaseSprite->getContentSize().height * 0.425 / gameResult->getContentSize().height);
}

#pragma mark CoinSection
void LPTGameResultStatus::addCoinsSection(double coinValue) {
    LPTCoinComponent *coinComponent = LPTCoinComponent::createCoinComponent(kBetAmountBaseImageName, Vec2(resultBaseSprite->getContentSize().width * 0.5, resultBaseSprite->getContentSize().height * 0.125), kCoinBaseTag, Vec2(0.5, 0));
    coinComponent->setScale((resultBaseSprite->getContentSize().height * 0.35) / coinComponent->getContentSize().height);
    resultBaseSprite->addChild(coinComponent);
    coinComponent->setGameComponentUI(coinValue);
}

#pragma mark AddButtons
/**
 addGameButton: This method will home button and rematch button
 -params:
 imageName: name of the image.
 tag: tag of the image.
 position: position of the button.
 */
void LPTGameResultStatus::addGameButton(string imageName, int tag, Vec2 position) {
    Button *gameButton = createButton(imageName, tag, position, this, CC_CALLBACK_2(LPTGameResultStatus::buttonCallback, this), 1.0);
    gameButton->setScale(this->getContentSize().height * 0.1 / gameButton->getContentSize().height);
}

#pragma mark CoinAnimation
/**
 This will addCoin Animatin on winning
 */
void LPTGameResultStatus::runCoinAnimation(bool isGameWon) {
    for(int coinCounter = 0; coinCounter < 10; coinCounter++) {
        float sign = isGameWon ? -1 : 1;
        
        BaseSprite *coinSprite = BaseSprite::createWithFileName(kGameSelectionTopBarCoinImageName);
        coinSprite->setPosition(Vec2(resultBaseSprite->getContentSize().width * 0.525, resultBaseSprite->getContentSize().height * 0.35));
        resultBaseSprite->addChild(coinSprite);
        manageSoundEffects();
        coinSprite->runAction(Sequence::create(DelayTime::create(coinCounter * 0.15), Spawn::create(FadeTo::create(0.1, 255), MoveBy::create(0.5, Vec2(resultBaseSprite->getContentSize().width * 0.20 * sign, -resultBaseSprite->getContentSize().height * 0.12)), NULL), Spawn::create(FadeTo::create(0.1, 100), MoveBy::create(0.5, Vec2(resultBaseSprite->getContentSize().width * 0.125 * sign, resultBaseSprite->getContentSize().height * 0.25)), NULL), RemoveSelf::create(), NULL));
    }
}

#pragma mark - Manage sound
void LPTGameResultStatus::manageSoundEffects() {
    if(SoundManager::isBackgroundMusicPlaying()) {
        SoundManager::stopMusic();
    }
    SoundManager::playSound(COINS_SOUND);
}


#pragma mark - Button callback
void LPTGameResultStatus::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case kHomeButtonTag:
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    Director::getInstance()->replaceScene(LPTGameSelectionScene::createScene());
                    break;
                case kRematchButtonTag:
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    if(PLAYER->getGameSelected() == GAME_ONE_ON_ONE || PLAYER->getGameSelected() == GAME_WITH_FRIEND) {
//                        Director::getInstance()->replaceScene(LPTGameSceneRW::createScene());
                        if(delegate) delegate->reMatchButtonAction();
                    }else {
                        Director::getInstance()->replaceScene(LPTGameScene::createScene());
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
void LPTGameResultStatus::onExit() {
    BaseLayer::onExit();
}
