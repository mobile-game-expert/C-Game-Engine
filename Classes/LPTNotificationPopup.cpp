//
//  LPTNotificationPopup.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 15/10/18.
//

#include "LPTNotificationPopup.hpp"
#include "LPTProfilePicture.hpp"
#include "LPTPlayerMatchingScene.hpp"

#define PROFILE_IMAGE_TAG               10001
#define PLAY_BUTTON_SENT_TAG            10002
#define CLOSE_BUTTON_TAG                10003
#define COINS_IMAGE_TAG                 10004
#define REJECT_BUTTON_SENT_TAG          10005
#define PLAY_BUTTON_ACCEPT_TAG          10006
#define REJECT_BUTTON_ACCEPT_TAG        10007

#define kCloseImageName                 "close.png"

#define kChallengeText                  "has challenged \nyou for "
#define kAcceptChallengeText            "has accepted\nyour challenge."
#define kRejectChallengeText            "Sorry, can't play right now."

LPTNotificationPopup* LPTNotificationPopup::createLayer(LPTOpponent *opponent, string notificationType) {
    auto layer = LPTNotificationPopup::create();
    layer->initWithOpponent(opponent, notificationType);
    return layer;
}

bool LPTNotificationPopup::initWithOpponent(LPTOpponent *opponent, string notificationType) {
    if (!BaseLayer::init()) {
        return false;
    }
    this->opponent = opponent;
    this->notificationType = notificationType;
    Color4B notificationColor = notificationType.compare(kRejectChallenge) == 0 ? Color4B(0, 0, 0, 255) : Color4B(26, 57, 5, 180);
    float scalefactor = GameUtils::isIpad ? 0.15 : 0.2;
    baseColorLayer = LayerColor::create(notificationColor, this->getContentSize().width*0.7, this->getContentSize().height*scalefactor);
    this->addChild(baseColorLayer);
    return true;
}

void LPTNotificationPopup::onEnter() {
    BaseLayer::onEnter();
    if(notificationType.compare(kSendChallenge) == 0 || notificationType.compare(kAcceptChallenge) == 0)
        setupAcceptChallengeUI();
    else if(notificationType.compare(kRejectChallenge) == 0)
        setupRejectChallengeUI();
}

void LPTNotificationPopup::onExit() {
    BaseLayer::onExit();
}

