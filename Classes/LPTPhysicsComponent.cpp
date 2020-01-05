//
//  LPTPhysicsComponent.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 22/09/18.
//

#include "LPTPhysicsComponent.hpp"

b2Body*  LPTPhysicsComponent::createBodyDefinition(b2BodyType bodyType, Vec2 position, BaseSprite *userdata, b2World *world) {
    b2BodyDef bodyDefinition;
    bodyDefinition.type = bodyType;
    bodyDefinition.position.Set(position.x / kPixelToMeterRatio, position.y / kPixelToMeterRatio);
    bodyDefinition.userData = userdata;
    bodyDefinition.active = true;
    
    b2Body *body = world->CreateBody(&bodyDefinition);
    body->SetGravityScale(0);
    return body;
}


b2FixtureDef LPTPhysicsComponent::createFixtureDefinition(b2Shape *bodyShape, float restitution, float density, float friction, bool isSensorBody, int categoryBits, int groupIndex) {
    b2FixtureDef fixtureDef;
    fixtureDef.shape                =   bodyShape;
    fixtureDef.restitution          =   restitution;
    fixtureDef.density              =   density;
    fixtureDef.friction             =   friction;
    fixtureDef.isSensor             =   isSensorBody;
    fixtureDef.filter.categoryBits  =   categoryBits;
    fixtureDef.filter.groupIndex    =   groupIndex;
    return fixtureDef;
}

