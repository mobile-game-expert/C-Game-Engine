//
//  LPTRopeSegment.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 20/09/18.
//

#include "LPTRopeSegment.hpp"

LPTRopeSegment* LPTRopeSegment::createSprite(string imageName) {
    auto layer = LPTRopeSegment::create();
    layer->initWithFile(imageName);
    return layer;
}

bool LPTRopeSegment::initWithFile(string imageName) {
    if (!BaseSprite::initWithFile(imageName)) {
        return false;
    }
    return true;
}

void LPTRopeSegment::onEnter() {
    BaseSprite::onEnter();
}

void LPTRopeSegment::setupRopeProperties(Vec2 position, int tag, int userDataType) {
    ropeInitialPosition = position;
    this->setPosition(position);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setTag(tag);
    this->setType(userDataType);
}

void LPTRopeSegment::setRopeSegmentPosition(Vec2 position) {
    this->setPosition(position);
}

float LPTRopeSegment::getRopeMaxSlantValue() {
    float slantValue = ropeInitialPosition.x - this->getPosition().x;
    return slantValue;
}

void LPTRopeSegment::addBodyToRopeSegment(double density, float restitution, float friction, bool isDynamicBody, b2World *world, Vec2 parentDisplacement, bool isBullet) {
    
    ropeSegmentBody = LPTPhysicsComponent::createBodyDefinition(isDynamicBody == true ? b2_dynamicBody : b2_staticBody, this->getPosition() + parentDisplacement, this, world);
    ropeSegmentBody->SetBullet(isBullet);
    ropeSegmentBody->SetFixedRotation(true);

//    b2CircleShape ropeSegmentBodyShape;
//    ropeSegmentBodyShape.m_radius = this->getContentSize().width * 0.5 * this->getScale() / kPixelToMeterRatio;
    b2PolygonShape ropeSegmentBodyShape;
    ropeSegmentBodyShape.SetAsBox((this->getContentSize().width * 0.5 * this->getScale()) / kPixelToMeterRatio, (this->getContentSize().height * 0.5 * this->getScale()) / kPixelToMeterRatio);
    
    LPTFixtureData *fData = new LPTFixtureData;
    fData->tag = TAG_ROPE_SEGMENT;
    b2FixtureDef strikerFixtureDefinition  = LPTPhysicsComponent::createFixtureDefinition(&ropeSegmentBodyShape, restitution, density, friction, false, Wall_Category_Bit, 1);
    strikerFixtureDefinition.userData = fData;

    ropeSegmentBody->CreateFixture(&strikerFixtureDefinition);
}

void LPTRopeSegment::onExit() {
    BaseSprite::onExit();
}
