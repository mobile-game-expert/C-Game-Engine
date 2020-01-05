//
//  LPTDifficultySelection.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 28/09/18.
//

#include "LPTDifficultySelection.hpp"
#include "LPTTopBar.hpp"
#include "LPTBetSelectionScene.hpp"
#include "LPTGameScene.hpp"
#include "LPTGameSelectionScene.hpp"
#include "LPTInteractiveTutorialScene.hpp"

#define kMaxNumberOfTicket                      3

#pragma mark - Layer Life Cycle
BaseScene* LPTDifficultySelection::createScene() {
    auto scene = BaseScene::create();
    auto layer = LPTDifficultySelection::create();
    scene->addChild(layer);
    return scene;
}

bool LPTDifficultySelection::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    
    return true;
}

void LPTDifficultySelection::onEnter() {
    BaseLayer::onEnter();
    this->removeAllChildrenWithCleanup(true);
    this->setBackgroundSprite(kGameSelectionBGImageName);
    APP_MANAGER->gameState = DIFFICULTY_SELECTION;
    setupUI();
    this->addGameTopBar();
}

void LPTDifficultySelection::onExit() {
    BaseLayer::onExit();
}

#pragma mark - TOPBAR
void LPTDifficultySelection::addGameTopBar() {
    LPTTopBar *layer = GameUtils::createTopBar(this, kGameTopBarImageName);
    layer->addGameSelectionSceneTopBarUI(PLAYER->getPlayerProfilePicture(), 2000);
    layer->addProgressBar(PLAYER->getPlayerGameXP(), GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel()));
}

#pragma mark - UI setup
void LPTDifficultySelection::setupUI() {
    LPTHorizontalScrollView *tableLayer = LPTHorizontalScrollView::createLayer();
    //GameSelection
    vector<ValueMap> imagesName;
    for(int imageCounter = 1; imageCounter <= kMaxNumberOfTicket; imageCounter++) {
        string imageName = StringUtils::format("%s%d.png", kDifficultySelectionImageName, imageCounter);
        ValueMap valueMap;
        valueMap[kImageName] = imageName;
        imagesName.push_back(valueMap);
    }
    
    //GameSelection
    tableLayer->setupUI(imagesName, Vec2::ZERO, Size(this->getContentSize().width, this->getContentSize().height * 0.85));
    tableLayer->setSelectionLayerDelegate(this);
    this->addChild(tableLayer);
    
    //BackButton
    Button *backButton = createButton(kGameSelectionBackButtonImageName, kDifficultySelectionBackButtonTag, Vec2(this->getContentSize().width * 0.03, this->getContentSize().height * 0.1), this, CC_CALLBACK_2(LPTDifficultySelection::buttonCallback, this), 1);
    backButton->setAnchorPoint(Vec2(0, 0.5));
    backButton->setScale(this->getContentSize().width * 0.15 / backButton->getContentSize().width);
    backButton->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, backButton->getScale())), NULL));
}

#pragma mark - Button callback
void LPTDifficultySelection::buttonCallback(Ref* sender, Widget::TouchEventType type) {
    Button *button = (Button*)sender;
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::CANCELED:
            break;
        case Widget::TouchEventType::ENDED:
        {
            SoundManager::playSound(BUTTON_TAP_SOUND);
            switch (button->getTag()) {
                case kDifficultySelectionBackButtonTag:
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    Director::getInstance()->replaceScene(LPTGameSelectionScene::createScene());
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

#pragma mark DelegateMethod
void LPTDifficultySelection::tableCellSelectedWithIndex(int index) {
    SoundManager::playSound(BUTTON_TAP_SOUND);
    switch(index) {
        case BEGINNER:
            UserDefault::getInstance()->setIntegerForKey(kAIMode, BEGINNER);
            break;
        case INTERMEDIATE:
            UserDefault::getInstance()->setIntegerForKey(kAIMode, INTERMEDIATE);
            break;
        case EXPERT:
            UserDefault::getInstance()->setIntegerForKey(kAIMode, EXPERT);
            break;
    }
    SoundManager::playSound(SCENE_CHANGE_SOUND);
    Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, LPTGameScene::createScene()));
//    if(APP_MANAGER->getTutorialShownStatus()) {
//        
//    }else {
//        Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, LPTInteractiveTutorialScene::createScene()));
//    }
}
