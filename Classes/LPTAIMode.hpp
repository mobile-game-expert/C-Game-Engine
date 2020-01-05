//
//  LPTAIMode.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 24/09/18.
//

#ifndef LPTAIMode_hpp
#define LPTAIMode_hpp

#include <stdio.h>
#include "LPTStriker.hpp"
#include "LPTHeaders.h"
#include <Box2D/Box2D.h>

struct AIProperties {
    GAME_MODE aiMode;
    Vec2 positionOnRope;
    int strikerIndex;
    Vec2 impluse;
    int direction;
};

class LPTAIMode {
public:
    
    UserPlaySide playSide;
    static LPTAIMode* sharedInstance(UserPlaySide playSide = PLAY_AREA_RIGHT);
    AIProperties aiModeProperties;
    void setupAIProperties();
    int nextMoveTime();
    int directionOfMovement();
    void removeSharedInstance();
private:
    Vec2 getStrikerPositionInRope();
    Vec2 getPositionOnRope();
    Vec2 getImpulseOfStriker();
    
};
#endif /* LPTAIMode_hpp */
