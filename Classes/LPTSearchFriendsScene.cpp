//
//  LPTSearchFriendsScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 08/10/18.
//

#include "LPTSearchFriendsScene.hpp"
#include "LPTTopBar.hpp"
#include "LPTBetSelectionScene.hpp"


#define TOP_BAR_TAG                     7000
#define BACK_BUTTON_TAG                 7001
#define EDITBOX_SEARCH_TAG              7003
#define SEARCH_BUTTON_TAG               7004
#define FB_BUTTON_TAG                   7005
#define SEARCHBARBASE_TAG               7006
#define FRIEND_CELL_BASE_TAG            7100

#define kSearchBarBaseImageName         "searchBase.png"
#define kSearchIconImageName            "search.png"
#define kSearchButtonImageName          "challenge.png"
#define kFBFriendInviteImageName        "fbFriendInvite.png"
#define kSearchEditBoxImageName         "searchBox.png"

#define kSearchFieldPlacegholderTextField   "Please enter Unique ID"
#define kSearchButtonText                   "Search"

#pragma mark - CreateScene
BaseScene* LPTSearchFriendsScene::createScene() {
    auto scene = BaseScene::create();
    auto layer = LPTSearchFriendsScene::create();
    scene->addChild(layer);
    
    return scene;
}

#pragma mark - SceneMethod
bool LPTSearchFriendsScene::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    setBackgroundSprite(kGamePlayBG);
    LayerColor *layerColor = LayerColor::create(Color4B(16, 6, 8, 220), this->getContentSize().width, this->getContentSize().height);
    this->addChild(layerColor);
    //Getting online users
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setZoneRequestListener(this);
    warpClientRef->getOnlineUsers();
    return true;
}

void LPTSearchFriendsScene::onEnter() {
    BaseLayer::onEnter();
//    setupData();
    LPTServerListener::getInstance()->setLPTServerDelegate(this);
    setupUI();
}

void LPTSearchFriendsScene::onExit() {
    LPTServerListener::getInstance()->setLPTServerDelegate(NULL);
    BaseLayer::onExit();
}

#pragma mark - Setup data
void LPTSearchFriendsScene::setupData() {
    getTotalUsersCount();
}

