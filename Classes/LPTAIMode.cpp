//
//  LPTAIMode.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 24/09/18.
//

#include "LPTAIMode.hpp"

#define kMinimumRangeX                           -40
#define kRopePositionRangeX                     (GameUtils::isIphone?(GET_WIDTH * 0.02):(GET_WIDTH * 0.01))
#define kRopePositionRangeYBeginner              GET_HEIGHT * 0.05
#define kRopePositionRangeYIntermediate          GET_HEIGHT * 0.03
#define kRopePositionRangeYAdvanced              GET_HEIGHT * 0.02

#define kBeginnerNextMoveMinDifference              2.0f //2.0
#define kBeginnerNextMoveMaxDifference              4.0f //3.0
#define kIntermediateNextMoveMinDifference          1.0f //1.0
#define kIntermediateNextMoveMaxDifference          3.0f //2.0
#define kAdvancedNextMoveMinDifference              0.0f
#define kAdvancedNextMoveMaxDifference              2.0f // 1.0

static LPTAIMode *_sharedInstance = NULL;

LPTAIMode* LPTAIMode::sharedInstance(UserPlaySide playSide) {
    if (!_sharedInstance) {
        _sharedInstance = new LPTAIMode();
        playSide = playSide;
    }
    return _sharedInstance;
}

void LPTAIMode::removeSharedInstance() {
    if(_sharedInstance) {
        _sharedInstance = NULL;
    }
}

void LPTAIMode::setupAIProperties() {
    aiModeProperties.aiMode = (GAME_MODE)UserDefault::getInstance()->getIntegerForKey(kAIMode, BEGINNER);
    aiModeProperties.direction = directionOfMovement();
    aiModeProperties.positionOnRope = getPositionOnRope();
    aiModeProperties.impluse = getImpulseOfStriker();
//    log(".//direction : %d",aiModeProperties.direction);
//    log(".//aiMode x : %f, y: %f",aiModeProperties.positionOnRope.x, aiModeProperties.positionOnRope.y );
}

int LPTAIMode::directionOfMovement() {
    if(playSide == PLAY_AREA_LEFT) { //Opponent play area side
        return -1;
    } else {
        return 1;
    }
}

Vec2 LPTAIMode::getPositionOnRope() {
    switch (aiModeProperties.aiMode) {
        case BEGINNER: {
            auto x = fmod(std::rand(), kRopePositionRangeX);
            int range = kRopePositionRangeYBeginner;
//            log("Value of beginner: %d", range);
            auto y = std::rand() % range;
            auto direction = arc4random() % 2 == 0 ? 1 : -1;
            return Vec2(x * aiModeProperties.direction, y * direction);
        }
            break;
        case INTERMEDIATE: {
            auto x = fmod(std::rand(), kRopePositionRangeX) ;
            int range = kRopePositionRangeYIntermediate;
//            log("Value of beginner: %d", range);
            auto y = std::rand() % range;
            auto direction = arc4random() % 2 == 0 ? 1 : -1;
            return Vec2(x * aiModeProperties.direction, y * direction);
        }
            break;
        case EXPERT: {
            auto x = fmod(std::rand(), kRopePositionRangeX);
            int range = kRopePositionRangeYAdvanced;
//            log("Value of beginner: %d", range);
            auto y = std::rand() % range;
            auto direction = arc4random() % 2 == 0 ? 1 : -1;
            return Vec2(x * aiModeProperties.direction, y * direction);
        }
            break;
        default:
            break;
    }
}

int LPTAIMode::nextMoveTime() {
    float seconds;
    switch (aiModeProperties.aiMode) {
        case BEGINNER: {
            seconds = kBeginnerNextMoveMinDifference + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(kBeginnerNextMoveMaxDifference-kBeginnerNextMoveMinDifference)));
        }
            break;
        case INTERMEDIATE: {
            seconds = kIntermediateNextMoveMinDifference + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(kIntermediateNextMoveMaxDifference-kIntermediateNextMoveMinDifference)));
        }
            break;
        case EXPERT: {
            seconds = kAdvancedNextMoveMinDifference + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(kAdvancedNextMoveMaxDifference-kAdvancedNextMoveMinDifference)));
        }
            break;
        default:
            seconds = kBeginnerNextMoveMinDifference;
            break;
    }
//    log("seconds :%f", seconds);
    return seconds;
}

Vec2 LPTAIMode::getImpulseOfStriker() {
    Vec2 impluse;
//    impluse = Vec2(500 * aiModeProperties.direction, 0.5);
    impluse = Vec2(500 , 0.5);
    return impluse;
}
