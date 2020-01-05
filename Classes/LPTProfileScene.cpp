//
//  LPTProfileScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati on 17/09/18.
//

#include "LPTProfileScene.hpp"
#include "LPTProfilePicture.hpp"

#define BACK_BUTTON_TAG                 100
#define PROFILE_PICTURE_TAG             99

BaseScene* LPTProfileScene::createScene() {
    auto scene = BaseScene::create();
    auto layer = LPTProfileScene::create();
    scene->addChild(layer);
    return scene;
}

bool LPTProfileScene::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    setBackgroundSprite(kMainMenuBG);
    createButton(kBackButtonImageName, BACK_BUTTON_TAG, Vec2(winSize.width*0.1, winSize.height*0.9), this, CC_CALLBACK_2(LPTProfileScene::buttonCallback, this), 1);
    createTTFLabel(PLAYER->getUserName(), Vec2(winSize.width*0.5, winSize.height*0.5), Vec2::ANCHOR_MIDDLE, this, Color3B(100, 150, 250), kArialFont, 50);
    return true;
}

void LPTProfileScene::onEnter() {
    BaseLayer::onEnter();
    setupProfileUI();
}

void LPTProfileScene::onExit() {
    BaseLayer::onExit();
}

void LPTProfileScene::setupProfileUI() {
    
}

void LPTProfileScene::buttonCallback(Ref* sender, Widget::TouchEventType type) {
    Button *button = (Button*)sender;
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::CANCELED:
            break;
        case Widget::TouchEventType::ENDED:
        {
            switch (button->getTag()) {
                case BACK_BUTTON_TAG:
                    break;
                case PROFILE_PICTURE_TAG:
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
