//
//  BaseLayer.cpp
//  Max_Damage
//
//  Created by Davinder on 19/05/17.
//
//

#include "BaseLayer.hpp"
#include "LPTLoginScene.hpp"

bool BaseLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    winSize = Director::getInstance()->getWinSize();
    origin = Director::getInstance()->getVisibleOrigin();
    APP_MANAGER->checkInternetConnection();
    return true;
}

void BaseLayer::onEnter()
{
    Layer::onEnter();
    APP_MANAGER->checkInternetConnection();
    this->updateGameDataSyncStatus();
}

#pragma mark UPDATE_DATA_SYNC
void BaseLayer::updateGameDataSyncStatus() {
    if(!APP_MANAGER->isInternetAvilable && !APP_MANAGER->getDataSyncStatus()){
        APP_MANAGER->setDataSyncStatus(true);
    }
}
#pragma mark UPDATE_DATA_SYNC_ENDED
void BaseLayer::onExit()
{
    Layer::onExit();
}

void BaseLayer::setBackgroundSprite(string bgSprite){
    Sprite *bg = Sprite::create(bgSprite);
    bg->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    bg->setLocalZOrder(-1000);
    bg->setOpacity(255);
    this->addChild(bg);
}

void BaseLayer :: menuCallBack(Ref* pSender)
{
    log("Base Layer Called");
}

MenuItemSprite* BaseLayer :: createMenuItemSprite(BaseSprite *spriteNormal, BaseSprite *spriteSelected, int tag, Vec2 pos, Vec2 offset, const ccMenuCallback& callback,bool isAnimated, BaseSprite *child){
    
    auto selectNormal = spriteNormal;
    auto   selectSelected = spriteSelected;
    if(isAnimated)
    {
        selectSelected->setScale(0.98);
        selectSelected->setOpacity(200);
    }
    MenuItemSprite *menuItem = MenuItemSprite::create(selectNormal, selectSelected, callback == nullptr ? CC_CALLBACK_1(BaseLayer::menuCallBack, this): callback);
    menuItem->setTag(tag);
    menuItem->setPosition(Vec2(pos.x - (spriteNormal->getContentSize().width * offset.x), pos.y - (spriteNormal->getContentSize().height * offset.y) ));
    
    if(child != NULL){
        child->setPosition(menuItem->getContentSize().width/2, menuItem->getContentSize().height/2);
        menuItem->addChild(child);
    }
    
    return menuItem;
    
}

MenuItemSprite* BaseLayer :: createMenuItemSpriteWithLabel(BaseSprite* spriteNormal, BaseSprite* spriteSelected, string text,Color3B textColor, string font, Vec2 pos, int tag, const ccMenuCallback& callback){
    
    auto iconNormal = spriteNormal;
    auto iconSelected = spriteSelected;
    MenuItemSprite *menuItemSprite = MenuItemSprite::create(iconNormal, iconSelected, callback);
    menuItemSprite->setPosition(pos);
    menuItemSprite->setTag(tag);
    
    //normal label
    auto labelNormal = Label::createWithBMFont(font, text.c_str());
    labelNormal->setPosition(Vec2(menuItemSprite->getContentSize().width/2, menuItemSprite->getContentSize().height * 0.4f));
    labelNormal->setColor(textColor);
    if(labelNormal->getContentSize().width > iconNormal->getContentSize().width*0.95)
    {
        labelNormal->setScale(iconNormal->getContentSize().width*0.95/labelNormal->getContentSize().width);
    }
    menuItemSprite->addChild(labelNormal);
    return menuItemSprite;
}

Label* BaseLayer :: createTTFLabel(string text, Vec2 pos, Vec2 anchorPoint, Node *parent, Color3B color, string font, int fontSize, int tag){
    Label *label = Label::createWithTTF(text.c_str(), font.c_str(), fontSize);
    label->setAnchorPoint(anchorPoint);
    label->setPosition(pos);
    label->setColor(color);
    label->setTag(tag);
    parent->addChild(label);
    return label;
}

MenuItemLabel* BaseLayer::createMenuItemLabel_TTF(string font, Color3B color, string labelString,const ccMenuCallback& callback, int tag, float fontSize){
    Label *label = Label::createWithTTF(labelString.c_str(), font.c_str(), fontSize);
    label->setColor(color);

    MenuItemLabel *menuItemLabel = MenuItemLabel::create(label, callback);
    menuItemLabel->setTag(tag);
    return menuItemLabel;
}

