//
//  LPTPlayerProfileScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 01/10/18.
//

#include "LPTPlayerProfileScene.hpp"
#include "LPTPlayerProfileLayer.hpp"

LPTPlayerProfileScene* LPTPlayerProfileScene::createScene() {
    auto scene = LPTPlayerProfileScene::create();
    return scene;
}

bool LPTPlayerProfileScene::init() {
    if (!BaseScene::init()) {
        return false;
    }
    return true;
}

void LPTPlayerProfileScene::onEnter() {
    BaseScene::onEnter();
    
    LPTPlayerProfileLayer *playerProfileLayer  = LPTPlayerProfileLayer::createLayer(kBackToGameSelectionSceneBackButtonTag, PLAYER->getPlayerProfileData());
    this->addChild(playerProfileLayer);
    playerProfileLayer->addPlayerXP(PLAYER->getPlayerGameXP(), GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel()));
}

void LPTPlayerProfileScene::onExit() {
    BaseScene::onExit();
}
