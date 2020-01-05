//
//  LPTPhysicsComponent.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 22/09/18.
//

#ifndef LPTPhysicsComponent_hpp
#define LPTPhysicsComponent_hpp

#include <stdio.h>
#include "Box2D/Box2D.h"
#include "BaseSprite.hpp"

using namespace cocos2d;
using namespace std;

class LPTPhysicsComponent : public Ref {
public:
    static b2Body* createBodyDefinition(b2BodyType, Vec2 position, BaseSprite *userdata, b2World *world);
    static b2FixtureDef createFixtureDefinition(b2Shape *bodyShape, float restitution, float density, float friction, bool isSensorBody, int categoryBits, int groupIndex); //
};
#endif /* LPTPhysicsComponent_hpp */
