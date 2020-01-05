//
//  LPTStriker.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 19/09/18.
//

#include "LPTStriker.hpp"

#define kPointHandTag           78522

LPTStriker* LPTStriker::createStriker() {
    auto striker = LPTStriker::create();
    striker->initWithSpriteName(kStrikerImageName);
    return striker;
}

bool LPTStriker::initWithSpriteName(string imageName) {
    if (!BaseSprite::initWithFile(imageName)) {
        return false;
    }
//    schedule(schedule_selector(LPTStriker::clearSomePreviousPositions),2.0); //Position Forcasting
    return true;
}

void LPTStriker::onEnter() {
    BaseSprite::onEnter();
}
/* Position Forcasting
void LPTStriker::clearSomePreviousPositions(float dt){
    log("clear somePreviousPositions %lu positions",somePreviousPositions.size());
    somePreviousPositions.clear();
}

bool LPTStriker::wasOnThisPosition(b2Vec2 pos) {
    bool wasOn = false;
    if (!somePreviousPositions.empty()) {
        for (int i = 0; i < somePreviousPositions.size(); i++) {
            b2Vec2 storedPos = somePreviousPositions.at(i);
            if (storedPos == pos) {
                wasOn = true;
                break;
            }
        }
    }
    return wasOn;
}
*/
void LPTStriker::setStrikerProperty(int tag, Vec2 position, bool isTouchable, float scale) {
    this->setPosition(position);
    this->setTag(tag);
    this->setScale(scale);
    isStrikerTouchable = isTouchable;
    userDataType = kUserDataTypeStriker;
//    this->setOpacity(0);
}

void LPTStriker::setStrikerDragging(bool isDragging) {
    isStrikerDragging = isDragging;
}

void LPTStriker::addBodyToStriker(b2BodyType bodyType,b2World *world) {
    strikerBody = LPTPhysicsComponent::createBodyDefinition(bodyType, this->getPosition(), this, world);
//    b2_dynamicBody b2_staticBody
    b2CircleShape strikerBodyShape;
    strikerBodyShape.m_radius = (this->getContentSize().width * 0.45 * this->getScale()) / kPixelToMeterRatio;
    LPTFixtureData *fData = new LPTFixtureData;
    fData->tag = TAG_STRIKER_BODY;
    b2FixtureDef strikerFixtureDefinition;
    if(PLAYER->getGameSelected() == GAME_WITH_COMPUTER){
        strikerFixtureDefinition = LPTPhysicsComponent::createFixtureDefinition(&strikerBodyShape, 0.07, 0.5, 1.0, false, CategoryBit_Striker, GroupIndex_Striker);
    }else {
        strikerFixtureDefinition = LPTPhysicsComponent::createFixtureDefinition(&strikerBodyShape, 0.1, 1.0, 0.1, false, CategoryBit_Striker, GroupIndex_Striker);
    }
    strikerFixtureDefinition.userData = fData;
    strikerBody->CreateFixture(&strikerFixtureDefinition);
}

void LPTStriker::maintainXVelocity() {
    float maxVelocityInX = (GameUtils::isIphoneX? 350.0 : 75.0);
    //
    if(maxVelocityInX < getStrikerBody()->GetLinearVelocity().x && playedByMe) {
        getStrikerBody()->SetLinearVelocity(b2Vec2(maxVelocityInX, getStrikerBody()->GetLinearVelocity().y));
//        CCLOG("Value of velocityX: %f", getStrikerBody()->GetLinearVelocity().x);
    }
    if(-maxVelocityInX >  getStrikerBody()->GetLinearVelocity().x && !playedByMe) {
        getStrikerBody()->SetLinearVelocity(b2Vec2(-maxVelocityInX, getStrikerBody()->GetLinearVelocity().y));
//        CCLOG("(Opposite) Value of velocityX: %f", getStrikerBody()->GetLinearVelocity().x);
    }
}

void LPTStriker::dumpStrikerVelocity() {
    float maxStrikerSpeedBeforeDamp = 50.0;
    if(getStrikerBody()->GetLinearVelocity().x < maxStrikerSpeedBeforeDamp && playedByMe ) {
        getStrikerBody()->SetLinearDamping(3.0);
    }else if(getStrikerBody()->GetLinearVelocity().x > 0 && !playedByMe){
        getStrikerBody()->SetLinearDamping(3.0);
    }
    else {
        getStrikerBody()->SetLinearDamping(0.2);
//        log("Before Damping Value of velocityX: %f", getStrikerBody()->GetLinearVelocity().x);
    }
    getStrikerBody()->SetAngularDamping(0.7);

    //TEMP
    if(isStrikerHitFromCenter) {
        isStrikerHitFromCenter = false;
        getStrikerBody()->SetLinearVelocity(b2Vec2(getStrikerBody()->GetLinearVelocity().x, getStrikerBody()->GetLinearVelocity().y * 0.2));
    }
    
    if(getStrikerBody()->GetLinearVelocity().x > 10) {
//        log("Striker Velocity %f", getStrikerBody()->GetLinearVelocity().y);
    }
}

