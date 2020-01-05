//
//  LPTPopUp.cpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 27/09/18.
//

#include "LPTPopUp.hpp"

#define OK_BUTTON_TAG           12000
#define CANCEL_BUTTON_TAG       12001

//#define CancelButtonBG          "cancelButtonBg.png"
//#define OKButtonBG              "okButtonBg.png"
//#define PopUpBG                 "popupBg.png"
#define CancelButtonBG          "Cancel.png"
#define OKButtonBG              "Ok.png"
#define PopUpBG                 "popup.png"

#define buttonTitleFontSize     28

LPTPopUp* LPTPopUp::showPopUpWith(string title, string msg, bool areTwoButtonPopUp , Popup_Type type,string okTitle ,string cancelTitle) {
    
    SoundManager::playSound(POPUP_SOUND);
    auto popUpLayer = LPTPopUp::create();
    popUpLayer->setTag((int)type);
    //===This button
    auto touchRestrictButton = popUpLayer->createButton(kMainMenuBG, Vec2(popUpLayer->getContentSize().width * 0.5, popUpLayer->getContentSize().height * 0.5), 0);
    touchRestrictButton->setOpacity(0);
    popUpLayer->addChild(touchRestrictButton);
    
    popUpLayer->setUpUI(popUpLayer, title, msg, areTwoButtonPopUp, okTitle, cancelTitle);
    return popUpLayer;
}

bool LPTPopUp::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    
    return true;
}
void LPTPopUp::onEnter() {
    BaseLayer::onEnter();
}
void LPTPopUp::onExit() {
    BaseLayer::onExit();
}

void LPTPopUp::setUpUI(LPTPopUp *popUp,string title, string msg, bool areTwoButtonPopUp, string okTitle ,string cancelTitle) {
    BaseSprite* bg = popUp->createSprite(PopUpBG, 0, Vec2(popUp->getContentSize().width * 0.5,popUp->getContentSize().height * 0.5), popUp);
    
    bg->setScale(popUp->getContentSize().height * 0.55 / bg->getContentSize().height );
    //---Title
    auto titlelabel = popUp->createBMFLabel(title, Vec2(bg->getContentSize().width * 0.5, bg->getContentSize().height * 0.9), Vec2::ANCHOR_MIDDLE, Color3B::WHITE, kArialFont, 40);
    bg->addChild(titlelabel);
    //---Message Body
    auto msgBody = popUp->createBMFLabel(msg, Vec2(bg->getContentSize().width * 0.5, bg->getContentSize().height * 0.6), Vec2::ANCHOR_MIDDLE, Color3B::WHITE, kArialFont, 10);
    msgBody->setScale(0.8);
    bg->addChild(msgBody);
    
    if(areTwoButtonPopUp){
        //--- OK
        auto okButton = popUp->createButton(OKButtonBG, OK_BUTTON_TAG, Vec2(bg->getContentSize().width * 0.3, bg->getContentSize().height * 0.15), bg, CC_CALLBACK_2(LPTPopUp::buttonCallback, popUp), 1.0);
        okButton->setTitleText(okTitle);
        okButton->setTitleFontSize(buttonTitleFontSize);
        
        //--- Cancel
        auto cancelButton = popUp->createButton(CancelButtonBG, CANCEL_BUTTON_TAG, Vec2(bg->getContentSize().width * 0.7, bg->getContentSize().height * 0.15), bg, CC_CALLBACK_2(LPTPopUp::buttonCallback, popUp), 1.0);
        cancelButton->setTitleText(cancelTitle);
        cancelButton->setTitleFontSize(buttonTitleFontSize);
    }else {
        //--- OK
        auto okButton = popUp->createButton(OKButtonBG, OK_BUTTON_TAG, Vec2(bg->getContentSize().width * 0.5, bg->getContentSize().height * 0.15), bg, CC_CALLBACK_2(LPTPopUp::buttonCallback, popUp), 1.0);
        okButton->setTitleText(okTitle);
        okButton->setTitleFontSize(buttonTitleFontSize);
    }
    
}

void LPTPopUp::setPopUpDelegate(LPTPopUpDelegate *delegate) {
    this->delegate = delegate;
}

#pragma mark - Button callback
/**
 buttonCallback : This methods will process button callbacks.
 - param :
 - sender : Button whose calls this method.
 - type : Type of event that has occured.
 */
void LPTPopUp::buttonCallback(Ref* sender, Widget::TouchEventType type){
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
                case OK_BUTTON_TAG:
                    if (this->delegate != NULL) {
                        this->delegate->okButtonAction((Popup_Type)this->getTag());
                    }else {
                        this->removeFromParentAndCleanup(true);
                    }
                    break;
                case CANCEL_BUTTON_TAG:
                    if (this->delegate != NULL) {
                        this->delegate->cancelButtonAction((Popup_Type)this->getTag());
                    }else {
                        this->removeFromParentAndCleanup(true);
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
