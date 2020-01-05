//
//  LPTPlayerMatchingScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 17/09/18.
//

#include "LPTPlayerMatchingScene.hpp"
#include "LPTTopBar.hpp"
#include "LPTPlayerMatchingLayer.hpp"
#include "LPTBetSelectionScene.hpp"
#include "LPTGameScene.hpp"
#include "LPTGameSceneRW.hpp"

#define BACK_BUTTON_TAG                 1100
#define PLAY_BUTTON_TAG                 1101
#define TOP_LAYER_TAG                   1102
#define SEARCHING_LAYER_TAG             1103

#define kAutomaticStopTime              3
#define kMovetoNextSceneDelayTime       4

BaseScene* LPTPlayerMatchingScene::createScene() {
    auto scene = BaseScene::create();
    auto layer = LPTPlayerMatchingScene::create();
    scene->addChild(layer);
    return scene;
}

bool LPTPlayerMatchingScene::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    setupAppwarp();
    return true;
}

void LPTPlayerMatchingScene::onEnter() {
    BaseLayer::onEnter();
    PLAYER->setIsAdmin(false);
    LPTServerListener::getInstance()->setLPTServerDelegate(this);
    OPPONENT->removeSharedInstance();
    this->removeAllChildrenWithCleanup(true);
    this->setBackgroundSprite(kGameSelectionBGImageName);
    APP_MANAGER->gameState = PLAYER_MATCHING;
    addGameTopBar();
    setupUI();
    addPlayerMatchingLayer();
//    addScheduleAutomaticSearchStop();//Temp Code
}

void LPTPlayerMatchingScene::onExit() {
    this->unschedule(schedule_selector(LPTPlayerMatchingScene::stopSearch));
    APP_MANAGER->isPlayingWithFriend = false;
    PLAYER->roomName = "";
    PLAYER->roompassword = "";
    LPTServerListener::getInstance()->setLPTServerDelegate(NULL);
    BaseLayer::onExit();
}

#pragma mark - Setup appWarp setting
void LPTPlayerMatchingScene::setupAppwarp() {
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
    warpClientRef->setNotificationListener(this);
    warpClientRef->setRoomRequestListener(this);
    warpClientRef->setZoneRequestListener(this);
    if (warpClientRef->getState() != AppWarp::ConnectionState::connected) {
        if(warpClientRef->getState() == AppWarp::ConnectionState::connecting){
            warpClientRef->disconnect();
        }else if(warpClientRef->getState() == AppWarp::ConnectionState::recovering) {
            warpClientRef->recoverConnectionWithSessionID(UserDefault::getInstance()->getIntegerForKey(APPWARP_SESSION), PLAYER->getPlayerUniqueName());
        }else {
            warpClientRef->connect(PLAYER->getPlayerUniqueName());
        }
    } else {
        warpClientRef->joinRoomWithProperties(PLAYER->setTableProperties(PLAYER->roomName, PLAYER->roompassword, PLAYER->getBetSelected(), 1, 1));
    }
}

#pragma mark - TOPBAR
void LPTPlayerMatchingScene::addGameTopBar() {
    LPTTopBar *layer = GameUtils::createTopBar(this, kGameTopBarImageName);
    layer->setTag(TOP_LAYER_TAG);
    layer->addGameSelectionSceneTopBarUI(PLAYER->getPlayerProfilePicture(), 2000);
    layer->addProgressBar(PLAYER->getPlayerGameXP(), GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel()));
}

#pragma mark - UI setup
void LPTPlayerMatchingScene::setupUI() {
    
    //BackButton
    Button *backButton = createButton(kGameSelectionBackButtonImageName, BACK_BUTTON_TAG, Vec2(this->getContentSize().width * 0.03, this->getContentSize().height * 0.1), this, CC_CALLBACK_2(LPTPlayerMatchingScene::buttonCallback, this), 1);
    backButton->setAnchorPoint(Vec2(0, 0.5));
    backButton->setScale(this->getContentSize().width * 0.15 / backButton->getContentSize().width);
    backButton->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, backButton->getScale())), NULL));
    
}

