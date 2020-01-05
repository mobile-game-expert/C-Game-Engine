//
//  LPTStoreLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati thakur on 17/09/18.
//

#include "LPTStoreLayer.hpp"
#include "LPTTopBar.hpp"
#include "LPTGameSelectionScene.hpp"
#include "LPTStoreItemComponent.hpp"
#include "LPTEvents.hpp"

#define TOP_LAYER_TAG                   6002
#define STORE_POPUP_TAG                 6003
#define STORE_ITEM_BOX_BASE_TAG         6100
#define BUY_BUTTON_BASE_TAG             6200

#define StoreItemPlistName              "StoreItemList.plist"

#define kStoreItemsInARow                       3

//
LPTStoreLayer* LPTStoreLayer::createLayer(int backButtonTag) {
    auto layer = LPTStoreLayer::create();
    layer->initWithTag(backButtonTag);
    return layer;
}

bool LPTStoreLayer::initWithTag(int tag) {
    if (!BaseLayer::init()) {
        return false;
    }
    backButtonTag = tag;
    setBackgroundSprite(kGameSelectionBGImageName);
    LayerColor *layerColor = LayerColor::create(Color4B(16, 6, 8, 150), this->getContentSize().width, this->getContentSize().height);
    this->addChild(layerColor);
    sdkbox::IAP::setListener(this); //IAP purchase listener
    return true;
}

void LPTStoreLayer::onEnter() {
    BaseLayer::onEnter();
    getStoreDataFromPlist();
    setupUI();
    setupScrollView();
}

void LPTStoreLayer::onExit() {
    LPTServerListener::getInstance()->setLPTServerDelegate(NULL);
    BaseLayer::onExit();
}
#pragma mark - Setup UI
void LPTStoreLayer::setupUI() {
    //TouchPreventionButton
    auto touchRestrictButton = this->createButton(kSignupBG, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5), 0);
    touchRestrictButton->setOpacity(0);
    touchRestrictButton->setContentSize(Size(this->getContentSize().width, this->getContentSize().height));
    this->addChild(touchRestrictButton);
    
    
    //TopBar
    LPTTopBar *layer = GameUtils::createTopBar(this, kUserProfileTopBarImageName);
    layer->setTag(TOP_LAYER_TAG);
    layer->addUserProfileTopBar(backButtonTag, kStoreSceneTitle, CC_CALLBACK_2(LPTStoreLayer::buttonCallback, this));
    
    //Store popup  kPopupMatchMaking
    storeItemBaseSprite = BaseLayer::createSprite(kStorePopupImageName, STORE_POPUP_TAG, Vec2(this->getContentSize().width * 0.5 , (this->getContentSize().height - layer->getContentSize().height) * 0.5), this);
    float scaleFactor = GameUtils::isIpad ? (this->getContentSize().width * 0.97 / storeItemBaseSprite->getContentSize().width) : ((this->getContentSize().height - layer->getContentSize().height) * 0.97 / storeItemBaseSprite->getContentSize().height);
    storeItemBaseSprite->setScale(scaleFactor);
    
}

#pragma mark - Store item data setup
void LPTStoreLayer::getStoreDataFromPlist() {
    ValueVector storeItems = FileUtils::getInstance()->getValueVectorFromFile(StoreItemPlistName);
    for(auto iterator = storeItems.begin(); iterator != storeItems.end(); iterator++) {
        StoreItem item;
        ValueMap itemDict = iterator->asValueMap();
        item.title = itemDict["storeItemTitle"].asString();
        item.buysCoins = itemDict["storeItemBuyCoins"].asString();
        item.forPrice = itemDict["storeItemPrice"].asString();
        item.coinImageName = itemDict["storeItemImage"].asString();
        arrayStoreItems.push_back(item);
    }
}

#pragma mark - setup Scroll view
void LPTStoreLayer::setupScrollView() {
    //add scroll view
    Sprite *baseSprite = Sprite::create();
    baseSprite->setPosition(Vec2(storeItemBaseSprite->getContentSize().width*0.5, storeItemBaseSprite->getContentSize().height *0.5));
    baseSprite->setContentSize(Size(storeItemBaseSprite->getContentSize().width*0.95, storeItemBaseSprite->getContentSize().height *0.925));
    storeItemBaseSprite->addChild(baseSprite);
    Size scrollOuterSize = Size(baseSprite->getContentSize().width, baseSprite->getContentSize().height);
    scrollViewStoreItem = ui::ScrollView::create();
    scrollViewStoreItem->setContentSize(scrollOuterSize);
    scrollViewStoreItem->setPosition(Vec2(0,0));
    scrollViewStoreItem->setContentSize(scrollOuterSize);
    scrollViewStoreItem->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollViewStoreItem->setScrollBarEnabled(false);
    scrollViewStoreItem->setInnerContainerSize(scrollOuterSize);
    scrollViewStoreItem->setTouchEnabled(true);
    scrollViewStoreItem->setBounceEnabled(false);
    scrollViewStoreItem->setSwallowTouches(false);
//    scrollViewStoreItem->setBackGroundColor(Color3B::WHITE);
//    scrollViewStoreItem->setBackGroundColorType(ui::ScrollView::BackGroundColorType::SOLID);
    baseSprite->addChild(scrollViewStoreItem);
    createStoreItemsUI();
    
}

