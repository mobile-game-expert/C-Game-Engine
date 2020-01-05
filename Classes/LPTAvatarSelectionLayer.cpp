//
//  LPTAvatarSelectionLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 16/10/18.
//

#include "LPTAvatarSelectionLayer.hpp"



#define AVATAR_POPUP_TAG                11001
#define DONE_BUTTON_TAG                 11002
#define CANCEL_BUTTON_TAG               11003
#define AVATAR_BASE_TAG                 11100

#define kTotalAvatars                   6
#define kAvatarsInARow                  3
#define kColumnMargin                   0.0
#define kRowMargin                      0.5

#define kDoneText                       "Done"
#define kCancelText                     "Cancel"
#define kSelectionAvatarImageName       "selectionAvatar.png"


LPTAvatarSelectionLayer* LPTAvatarSelectionLayer::createLayer() {
    auto layer = LPTAvatarSelectionLayer::create();
    layer->initWithTag();
    return layer;
}

bool LPTAvatarSelectionLayer::initWithTag() {
    if (!BaseLayer::init()) {
        return false;
    }
    LayerColor *layerColor = LayerColor::create(Color4B(16, 6, 8, 150), this->getContentSize().width, this->getContentSize().height);
    this->addChild(layerColor);
    
    return true;
}

void LPTAvatarSelectionLayer::onEnter() {
    BaseLayer::onEnter();
    previousAvatar = PLAYER->getPlayerProfilePicture();
    setupUI();
    setupScrollView();
}

void LPTAvatarSelectionLayer::onExit() {
    LPTServerListener::getInstance()->setLPTServerDelegate(NULL);
    BaseLayer::onExit();
}

#pragma mark - setDelegate
void LPTAvatarSelectionLayer::setAvatarDelegate(LPTAvatarSelectionLayerDelegate* delegate) {
    this->avatarDelegate = delegate;
}

#pragma mark - Setup UI
void LPTAvatarSelectionLayer::setupUI() {
    //TouchPreventionButton
    auto touchRestrictButton = this->createButton(kSignupBG, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5), 0);
    touchRestrictButton->setOpacity(0);
    touchRestrictButton->setContentSize(Size(this->getContentSize().width, this->getContentSize().height));
    this->addChild(touchRestrictButton);
    
    int numberOfRows = (kTotalAvatars % kAvatarsInARow == 0) ? kTotalAvatars / kAvatarsInARow : (kTotalAvatars / kAvatarsInARow)+1;
    Sprite *sizeRefSprite = Sprite::create(kSelectionAvatarImageName);
    auto avatarBaseSize = Size(sizeRefSprite->getContentSize().width * (kAvatarsInARow + kColumnMargin), sizeRefSprite->getContentSize().height * (numberOfRows + kRowMargin));
    
    //Avatar popup  kPopupMatchMaking
    avatarBaseSprite = BaseLayer::createSprite(kStorePopupImageName, AVATAR_POPUP_TAG, Vec2(this->getContentSize().width * 0.5 , this->getContentSize().height * 0.5), this);
//    float scaleFactor = GameUtils::isIpad ? (this->getContentSize().width * 0.65 / avatarBaseSprite->getContentSize().width) : (this->getContentSize().height * 0.65 / avatarBaseSprite->getContentSize().height);
//    float scaleFactor = this->getContentSize().width * 0.75 / avatarBaseSprite->getContentSize().width;
//    avatarBaseSprite->setScale(scaleFactor);
    
    avatarBaseSprite->setContentSize(avatarBaseSize);
    float scaleFactor = this->getContentSize().height  * (GameUtils::isIpad ? 0.5 : 0.75) / avatarBaseSize.height;
    avatarBaseSprite->setScale(scaleFactor);
    
    doneButton = createButton(kTutorialPlayButtonImageName, DONE_BUTTON_TAG, Vec2(avatarBaseSprite->getContentSize().width*0.5, avatarBaseSprite->getContentSize().height * 0.125), avatarBaseSprite, CC_CALLBACK_2(LPTAvatarSelectionLayer::buttonCallback, this), 1);
    doneButton->setTitleText(kDoneText);
    doneButton->setTitleFontName(kOpenSans_Bold);
    doneButton->setTitleFontSize(48);
    
    cancelButton = createButton(kCancelImageName, CANCEL_BUTTON_TAG, Vec2(avatarBaseSprite->getContentSize().width*0.975, avatarBaseSprite->getContentSize().height * 0.975), avatarBaseSprite, CC_CALLBACK_2(LPTAvatarSelectionLayer::buttonCallback, this), 1);
    cancelButton->setScale(avatarBaseSprite->getContentSize().width * 0.075 / cancelButton->getContentSize().width);
    cancelButton->setLocalZOrder(10);
}