#pragma mark - Setup UI
void LPTSearchFriendsScene::setupUI() {
    //TopBar
    LPTTopBar *layer = GameUtils::createTopBar(this, kUserProfileTopBarImageName);
    layer->setTag(TOP_BAR_TAG);
    layer->addUserProfileTopBar(BACK_BUTTON_TAG, kFriendsSceneTitle, CC_CALLBACK_2(LPTSearchFriendsScene::buttonCallback, this));
    
    //Search Bar
    auto searchBarBase = BaseSprite::createWithFileName(kSearchBarBaseImageName);
    searchBarBase->setPosition(Vec2(this->getContentSize().width * 0.07, this->getContentSize().height - layer->getContentSize().height * 1.12));
    searchBarBase->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    searchBarBase->setScale(this->getContentSize().width * 0.65 / searchBarBase->getContentSize().width);
    searchBarBase->setTag(SEARCHBARBASE_TAG);
    this->addChild(searchBarBase);
    
    auto searchIconSprite = BaseSprite::createWithFileName(kSearchIconImageName);
    searchIconSprite->setPosition(Vec2(searchBarBase->getContentSize().width *0.01, searchBarBase->getContentSize().height*0.5));
    searchIconSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    searchIconSprite->setScale(searchBarBase->getContentSize().height*0.725/searchIconSprite->getContentSize().height);
    searchBarBase->addChild(searchIconSprite);
    
    BaseSprite *spriteForScale = BaseSprite::createWithFileName(kSearchEditBoxImageName);
    
    auto componentScale = searchBarBase->getContentSize().width * 0.85 / spriteForScale->getContentSize().width;
    auto userNameField = createEditBox(kSearchEditBoxImageName, Vec2(searchBarBase->getContentSize().width * 0.5, searchBarBase->getContentSize().height * 0.5), Vec2(0,0), "", EDITBOX_SEARCH_TAG, searchBarBase, EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS, EditBox::InputMode::SINGLE_LINE, this, componentScale);
    
    EditBox* username = (EditBox *)userNameField->getChildByTag(EDITBOX_SEARCH_TAG);
    username->setDelegate(this);
    username->setPlaceHolder(kSearchFieldPlacegholderTextField);
    username->setFont(kOpenSans_Bold, 60);
    username->setPlaceholderFont(kOpenSans_Bold, 60);
    
    //Search Button
    auto searchButton = createButton(kSearchButtonImageName,Vec2(searchBarBase->getPositionX() + searchBarBase->getContentSize().width * searchBarBase->getScale() * 1.01, searchBarBase->getPositionY()));
    searchButton->setTitleText(kSearchButtonText);
    searchButton->setTitleFontSize(54);
    searchButton->setTitleFontName(kOpenSans_Bold);
    searchButton->setTag(SEARCH_BUTTON_TAG);
    searchButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    searchButton->addTouchEventListener(CC_CALLBACK_2(LPTSearchFriendsScene::buttonCallback, this));
//    auto scaleFactor = (this->getContentSize().width + searchBarBase->getPositionX()  - searchBarBase->getContentSize().width * searchBarBase->getScale()  * 1.2) / searchButton->getContentSize().width;
    auto scaleFactor = searchBarBase->getContentSize().height * searchBarBase->getScale() / searchButton->getContentSize().height;
    searchButton->setScale(scaleFactor);
    this->addChild(searchButton);
    
    //Facebook friends invite
    auto fbButton = createButton(kFBFriendInviteImageName, Vec2(this->getContentSize().width*0.5, this->getContentSize().height *0.12));
    auto fbScaleFactor = GameUtils::isIphoneX ? 0.3 : 0.4;
    fbButton->setScale(this->getContentSize().width * fbScaleFactor/fbButton->getContentSize().width);
    fbButton->setTag(FB_BUTTON_TAG);
    fbButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    fbButton->addTouchEventListener(CC_CALLBACK_2(LPTSearchFriendsScene::buttonCallback, this));
    this->addChild(fbButton);
    
    //Add Table View
    Sprite *baseSprite = Sprite::create();
    baseSprite->setPosition(Vec2(this->getContentSize().width *0.5, (this->getContentSize().height - searchBarBase->getContentSize().height * 1.1 ) * 0.5));
    baseSprite->setContentSize(Size(this->getContentSize().width, (this->getContentSize().height - searchBarBase->getContentSize().height * 1.1 ) * 0.7));
    this->addChild(baseSprite);

    cellSize = Size(baseSprite->getContentSize().width*1.5, baseSprite->getContentSize().height);
    friendListTableView = TableView::create(this, Size(baseSprite->getContentSize().width, baseSprite->getContentSize().height));
    friendListTableView->setPosition(Vec2(0, 0));
    friendListTableView->setDelegate(this);
    friendListTableView->setDirection(ScrollView::Direction::HORIZONTAL);
    friendListTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    friendListTableView->setBounceable(true);
    baseSprite->addChild(friendListTableView);
}


#pragma mark - Setup Button Callback
void LPTSearchFriendsScene::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case BACK_BUTTON_TAG:
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5, LPTBetSelectionScene::createScene()));
                    break;
                case FB_BUTTON_TAG: {
                    string message = "\nYou must be login with facebook.";
                    if(PLAYER->getLoginState() != FB_USER) {
                        message = "\nYou must be login with facebook.";
                    }else {
                      //TODO do FB get user_friends here
                        message = "\n  Please request relevent permissions \n\n                 from Facebook";
                    }
                    auto popup = LPTPopUp::showPopUpWith(kInformation, message.c_str(), false, NORMAL_POPUP);
                    popup->setPopUpDelegate(this);
                    popup->setScale(1.2);
                    this->addChild(popup, 5000);
                }
                    break;
                case SEARCH_BUTTON_TAG: {
                    BaseSprite *baseSprite = (BaseSprite*)this->getChildByTag(SEARCHBARBASE_TAG);
                    Sprite *baseSpriteOfTextField = (Sprite*)baseSprite->getChildByTag(EDITBOX_SEARCH_TAG);
                    EditBox* username = (EditBox *)baseSpriteOfTextField->getChildByTag(EDITBOX_SEARCH_TAG);
                    string searchText = GameUtils::removeSubstrsValue(username->getText(), " ");
                    
                    //Animator...
                    LPTActivityIndicator::startAnimating(this);
                    LPTServerListener::getInstance()->fetchPlayerData(searchText, "", "", PLAYER->getPlayerAuthToken(), kFetchFriendsDataRequest);
                }
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