void LPTStoreLayer::createStoreItemsUI() {
    int numberOfRows = (arrayStoreItems.size() % kStoreItemsInARow == 0) ? arrayStoreItems.size() / kStoreItemsInARow : (arrayStoreItems.size() / kStoreItemsInARow)+1;
    
    Sprite *baseSpriteItemBox = Sprite::create();
    
    baseSpriteItemBox->setContentSize(Size(scrollViewStoreItem->getContentSize().width/kStoreItemsInARow, scrollViewStoreItem->getContentSize().height / numberOfRows));
    
    int currentElementIndex = 0;
    for(int rowCount = 0; rowCount < numberOfRows; rowCount++) {
        
        float positionY = scrollViewStoreItem->getContentSize().height - baseSpriteItemBox->getContentSize().height*(0.5+rowCount);
        
        for (int columnCount = 0; columnCount < kStoreItemsInARow; columnCount++) {
            
            StoreItem currentItem = arrayStoreItems.at(currentElementIndex);
            
            Sprite *baseSpriteItemBox = Sprite::create();
            baseSpriteItemBox->setContentSize(Size(scrollViewStoreItem->getContentSize().width/kStoreItemsInARow, scrollViewStoreItem->getContentSize().height / numberOfRows));
            float positionX = baseSpriteItemBox->getContentSize().width*(0.5+columnCount);
            baseSpriteItemBox->setPosition(Vec2(positionX, positionY));
            scrollViewStoreItem->addChild(baseSpriteItemBox,20);
            
            //Base Box
            LPTStoreItemComponent *storeItemBox = LPTStoreItemComponent::createStoreItemComponent(Vec2(baseSpriteItemBox->getContentSize().width*0.5, baseSpriteItemBox->getContentSize().height*0.5), STORE_ITEM_BOX_BASE_TAG +currentElementIndex);
            storeItemBox->setScale(baseSpriteItemBox->getContentSize().height*0.925 /storeItemBox->getContentSize().height);
            storeItemBox->setTag(STORE_ITEM_BOX_BASE_TAG+currentElementIndex);
            storeItemBox->setStoreItemsUI(currentItem, CC_CALLBACK_2(LPTStoreLayer::buttonCallback, this), BUY_BUTTON_BASE_TAG+currentElementIndex);
            baseSpriteItemBox->addChild(storeItemBox);
            currentElementIndex++;
        }
    }
}

#pragma mark - Setup Button Callback
void LPTStoreLayer::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case kBackToGameSceneBackButtonTag:
                    this->removeFromParentAndCleanup(true);
                    break;
                case kBackToGameSelectionSceneBackButtonTag:
                    Director::getInstance()->popScene();
                    break;
                default: //For Buy buttons
                    
                    int indexOfPurchase = button->getTag() - BUY_BUTTON_BASE_TAG;
                    StoreItem selectedItem = arrayStoreItems.at(indexOfPurchase);
                    //Uncomment next line when IAP is added and remove temporary code
//                    sdkbox::IAP::purchase(GameUtils::getIAPPurchaseName(indexOfPurchase)); // IAP purchase
                    
                    //**************Temporary code for purchase**************
                    int currentCoins = PLAYER->getTotalCoins() + GameUtils::convertStringToDouble(selectedItem.buysCoins);
                    PLAYER->setTotalCoins((double)currentCoins);
                    //Update coins in the Database
                    updateUserDocumentForCoins();
                    break;
            }
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
}


