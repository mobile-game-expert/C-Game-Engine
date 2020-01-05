//
//  BaseScene.cpp
//  Max_Damage
//
//  Created by Davinder on 19/05/17.
//
//

#include "BaseScene.hpp"

//BaseScene* BaseScene::create() {
//    BaseScene *pRet = new(std::nothrow) BaseScene();
//    if (pRet && pRet->init())
//    {
//        // pRet->autorelease();
//        
//        return pRet;
//    }
//    else
//    {
//        delete pRet;
//        pRet = nullptr;
//        return nullptr;
//    }
//}

bool BaseScene::init(){
    if (!Scene::init()){
        return false;   
    }
    winSize = Director::getInstance()->getWinSize();
    
    if(!SoundManager::isBackgroundMusicPlaying() && APP_MANAGER->gameState != GAME_SCENE) {
        SoundManager::playMusic(GameUtils::getBackgroundSoundNameForScene());
    } else if(APP_MANAGER->gameState == GAME_SCENE) {
        SoundManager::playMusic(GameUtils::getBackgroundSoundNameForScene());
    }
    return true;
}

void BaseScene::onEnter(){
    Scene::onEnter();
}

void BaseScene::onExit(){
    Scene::onExit();
}

void BaseScene::setBackgroundSprite(string bgSprite){
    Sprite *bg = Sprite::create(bgSprite);
    bg->setPosition(Vec2(winSize.width*0.5, winSize.height*0.5));
    bg->setLocalZOrder(-1000);
    this->addChild(bg);
}

