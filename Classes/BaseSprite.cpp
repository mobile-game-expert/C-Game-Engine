//
//  BaseSprite.cpp
//  Max_Damage
//
//  Created by Davinder on 19/05/17.
//
//

#include "BaseSprite.hpp"

BaseSprite* BaseSprite::createWithFileName(string fileName)
{
    BaseSprite *sprite = new (std::nothrow) BaseSprite();
    sprite->fileName = fileName;
    if (sprite && sprite->initWithFile(fileName))
    {
        sprite->autorelease();
        return sprite;
    }
    CCLOG("Fatal : %s",fileName.c_str());
    sprite->initWithFile(kPlayerProfileImageName);
    sprite->autorelease();
    return sprite;
}
void BaseSprite::setFileName(string fileName){
    this->fileName = fileName;
}
string BaseSprite::getFileName()
{
    return this->fileName;
}

void BaseSprite::playAnimationForSprite(Sprite *sprite,std::string plist,std::string frameName , int frameCount){
    
    Vector<SpriteFrame*> frames;
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile(plist);
    
    for (int i = 0; i < frameCount; i++)
    {
        std::string num = StringUtils::format("%d", i);
        frames.pushBack(cache->getSpriteFrameByName(frameName + num + ".png"));
    }
    Animation* theAnimation;
    theAnimation = Animation::createWithSpriteFrames(frames, 0.02f);
    theAnimation->retain();
    auto theAnimate = Animate::create(theAnimation);
    
    sprite->runAction(Repeat::create(theAnimate,1));
}

#pragma mark SettingType
/**
 Type will be used in Collision detection
 */
void BaseSprite::setType(int objectType)
{
    userDataType = objectType;
}
int BaseSprite::getType()
{
    return userDataType;
}

/**
 readyForDeletion will be used for deleting the object along with physics body.
 */
void BaseSprite::setReadyForDeletion(bool status)
{
    readyForDeletion = status;
}
bool BaseSprite::getReadyForDeletion()
{
    return readyForDeletion;
}
