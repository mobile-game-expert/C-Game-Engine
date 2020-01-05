//
//  LPTSearchFriendCell.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 09/10/18.
//

#include "LPTSearchFriendCell.hpp"

#define kBaseBoxImageName               "friendBox.png"
#define kOnlineStatusImageName          "online.png"
#define kOfflineStatusImageName         "offline.png"
#define kChallengeImageName             "challenge.png"
#define kChallengeSentImageName         "challengeAccept.png"

#define kChallengeText                  "Challenge"
#define kChallengeSentText              "Challenge Sent"
#define kOnlineText                     "Online"
#define kOfflineText                    "Offline"

#define CHALLENGE_BASE_TAG              7200
#define BASE_BOX_TAG                    7201

bool LPTSearchFriendCell::initWithFriendData(FriendData cellData, Size cellSize, int tag) {
    if (!TableViewCell::init())
    {
        return false;
    }
    setupCell(cellData, cellSize, tag);
    return true;
}

void LPTSearchFriendCell::setCellDelegate(LPTSearchFriendCellDelegate *delegate) {
    this->delegate = delegate;
}

void LPTSearchFriendCell::setupCell(FriendData cellData, Size cellSize, int tag) {
    BaseSprite *baseBoxSprite = BaseSprite::createWithFileName(kBaseBoxImageName);
    baseBoxSprite->setPosition(Vec2(cellSize.width*0.5, cellSize.height*0.5));
    baseBoxSprite->setScale(cellSize.width*0.98 / baseBoxSprite->getContentSize().width);
    baseBoxSprite->setTag(BASE_BOX_TAG);
    this->addChild(baseBoxSprite);
    
    //Challenge button
    Sprite *baseChallenge = Sprite::create(((cellData.challengeSent)? kChallengeSentImageName : kChallengeImageName));
    baseChallenge->setScale(baseBoxSprite->getContentSize().width *0.9/ baseChallenge->getContentSize().width);
    baseChallenge->setPosition(Vec2(baseBoxSprite->getContentSize().width*0.5, baseChallenge->getContentSize().height*0.85));
    baseChallenge->setTag(CHALLENGE_BASE_TAG);
    baseBoxSprite->addChild(baseChallenge);
    
    Button *btnChallenge = Button::create();
    btnChallenge->setTitleText(((cellData.challengeSent)? kChallengeSentText : kChallengeText));
    btnChallenge->setTitleFontSize(48);
    btnChallenge->setTag(tag);
    btnChallenge->setTitleFontName(kOpenSans_Bold);
    btnChallenge->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnChallenge->setPosition(Vec2(baseBoxSprite->getContentSize().width*0.5, baseChallenge->getContentSize().height*0.85));
    btnChallenge->setContentSize(baseChallenge->getContentSize());
    btnChallenge->addTouchEventListener(CC_CALLBACK_2(LPTSearchFriendCell::buttonCallback, this));
    baseBoxSprite->addChild(btnChallenge);
    
    //Profile pic
    BaseSprite *profile = BaseSprite::createWithFileName(cellData.profilePic);
    auto scaleFactor = GameUtils::isIpad ? 0.45 : (GameUtils::isIphoneX ? 0.3:  0.6);
    profile->setScale(baseBoxSprite->getContentSize().width * baseBoxSprite->getScale() * scaleFactor/ profile->getContentSize().width);
    profile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    profile->setPosition(Vec2(profile->getContentSize().width * profile->getScale() *0.7 , baseBoxSprite->getContentSize().height - profile->getContentSize().height * profile->getScale() * 0.7));
    baseBoxSprite->addChild(profile);
    
    //Unique name
    auto fontSize = GameUtils::isIpad ? 38 : 38;
    Label *lblUniqueName = Label::createWithTTF(cellData.userName , kOpenSans_Bold , fontSize);
    lblUniqueName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lblUniqueName->setPosition(Vec2(profile->getPositionX() + profile->getContentSize().width *0.85, profile->getPositionY() + profile->getContentSize().height *0.475));//baseBoxSprite->getContentSize().height*0.875
    lblUniqueName->setColor(Color3B::WHITE);
    baseBoxSprite->addChild(lblUniqueName);
    
    //Coins
    BaseSprite *coinSprite = BaseSprite::createWithFileName(kGameSelectionTopBarCoinImageName);
    coinSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    coinSprite->setScale(lblUniqueName->getContentSize().width*0.1/ coinSprite->getContentSize().width);
    coinSprite->setPosition(Vec2(lblUniqueName->getPositionX(), lblUniqueName->getPositionY() - lblUniqueName->getContentSize().height *1.05));
    baseBoxSprite->addChild(coinSprite);
    
    Label *lblCoins = Label::createWithTTF(StringUtils::format("%.0f", cellData.coins), kOpenSans_Bold , 32);
    lblCoins->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lblCoins->setPosition(Vec2(coinSprite->getPositionX() + coinSprite->getContentSize().width * coinSprite->getScale() * 1.1, coinSprite->getPositionY()));
    lblCoins->setColor(Color3B::WHITE);
    baseBoxSprite->addChild(lblCoins);
    
    //Status
    BaseSprite *statusSprite = BaseSprite::createWithFileName(((cellData.status)? kOnlineStatusImageName : kOfflineStatusImageName));
    statusSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    statusSprite->setScale(lblUniqueName->getContentSize().width*0.1/ statusSprite->getContentSize().width);
    statusSprite->setPosition(Vec2(lblUniqueName->getPositionX(), lblCoins->getPositionY() - lblUniqueName->getContentSize().height *1.05));
    baseBoxSprite->addChild(statusSprite);
    
    Label *lblStatus = Label::createWithTTF(((cellData.status)? kOnlineText : kOfflineText), kOpenSans_Bold , 28);
    lblStatus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lblStatus->setPosition(Vec2(statusSprite->getPositionX() + statusSprite->getContentSize().width * statusSprite->getScale()  * 1.1, statusSprite->getPositionY()));
    lblStatus->setColor(Color3B::WHITE);
    baseBoxSprite->addChild(lblStatus);
}

#pragma mark - Button callback
void LPTSearchFriendCell::buttonCallback(Ref* sender, Widget::TouchEventType type){
    Button *button = (Button*)sender;
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::CANCELED:
            break;
        case Widget::TouchEventType::ENDED:
        {
            SoundManager::playSound(BUTTON_TAP_SOUND);
            auto alreadyChallenged = false;
            if(button->getTitleText().compare(kChallengeText)) {
                alreadyChallenged = true;
            }else {
                Sprite *baseBox = (Sprite*)this->getChildByTag(BASE_BOX_TAG);
                Sprite *btnBase = (Sprite*)baseBox->getChildByTag(CHALLENGE_BASE_TAG);
                btnBase->setTexture(kChallengeSentImageName);
                button->setTitleText(kChallengeSentText);
                alreadyChallenged = false;
            }
            if (this->delegate != NULL) {
                this->delegate->challengeButtonPressed(button, alreadyChallenged);
            }
            
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
}