#pragma mark - Table View methods
void LPTSearchFriendsScene::tableCellTouched(TableView* table, TableViewCell* cell) {
    printf("Cell touched.");
}

Size LPTSearchFriendsScene::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    return Size(cellSize.height, cellSize.height);
}

Size LPTSearchFriendsScene::cellSizeForTable(TableView *table) {
    return Size(cellSize.height, cellSize.height);
}

TableViewCell* LPTSearchFriendsScene::tableCellAtIndex(TableView *table, ssize_t idx) {
    LPTSearchFriendCell *tableViewCell = (LPTSearchFriendCell *)table->dequeueCell();
    if (tableViewCell == NULL) {
        tableViewCell = new LPTSearchFriendCell();
    }else {
        tableViewCell->removeAllChildrenWithCleanup(true);
    }
    Size cellSize = cellSizeForTable(table);
    //Temp Friend Object
//    FriendData data;
//    data.userName = "PragatiThakur2";
//    data.coins = 1000;
//    data.status = idx % 2 == 0 ? ONLINE : OFFLINE;
//    data.challengeSent = idx % 2 == 0 ? true : false;
//    data.profilePic = kPlayerProfileImageName;
    FriendData data = arraySearchResult.at(idx);
    tableViewCell->initWithFriendData(data, cellSize, FRIEND_CELL_BASE_TAG+idx);
    tableViewCell->setCellDelegate(this);
    return tableViewCell;
}

ssize_t LPTSearchFriendsScene::numberOfCellsInTableView(TableView *table) {
    return arraySearchResult.size();
}

#pragma mark - Challenge ButtonPressed
void LPTSearchFriendsScene::challengeButtonPressed(Button *sender, bool alreadyChallenged) {
    if(alreadyChallenged) {
        auto popup = LPTPopUp::showPopUpWith(kInformation, kChallengeAlreadySent, false, NORMAL_POPUP);
        popup->setPopUpDelegate(this);
        this->addChild(popup, 5000);
    } else {
        log("%d", sender->getTag());
        int index = sender->getTag() - FRIEND_CELL_BASE_TAG;
        FriendData item = arraySearchResult.at(index);
        item.challengeSent = true;
        arraySearchResult.erase(arraySearchResult.begin() + index);
        arraySearchResult.insert(arraySearchResult.begin() +index, item);
        //Update in complete friend database array
        updateCompleteSearchArray(item);
        auto receiverName = item.userName;
        APP_MANAGER->sendChallangeNotification(receiverName);
    }
}

void LPTSearchFriendsScene::updateCompleteSearchArray(FriendData challengedUser) {
    
    for( int index = 0; index < arrayFriends.size(); index++) {
        FriendData friendItem = arrayFriends.at(index);
        if(friendItem.userName.find(challengedUser.userName) != string::npos) {
            arrayFriends.erase(arrayFriends.begin() + index);
            arrayFriends.insert(arrayFriends.begin() +index, challengedUser);
            break;
        }
    }
}

#pragma mark - Search User
void LPTSearchFriendsScene::searchFriendWithUsername(string searchText) {
    arraySearchResult.clear();
    for( int index = 0; index < arrayFriends.size(); index++) {
        FriendData friendItem = arrayFriends.at(index);
        string userName = GameUtils::lowerCase(friendItem.userName);
        if(userName.find(searchText) != string::npos) {
            arraySearchResult.push_back(friendItem);
        }
    }
    friendListTableView->reloadData();
}

#pragma mark - App42 Callback
void LPTSearchFriendsScene::getTotalUsersCount() {
//    LPTActivityIndicator::startAnimating(this);
//    StorageService *storageService = App42API::BuildStorageService();
//    storageService->FindAllDocumentsCount(APPWARP_DB_NAME, COLLECTION_USER,app42callback(LPTSearchFriendsScene::getTotalUsersCountResponse, this));
}

