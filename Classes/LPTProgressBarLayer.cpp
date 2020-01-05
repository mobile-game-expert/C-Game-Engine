//
//  LPTProgressBarLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 04/12/18.
//

#include "LPTProgressBarLayer.hpp"


#define     kXP_STAR_IMAGE_TAG              85213
#define     kXP_BAR_BASE_TAG                85214
#define     kXP_BAR_TAG                     85215

LPTProgressBarLayer* LPTProgressBarLayer::createLayer() {
    auto layer = LPTProgressBarLayer::create();
    layer->init();
    return layer;
}

bool LPTProgressBarLayer::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}

void LPTProgressBarLayer::setupUI(double currentXP, double totalXP, float scale) {
    
    Sprite *barSprite = BaseLayer::createSprite("xp_bar.png", kXP_BAR_BASE_TAG, (GameUtils::isIphoneX ? Vec2(this->getContentSize().width * 0.023, - this->getContentSize().height * 0.0125) : Vec2(this->getContentSize().width * 0.0125, -this->getContentSize().height * 0.015)), this);
    barSprite->setScale(scale);
    barSprite->setAnchorPoint(Vec2::ZERO);
    
    this->setContentSize(barSprite->getContentSize());
    
    LoadingBar *bar = LoadingBar::create("xp_filled_bar.png", (currentXP / totalXP) * 100);
    bar->setDirection(LoadingBar::Direction::LEFT);
    bar->setPosition(Vec2(barSprite->getContentSize().width * 0.5, barSprite->getContentSize().height * 0.5));
    barSprite->addChild(bar);
    
    
    //XP Image
    BaseLayer::createSprite("xp_star.png", kXP_STAR_IMAGE_TAG, Vec2(barSprite->getContentSize().width * 0.0, barSprite->getContentSize().height * 0.5), barSprite);
    
    //
    string xp_data = StringUtils::format("%.0f / %.0f", currentXP, totalXP);
    createTTFLabel(xp_data, Vec2(barSprite->getContentSize().width * 0.5, barSprite->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE, barSprite, Color3B::WHITE, kNOUGAT_EXTRABLACK_1, 25);
}


void LPTProgressBarLayer::onEnter() {
    BaseLayer::onEnter();
}

void LPTProgressBarLayer::onExit() {
    BaseLayer::onExit();
}
