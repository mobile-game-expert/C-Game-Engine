//
//  LPTResultScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Apple on 14/09/18.
//

#include "LPTResultScene.hpp"
#define BACK_BUTTON_TAG                 100

BaseScene* LPTResultScene::createScene() {
    auto scene = BaseScene::create();
    auto layer = LPTResultScene::create();
    scene->addChild(layer);
    return scene;
}

bool LPTResultScene::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    setBackgroundSprite("MainMenuBG.png");
    createButton("BackBtn.png", BACK_BUTTON_TAG, Vec2(winSize.width*0.1, winSize.height*0.9), this, CC_CALLBACK_2(LPTResultScene::buttonCallback, this));
    createTTFLabel(PLAYER->getUserName(), Vec2(winSize.width*0.5, winSize.height*0.5), Vec2::ANCHOR_MIDDLE, this, Color3B(100, 150, 250), kArialFont, 50);
    return true;
}

void LPTResultScene::onEnter() {
    BaseLayer::onEnter();
}

void LPTResultScene::onExit() {
    BaseLayer::onExit();
}

void LPTResultScene::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                default:
                    break;
            }
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
}
