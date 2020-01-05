//
//  LPTSettingsScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 17/09/18.
//

#include "LPTSettingsScene.hpp"
#include "LPTSettingsLayer.hpp"

LPTSettingsScene* LPTSettingsScene::createScene() {
    auto scene = LPTSettingsScene::create();
    return scene;
}

bool LPTSettingsScene::init() {
    if (!BaseScene::init()) {
        return false;
    }
    return true;
}

void LPTSettingsScene::onEnter() {
    BaseScene::onEnter();
    LPTSettingsLayer *settingLayer = LPTSettingsLayer::createLayer(kBackToGameSelectionSceneBackButtonTag);
    this->addChild(settingLayer);
}

void LPTSettingsScene::onExit() {
    BaseScene::onExit();
}
