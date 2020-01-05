//
//  LPTRopeSegment.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 20/09/18.
//

#ifndef LPTRopeSegment_hpp
#define LPTRopeSegment_hpp

#include <stdio.h>
#include "LPTHeaders.h"

#define TAG_ROPE_SEGMENT       2510

class LPTRopeSegment : public BaseSprite {
public:
    b2Body *ropeSegmentBody;
    //
    CREATE_FUNC(LPTRopeSegment);
    //
    Vec2 ropeInitialPosition;
    //
    void setupRopeProperties(Vec2 position, int tag, int userDataType);
    void setRopeSegmentPosition(Vec2 position);
    float getRopeMaxSlantValue();
    void addBodyToRopeSegment(double density, float restitution, float friction, bool isDynamicBody, b2World *world, Vec2 parentDisplacement, bool isBullet);
    
    //SpriteMethod
    static LPTRopeSegment *createSprite(string imageName);
    virtual bool initWithFile(string imageName);
    virtual void onEnter();
    virtual void onExit();
private:
    
};
#endif /* LPTRopeSegment_hpp */