#pragma mark - AddPlayerScreen
void LPTPlayerMatchingScene::addPlayerMatchingLayer() {
    
    LPTTopBar *topbar = (LPTTopBar*)this->getChildByTag(TOP_LAYER_TAG);
    
    LPTPlayerMatchingLayer *matchingLayer = LPTPlayerMatchingLayer::createLayer();
    matchingLayer->setupUIForPlayerMatching(GameUtils::getUserBetValueFromSelectedBet(PLAYER->getBetSelected()), true, kPlayerProfileImageName, PLAYER->getPlayerProfilePicture(), PLAYER->getPlayerUniqueName().c_str(), "");
    matchingLayer->setContentSize(Size(this->getContentSize().width, this->getContentSize().height - topbar->getContentSize().height * topbar->getScale()));
    matchingLayer->setTag(SEARCHING_LAYER_TAG);
    this->addChild(matchingLayer, 10000);
}

#pragma mark - Automatic User Serach stop
void LPTPlayerMatchingScene::addScheduleAutomaticSearchStop() {
    this->scheduleOnce(schedule_selector(LPTPlayerMatchingScene::stopSearch), kAutomaticStopTime);
}

void LPTPlayerMatchingScene::stopSearch(float delta) {
    //Create OpponentProfile
    string botName = GameUtils::getBotName();
//    OPPONENT->createProfileForBotWith(botName, "avatar_4.png");
    
    LPTPlayerMatchingLayer *matchingLayer = (LPTPlayerMatchingLayer*)this->getChildByTag(SEARCHING_LAYER_TAG);
    matchingLayer->stopSearchingAnimation();
    matchingLayer->runCoinAnimationForUser(true);
    matchingLayer->runCoinAnimationForUser(false);
    matchingLayer->updateOpponentProfile(OPPONENT->getProfilePic());
    PLAYER->setTotalCoins(PLAYER->getTotalCoins() - (GameUtils::getUserBetValueFromSelectedBet(PLAYER->getBetSelected())));
    matchingLayer->updateCoinsAfterBet(0, kMatchingSceneChipCoinTagForUser);
    matchingLayer->updateCoinsAfterBet(0, kMatchingSceneChipCoinTagForOpponent);
    matchingLayer->updateOpponentUniqueName(OPPONENT->getUniqueName()); // Add opponent name here
    matchingLayer->updateBetAmount(GameUtils::getUserBetValueFromSelectedBet(PLAYER->getBetSelected()) *2);
    //Update coins of user in topbar
    LPTTopBar *topbar = (LPTTopBar*)this->getChildByTag(TOP_LAYER_TAG);
    topbar->updateCoinText(PLAYER->getTotalCoins());
    
    //Update coins in the Database
//    updateUserDocumentForCoins();
    
    //Move to game scene
    this->scheduleOnce(schedule_selector(LPTPlayerMatchingScene::moveToGameScene), kMovetoNextSceneDelayTime);
}

void LPTPlayerMatchingScene::moveToGameScene(float delta) {
    SoundManager::playSound(SCENE_CHANGE_SOUND);
    Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, LPTGameSceneRW::createScene()));
}

#pragma mark - Button Callback
void LPTPlayerMatchingScene::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case BACK_BUTTON_TAG:{
                    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0, LPTBetSelectionScene::createScene()));
                }
                    break;
                case PLAY_BUTTON_TAG:
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

void LPTPlayerMatchingScene::updateUserDocumentForCoinsResponse(void *response) {
    App42StorageResponse *storageResponse = (App42StorageResponse*)response;
    printf("\ncode=%d",storageResponse->getCode());
    printf("\nResponse Body=%s",storageResponse->getBody().c_str());
    if (storageResponse->isSuccess) {
        printf("Puchase Success");
    } else {
        printf("\nerrordetails:%s",storageResponse->errorDetails.c_str());
        printf("\nerrorMessage:%s",storageResponse->errorMessage.c_str());
        printf("\nappErrorCode:%d",storageResponse->appErrorCode);
        printf("\nhttpErrorCode:%d",storageResponse->httpErrorCode);
    }
}

