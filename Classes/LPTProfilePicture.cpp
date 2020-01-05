//
//  LPTProfilePicture.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 18/09/18.
//

#include "LPTProfilePicture.hpp"
#include "LPTPlayerProfileScene.hpp"

LPTProfilePicture* LPTProfilePicture::createLayer() {
    auto layer = LPTProfilePicture::create();
    layer->init();
    return layer;
}

bool LPTProfilePicture::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}


void LPTProfilePicture::onEnter() {
    BaseLayer::onEnter();
}

void LPTProfilePicture::onExit() {
    BaseLayer::onExit();
}

ClippingNode* LPTProfilePicture::createProfilePictureView(string userImage, string stencilImage, Vec2 position, int tag, bool intractionEnable, std::function<void(Ref*, Widget::TouchEventType)> callback) {
//    LayerColor *layerColor = LayerColor::create(Color4B(255, 255, 255, 255),this->getContentSize().width, this->getContentSize().height);
//    this->addChild(layerColor);
    
    auto stencil = Sprite::create(stencilImage); //mask image
    stencil->setAnchorPoint(Vec2(0.5, 0.5));
    stencil->setContentSize(this->getContentSize());
    
    auto clipper = ClippingNode::create();
    clipper->setStencil(stencil);
    clipper->setAlphaThreshold(0.5f);
    clipper->setTag(tag);
    
    if(userImage == "")
        userImage = "avatar_1.png";
    auto content = Sprite::create(userImage); // profile image
    
    content->setTag(tag);
    content->setAnchorPoint(Vec2(0.5,0.5));
    content->setContentSize(this->getContentSize());
    clipper->addChild(content);
    clipper->setPosition(position);

    // For user intraction
    auto btnProfile = Button::create(stencilImage); //mask image
    btnProfile->setContentSize(clipper->getContentSize());
    btnProfile->setPosition(Vec2(clipper->getContentSize().width*0.5, clipper->getContentSize().height*0.5));
    int tagButton = tag == kUserProfilePicTag ? kUserProfilePicButtonTag : kOpponentProfilePicButtonTag;
    btnProfile->setTag(tagButton);
    btnProfile->setOpacity(0);
    btnProfile->setVisible(intractionEnable);
    btnProfile->addTouchEventListener(callback);
    clipper->addChild(btnProfile,10);
    
    this->addChild(clipper, 10);
    return clipper;
}

void LPTProfilePicture::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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
                default:
                    LPTPlayerProfileScene *scene = LPTPlayerProfileScene::createScene();
                    SoundManager::playSound(SCENE_CHANGE_SOUND);
                    Director::getInstance()->replaceScene(scene);
                    break;
            }
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
}

void LPTProfilePicture::updatePicture(string image, int tag) {
    
    ClippingNode *profilePic = (ClippingNode*)this->getChildByTag(tag);
    Sprite *contentSprite = (Sprite *)profilePic->getChildByTag(tag);
    contentSprite->removeFromParentAndCleanup(true);
    auto content = Sprite::create(image); // profile image
    content->setAnchorPoint(Vec2(0.5,0.5));
    content->setTag(tag);
    content->setContentSize(this->getContentSize());
    profilePic->addChild(content);
}

void LPTProfilePicture::updatePictureWithSpriteFrame(SpriteFrame *image, int tag) {
    ClippingNode *profilePic = (ClippingNode*)this->getChildByTag(tag);
    Sprite *contentSprite = (Sprite *)profilePic->getChildByTag(tag);
    contentSprite->removeFromParentAndCleanup(true);
    auto content = Sprite::createWithSpriteFrame(image); // profile image
    content->setAnchorPoint(Vec2(0.5,0.5));
    content->setTag(tag);
    content->setContentSize(this->getContentSize());
    profilePic->addChild(content);
}
