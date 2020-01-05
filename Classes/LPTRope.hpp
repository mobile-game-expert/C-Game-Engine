//
//  LPTRope.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 20/09/18.
//

#ifndef LPTRope_hpp
#define LPTRope_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTRopeSegment.hpp"

class LPTRope : public BaseLayer {
public:
    
    //
    vector<LPTRopeSegment*> ropeSegmentArray;
    vector<b2Body*> ropeSegmentBody;
    int maxRopeSegment = 20;
    bool updateRopeAccordingToBall;
    //
    CREATE_FUNC(LPTRope);
    static LPTRope *createRope();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    //
    //
    void setupUI(Size contentSize, Vec2 position, b2World *world);
    void createRopeSegment(b2World* physicsWorld, bool isBullet);
//    void updatePositionOfRopeSegment(int currentSelectedIndex, float forceVariable, float maximumSlant);
    void createRopeJoint(b2World* physicsWorld);
    void removePhysicsJoint();
    //
    void changeLinearDamping();
    void resetAllRopeSegmentToInitialPosition();
    void createRopeTexture(int segment, bool isRopeAccordingToBall, Vec2 ropeNewPosition, bool isStrikerDragging);
    //RopeExtension
    float ropeExtensionValue = 0;
    Vec2 initialPosition;
};
#endif /* LPTRope_hpp */
