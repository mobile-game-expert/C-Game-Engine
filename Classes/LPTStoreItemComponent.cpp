//
//  LPTStoreItemComponent.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 05/10/18.
//

#include "LPTStoreItemComponent.hpp"

#define kStoreItemBoxImageName                      "storeItembox.png"
#define kBuyButtonImageName                         "buy_btn.png"


LPTStoreItemComponent* LPTStoreItemComponent::createStoreItemComponent( Vec2 position, int tag) {
    auto storeItemComponent = LPTStoreItemComponent::create();
    storeItemComponent->initWithSpriteName(position, tag);
    return storeItemComponent;
}

bool LPTStoreItemComponent::initWithSpriteName(Vec2 position, int tag){
    if (!BaseSprite::initWithFile(kStoreItemBoxImageName)) {
        return false;
    }
    this->setPosition(position);
    this->setTag(tag);
    return true;
}

#pragma mark
void LPTStoreItemComponent::onEnter() {
    BaseSprite::onEnter();
}

void LPTStoreItemComponent::onExit() {
    BaseSprite::onExit();
}

#pragma mark SetGameComponentUI
void LPTStoreItemComponent::setStoreItemsUI(StoreItem currentItem, std::function<void(Ref*, Widget::TouchEventType)> callback, int buyButtonTag) {
    
    //Title Label
    Label *label = Label::createWithTTF(currentItem.title.c_str(), kOpenSans_Bold, 48);
    label->setPosition(Vec2(this->getContentSize().width * 0.5,  this->getContentSize().height * 0.9));
    label->setColor(Color3B::WHITE);
    label->setTag(this->getTag());
    this->addChild(label);
    
    //Coin Images
    BaseSprite *coin = BaseSprite::createWithFileName(currentItem.coinImageName);
    coin->setPosition(Vec2(this->getContentSize().width*0.35, this->getContentSize().height*0.5));
    coin->setTag(this->getTag());
    this->addChild(coin);
    coin->setScale(this->getContentSize().height*0.85 / coin->getContentSize().height);
    
    //Pricing Label
    Label *coinsLabel = Label::createWithTTF(currentItem.buysCoins.c_str(), kOpenSans_Bold, 64);
    coinsLabel->setPosition(Vec2(this->getContentSize().width * 0.75,  this->getContentSize().height * 0.7));
    coinsLabel->setColor(Color3B(254, 200, 35));
    coinsLabel->setTag(this->getTag());
    this->addChild(coinsLabel);
    
    Label *forLabel = Label::createWithTTF("for", kOpenSans_Bold, 36);
    forLabel->setPosition(Vec2(this->getContentSize().width * 0.75, coinsLabel->getPositionY() - coinsLabel->getContentSize().height*0.6));
    forLabel->setColor(Color3B::WHITE);
    forLabel->setTag(this->getTag());
    this->addChild(forLabel);
    
    Label *priceLabel = Label::createWithTTF(currentItem.forPrice, kOpenSans_Bold, 52);
    priceLabel->setPosition(Vec2(this->getContentSize().width * 0.75, forLabel->getPositionY() - forLabel->getContentSize().height*1));
    priceLabel->setColor(Color3B::WHITE);
    priceLabel->setTag(this->getTag());
    this->addChild(priceLabel);
    
    //Buy Button
    Button *btnBuy = Button::create(kBuyButtonImageName);
    btnBuy->setPosition(Vec2(this->getContentSize().width *0.5, this->getContentSize().height*0.15));
    btnBuy->setTag(buyButtonTag);
    this->addChild(btnBuy);
    btnBuy->addTouchEventListener(callback);
    btnBuy->setScale(this->getContentSize().width*0.95 / btnBuy->getContentSize().width);
}

