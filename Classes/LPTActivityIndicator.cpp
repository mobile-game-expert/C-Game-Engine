//
//  LPTActivityIndicator.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 14/09/18.
//

#include "LPTActivityIndicator.hpp"

#define TAG_ACTIVITY_INDICATOR                  999
#define RES_BACKGROUND                          "mainMenuBG.png"
#define RES_ACTIVITY_INDICATOR                  "loader.png"

void LPTActivityIndicator::startAnimating(Node *parent,string msg) {
    auto layer = LPTActivityIndicator::create();
    layer->setTag(TAG_ACTIVITY_INDICATOR);
    parent->addChild(layer,5000);
    
    auto touchRestrictButton = layer->createButton(RES_BACKGROUND, Vec2(parent->getContentSize().width * 0.5, parent->getContentSize().height * 0.5), 0);
    touchRestrictButton->setOpacity(0);
    layer->addChild(touchRestrictButton);

    auto indicator = layer->createSprite(RES_ACTIVITY_INDICATOR, 0, Vec2(layer->getContentSize().width * 0.5, layer->getContentSize().height * 0.5), layer);
    indicator->runAction(RepeatForever::create(RotateBy::create(0.3, 90)));
    
    //===Message
    auto msgLabel = layer->createBMFLabel(msg, Vec2(layer->getContentSize().width*0.5,layer->getContentSize().height*0.4),Vec2(0.5, 0.5), Color3B::WHITE, kArialFont, 50);
    layer->addChild(msgLabel);
}

void LPTActivityIndicator::stopAnimating(Node *parent) {
    auto activityIndicator = (LPTActivityIndicator*)parent->getChildByTag(TAG_ACTIVITY_INDICATOR);
    if (activityIndicator != NULL) {
        activityIndicator->removeAllChildrenWithCleanup(true);
        activityIndicator->removeFromParentAndCleanup(true);
    }
}