#pragma mark - App42 call
void LPTStoreLayer::updateUserDocumentForCoins() {
    LPTActivityIndicator::startAnimating(this);
    string key = FIELD_UNAME;
    string value = PLAYER->getPlayerUniqueName();
    
    LPTServerListener::getInstance()->setLPTServerDelegate(this);
    LPTServerListener::getInstance()->updateUserGameData(PLAYER->getPlayerGameXP(), PLAYER->getTotalCoins(), PLAYER->getPlayerGameWon(), PLAYER->getPlayerGamePlayed(), PLAYER->getPlayerGameLevel(), PLAYER->getPlayerAuthToken(), "false", kGameModelUpdateRequest);
    
//    App42Object *obj = PLAYER->getApp42ObjectForUpdation();
//    StorageService *storageService = App42API::BuildStorageService();
//    storageService->UpdateDocumentByKeyValue(APPWARP_DB_NAME, COLLECTION_USER, key.c_str(), value.c_str(), obj, app42callback(LPTStoreLayer::updateUserDocumentForCoinsResponse, this));
}

void LPTStoreLayer::updateUserDocumentForCoinsResponse(void *response) {
    LPTActivityIndicator::stopAnimating(this);
    App42StorageResponse *storageResponse = (App42StorageResponse*)response;
    printf("\ncode=%d",storageResponse->getCode());
    printf("\nResponse Body=%s",storageResponse->getBody().c_str());
    if (storageResponse->isSuccess) {
        printf("Puchase Success");
        //Purchase popup
        auto popup = LPTPopUp::showPopUpWith(kInformation, kCoinsAddedToProfile, false, NORMAL_POPUP);
        popup->setPopUpDelegate(this);
        this->addChild(popup, 50000000);
    } else {
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, storageResponse->errorDetails.c_str(), false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);
        
//        MessageBox(storageResponse->errorDetails.c_str(), kAlertErrorText);
        printf("\nerrordetails:%s",storageResponse->errorDetails.c_str());
        printf("\nerrorMessage:%s",storageResponse->errorMessage.c_str());
        printf("\nappErrorCode:%d",storageResponse->appErrorCode);
        printf("\nhttpErrorCode:%d",storageResponse->httpErrorCode);
    }
}

#pragma mark - Game popup callabck
void LPTStoreLayer::okButtonAction(Popup_Type type) {
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

void LPTStoreLayer::cancelButtonAction(Popup_Type type) {
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

#pragma mark - IAP methods
void LPTStoreLayer::onSuccess(sdkbox::Product const& p) {
    log("onSuccess : %s : %s", p.name.c_str(), p.id.c_str());
    double purchasedCoins = GameUtils::getPurchasedCoinsFromIAPName(p.name.c_str());
    double currentCoins = PLAYER->getTotalCoins() + purchasedCoins;
    PLAYER->setTotalCoins(currentCoins);
    
    //Update coins in the Database
    updateUserDocumentForCoins();
    //Purchase success popup
    auto popup = LPTPopUp::showPopUpWith(kInformation, kCoinsAddedToProfile, false, NORMAL_POPUP);
    popup->setPopUpDelegate(this);
    this->addChild(popup, 50000000);
    //--
    APP_MANAGER->totalSpendsPerSession =  APP_MANAGER->totalSpendsPerSession + p.priceValue;
    APP_MANAGER->totalSpendsByUser = APP_MANAGER->totalSpendsByUser + p.priceValue;
    APP_MANAGER->setTotalSpendsByUser(APP_MANAGER->totalSpendsByUser);
    LPTEvents::logUserTotalSpentsPerSession(APP_MANAGER->totalSpendsPerSession);
    LPTEvents::logUserTotalSpents(APP_MANAGER->totalSpendsByUser);
    LPTEvents::logIAPPackPurchased(p.priceValue);
}

void LPTStoreLayer::onFailure(sdkbox::Product const& p, const std::string &msg) {
    log("onFailure : %s - %s", p.name.c_str(), msg.c_str());
    //Purchase Failure popup
    auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kErrorWhilePurchasing, false, NORMAL_POPUP);
    popup->setPopUpDelegate(this);
    this->addChild(popup, 50000000);
}

void LPTStoreLayer::onCanceled(sdkbox::Product const& p) {
    log("onCanceled : %s", p.id.c_str());
}

#pragma mark ServerResponse
void LPTStoreLayer::serverResponseComplete(int tag){
    LPTActivityIndicator::stopAnimating(this);
    auto popup = LPTPopUp::showPopUpWith(kInformation, kCoinsAddedToProfile, false, NORMAL_POPUP);
    popup->setPopUpDelegate(this);
    this->addChild(popup, 50000000);
}
void LPTStoreLayer::serverListenerResponseFaliure(int tag, string message) {
    LPTActivityIndicator::stopAnimating(this);
    LPTPopUp *pop =  LPTPopUp::showPopUpWith(kAlertErrorText, message, false, NORMAL_POPUP);
    this->addChild(pop);
}
void LPTStoreLayer::serverResponseCompletedWithData(picojson::array data) {
    
}