void LPTSearchFriendsScene::fetchAllUsers() {
//    LPTActivityIndicator::startAnimating(this);
//    StorageService *storageService = App42API::BuildStorageService();
//    //To find alll the documents
//    storageService->FindAllDocuments(APPWARP_DB_NAME, COLLECTION_USER,app42callback(LPTSearchFriendsScene::fetchAllUsersResponse, this));
}

void LPTSearchFriendsScene::getTotalUsersCountResponse(void* response) {
//    LPTActivityIndicator::stopAnimating(this);
//    App42StorageResponse *storageResponse = (App42StorageResponse*)response;
//    printf("\ncode=%d",storageResponse->getCode());
//    printf("\nResponse Body=%s",storageResponse->getBody().c_str());
//    if (storageResponse->isSuccess) {
//        printf("\nTotalRecords=%d",storageResponse->getTotalRecords());
//
//        int totalRecords = storageResponse->getTotalRecords();
//        StorageService *storageService = App42API::BuildStorageService();
//        storageService->FindAllDocuments(APPWARP_DB_NAME, COLLECTION_USER, app42callback(LPTSearchFriendsScene::fetchAllUsersResponse, this));
//        LPTActivityIndicator::startAnimating(this);
//        if(totalRecords > 100){
//            int iterations = (totalRecords - 100) / 100;
//            if(totalRecords%100 != 0)
//                iterations++;
//
//            for(int i=100; i<=(iterations*100); i=i+100) {
//                LPTActivityIndicator::startAnimating(this);
//                storageService->FindAllDocuments(APPWARP_DB_NAME, COLLECTION_USER, i+100, i, app42callback(LPTSearchFriendsScene::fetchAllUsersResponse, this));
//            }
//        }
//    } else {
//        printf("\nerrordetails:%s",storageResponse->errorDetails.c_str());
//        printf("\nerrorMessage:%s",storageResponse->errorMessage.c_str());
//        printf("\nappErrorCode:%d",storageResponse->appErrorCode);
//        printf("\nhttpErrorCode:%d",storageResponse->httpErrorCode);
//    }
}

void LPTSearchFriendsScene::fetchAllUsersResponse(void *response) {
//    LPTActivityIndicator::stopAnimating(this);
//    App42StorageResponse *storageResponse = (App42StorageResponse*)response;
//    printf("\ncode=%d",storageResponse->getCode());
//    printf("\nResponse Body=%s",storageResponse->getBody().c_str());
//     if (storageResponse->isSuccess) {
//
//         App42Storage storage = storageResponse->storages.at(0);
//         for(std::vector<JSONDocument>::iterator it = storage.jsonDocArray.begin(); it != storage.jsonDocArray.end(); ++it) {
//             picojson::value v;
//             picojson::parse(v, it->getJsonDoc().c_str());
//             log("%s", it->getJsonDoc().c_str());
//             picojson::object object = v.get<picojson::object>();
//             FriendData data;
//             data.userName =object[FIELD_UNAME].get<string>();
//             log("%f",object[FIELD_TOTAL_COINS].get<double>());
//             data.coins = object[FIELD_TOTAL_COINS].get<double>();
//             data.profilePic = object[FIELD_PROFILE_PICTURE].get<string>();
//             data.status = checkIfUserOnline(data.userName);
//             data.challengeSent =  false;
//             arrayFriends.push_back(data);
//         }
////         arraySearchResult = arrayFriends;
//         friendListTableView->reloadData();
//     } else {
//         auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoUserFound, false, NORMAL_POPUP);
//         popup->setPopUpDelegate(this);
//         this->addChild(popup, 5000);
//     }
//
}



