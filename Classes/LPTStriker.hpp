//
//  LPTStriker.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 19/09/18.
//

#ifndef LPTStriker_hpp
#define LPTStriker_hpp

#include <stdio.h>
#include "LPTHeaders.h"

#define TAG_STRIKER_BODY        21030

class LPTStriker : public BaseSprite {
public:
    
    bool isStrikerDragging  =   false;
    bool isStrikerTouchable =   false;
    bool isStrikerHitFromCenter = false;
    
    CREATE_FUNC(LPTStriker);
    static LPTStriker *createStriker();
    virtual bool initWithSpriteName(string strikerImageName);
    virtual void onEnter();
    virtual void onExit();
    //
    b2Body *strikerBody;
    bool playedByMe;
    b2Vec2 previousPosition;
    b2Vec2 intialPosition;
    b2Vec2 velocity;
    tm previousTime;
    Vec2 spritePreviousPosition;
    /*Position Forcasting
    vector<b2Vec2> somePreviousPositions;
    void clearSomePreviousPositions(float dt);
    bool wasOnThisPosition(b2Vec2 pos);
     */
    //Methods
    void setStrikerDragging(bool isDragging);
    void setStrikerProperty(int tag, Vec2 position, bool isTouchable, float scale);
    void updateStrikerPhysicsProperty(int categoryBit, int collisionBit, int contactBit);
    void addBodyToStriker(b2BodyType bodyType, b2World *world);
    void maintainXVelocity();
    void dumpStrikerVelocity();
    void removeBodyFromStriker();
    b2Body* getStrikerBody();
    void addLabelToStriker();
#pragma mark
    void addPointingHand();
    void removePointingHand();
    bool hasPointingHand();
    void forceToReachSpecificPosition(b2Vec2 pos,float speed, b2Body *body);
    string strikerState(string user,int index, b2Vec2 position, b2Vec2 linerVelocity, float angularVelocity,long time);
private:
protected:
};


#endif /* LPTStriker_hpp */
