//
//  BaseSprite.hpp
//  Max_Damage
//
//  Created by Davinder on 19/05/17.
//
//

#ifndef BaseSprite_hpp
#define BaseSprite_hpp

#include <stdio.h>
#include <cocos2d.h>
#include <cocos-ext.h>
#include "LPTAppManager.hpp"
#include "GameUtils.hpp"
#include "SoundManager.h"
#include "GameConstants.h"

using namespace std;
using namespace cocos2d;
using namespace utils;
using namespace ui;

class BaseSprite : public Sprite
{
private:
    string fileName;
    bool readyForDeletion = false;
public:
    int userDataType;
    static BaseSprite* createWithFileName(string fileName);
    string getFileName();
    void setFileName(string fileName);
    void playAnimationForSprite(Sprite *sprite,std::string plist,std::string frameName , int frameCount);
    
    //PysicsRelated
    void setType(int objectType);
    int getType();
    bool getReadyForDeletion();
    void setReadyForDeletion(bool status);
};
#endif /* BaseSprite_hpp */
