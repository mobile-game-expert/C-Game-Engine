//
//  LPTCoinComponent.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 03/10/18.
//

#include "LPTCoinComponent.hpp"

#define kChipCoinTag                            55555
#define kChipCoinLabelTag                       55556

LPTCoinComponent* LPTCoinComponent::createCoinComponent(string coinBaseImageName, Vec2 position, int tag, Vec2 anchorPoint) {
    auto coinComponent = LPTCoinComponent::create();
    coinComponent->initWithSpriteName(coinBaseImageName, position, tag, anchorPoint);
    return coinComponent;
}

bool LPTCoinComponent::initWithSpriteName(string coinBaseImageName, Vec2 position, int tag, Vec2 anchorPoint){
    if (!BaseSprite::initWithFile(coinBaseImageName)) {
        return false;
    }
    this->setPosition(position);
    this->setAnchorPoint(anchorPoint);
    this->setTag(tag);
    return true;
}

#pragma mark SetGameComponentUI
/**
 This method will create coinComponent of GameScene
*/
void LPTCoinComponent::setGameComponentUI(double chipCoinValue) {
    
    BaseSprite *chipCoin = BaseSprite::createWithFileName(kGameCoinChipImageName);
    chipCoin->setPosition(Vec2(this->getContentSize().width * 0.55, this->getContentSize().height * 0.625));
    chipCoin->setTag(kChipCoinTag);
    this->addChild(chipCoin);
    chipCoin->setScale(this->getContentSize().height * 0.6 / chipCoin->getContentSize().height);
    
    string coins = StringUtils::format("%.0f", chipCoinValue);
    Label *chipCoinLabel = Label::createWithTTF(coins.c_str(), kOpenSans_Bold, 40);
    chipCoinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    chipCoinLabel->setPosition(Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.2));
    chipCoinLabel->setColor(Color3B(254, 200, 35));
    chipCoinLabel->setTag(kChipCoinLabelTag);
    this->addChild(chipCoinLabel);
    this->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, this->getScale())), NULL));
}
/**
 
*/
void LPTCoinComponent::setGameSelectionUI(std::function<void(Ref*, Widget::TouchEventType)> callback, int buyButtonTag) {
    //COIN BUTTON
    BaseSprite *chipCoin = BaseSprite::createWithFileName(kGameSelectionTopBarCoinImageName);
    chipCoin->setPosition(Vec2(this->getContentSize().width * 0.0, this->getContentSize().height * 0.5));
    chipCoin->setTag(kChipCoinTag);
    this->addChild(chipCoin);
    chipCoin->setScale(this->getContentSize().height * 1.1 / chipCoin->getContentSize().height);
    
    //BUY COIN BUTTON
    Button *buyCoin = Button::create(kGameSelectionBuyCoinButtonImageName);
    buyCoin->setPosition(Vec2(this->getContentSize().width, this->getContentSize().height * 0.5));
    buyCoin->setTag(buyButtonTag);
    this->addChild(buyCoin);
    
    buyCoin->addTouchEventListener(callback);
    buyCoin->setAnchorPoint(Vec2(1, 0.5));
    buyCoin->setScale(this->getContentSize().height / buyCoin->getContentSize().height
                      );
    
    string coinsValue = StringUtils::format("%.0f", PLAYER->getTotalCoins());
    Label *chipCoinLabel = Label::createWithTTF(coinsValue.c_str(), kOpenSans_Bold, 40);
    chipCoinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    chipCoinLabel->setPosition(Vec2(this->getContentSize().width * 0.2, this->getContentSize().height * 0.5));
    chipCoinLabel->setColor(Color3B(254, 200, 35));
    chipCoinLabel->setTag(kChipCoinLabelTag);
    this->addChild(chipCoinLabel);
    this->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, this->getScale())), NULL));
}


#pragma mark updateCoinText
void LPTCoinComponent::updateCoinText(double coinValue) {
    string coins = StringUtils::format("%.0f", coinValue);
    Label *chipCoinLabel = (Label *)this->getChildByTag(kChipCoinLabelTag);
    chipCoinLabel->setString(coins);
}

#pragma mark

void LPTCoinComponent::onEnter() {
    BaseSprite::onEnter();
}

void LPTCoinComponent::onExit() {
    BaseSprite::onExit();
}