Label* BaseLayer ::createBMFLabel(string text, Vec2 pos, Vec2 anchorPoint, Color3B color, string font, int fontSize , int tag ){
    Label *label = Label::createWithBMFont(font, text);
    label->setPosition(pos);
    label->setAnchorPoint(anchorPoint);
    label->setTag(tag);
    label->setColor(color);
    return label;
}

MenuItemLabel* BaseLayer::createMenuItemLabel_BMF(string fontFile, string labelString,const ccMenuCallback& callback, int tag, float fontSize){
    Label *label = Label::createWithBMFont(fontFile, labelString);
    label->setBMFontSize(fontSize);
    
    MenuItemLabel *menuItemLabel = MenuItemLabel::create(label, callback);
    menuItemLabel->setTag(tag);
    return menuItemLabel;
}

BaseSprite* BaseLayer::createSprite(string sprite, int tag, Vec2 pos, Node *parent,int zOrder){
    auto mSprite = BaseSprite::createWithFileName(sprite);
    mSprite->setPosition(pos);
    mSprite->setTag(tag);
    parent->addChild(mSprite);
    return mSprite;
}

Button* BaseLayer::createButton(string normalImg,Vec2 pos,int tag){
    Button *button = Button::create(normalImg);
    button->setPosition(pos);
    button->setTag(tag);
    return button;
}

Button* BaseLayer::createButton(string fileName, int tag, Vec2 pos, Node *parent, std::function<void(Ref*, Widget::TouchEventType)> callback, float scale){
    Button *btn = Button::create(fileName);
    btn->setPosition(pos);
    btn->setTag(tag);
    btn->setScale(scale);
    parent->addChild(btn);
    btn->addTouchEventListener(callback);
    return btn;
}

void BaseLayer::createButtonWithText(string text, int tag, Vec2 pos, Node *parent, std::function<void(Ref*, Widget::TouchEventType)> callback){
    auto btn = Button::create();
    btn->setTitleText(text);
    btn->setTitleFontSize(35);
    btn->setPosition(pos);
    btn->setTag(tag);
    btn->setContentSize(Size(500,500));
    parent->addChild(btn);
    btn->addTouchEventListener(callback);
}


Sprite* BaseLayer::createEditBox(string textFieldBGSpriteName, Vec2 pos, Vec2 offSet, string placeHolder, int tag, Node *parent, EditBox::InputFlag inputFlag, EditBox::InputMode inputMode, Node *delegate, float scaleFactor){
    Sprite *spriteTextFeild = Sprite::create(textFieldBGSpriteName);
    spriteTextFeild->setPosition(pos.x - offSet.x, pos.y - offSet.y);
    spriteTextFeild->setTag(tag);
    spriteTextFeild->setScale(scaleFactor);
    spriteTextFeild->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    parent->addChild(spriteTextFeild);
    
    auto placeHolderLabel = Label::createWithBMFont(kArialFont, placeHolder);
    placeHolderLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    placeHolderLabel->setScale(1.0);
    placeHolderLabel->setPosition(spriteTextFeild->getContentSize().width*0.025, spriteTextFeild->getContentSize().height * 1.05);
    spriteTextFeild->addChild(placeHolderLabel);
    
    Scale9Sprite *editBoxSprite = Scale9Sprite::create(kTextFieldTextComponent);
    editBoxSprite->setScale(spriteTextFeild->getContentSize().width * 0.5 / editBoxSprite->getContentSize().width);
    
    EditBox *_editName;
    _editName = EditBox::create(Size(spriteTextFeild->getContentSize().width * 0.9, spriteTextFeild->getContentSize().height * 0.8), editBoxSprite);
    _editName->setPosition(Vec2(spriteTextFeild->getContentSize().width/2, spriteTextFeild->getContentSize().height * 0.5));
    _editName->setFontName(kArialFont);
    _editName->setFontSize(35);
    _editName->setFontColor(kFontColorWhite);
    _editName->setPlaceHolder(" ");
    _editName->setMaxLength(60);
    _editName->setInputFlag(inputFlag);
    _editName->setName(textFieldBGSpriteName);
    _editName->setTag(spriteTextFeild->getTag());
    _editName->setInputMode(inputMode);
    
    _editName->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
    _editName->setTag(spriteTextFeild->getTag());
    spriteTextFeild->addChild(_editName);
    
    return spriteTextFeild;
}

#pragma mark MOVE_TO_LOGIN
void BaseLayer::moveToLoginScene(LOGOUT_TYPE logoutType) {
    SoundManager::playSound(SCENE_CHANGE_SOUND);
    Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, LPTLoginScene::createScene(logoutType)));
}