#pragma mark - setup Scroll view
void LPTAvatarSelectionLayer::setupScrollView() {
    //add scroll view
    Sprite *baseSprite = Sprite::create();
    baseSprite->setPosition(Vec2(avatarBaseSprite->getContentSize().width*0.5, avatarBaseSprite->getContentSize().height * 0.975));
    baseSprite->setContentSize(Size(avatarBaseSprite->getContentSize().width*0.95, avatarBaseSprite->getContentSize().height * 0.925 - doneButton->getContentSize().height));
    baseSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    avatarBaseSprite->addChild(baseSprite);
    Size scrollOuterSize = Size(baseSprite->getContentSize().width, baseSprite->getContentSize().height);
    avatarScrollView = ui::ScrollView::create();
    avatarScrollView->setContentSize(scrollOuterSize);
    avatarScrollView->setPosition(Vec2(0,0));
    avatarScrollView->setContentSize(scrollOuterSize);
    avatarScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    avatarScrollView->setScrollBarEnabled(false);
    avatarScrollView->setInnerContainerSize(scrollOuterSize);
    avatarScrollView->setTouchEnabled(true);
    avatarScrollView->setBounceEnabled(false);
    avatarScrollView->setSwallowTouches(false);
//        avatarScrollView->setBackGroundColor(Color3B::WHITE);
//        avatarScrollView->setBackGroundColorType(ui::ScrollView::BackGroundColorType::SOLID);
    baseSprite->addChild(avatarScrollView);
    createAvatarsUI();
    
}

void LPTAvatarSelectionLayer::createAvatarsUI() {
    int numberOfRows = (kTotalAvatars % kAvatarsInARow == 0) ? kTotalAvatars / kAvatarsInARow : (kTotalAvatars / kAvatarsInARow)+1;
    
    Sprite *baseSpriteItemBox = Sprite::create();
    
    baseSpriteItemBox->setContentSize(Size(avatarScrollView->getContentSize().width/kAvatarsInARow, avatarScrollView->getContentSize().height / numberOfRows));
    
    int currentElementIndex = 0;
    for(int rowCount = 0; rowCount < numberOfRows; rowCount++) {
        
        float positionY = avatarScrollView->getContentSize().height - baseSpriteItemBox->getContentSize().height*(0.5+rowCount);
        int itemsInCurrentRow = (kTotalAvatars - rowCount * kAvatarsInARow) / kAvatarsInARow  == 0 ? (kTotalAvatars - rowCount * kAvatarsInARow) % kAvatarsInARow : kAvatarsInARow;
        for (int columnCount = 0; columnCount < itemsInCurrentRow; columnCount++) {
            
            Sprite *baseSpriteItemBox = Sprite::create();
            avatarCellSize = Size(avatarScrollView->getContentSize().width/kAvatarsInARow, avatarScrollView->getContentSize().height / numberOfRows);
            baseSpriteItemBox->setContentSize(avatarCellSize);
            float positionX = baseSpriteItemBox->getContentSize().width*(0.5+columnCount);
            baseSpriteItemBox->setPosition(Vec2(positionX, positionY));
            baseSpriteItemBox->setTag(AVATAR_BASE_TAG + currentElementIndex);
            avatarScrollView->addChild(baseSpriteItemBox,20);

            auto avatar = createButton(StringUtils::format("avatar_%d.png",currentElementIndex), AVATAR_BASE_TAG + currentElementIndex, Vec2(baseSpriteItemBox->getContentSize().width * 0.5, baseSpriteItemBox->getContentSize().height * 0.5), baseSpriteItemBox, CC_CALLBACK_2(LPTAvatarSelectionLayer::buttonCallback, this), 1);
            avatar->setScale(baseSpriteItemBox->getContentSize().height * 0.8 / avatar->getContentSize().height);
            currentElementIndex++;
        }
    }
}

#pragma mark - Setup Button Callback
void LPTAvatarSelectionLayer::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case DONE_BUTTON_TAG: {
                    //call to update user document
                    if(selectedAvatar.compare("") != 0) {
                        updateUserDocumentForProfilePic();
                    } else {
                        auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kNoAvatarSelected, false, NORMAL_POPUP);
                        popup->setPopUpDelegate(this);
                        this->getParent()->addChild(popup, 5000);
                    }
                    
                }
                    break;
                case CANCEL_BUTTON_TAG: {
                    this->removeFromParentAndCleanup(true);
                }
                    break;
                default: //For Avatar Selection
                    int indexOfAvatar = button->getTag() - AVATAR_BASE_TAG;
                    selectedAvatar = StringUtils::format("avatar_%d.png",indexOfAvatar);
                    addSelectionGlowOnAvatar(indexOfAvatar);
                    break;
            }
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
}

