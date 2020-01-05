//
//  LPTInteractiveTutorialScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 29/11/18.
//

#include "LPTInteractiveTutorialScene.hpp"
#include "LPTInteractiveTutorialLayer.hpp"

#pragma mark - Layer Life Cycle
LPTInteractiveTutorialScene* LPTInteractiveTutorialScene::createScene() {
    auto scene = LPTInteractiveTutorialScene::create();
    return scene;
}

bool LPTInteractiveTutorialScene::init() {
    if (!BaseScene::init()) {
        return false;
    }
    return true;
}

void LPTInteractiveTutorialScene::onEnter() {
    BaseScene::onEnter();
    setupUI();
}

void LPTInteractiveTutorialScene::onExit() {
    BaseScene::onExit();
}

#pragma mark - Setup UI
void LPTInteractiveTutorialScene::setupUI() {
    
    LPTInteractiveTutorialLayer *layer = LPTInteractiveTutorialLayer::createLayer(kBackToGameSelectionSceneBackButtonTag);
    this->addChild(layer);
}
