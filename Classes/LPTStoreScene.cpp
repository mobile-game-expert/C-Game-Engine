//
//  LPTStoreScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati thakur on 17/09/18.
//

#include "LPTStoreScene.hpp"
#include "LPTTopBar.hpp"
#include "LPTGameSelectionScene.hpp"
#include "LPTStoreItemComponent.hpp"
#include "LPTStoreLayer.hpp"

BaseScene* LPTStoreScene::createScene() {
    auto scene = LPTStoreScene::create();
    return scene;
}

bool LPTStoreScene::init() {
    if (!BaseScene::init()) {
        return false;
    }
    return true;
}

void LPTStoreScene::onEnter() {
    BaseScene::onEnter();
    setupUI();
}

void LPTStoreScene::onExit() {
    BaseScene::onExit();
}
#pragma mark - Setup UI
void LPTStoreScene::setupUI() {

    LPTStoreLayer *layer = LPTStoreLayer::createLayer(kBackToGameSelectionSceneBackButtonTag);
    this->addChild(layer);
    
}
