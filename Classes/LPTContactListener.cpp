
#include "LPTContactListener.h"
#include "LPTGameScene.hpp"
#include "LPTGameSceneRW.hpp"
#include "BaseSprite.hpp"


LPTContactListener::LPTContactListener(BaseLayer *object)
{
    if (PLAYER->getGameSelected() == GAME_WITH_COMPUTER) {
        this->gameSceneObject = (LPTGameScene*)object;
    }else {
        this->gameSceneObject_rw = (LPTGameSceneRW*)object;
    }
}

void LPTContactListener::BeginContact(b2Contact *contact){
    
    b2Body *bodyA =  contact->GetFixtureA()->GetBody();
    b2Body *bodyB =  contact->GetFixtureB()->GetBody();
    LPTFixtureData *fDataBodyA = (LPTFixtureData *)contact->GetFixtureA()->GetUserData();
    LPTFixtureData *fDataBodyB = (LPTFixtureData *)contact->GetFixtureB()->GetUserData();
    BaseSprite *contactSpriteA = (BaseSprite*)bodyA->GetUserData();
    BaseSprite *contactSpriteB = (BaseSprite*)bodyB->GetUserData();
  
    
    if (contactSpriteA == NULL || contactSpriteB == NULL) {
        return;
    }
    float speedLimit = 8.0;
    if((contactSpriteA->userDataType == kUserDataTypeStriker && contactSpriteB->userDataType == kUserDataTypeStriker)) {
        LPTStriker *strikerA = (LPTStriker*)contactSpriteA;
        LPTStriker *strikerB = (LPTStriker*)contactSpriteB;
        if (bodyA->GetLinearVelocity().x < -speedLimit || bodyA->GetLinearVelocity().x > speedLimit) {
            SoundManager::playSound(STRIKER_STRIKER_SOUND);
        }else if(bodyB->GetLinearVelocity().x < -speedLimit || bodyB->GetLinearVelocity().x > speedLimit){
            SoundManager::playSound(STRIKER_STRIKER_SOUND);
        }else if ((strikerA->velocity.x < -speedLimit || strikerA->velocity.x > speedLimit) || (strikerB->velocity.x < -speedLimit || strikerB->velocity.x > speedLimit)){
//            SoundManager::playSound(STRIKER_STRIKER_SOUND);
        }
    }
    if((contactSpriteA->userDataType == kUserDataTypeStriker && contactSpriteB->userDataType == kUserDataTypeDeflector) || (contactSpriteA->userDataType == kUserDataTypeDeflector && contactSpriteB->userDataType == kUserDataTypeStriker)) {
//        log("Deflector Vs Striker");
        if (contactSpriteA->userDataType == kUserDataTypeStriker) {
            LPTStriker *striker = (LPTStriker*)contactSpriteA;
            if (bodyA->GetLinearVelocity().x < -speedLimit || bodyA->GetLinearVelocity().x > speedLimit) {
                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }else if (striker->velocity.x < -speedLimit * 0.5 || striker->velocity.x > speedLimit * 0.5){
//                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }
        }else {
            LPTStriker *striker = (LPTStriker*)contactSpriteB;
            if (bodyB->GetLinearVelocity().x < -speedLimit || bodyB->GetLinearVelocity().x > speedLimit) {
                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }else if (striker->velocity.x < -speedLimit * 0.5 || striker->velocity.x > speedLimit * 0.5){
//                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }
        }
    }
    if((contactSpriteA->userDataType == kUserDataTypeStriker && contactSpriteB->userDataType == kUserDataTypeSensor) || (contactSpriteA->userDataType == kUserDataTypeSensor && contactSpriteB->userDataType == kUserDataTypeStriker)) {
//        log("Sensor Vs Striker");
        if (contactSpriteA->userDataType == kUserDataTypeStriker) {
            LPTStriker *striker = (LPTStriker*)contactSpriteA;
            if (bodyA->GetLinearVelocity().x < -speedLimit || bodyA->GetLinearVelocity().x > speedLimit) {
                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }else if (striker->velocity.x < -speedLimit * 0.5 || striker->velocity.x > speedLimit * 0.5){
//                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }
        }else {
            LPTStriker *striker = (LPTStriker*)contactSpriteB;
            if (bodyB->GetLinearVelocity().x < -speedLimit || bodyB->GetLinearVelocity().x > speedLimit) {
                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }else if (striker->velocity.x < -speedLimit * 0.5 || striker->velocity.x > speedLimit * 0.5){
//                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }
        }
    }
    if((contactSpriteA->userDataType == kUserDataTypeStriker && contactSpriteB->userDataType == kUserDataTypeWall) || (contactSpriteA->userDataType == kUserDataTypeWall && contactSpriteB->userDataType == kUserDataTypeStriker)) {
        if (contactSpriteA->userDataType == kUserDataTypeStriker) {
            LPTStriker *striker = (LPTStriker*)contactSpriteA;
            if (bodyA->GetLinearVelocity().x < -speedLimit || bodyA->GetLinearVelocity().x > speedLimit) {
                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }else if (striker->velocity.x < -speedLimit || striker->velocity.x > speedLimit){
//                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }
        }else {
            LPTStriker *striker = (LPTStriker*)contactSpriteB;
            if (bodyB->GetLinearVelocity().x < -speedLimit || bodyB->GetLinearVelocity().x > speedLimit) {
                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }else if (striker->velocity.x < -speedLimit || striker->velocity.x > speedLimit){
//                SoundManager::playSound(STRIKER_WOODEN_SOUND);
            }
        }
    }
    
    if (PLAYER->getGameSelected() == GAME_WITH_COMPUTER) {
        //GameScene Object
        if((contactSpriteA->userDataType == kUserDataTypeStriker && contactSpriteB->userDataType == kUserDataTypeRope)|| (contactSpriteA->userDataType == kUserDataTypeRope && contactSpriteB->userDataType == kUserDataTypeStriker)) {
            this->gameSceneObject->collisionBetweenRopeAndStrikerSarted = true;
        }
        if((contactSpriteA->userDataType == kUserDataTypeStriker && contactSpriteB->userDataType == kUserDataTypeOpponentRope)|| (contactSpriteA->userDataType == kUserDataTypeOpponentRope && contactSpriteB->userDataType == kUserDataTypeStriker)) {
            this->gameSceneObject->collisionBetweenRopeAndStrikerStartedForOpponent = true;
        }
    }else {
        
    }
}

void LPTContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
    b2Body *bodyA =  contact->GetFixtureA()->GetBody();
    b2Body *bodyB =  contact->GetFixtureB()->GetBody();
    BaseSprite *contactSpriteA = (BaseSprite*)bodyA->GetUserData();
    BaseSprite *contactSpriteB = (BaseSprite*)bodyB->GetUserData();

    if (contactSpriteA == NULL || contactSpriteB == NULL) {
        return;
    }
    if (PLAYER->getGameSelected() == GAME_WITH_COMPUTER) {
    }else {
        
    }
}

void LPTContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
    b2Body *bodyA =  contact->GetFixtureA()->GetBody();
    b2Body *bodyB =  contact->GetFixtureB()->GetBody();
    LPTFixtureData *fDataBodyA = (LPTFixtureData *)contact->GetFixtureA()->GetUserData();
    LPTFixtureData *fDataBodyB = (LPTFixtureData *)contact->GetFixtureB()->GetUserData();
    BaseSprite *contactSpriteA = (BaseSprite*)bodyA->GetUserData();
    BaseSprite *contactSpriteB = (BaseSprite*)bodyB->GetUserData();
    
    if (PLAYER->getGameSelected() == GAME_WITH_COMPUTER) {
        if (fDataBodyA != NULL && fDataBodyB !=  NULL) {
            b2Body *strikerBody     = bodyA;
            b2Body *ropeSegmentBody = bodyB;
            if (fDataBodyB->tag == TAG_STRIKER_BODY) {
                strikerBody = bodyB;
            }
            if (fDataBodyA->tag == TAG_ROPE_SEGMENT) {
                ropeSegmentBody = bodyA;
            }
            if ((fDataBodyA->tag == TAG_ROPE_SEGMENT && fDataBodyB->tag == TAG_STRIKER_BODY) || (fDataBodyB->tag == TAG_ROPE_SEGMENT && fDataBodyA->tag == TAG_STRIKER_BODY)) {
                if ((strikerBody->GetPosition().x * kPixelToMeterRatio)< gameSceneObject->winSize.width * 0.5) {
                    if (strikerBody->GetPosition().x < ropeSegmentBody->GetPosition().x) {
                        strikerBody->ApplyLinearImpulseToCenter(b2Vec2(strikerBody->GetMass() * (GameUtils::isIphoneX ? 10000 :  100), 0.0), true);
                    }
                }else {
                    if (strikerBody->GetPosition().x > ropeSegmentBody->GetPosition().x) {
                        strikerBody->ApplyLinearImpulseToCenter(b2Vec2(-strikerBody->GetMass() * (GameUtils::isIphoneX ? 10000 :  100), 0.0), true);
                    }
                }
                
            }
        }
    }else {
        
    }
}

void LPTContactListener::EndContact(b2Contact *contact) {
    b2Body *bodyA =  contact->GetFixtureA()->GetBody();
    b2Body *bodyB =  contact->GetFixtureB()->GetBody();
    LPTFixtureData *fDataBodyA = (LPTFixtureData *)contact->GetFixtureA()->GetUserData();
    LPTFixtureData *fDataBodyB = (LPTFixtureData *)contact->GetFixtureB()->GetUserData();
    BaseSprite *contactSpriteA = (BaseSprite*)bodyA->GetUserData();
    BaseSprite *contactSpriteB = (BaseSprite*)bodyB->GetUserData();
    
    if (contactSpriteA == NULL || contactSpriteB == NULL) {
        return;
    }
    
    if (PLAYER->getGameSelected() == GAME_WITH_COMPUTER) {
    }else {
        
    }
}