#pragma mark - Setup UI
void LPTNotificationPopup::setupChallengeUI() {
    
    //Close Button
    auto closeButton = createButton(kCloseImageName, CLOSE_BUTTON_TAG, Vec2(baseColorLayer->getContentSize().width * 0.05, baseColorLayer->getContentSize().height * 0.5), baseColorLayer, CC_CALLBACK_2(LPTNotificationPopup::buttonCallback, this), 1);
    closeButton->setScale(baseColorLayer->getContentSize().height * 0.5 / closeButton->getContentSize().height);
//    closeButton->setTitleText(kPlayText);
    closeButton->setTitleFontName(kOpenSans_Bold);
    closeButton->setTitleFontSize(36);
    
    //Profile Image
    LayerColor *layer = LayerColor::create(Color4B(255, 250, 20, 0), baseColorLayer->getContentSize().height * 0.9, baseColorLayer->getContentSize().height * 0.9);
    layer->setPosition( Vec2(closeButton->getPositionX() + closeButton->getContentSize().width * 0.25, baseColorLayer->getContentSize().height * 0.05));
    baseColorLayer->addChild(layer);

    LPTProfilePicture *profilePicture = new LPTProfilePicture();
    profilePicture->setContentSize(layer->getContentSize());
    profilePicture->setPosition(Vec2(0,0));
    profilePicture->createProfilePictureView(opponent->getProfilePic(), kPlayerProfileImageName, Vec2(profilePicture->getContentSize().width * 0.5, profilePicture->getContentSize().height * 0.5),PROFILE_IMAGE_TAG, false);
    layer->addChild(profilePicture, 10);
    
    //Challenge Text
    auto coinLbl = createTTFLabel(StringUtils::format("%s %s %.0f coins bet.",opponent->getUniqueName().c_str(), kChallengeText , GameUtils::getUserBetValueFromSelectedBet(opponent->getBetSelected())) , Vec2(layer->getPositionX() + layer->getContentSize().width * layer->getScale() * 1.1, baseColorLayer->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE_LEFT, baseColorLayer, Color3B::WHITE, kOpenSans_Bold, 22);
    
//    auto coins = createSprite(kBetCoinImageName, COINS_IMAGE_TAG, Vec2(coinLbl->getPositionX() + coinLbl->getContentSize().width * 1.05, coinLbl->getContentSize().height), baseColorLayer);
//    coins->setScale(coinLbl->getContentSize().height * 0.8 / coins->getContentSize().height);
    
    //Play button
    auto playButton = createButton(kTutorialPlayButtonImageName, PLAY_BUTTON_SENT_TAG, Vec2(baseColorLayer->getContentSize().width * 0.9, baseColorLayer->getContentSize().height * 0.5), baseColorLayer, CC_CALLBACK_2(LPTNotificationPopup::buttonCallback, this), 1);
    playButton->setScale(baseColorLayer->getContentSize().height * 0.75 / playButton->getContentSize().height);
    playButton->setTitleText(kPlayText);
    playButton->setTitleFontName(kOpenSans_Bold);
    playButton->setTitleFontSize(36);
}

void LPTNotificationPopup::setupAcceptChallengeUI() {
    
    //Close Button
//    auto closeButton = createButton(kCloseImageName, CLOSE_BUTTON_TAG, Vec2(baseColorLayer->getContentSize().width * 0.05, baseColorLayer->getContentSize().height * 0.5), baseColorLayer, CC_CALLBACK_2(LPTNotificationPopup::buttonCallback, this), 1);
//    closeButton->setScale(baseColorLayer->getContentSize().height * 0.5 / closeButton->getContentSize().height);
//    closeButton->setTitleFontName(kOpenSans_Bold);
//    closeButton->setTitleFontSize(36);
    
    //Profile Image
    LayerColor *layer = LayerColor::create(Color4B(255, 250, 20, 0), baseColorLayer->getContentSize().height * 0.9, baseColorLayer->getContentSize().height * 0.9);
    layer->setPosition( Vec2(baseColorLayer->getContentSize().width * 0.05, baseColorLayer->getContentSize().height * 0.05));
    baseColorLayer->addChild(layer);
    
    LPTProfilePicture *profilePicture = new LPTProfilePicture();
    profilePicture->setContentSize(layer->getContentSize());
    profilePicture->setPosition(Vec2(0,0));
    profilePicture->createProfilePictureView(opponent->getProfilePic(), kPlayerProfileImageName, Vec2(profilePicture->getContentSize().width * 0.5, profilePicture->getContentSize().height * 0.5),PROFILE_IMAGE_TAG, false);
    layer->addChild(profilePicture, 10);
    
    //Challenge Text
    log("%d",opponent->getBetSelected());
    string challengeText = notificationType.compare(kSendChallenge) == 0 ? StringUtils::format("%s %s %.0f coins bet.",opponent->getUniqueName().c_str(), kChallengeText , GameUtils::getUserBetValueFromSelectedBet(opponent->getBetSelected())) : StringUtils::format("%s %s",opponent->getUniqueName().c_str(), kAcceptChallengeText);
    int fontSize = GameUtils::isIpad ? 20 : (GameUtils::isIphoneX ? 44 : 22);
    auto coinLbl = createTTFLabel( challengeText, Vec2(layer->getPositionX() + layer->getContentSize().width * layer->getScale() * 1.1, baseColorLayer->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE_LEFT, baseColorLayer, Color3B::WHITE, kOpenSans_Bold, fontSize);
    
    //    auto coins = createSprite(kBetCoinImageName, COINS_IMAGE_TAG, Vec2(coinLbl->getPositionX() + coinLbl->getContentSize().width * 1.05, coinLbl->getContentSize().height), baseColorLayer);
    //    coins->setScale(coinLbl->getContentSize().height * 0.8 / coins->getContentSize().height);
    
    //Play button
    auto playButton = createButton(kTutorialPlayButtonImageName, Vec2(baseColorLayer->getContentSize().width * 0.975, baseColorLayer->getContentSize().height * 0.5));
    playButton->setTag(notificationType.compare(kSendChallenge) == 0 ? PLAY_BUTTON_SENT_TAG : PLAY_BUTTON_ACCEPT_TAG);
    playButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    playButton->addTouchEventListener(CC_CALLBACK_2(LPTNotificationPopup::buttonCallback, this));
    playButton->setScale(baseColorLayer->getContentSize().height * 0.7 / playButton->getContentSize().height);
    playButton->setTitleText(kPlayText);
    playButton->setTitleFontName(kOpenSans_Bold);
    playButton->setTitleFontSize(36);
    baseColorLayer->addChild(playButton);
    
    //Reject button
    float horizontalSpacingFactor = GameUtils::isIpad ? 0.7 : (GameUtils::isIphoneX ? 1.5 : 0.85);
    auto rejectButton = createButton(kCancelImageName, (notificationType.compare(kSendChallenge) == 0 ? REJECT_BUTTON_SENT_TAG : REJECT_BUTTON_ACCEPT_TAG), Vec2(playButton->getPositionX() - playButton->getContentSize().width * horizontalSpacingFactor, baseColorLayer->getContentSize().height * 0.5), baseColorLayer, CC_CALLBACK_2(LPTNotificationPopup::buttonCallback, this), 1);
    rejectButton->setScale(playButton->getContentSize().height * 0.8 * playButton->getScale() / rejectButton->getContentSize().height);
}

void LPTNotificationPopup::setupRejectChallengeUI() {
    
    //Close Button
    auto closeButton = createButton(kCancelImageName, CLOSE_BUTTON_TAG, Vec2(baseColorLayer->getContentSize().width * 0.05, baseColorLayer->getContentSize().height * 0.5), baseColorLayer, CC_CALLBACK_2(LPTNotificationPopup::buttonCallback, this), 1);
    closeButton->setScale(baseColorLayer->getContentSize().height * 0.5 / closeButton->getContentSize().height);
    //    closeButton->setTitleText(kPlayText);
    closeButton->setTitleFontName(kOpenSans_Bold);
    closeButton->setTitleFontSize(36);
    
    //Profile Image
    LayerColor *layer = LayerColor::create(Color4B(255, 250, 20, 0), baseColorLayer->getContentSize().height * 0.9, baseColorLayer->getContentSize().height * 0.9);
    layer->setPosition( Vec2(closeButton->getPositionX() + closeButton->getContentSize().width * 0.75, baseColorLayer->getContentSize().height * 0.05));
    baseColorLayer->addChild(layer);
    
    LPTProfilePicture *profilePicture = new LPTProfilePicture();
    profilePicture->setContentSize(layer->getContentSize());
    profilePicture->setPosition(Vec2(0,0));
    profilePicture->createProfilePictureView(opponent->getProfilePic(), kPlayerProfileImageName, Vec2(profilePicture->getContentSize().width * 0.5, profilePicture->getContentSize().height * 0.5),PROFILE_IMAGE_TAG, false);
    layer->addChild(profilePicture, 10);
    
    //Challenge Text
    auto coinLbl = createTTFLabel(StringUtils::format("%s",kRejectChallengeText) , Vec2(layer->getPositionX() + layer->getContentSize().width * layer->getScale() * 1.1, baseColorLayer->getContentSize().height * 0.5), Vec2::ANCHOR_MIDDLE_LEFT, baseColorLayer, Color3B::WHITE, kOpenSans_Bold, 22);

}

#pragma mark - Button callback
void LPTNotificationPopup::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                case PLAY_BUTTON_SENT_TAG:
                case PLAY_BUTTON_ACCEPT_TAG:
                    this->removeFromParentAndCleanup(true);
                    updateOpponentObject();
                    PLAYER->setBetSelected(opponent->getBetSelected());
                    APP_MANAGER->isPlayingWithFriend = true;
                    if(button->getTag() == PLAY_BUTTON_SENT_TAG) {
                        PLAYER->isAdmin = true;
                        PLAYER->roomName = PLAYER->getPlayerUniqueName();
                        PLAYER->roompassword = PLAYER->getPlayerUniqueName();
                        APP_MANAGER->acceptChallangeNotification(opponent->getUniqueName());
                    } else {
                        PLAYER->isAdmin = false;
                        PLAYER->roomName = opponent->getUniqueName();
                        PLAYER->roompassword = opponent->getUniqueName();
                    }
                    Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0, LPTPlayerMatchingScene::createScene()));
                    break;
                case CLOSE_BUTTON_TAG:
                    this->removeFromParentAndCleanup(true);
                    break;
                case REJECT_BUTTON_SENT_TAG:
                case REJECT_BUTTON_ACCEPT_TAG:
                    APP_MANAGER->rejectChallangeNotification(opponent->getUniqueName());
                    this->removeFromParentAndCleanup(true);
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

#pragma mark - Update Opponent shared instance
void LPTNotificationPopup::updateOpponentObject() {
    OPPONENT->setUniqueName(opponent->getUniqueName());
    OPPONENT->setCoins(opponent->getCoins());
    OPPONENT->setProfilePic(opponent->getProfilePic());
    OPPONENT->setTotalLose(opponent->getTotalLose());
    OPPONENT->setTotalWins(opponent->getTotalWins());
    OPPONENT->setBetSelected(opponent->getBetSelected());
}
