//
//  LPTSideMenuLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 08/10/18.
//

#include "LPTSideMenuLayer.hpp"

#define kBaseSideMenuTag            333333

LPTSideMenuLayer* LPTSideMenuLayer::createLayer() {
    auto layer = LPTSideMenuLayer::create();
    return layer;
}

bool LPTSideMenuLayer::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}

void LPTSideMenuLayer::onEnter() {
    BaseLayer::onEnter();
}

void LPTSideMenuLayer::setupUI(Size layerSize, std::function<void(Ref*, Widget::TouchEventType)> callback) {
    //kSideMenuPopUpImageName
    this->setContentSize(layerSize);
    sideMenuBase = BaseLayer::createSprite(kSideMenuPopUpImageName, kBaseSideMenuTag, Vec2(0, 0), this);
    sideMenuBase->setPosition(Vec2(this->getContentSize().width * (GameUtils::isIphoneX ? 1.025 :  1.035), this->getContentSize().height * 1.02));
    sideMenuBase->setScale(this->getContentSize().width * (GameUtils::isIphoneX ? 0.6 :  0.9) / sideMenuBase->getContentSize().width);
    sideMenuBase->setAnchorPoint(Vec2(1, 1));
    
    //
    LayerColor *layerColor = LayerColor::create(Color4B(255, 255, 255, 0), this->getContentSize().width, this->getContentSize().height);
    this->addChild(layerColor);
    // AddButtonLayer
    this->addButtons(callback);
}

void LPTSideMenuLayer::addButtons(std::function<void(Ref*, Widget::TouchEventType)> callback) {
    Vec2 initialPosition    =   Vec2(sideMenuBase->getContentSize().width * 0.5, sideMenuBase->getContentSize().height * 0.05);
    int initialTag          =   kLeaveGameButtonTagInSettingScene;
    vector<string>imageName;
    imageName.push_back(kLeaveGameSettingSceneImageName);
    imageName.push_back(kShopButtonSettingSceneImageName);
    imageName.push_back(kSettingButtonInSettingSceneImageName);

    for(int buttonCounter = 0; buttonCounter < 3; buttonCounter++) {
        Button *settingButtons = createButton(imageName[buttonCounter], initialTag, initialPosition, sideMenuBase, callback, 1);
        settingButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        initialPosition.y = initialPosition.y + settingButtons->getContentSize().height * 1.1;
        initialTag          =   initialTag + 1;
    }
}

void LPTSideMenuLayer::onExit() {
    BaseLayer::onExit();
}