#pragma mark - =========== Get Room ==================
void LPTPlayerMatchingScene::getRoomWithProperties(std::map<std::string, std::string> tableProperty) {
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->getRoomWithProperties(tableProperty);
}

#pragma mark ========= Recover Appwar Connection ===========

void LPTPlayerMatchingScene::scheduleRecover()
{
    printf(__PRETTY_FUNCTION__);
    if (!this->isScheduled(schedule_selector(LPTPlayerMatchingScene::recover)))
    {
        this->schedule(schedule_selector(LPTPlayerMatchingScene::recover), 5.0f);
        printf("Reconnecting ...");
    }
}

void LPTPlayerMatchingScene::unscheduleRecover()
{
    printf(__PRETTY_FUNCTION__);
    unschedule(schedule_selector(LPTPlayerMatchingScene::recover));
}

void LPTPlayerMatchingScene::recover(float dt)
{
    printf(__PRETTY_FUNCTION__);
    AppWarp::Client::getInstance()->recoverConnection();
}
#pragma mark - AppWapp Multiplayer Callbacks
//ConnectionRequestListener
void LPTPlayerMatchingScene::onConnectDone(int res, int reasonCode) {
    LPTPlayerMatchingLayer *matchingLayer = (LPTPlayerMatchingLayer*)this->getChildByTag(SEARCHING_LAYER_TAG);
//        updatePlayerAppwarpStatus
    if (res == AppWarp::ResultCode::success)
    {
        printf("\nonConnectDone .. SUCCESS..session=%d\n",AppWarp::AppWarpSessionID);
        APP_MANAGER->isConnectedToAppWarp = true;
        AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
        map<string,string> table = PLAYER->setTableProperties(PLAYER->roomName, PLAYER->roompassword, PLAYER->getBetSelected(), 1, 1);
        warpClientRef->joinRoomWithProperties(table);
        UserDefault::getInstance()->setIntegerForKey(APPWARP_SESSION, warpClientRef->getSessionID());
    }else if (res == AppWarp::ResultCode::auth_error)
    {
        unscheduleRecover();
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->connect(PLAYER->getPlayerUniqueName());
        printf("\nonConnectDone .. Failed with auth_error and reasonCode = %d..session=%d\n",reasonCode,AppWarp::AppWarpSessionID);
        matchingLayer->updatePlayerAppwarpStatus("Auth Error");
        if(reasonCode == 21) matchingLayer->updatePlayerAppwarpStatus("Recovering...");
    }
    else if (res == AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. SUCCESS with success_recovered..session=%d\n",AppWarp::AppWarpSessionID);
        matchingLayer->updatePlayerAppwarpStatus("Recovered");
        APP_MANAGER->isConnectedToAppWarp = true;
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        map<string,string> table = PLAYER->setTableProperties(PLAYER->roomName, PLAYER->roompassword, PLAYER->getBetSelected(), 1, 1);
        warpClientRef->joinRoomWithProperties(table);
        UserDefault::getInstance()->setIntegerForKey(APPWARP_SESSION, warpClientRef->getSessionID());
    }
    else if (res == AppWarp::ResultCode::connection_error_recoverable)
    {
        APP_MANAGER->isConnectedToAppWarp = false;
        matchingLayer->updatePlayerAppwarpStatus("Recovering");
        scheduleRecover();
        printf("\nonConnectDone .. FAILED..connection_error_recoverable..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (res == AppWarp::ResultCode::bad_request)
    {
        unscheduleRecover();
        APP_MANAGER->isConnectedToAppWarp = false;
        printf("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
        matchingLayer->updatePlayerAppwarpStatus("Bad Request");
    }
    else
    {
        APP_MANAGER->isConnectedToAppWarp = false;
        unscheduleRecover();
        printf("\nonConnectDone .. FAILED with reasonCode=%d..session=%d\n",reasonCode,AppWarp::AppWarpSessionID);
        matchingLayer->updatePlayerAppwarpStatus("Connection Failed");
    }
}

void LPTPlayerMatchingScene::onDisconnectDone(int res) {
    printf(__PRETTY_FUNCTION__);
    if (res == AppWarp::ResultCode::success) {
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->connect(PLAYER->getPlayerUniqueName());
    }
}

//RoomRequestListener
void LPTPlayerMatchingScene::onSubscribeRoomDone(AppWarp::room revent) {
    printf(__PRETTY_FUNCTION__);
    //== Should go to Game Scene (Opponent)
    if (revent.result==0)
    {
        PLAYER->setRoomId(revent.roomId);
        if (revent.owner == PLAYER->getPlayerUniqueName()) {
            PLAYER->setIsAdmin(true);
        }else {
         PLAYER->setIsAdmin(false);
        }
        AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->getLiveRoomInfo(revent.roomId);
        //    warpClientRef->getOnlineUsers();
    }
    
}
void LPTPlayerMatchingScene::onJoinRoomDone(AppWarp::room revent) {
    printf(__PRETTY_FUNCTION__);
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    LPTPlayerMatchingLayer *matchingLayer = (LPTPlayerMatchingLayer*)this->getChildByTag(SEARCHING_LAYER_TAG);
    if (revent.result==0)
    {
        warpClientRef->subscribeRoom(revent.roomId);
        matchingLayer->updatePlayerAppwarpStatus("Waiting for player");
    }
    else
    {
        map<string,string> table = PLAYER->setTableProperties(PLAYER->roomName, PLAYER->roompassword, PLAYER->getBetSelected(), 1, 1);
        warpClientRef->createRoom(PLAYER->getPlayerUniqueName(), PLAYER->getPlayerUniqueName(), ROOM_USER_LIMIT, table);
        matchingLayer->updatePlayerAppwarpStatus("Players Connected");
    }
}
void LPTPlayerMatchingScene::onLeaveRoomDone (AppWarp::room revent) {
    printf(__PRETTY_FUNCTION__);
}
void LPTPlayerMatchingScene::onGetLiveRoomInfoDone(AppWarp::liveroom revent){
    printf(__PRETTY_FUNCTION__);
    if (revent.result == 0) {
        log("Total users in room = %d",(int)revent.users.size());
        if ((int)revent.users.size() >= 2) {
            for (int i = 0; i < revent.users.size(); i++) {
                string name = revent.users.at(i);
                if (name != PLAYER->getPlayerUniqueName()) {
                
                    getOpponentDetailsFromUserName(name);
                    break;
                }
            }
        }
    }
}
void LPTPlayerMatchingScene::onUpdatePropertyDone(AppWarp::liveroom revent) {
    printf(__PRETTY_FUNCTION__);
    if (revent.result == 0) {
        //== should Go To Game Scene (Admin)
        if (revent.users.size() >= 2) {
            for (int i = 0; i < revent.users.size(); i++) {
                string name = revent.users.at(i);
                if (name != PLAYER->getPlayerUniqueName()) {
                    getOpponentDetailsFromUserName(name);
                    break;
                }
            }
        }
    }
}
//ZoneRequestListener
void LPTPlayerMatchingScene::onCreateRoomDone (AppWarp::room revent) {
    printf(__PRETTY_FUNCTION__);
    if (revent.result == 0) {
        PLAYER->setIsAdmin(true);
        AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
        map<string,string> table = PLAYER->setTableProperties(PLAYER->roomName, PLAYER->roompassword, PLAYER->getBetSelected(), 1, 1);
        warpClientRef->joinRoomWithProperties(table);
    }
}
void LPTPlayerMatchingScene::onDeleteRoomDone (AppWarp::room revent) {
    printf(__PRETTY_FUNCTION__);
}
void LPTPlayerMatchingScene::onGetAllRoomsDone (AppWarp::liveresult res) {
    printf(__PRETTY_FUNCTION__);
}
void LPTPlayerMatchingScene::onGetOnlineUsersDone (AppWarp::liveresult res) {
    printf(__PRETTY_FUNCTION__);
    auto list = res.list;
    for(int index = 0; index < res.list.size(); index++) {
        printf("\nUsername :%s", res.list[index].c_str());
    }
}
void LPTPlayerMatchingScene::onGetLiveUserInfoDone (AppWarp::liveuser uevent) {
    printf(__PRETTY_FUNCTION__);
}
void LPTPlayerMatchingScene::onGetMatchedRoomsDone(AppWarp::matchedroom mevent) {
    printf(__PRETTY_FUNCTION__);
    if(mevent.roomData.size() == 0){
        map<string,string> table = PLAYER->setTableProperties(PLAYER->roomName, PLAYER->roompassword, PLAYER->getBetSelected(), 1, 1);
        AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->createRoom(PLAYER->getPlayerUniqueName(), PLAYER->getPlayerUniqueName(), ROOM_USER_LIMIT, table);
    }
    else {
        for(int i=0; i<mevent.roomData.size(); i++){
            PLAYER->setRoomId(mevent.roomData.at(i).roomId);
            AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
            warpClientRef->joinRoom(mevent.roomData.at(i).roomId);
            break;
            
        }
    }
}

//NotificationListener
void LPTPlayerMatchingScene::onRoomCreated(AppWarp::room rData) {
    printf(__PRETTY_FUNCTION__);
}

void LPTPlayerMatchingScene::onUserLeftRoom(AppWarp::room rData, std::string user) {
    printf(__PRETTY_FUNCTION__);
}

void LPTPlayerMatchingScene::onUserJoinedRoom(AppWarp::room rData, std::string user) {
    printf(__PRETTY_FUNCTION__);
    //Update room properties
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->updateRoomProperties(rData.roomId,PLAYER->setTableProperties(PLAYER->roomName, PLAYER->roompassword, PLAYER->getBetSelected(), 2, 2),vector<string>());
    
}

void LPTPlayerMatchingScene::onChatReceived(AppWarp::chat chatevent) {
    printf(__PRETTY_FUNCTION__);
}

void LPTPlayerMatchingScene::onUpdatePeersReceived(AppWarp::byte update[], int len, bool isUDP) {
    printf(__PRETTY_FUNCTION__);
}


#pragma mark App42
void LPTPlayerMatchingScene::getOpponentDetailsFromUserName(string userName) {
    OPPONENT->setUniqueName(userName);
     LPTServerListener::getInstance()->setLPTServerDelegate(this);
    LPTServerListener::getInstance()->fetchPlayerData(userName.c_str(), "", "", PLAYER->getPlayerAuthToken(), kFetchOpponentDetails);
    //Fetching opponent data
//    StorageService *sServices = App42API::BuildStorageService();
//    sServices->FindDocumentByKeyValue(APPWARP_DB_NAME, COLLECTION_USER, FIELD_UNAME, OPPONENT->getUniqueName().c_str(), app42callback(LPTPlayerMatchingScene::fetchExistingUserData, this));
}

void LPTPlayerMatchingScene::fetchExistingUserData(void* response) {

    App42StorageResponse *res = (App42StorageResponse*)response;
    if (res->isSuccess) {
        App42Storage storage = res->storages.at(0);
        JSONDocument doc = storage.jsonDocArray.at(0);
        OPPONENT->createProfileWith(doc.getJsonDoc().c_str());
        this->stopSearch(0.0);
    } else {
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, res->errorDetails.c_str(), false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
//        MessageBox(res->errorDetails.c_str(), kAlertErrorText);
        //        Director::getInstance()->replaceScene(LPTLoginScene::createScene());
    }
}
#pragma mark ServerCalls
void LPTPlayerMatchingScene::serverResponseComplete(int tag) {
    LPTActivityIndicator::stopAnimating(this);
    if(tag == kFetchOpponentDetails) {
        log("enter to room");
        this->stopSearch(0.0);
    }
}
void LPTPlayerMatchingScene::serverListenerResponseFaliure(int tag, string message) {
    LPTActivityIndicator::stopAnimating(this);
    LPTPopUp *pop =  LPTPopUp::showPopUpWith(kAlertErrorText, message, false, NORMAL_POPUP);
    this->addChild(pop, 10000);
}
void LPTPlayerMatchingScene::serverResponseCompletedWithData(picojson::array data) {
    
}