#pragma mark - Game popup callabck
void LPTSearchFriendsScene::okButtonAction(Popup_Type type) {
    switch (type) {
        case LOGOUT_POPUP:
            
            break;
        case GAMEQUIT_POPUP:
            
            break;
        case NORMAL_POPUP:
            break;
        default:
            
            break;
    }
    LPTPopUp * popUp = (LPTPopUp *)this->getChildByTag((int)type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

void LPTSearchFriendsScene::cancelButtonAction(Popup_Type type) {
    switch (type) {
        case LOGOUT_POPUP:
            
            break;
        case GAMEQUIT_POPUP:
            
            break;
        case NORMAL_POPUP:
            break;
        default:
            
            break;
    }
    LPTPopUp * popUp = (LPTPopUp *)this->getChildByTag(type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Editbox Delegate
void LPTSearchFriendsScene::editBoxTextChanged(EditBox* editBox, const std::string& text) {
    string searchString = editBox->getText();
    if(searchString.empty()) {
        arraySearchResult.clear();
        friendListTableView->reloadData();
    } else {
//        searchFriendWithUsername(GameUtils::lowerCase(editBox->getText()));
    }
}

void LPTSearchFriendsScene::editBoxReturn(EditBox* editBox) {
    editBox->detachWithIME();
    

}

#pragma mark - Online users
void LPTSearchFriendsScene::onGetOnlineUsersDone (AppWarp::liveresult res) {
    printf(__PRETTY_FUNCTION__);
    arrayOnlineUsers.clear();
    auto list = res.list;
    for(int index = 0; index < res.list.size(); index++) {
        printf("\nUsername :%s", res.list[index].c_str());
        arrayOnlineUsers.push_back(res.list[index]);
    }
}
#pragma mark ServerResponse
void LPTSearchFriendsScene::serverResponseComplete(int tag) {
        LPTActivityIndicator::stopAnimating(this);
    //    App42StorageResponse *storageResponse = (App42StorageResponse*)response;
    //    printf("\ncode=%d",storageResponse->getCode());
    //    printf("\nResponse Body=%s",storageResponse->getBody().c_str());
    //     if (storageResponse->isSuccess) {
    //
    //         App42Storage storage = storageResponse->storages.at(0);
    //         for(std::vector<JSONDocument>::iterator it = storage.jsonDocArray.begin(); it != storage.jsonDocArray.end(); ++it) {
    //             picojson::value v;
    //             picojson::parse(v, it->getJsonDoc().c_str());
    //             log("%s", it->getJsonDoc().c_str());
    //             picojson::object object = v.get<picojson::object>();
    //             FriendData data;
    //             data.userName =object[FIELD_UNAME].get<string>();
    //             log("%f",object[FIELD_TOTAL_COINS].get<double>());
    //             data.coins = object[FIELD_TOTAL_COINS].get<double>();
    //             data.profilePic = object[FIELD_PROFILE_PICTURE].get<string>();
    //             data.status = checkIfUserOnline(data.userName);
    //             data.challengeSent =  false;
    //             arrayFriends.push_back(data);
    //         }
    ////         arraySearchResult = arrayFriends;
    //         friendListTableView->reloadData();
    //     } else {
    //         auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoUserFound, false, NORMAL_POPUP);
    //         popup->setPopUpDelegate(this);
    //         this->addChild(popup, 5000);
    //     }
    //
}
void LPTSearchFriendsScene::serverListenerResponseFaliure(int tag, string message) {
    LPTActivityIndicator::stopAnimating(this);
    auto popup = LPTPopUp::showPopUpWith(kNoUserFound, message, false, NORMAL_POPUP);
    popup->setPopUpDelegate(this);
    this->addChild(popup, 5000);
}

void LPTSearchFriendsScene::serverResponseCompletedWithData(picojson::array data) {
    
    LPTActivityIndicator::stopAnimating(this);
    arraySearchResult.clear();
    arrayFriends.clear();
    
    for(int friendsCounter = 0; friendsCounter < data.size(); friendsCounter++) {
        picojson::object userProfileData = data[friendsCounter].get<picojson::object>();
        FriendData fdata;
        fdata.userName =userProfileData[FIELD_UNAME].get<string>();
        fdata.coins = userProfileData[FIELD_TOTAL_COINS].get<double>();
        fdata.profilePic = userProfileData[FIELD_PROFILE_PICTURE].get<string>();
        fdata.status = userProfileData[FIELD_Online_Status].get<bool>();
        fdata.challengeSent =  false;
        arrayFriends.push_back(fdata);
    }
    arraySearchResult = arrayFriends;
    friendListTableView->reloadData();
}