void LPTAvatarSelectionLayer::addSelectionGlowOnAvatar(int selectedIndex) {
    for(int index = 0; index < kTotalAvatars; index++) {
        Sprite *currentSprite = (Sprite*)avatarScrollView->getChildByTag(AVATAR_BASE_TAG + index);
        if(index == selectedIndex) {
            currentSprite->setTexture(kSelectionAvatarImageName);
        } else {
            currentSprite->setTexture("");
        }
        currentSprite->setContentSize(avatarCellSize);
    }
}

#pragma mark - Game popup callback
void LPTAvatarSelectionLayer::okButtonAction(Popup_Type type) {
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
    LPTPopUp * popUp = (LPTPopUp *)this->getParent()->getChildByTag((int)type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

void LPTAvatarSelectionLayer::cancelButtonAction(Popup_Type type) {
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
    LPTPopUp * popUp = (LPTPopUp *)this->getParent()->getChildByTag(type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

#pragma mark - App42 Call
void LPTAvatarSelectionLayer::updateUserDocumentForProfilePic() {
    LPTActivityIndicator::startAnimating(this);
    string key =  FIELD_UNAME;
    string value = PLAYER->getPlayerUniqueName();
    PLAYER->setProfilePic(selectedAvatar);
    LPTServerListener::getInstance()->setLPTServerDelegate(this);
    LPTServerListener::getInstance()->updateUserProfilePic(PLAYER->getPlayerEmailID(), PLAYER->getPlayerProfilePicture(), PLAYER->getPlayerAuthToken(), kUpdateUserProfileRequest);
    
    
//    App42Object *obj = PLAYER->getApp42ObjectForUpdation();
//    StorageService *storageService = App42API::BuildStorageService();
//    storageService->UpdateDocumentByKeyValue(APPWARP_DB_NAME, COLLECTION_USER, key.c_str(), value.c_str(), obj, app42callback(LPTAvatarSelectionLayer::updateUserDocumentForProfilePicResponse, this));
}

void LPTAvatarSelectionLayer::updateUserDocumentForProfilePicResponse(void *response) {
    LPTActivityIndicator::stopAnimating(this);
    App42StorageResponse *storageResponse = (App42StorageResponse*)response;
    printf("\ncode=%d",storageResponse->getCode());
    printf("\nResponse Body=%s",storageResponse->getBody().c_str());
    if (storageResponse->isSuccess) {
        printf("image updated Success");
        PLAYER->setProfilePic(selectedAvatar);
        this->avatarDelegate->profilePictureUpdated(selectedAvatar);
        
    } else {
        PLAYER->setProfilePic(previousAvatar);
        this->avatarDelegate->profilePictureUpdated(previousAvatar);
        
        LPTPopUp *gamePopup = LPTPopUp::showPopUpWith(kAlertErrorText, storageResponse->errorDetails.c_str(), false, NORMAL_POPUP);
        this->addChild(gamePopup, 10000);

//        MessageBox(storageResponse->errorDetails.c_str(), kAlertErrorText);
        printf("\nerrordetails:%s",storageResponse->errorDetails.c_str());
        printf("\nerrorMessage:%s",storageResponse->errorMessage.c_str());
        printf("\nappErrorCode:%d",storageResponse->appErrorCode);
        printf("\nhttpErrorCode:%d",storageResponse->httpErrorCode);
    }
    this->removeFromParentAndCleanup(true);
}

void LPTAvatarSelectionLayer::serverResponseComplete(int tag){
    LPTActivityIndicator::stopAnimating(this);
    PLAYER->setProfilePic(selectedAvatar);
    this->avatarDelegate->profilePictureUpdated(selectedAvatar);
    this->removeFromParentAndCleanup(true);
}

void LPTAvatarSelectionLayer::serverListenerResponseFaliure(int tag, string message) {
    PLAYER->setProfilePic(previousAvatar);
    
    LPTPopUp *pop =  LPTPopUp::showPopUpWith(kAlertErrorText, message, false, NORMAL_POPUP);
    this->addChild(pop);
    
    this->avatarDelegate->profilePictureUpdated(previousAvatar);
    LPTActivityIndicator::stopAnimating(this);
}
void LPTAvatarSelectionLayer::serverResponseCompletedWithData(picojson::array data) {
    
}
