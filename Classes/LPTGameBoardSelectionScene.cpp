//
//  LPTGameBoardSelectionScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 05/10/18.
//

#include "LPTGameBoardSelectionScene.hpp"
#include "LPTTopBar.hpp"

#define TOP_BAR_TAG             5001
#define BACK_BUTTON_TAG         5002
#define TABLE_POPUP_TAG         5003

#pragma mark - CreateScene
BaseScene* LPTGameBoardSelectionScene::createScene() {
    auto scene = BaseScene::create();
    auto layer = LPTGameBoardSelectionScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool LPTGameBoardSelectionScene::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    this->setBackgroundSprite(kGameSelectionBGImageName);
    return true;
}

void LPTGameBoardSelectionScene::onEnter() {
    BaseLayer::onEnter();
    setupUI();
}

void LPTGameBoardSelectionScene::onExit() {
    BaseLayer::onExit();
}

void LPTGameBoardSelectionScene::setupUI() {
    //BackButton
    Button *backButton = createButton(kGameSelectionBackButtonImageName, BACK_BUTTON_TAG, Vec2(this->getContentSize().width * 0.03, this->getContentSize().height * 0.1), this, CC_CALLBACK_2(LPTGameBoardSelectionScene::buttonCallback, this), 1);
    backButton->setAnchorPoint(Vec2(0, 0.5));
    backButton->setScale(this->getContentSize().width * 0.15 / backButton->getContentSize().width);
    backButton->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, backButton->getScale())), NULL));
    
    //Top bar
    LPTTopBar *layer = GameUtils::createTopBar(this, kGameTopBarImageName);
    layer->setTag(TOP_BAR_TAG);
    layer->addGameSelectionSceneTopBarUI(PLAYER->getPlayerProfilePicture(), 2000);
    
    //Table popup
    auto tablePopup = BaseLayer::createSprite(kPopupMatchMaking, TABLE_POPUP_TAG, Vec2(this->getContentSize().width * 0.5 , (this->getContentSize().height - layer->getContentSize().height) * 0.5), this);
    tablePopup->setScale(this->getContentSize().height * 0.62 / tablePopup->getContentSize().height);
    
    //Choose Your Side Label
    auto lblChooseSide = BaseLayer::createTTFLabel(kChooseYourSide, Vec2(tablePopup->getContentSize().width * tablePopup->getScale() * 0.5, tablePopup->getPositionY() + tablePopup->getContentSize().height * tablePopup->getScale() * 0.4), Vec2::ANCHOR_MIDDLE, this, Color3B::WHITE, kOpenSans_Bold, 48);
    BaseLayer::createTTFLabel(kSelectSideYouWantToPlay, Vec2(tablePopup->getContentSize().width * tablePopup->getScale() * 0.5,  lblChooseSide->getPositionY() - lblChooseSide->getContentSize().height * 0.6), Vec2::ANCHOR_MIDDLE, this, Color3B::WHITE, kOpenSans_Bold, 24);
}

#pragma mark - Button Callback
void LPTGameBoardSelectionScene::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case BACK_BUTTON_TAG: {
                    /*Temp code to update matching screen
                     LPTPlayerMatchingLayer *matchingLayer = (LPTPlayerMatchingLayer*)this->getChildByTag(SEARCHING_LAYER_TAG);
                     matchingLayer->stopSearchingAnimation();
                     matchingLayer->runCoinAnimationForUser(true);
                     matchingLayer->runCoinAnimationForUser(false);
                     matchingLayer->updateOpponentProfile("avatar_3.png");
                     matchingLayer->updateCoinsAfterBet(5000, kMatchingSceneChipCoinTagForUser);
                     matchingLayer->updateCoinsAfterBet(5000, kMatchingSceneChipCoinTagForOpponent);
                     matchingLayer->updateOpponentUniqueName(GameUtils::getBotName()); */
                }
                    //                    Director::getInstance()->replaceScene(LPTGameSelectionScene::createScene());
                    break;
                case TOP_BAR_TAG:
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
