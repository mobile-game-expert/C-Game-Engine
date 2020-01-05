//
//  LPTTutorialScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 11/10/18.
//

#include "LPTTutorialScene.hpp"
#include "LPTTopBar.hpp"
#include "LPTTutorialLayer.hpp"

#pragma mark - Layer Life Cycle
LPTTutorialScene* LPTTutorialScene::createScene() {
    auto scene = LPTTutorialScene::create();
    return scene;
}

bool LPTTutorialScene::init() {
    if (!BaseScene::init()) {
        return false;
    }
    return true;
}

void LPTTutorialScene::onEnter() {
    BaseScene::onEnter();
    setupUI();
}

void LPTTutorialScene::onExit() {
    BaseScene::onExit();
}

#pragma mark - Setup UI
void LPTTutorialScene::setupUI() {
    
    LPTTutorialLayer *layer = LPTTutorialLayer::createLayer(kBackToSettingSceneBackButtonTag);
    this->addChild(layer);
    
}
