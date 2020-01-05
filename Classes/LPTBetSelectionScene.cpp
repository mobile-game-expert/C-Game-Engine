//
//  LPTBetSelectionScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 28/09/18.
//

#include "LPTBetSelectionScene.hpp"
#include "LPTTopBar.hpp"
#include "LPTDifficultySelection.hpp"
#include "LPTGameSelectionScene.hpp"
#include "LPTPlayerMatchingScene.hpp"
#include "LPTStoreScene.hpp"
#include "LPTSearchFriendsScene.hpp"
#include "LPTEvents.hpp"

#define kMaxNumberOfTicket                      8

#pragma mark - Layer Life Cycle
BaseScene* LPTBetSelectionScene::createScene() {
    auto scene = BaseScene::create();
    auto layer = LPTBetSelectionScene::create();
    scene->addChild(layer);
    return scene;
}

bool LPTBetSelectionScene::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}

void LPTBetSelectionScene::onEnter() {
    BaseLayer::onEnter();
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setZoneRequestListener(this);
    warpClientRef->setRoomRequestListener(this);
    //===Delete Room
    if (PLAYER->isAdmin) {
        warpClientRef->deleteRoom(PLAYER->getRoomId());
    }else {
        warpClientRef->leaveRoom(PLAYER->getRoomId());
    }
    
    this->removeAllChildrenWithCleanup(true);
    this->setBackgroundSprite(kGameSelectionBGImageName);
    
    APP_MANAGER->gameState = BET_SELECTION;
    
    setupBetsData();
    setupUI();
    this->addGameTopBar();
}

void LPTBetSelectionScene::onExit() {
    BaseLayer::onExit();
}
#pragma mark - Bets Data
void LPTBetSelectionScene::setupBetsData() {
    arrayBets.push_back(StringUtils::format("%.d", BET_50));
    arrayBets.push_back(StringUtils::format("%.d", BET_100));
    arrayBets.push_back(StringUtils::format("%.d", BET_500));
    arrayBets.push_back(StringUtils::format("%.d", BET_750));
    arrayBets.push_back(StringUtils::format("%.d", BET_1000));
    arrayBets.push_back(StringUtils::format("%.d", BET_1500));
    arrayBets.push_back(StringUtils::format("%.d", BET_2000));
    arrayBets.push_back(StringUtils::format("%.d", BET_2500));
}

#pragma mark - TOPBAR
void LPTBetSelectionScene::addGameTopBar() {
    
    LPTTopBar *layer = GameUtils::createTopBar(this, kGameTopBarImageName);
    layer->addGameSelectionSceneTopBarUI(PLAYER->getPlayerProfilePicture(), 2000);
    layer->addProgressBar(PLAYER->getPlayerGameXP(), GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel()));
    
//        layer->addProgressBar(PLAYER->getPlayerGameXP(), PLAYER->getlevelMaxXP());
}

#pragma mark - UI setup
void LPTBetSelectionScene::setupUI() {
    LPTHorizontalScrollView *tableLayer = LPTHorizontalScrollView::createLayer();
    vector<ValueMap> cellData;
    for(int imageCounter = 1; imageCounter <= kMaxNumberOfTicket; imageCounter++) {
        string imageName = StringUtils::format("%s%d.png", kGameSelectionBetTableName, imageCounter);
        
//         string imageName = imageCounter > 3 ? StringUtils::format("%s1.png", kGameSelectionBetTableName) : StringUtils::format("%s%d.png", kGameSelectionBetTableName, imageCounter);
        
        ValueMap valueMap;
        valueMap[kImageName]        =   imageName;
        valueMap[kPriceImageName]   =   kBetAmountImageName;
        valueMap[kPriceLabel]       =   arrayBets.at(imageCounter-1);
        valueMap[kTableName]        =   StringUtils::format("TABLE %d", imageCounter);
        cellData.push_back(valueMap);
    }
    //GameSelection
    tableLayer->setupUI(cellData, Vec2::ZERO, Size(this->getContentSize().width, this->getContentSize().height * 0.85));
    tableLayer->setSelectionLayerDelegate(this);
    this->addChild(tableLayer);
    
    //BackButton
    Button *backButton = createButton(kGameSelectionBackButtonImageName, kBetSelectionBackButtonTag, Vec2(this->getContentSize().width * 0.03, this->getContentSize().height * 0.1), this, CC_CALLBACK_2(LPTBetSelectionScene::buttonCallback, this), 1);
    backButton->setAnchorPoint(Vec2(0, 0.5));
    backButton->setScale(this->getContentSize().width * 0.15 / backButton->getContentSize().width);
    backButton->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, backButton->getScale())), NULL));
    
}

#pragma mark - Button callback
void LPTBetSelectionScene::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case kBetSelectionBackButtonTag:
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.3, LPTGameSelectionScene::createScene()));
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
void LPTBetSelectionScene::tableCellSelectedWithIndex(int index) {
    SoundManager::playSound(BUTTON_TAP_SOUND);
    switch(index) {
        case BET_1:
            PLAYER->setBetSelected(BET_1);
            break;
        case BET_2:
            PLAYER->setBetSelected(BET_2);
            break;
        case BET_3:
            PLAYER->setBetSelected(BET_3);
            break;
        case BET_4:
            PLAYER->setBetSelected(BET_4);
            break;
        case BET_5:
            PLAYER->setBetSelected(BET_5);
            break;
        case BET_6:
            PLAYER->setBetSelected(BET_6);
            break;
        case BET_7:
            PLAYER->setBetSelected(BET_7);
            break;
        case BET_8:
            PLAYER->setBetSelected(BET_8);
            break;
    }
    //--
    LPTEvents::logTableSelection(PLAYER->getBetSelected());
    if((PLAYER->getTotalCoins() - GameUtils::getUserBetValueFromSelectedBet(PLAYER->getBetSelected())) >= 0) {
        moveToNextScene();
    } else {
        auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kInsufficientCoins, true, SHOPCOIN_POPUP, kShopNowButtonText, kCancelButtonText);
        popup->setPopUpDelegate(this);
        this->addChild(popup, 5000);
    }
    
}

void LPTBetSelectionScene::moveToNextScene() {
    SoundManager::playSound(SCENE_CHANGE_SOUND);
    switch(PLAYER->getGameSelected()) {
        case GAME_ONE_ON_ONE:
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0, LPTPlayerMatchingScene::createScene()));
            break;
        case GAME_WITH_FRIEND:
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0, LPTSearchFriendsScene::createScene()));
            break;
    }
}

#pragma mark - Game popup callabck
void LPTBetSelectionScene::okButtonAction(Popup_Type type) {
    if(type == SHOPCOIN_POPUP) {
        SoundManager::playSound(SCENE_CHANGE_SOUND);
        Director::getInstance()->pushScene(LPTStoreScene::createScene());
    }
    LPTPopUp * popUp = (LPTPopUp *)this->getChildByTag((int)type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

void LPTBetSelectionScene::cancelButtonAction(Popup_Type type) {

    LPTPopUp * popUp = (LPTPopUp *)this->getChildByTag(type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

void LPTBetSelectionScene::onLeaveRoomDone (AppWarp::room revent){
    log("onLeaveRoomDone");
}
void LPTBetSelectionScene::onDeleteRoomDone (AppWarp::room revent){
    log("onDeleteRoomDone");
}
