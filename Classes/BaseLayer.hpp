//
//  BaseLayer.hpp
//  Max_Damage
//
//  Created by Davinder on 19/05/17.
//
//

#ifndef BaseLayer_hpp
#define BaseLayer_hpp

#include <stdio.h>
#include <cocos2d.h>
#include <cocos-ext.h>

#include "BaseSprite.hpp"
#include "LPTAppManager.hpp"
#include "GameUtils.hpp"
#include "SoundManager.h"
#include "GameConstants.h"


using namespace std;
using namespace cocos2d;
using namespace utils;
using namespace ui;

class BaseLayer : public Layer
{
public:
    /*Properties*/
    Size winSize;
    Vec2 origin;
    /*Methods*/
    CREATE_FUNC(BaseLayer);
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual void menuCallBack(Ref* pSender);
    void setBackgroundSprite(string sprite);
    //*************  Use This method to create Menu item  **************/
    MenuItemSprite* createMenuItemSprite(BaseSprite *spriteNormal, BaseSprite *spriteSelected, int tag, Vec2 pos, Vec2 offset, const ccMenuCallback& callback = nullptr,bool isAnimated = false, BaseSprite *child = NULL);
    //*************  Use This method to create Menu item with label  **************/
    MenuItemSprite* createMenuItemSpriteWithLabel(BaseSprite* spriteNormal, BaseSprite* spriteSelected, string text,Color3B textColor, string font, Vec2 pos, int tag, const ccMenuCallback& callback);
    //*************  Use This method to create a simple Sprite  **************/
    BaseSprite* createSprite(string sprite, int tag, Vec2 pos, Node *parent,int zOrder = 0);
    //*************  Use This method to create Labek with TTF font  **************/
    Label* createTTFLabel(string text, Vec2 pos, Vec2 anchorPoint, Node *parent, Color3B color, string font, int fontSize, int tag = 1111);
    //*************  Use This method to create MenuItemLabel with TTF  **************/
    MenuItemLabel* createMenuItemLabel_TTF(string font, Color3B color, string labelString,const ccMenuCallback& callback, int tag, float fontSize);
    //*************  Use This method to create Label with BMF  **************/
    Label* createBMFLabel(string text, Vec2 pos, Vec2 anchorPoint, Color3B color, string font, int fontSize, int tag = 1111);
    //*************  Use This method to create MenuItemLabel with BMF  **************/
    MenuItemLabel* createMenuItemLabel_BMF(string fontFile, string labelString,const ccMenuCallback& callback, int tag, float fontSize);
    //************* Use This method to create UIButton ******************/
    Button* createButton(string normalImg,Vec2 pos,int tag = 6000);
    Button* createButton(string fileName, int tag, Vec2 pos, Node *parent, std::function<void(Ref*, Widget::TouchEventType)> callback, float scale);
    void createButtonWithText(string text, int tag, Vec2 pos, Node *parent, std::function<void(Ref*, Widget::TouchEventType)> callback);
    //************* Create EditBox ******************/
    Sprite* createEditBox(string textFieldBGSpriteName, Vec2 pos, Vec2 offSet, string placeHolder, int tag, Node *parent, EditBox::InputFlag inputFlag, EditBox::InputMode inputMode, Node *delegate, float scaleFactor = 1);
    //********MOVE TO LOGIN
    void moveToLoginScene(LOGOUT_TYPE logoutType);
    void updateGameDataSyncStatus();
private:
protected:
};

#endif /* BaseLayer_hpp */
