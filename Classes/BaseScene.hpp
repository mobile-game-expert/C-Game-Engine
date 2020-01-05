//
//  BaseScene.hpp
//  Max_Damage
//
//  Created by Davinder on 19/05/17.
//
//

#ifndef BaseScene_hpp
#define BaseScene_hpp

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

class BaseScene : public Scene
{
public:
    /*Variables*/
    Size winSize;
    /*Methods*/
//    static BaseScene* create();
    CREATE_FUNC(BaseScene);
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    void setBackgroundSprite(string sprite);
private:
protected:
};
#endif /* BaseScene_hpp */