void LPTStriker::updateStrikerPhysicsProperty(int categoryBit, int collisionBit, int contactBit) {
//    PhysicsBody *physicsBody = this->getPhysicsBody();
//    physicsBody->setCategoryBitmask(categoryBit);
//    physicsBody->setCollisionBitmask(collisionBit);
//    physicsBody->setContactTestBitmask(contactBit);
}

void LPTStriker::removeBodyFromStriker() {
    this->getStrikerBody()->GetWorld()->DestroyBody(this->getStrikerBody());
}

b2Body* LPTStriker::getStrikerBody() {
    return strikerBody;
}

void LPTStriker::onExit() {
    BaseSprite::onExit();
}

#pragma mark AddPointingHand
void LPTStriker::addPointingHand() {
    if(!((BaseSprite *)this->getChildByTag(kPointHandTag))){
        BaseSprite *pointingHand = BaseSprite::createWithFileName(kPointHandImageName);
        pointingHand->setTag(kPointHandTag);
        pointingHand->setPosition(Vec2(this->getContentSize().width * 1.3, this->getContentSize().height * 0.25));
        //    pointingHand->setGlobalZOrder(1);
        if(!(PLAYER->getGameSelected() == GAME_WITH_COMPUTER)){
            if (!PLAYER->isAdmin){
                pointingHand->setFlippedX(true);
                pointingHand->setPosition(Vec2(-this->getContentSize().width * 0.3, this->getContentSize().height * 0.3));
            }
        }
        this->addChild(pointingHand, 1000);
    }
}

void LPTStriker::removePointingHand() {
    BaseSprite *pointingHand = (BaseSprite *)this->getChildByTag(kPointHandTag);
    if(pointingHand)
        pointingHand->removeFromParentAndCleanup(true);
}

bool LPTStriker::hasPointingHand() {
    BaseSprite *pointingHand = (BaseSprite *)this->getChildByTag(kPointHandTag);
    if(pointingHand){
        return true;
    }
    return false;
}

string LPTStriker::strikerState(string user,int index, b2Vec2 position, b2Vec2 linerVelocity, float angularVelocity, long time) {
    string dataFormat = "{\"User\":\"%s\",\"Tag\":%d,\"PositionX\":%f,\"PositionY\":%f,\"LVelocityX\":%f,\"LVelocityY\":%f,\"AVelocity\":%f,\"Time\":%ld}";
    __String *data = __String::createWithFormat(dataFormat.c_str(),user.c_str(),index,position.x,position.y,linerVelocity.x,linerVelocity.y,angularVelocity);
    return data->getCString();
}

void LPTStriker::forceToReachSpecificPosition(b2Vec2 pos,float speed, b2Body *body) {
    b2Vec2 targetPosition = pos;
    float targetSpeed = speed;
    
    b2Vec2 direction = targetPosition - body->GetPosition();
    float distanceToTravel = direction.Normalize();
    
    // For most of the movement, the target speed is ok
    float speedToUse = targetSpeed;
    
    // Check if this speed will cause overshoot in the next time step.
    // If so, we need to scale the speed down to just enough to reach
    // the target point. (Assuming here a step length based on 60 fps)
    float distancePerTimestep = speedToUse / 60.0f;
    if ( distancePerTimestep > distanceToTravel )
        speedToUse *= ( distanceToTravel / distancePerTimestep );
    
    // The rest is pretty much what you had already:
    b2Vec2 desiredVelocity = speedToUse * direction;
    b2Vec2 changeInVelocity = desiredVelocity - body->GetLinearVelocity();
    
    b2Vec2 force = body->GetMass() * 60.0f * changeInVelocity;
    body->ApplyForce( force, body->GetWorldCenter(), true );
}
void LPTStriker::addLabelToStriker() {
    Label *label = Label::createWithTTF(StringUtils::format("%d", this->getTag()).c_str(), kOpenSans_Bold, 60);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5));
    label->setColor(Color3B::WHITE);
    label->setTag(this->getTag());
    this->addChild(label, 1000000);
}
