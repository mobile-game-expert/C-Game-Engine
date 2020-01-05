//
//  LPTTutorialLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 11/10/18.
//

#ifndef LPTTutorialLayer_hpp
#define LPTTutorialLayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class LPTTutorialLayer : public BaseLayer {
public:
    CREATE_FUNC(LPTTutorialLayer);
    static LPTTutorialLayer* createLayer(int backButtonTag);
    virtual bool initWithTag(int backButtonTag);
    virtual void onEnter();
    virtual void onExit();
private:
    int backButtonTag = 0;
    BaseSprite* tutorialSprite;
    PageView* pageView;
    vector<string> arrayInstruction;
    void setupTutorialData();
    void setupUI();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void addSelectedIndicator();
    void changeSelectedStatusOfIndicator(int currentSelected);
    void addPageViewForTutorial(Vec2 position, Size pageViewSize);
    //Temp

};
#endif /* LPTTutorialLayer_hpp */
