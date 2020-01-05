//
//  LPTRope.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 20/09/18.
//

#include "LPTRope.hpp"
#define kDrawNodeLayerTag               6000


LPTRope* LPTRope::createRope() {
    auto layer = LPTRope::create();
    layer->init();
    return layer;
}

bool LPTRope::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}

void LPTRope::onEnter() {
    BaseLayer::onEnter();
}

void LPTRope::setupUI(Size contentSize, Vec2 position, b2World *world) {
    this->setContentSize(contentSize);
    LayerColor *layerColor = LayerColor::create(Color4B(255, 255, 255, 0), contentSize.width, contentSize.height);
    this->addChild(layerColor);
    this->setPosition(position);
//    this->createRopeSegment(world);
}

void LPTRope::createRopeSegment(b2World *world, bool isBullet) {
    Vec2 initialPosition = Vec2(this->getContentSize().width * 0.5, this->getContentSize().height);
   
    for (int ropeSegmentCounter = 0; ropeSegmentCounter < maxRopeSegment; ropeSegmentCounter++) {
         bool isDynamic = true;
        LPTRopeSegment *ropeSegment = LPTRopeSegment::createSprite(kRopeSegmentImageName);
        ropeSegment->setScale(this->getContentSize().height / (maxRopeSegment * ropeSegment->getContentSize().height));
        initialPosition.y = initialPosition.y - (ropeSegment->getContentSize().height * 0.5 * ropeSegment->getScale());
        int userDataType  = this->getTag() == kOppentRopeLayerTag ? kUserDataTypeOpponentRope : kUserDataTypeRope;
        ropeSegment->setupRopeProperties(initialPosition, ropeSegmentCounter, userDataType);
        log(this->getContentSize().height / (maxRopeSegment * ropeSegment->getContentSize().height));
    
        ropeSegment->setOpacity(0);
        
        if(ropeSegmentCounter == 0 || ropeSegmentCounter == maxRopeSegment - 1)
                isDynamic = false;
        
        this->addChild(ropeSegment);
        ropeSegmentArray.push_back(ropeSegment);
        
        ropeSegment->addBodyToRopeSegment(10000.0f, 0.0f, 0.0, isDynamic, world, this->getPosition(),isBullet);
        ropeSegmentBody.push_back(ropeSegment->ropeSegmentBody);
        
        initialPosition.y = initialPosition.y - (ropeSegment->getContentSize().height * 0.5 * ropeSegment->getScale());
    }
}

#pragma mark CREATEROPEJOINT
/**
 This method will create a rope like using b2RopeJoint and to limit the movement of the rope we have used the distance joint.
 */
void LPTRope::createRopeJoint(b2World* physicsWorld) {
        // Distance joint
    float dampingRatio = 0.6;
    float frequencyHz = 40;
        // Rope joint
    float kMaxWidth = 0.5;
    for (int ropeSegmentCounter = 1; ropeSegmentCounter < maxRopeSegment; ropeSegmentCounter++) {
        b2Body *ropeSegmentBody1 = ropeSegmentBody[ropeSegmentCounter - 1];
        b2Body *ropeSegmentBody2 = ropeSegmentBody[ropeSegmentCounter];

            //CreateDistance Joint For Joint Movement
        b2DistanceJointDef distantJointDefinition;
        distantJointDefinition.Initialize(ropeSegmentBody1, ropeSegmentBody2, ropeSegmentBody1->GetWorldCenter(), ropeSegmentBody2->GetWorldCenter());
        distantJointDefinition.collideConnected = false;
        distantJointDefinition.dampingRatio = dampingRatio;
        distantJointDefinition.frequencyHz = frequencyHz;
        physicsWorld->CreateJoint(&distantJointDefinition);

            //Create rope joint
        b2RopeJointDef ropeJointDefinition;
        ropeJointDefinition.maxLength = (ropeSegmentBody2->GetPosition() - ropeSegmentBody1->GetPosition()).Length() * kMaxWidth;
        ropeJointDefinition.collideConnected = false;
        ropeJointDefinition.localAnchorA = ropeJointDefinition.localAnchorB = b2Vec2_zero;
        ropeJointDefinition.bodyA = ropeSegmentBody1;
        ropeJointDefinition.bodyB = ropeSegmentBody2;
        physicsWorld->CreateJoint(&ropeJointDefinition);
    }
}
#pragma mark RemovingPhysicsJoint
void LPTRope::removePhysicsJoint() {
    for (int ropeSegmentCounter = 0; ropeSegmentCounter < maxRopeSegment; ropeSegmentCounter++) {
        LPTRopeSegment* ropeSegment = (LPTRopeSegment *)ropeSegmentArray[ropeSegmentCounter];
        if(ropeSegment->getPhysicsBody() != nullptr) {
            ropeSegment->getPhysicsBody()->removeFromWorld();
        }
    }
}
//void LPTRope::updatePositionOfRopeSegment(int currentSelectedIndex, float forceVariable, float maximumSlant) {
//
//    LPTRopeSegment *selectedRopeSegment = ropeSegmentArray[currentSelectedIndex];
//    if(selectedRopeSegment->getPosition().x - forceVariable > maximumSlant) {
//        selectedRopeSegment->setPositionX(selectedRopeSegment->ropeInitialPosition.x - forceVariable);
//    }
//
//    float maxForceInCurrentSelectedRopeSegment = maximumSlant -  selectedRopeSegment->getPosition().y;
//    float incrementalVariable = 0;
//    float lSideDifference = maxForceInCurrentSelectedRopeSegment / (currentSelectedIndex);
//    float rSideDifference = maxForceInCurrentSelectedRopeSegment / (maxRopeSegment - currentSelectedIndex);
//
//    for (int ropeSegmentCounter = 1; ropeSegmentCounter < maxRopeSegment - 1; ropeSegmentCounter++) {
//        LPTRopeSegment *ropeSegment = (LPTRopeSegment *)ropeSegmentArray[ropeSegmentCounter];
//        if(ropeSegmentCounter > currentSelectedIndex) {
//            incrementalVariable = incrementalVariable - rSideDifference;
//        }else if(ropeSegmentCounter < currentSelectedIndex){
//            incrementalVariable = incrementalVariable + lSideDifference;
//        }
//        if(ropeSegmentCounter != currentSelectedIndex)
//            ropeSegment->setRopeSegmentPosition(Vec2(ropeSegment->ropeInitialPosition.x - incrementalVariable, ropeSegment->ropeInitialPosition.y));
//    }
//}

void LPTRope::resetAllRopeSegmentToInitialPosition() {
    for (int ropeSegmentCounter = 0; ropeSegmentCounter < ropeSegmentArray.size(); ropeSegmentCounter++) {
        LPTRopeSegment *selectedRopeSegment = ropeSegmentArray[ropeSegmentCounter];
        b2Body *ropeBody    = ropeSegmentBody[ropeSegmentCounter];
        selectedRopeSegment->setPosition(selectedRopeSegment->ropeInitialPosition);
        ropeBody->SetTransform(b2Vec2((selectedRopeSegment->getPosition().x + this->getPosition().x) / kPixelToMeterRatio,  (selectedRopeSegment->getPosition().y + this->getPosition().y) / kPixelToMeterRatio), 0);
    }
}

void LPTRope::changeLinearDamping() {
    for (int ropeSegmentCounter = 0; ropeSegmentCounter < maxRopeSegment; ropeSegmentCounter++) {
        b2Body *ropeSegmentBody1 = ropeSegmentBody[ropeSegmentCounter];
        ropeSegmentBody1->SetLinearDamping(15);
    }
}

#pragma mark CREATEROPETEXTURE
void LPTRope::createRopeTexture(int segment, bool isRopeAccordingToBall, Vec2 ropeNewPosition, bool isStrikerDragging) {
    this->updateRopeAccordingToBall = isRopeAccordingToBall;
//    log("PositionIn X: %f  == Layer Position %f",ropeNewPosition.x, this->getPosition().x);
    ropeNewPosition =  this->convertToNodeSpace(ropeNewPosition);
    float drawNodePositionInX = this->getContentSize().width * 0.5;
//    log("Converted Position X: %f=======%f", drawNodePositionInX, ropeNewPosition.x);
    DrawNode *obj = (DrawNode *)this->getChildByTag(kDrawNodeLayerTag);
    if(obj) {
        obj->removeFromParentAndCleanup(true);
    }
    DrawNode * drawNodeObj = DrawNode::create();
    drawNodeObj->setPosition(Vec2(0, 0));
    drawNodeObj->setAnchorPoint(Vec2(0, 0));
    drawNodeObj->setTag(kDrawNodeLayerTag);
    this->addChild(drawNodeObj, 2000);
    
    if(segment >= 0 && isRopeAccordingToBall) {
        if(this->getTag() == kOppentRopeLayerTag) {
            if(drawNodePositionInX - ropeNewPosition.x >= 0.0  && isStrikerDragging) {
                ropeNewPosition.x = drawNodePositionInX;
            }
        }else {
            if(ropeNewPosition.x - drawNodePositionInX >= 0.0  && isStrikerDragging) {
                ropeNewPosition.x = drawNodePositionInX;
            }
        }
        drawNodeObj->drawSegment(Vec2(drawNodePositionInX, 0), Vec2(ropeNewPosition.x, ropeNewPosition.y), 2, Color4F::BLACK);
        drawNodeObj->drawSegment(Vec2(ropeNewPosition.x, ropeNewPosition.y), Vec2(drawNodePositionInX, this->getContentSize().height), 2, Color4F::BLACK);
        
//        if(this->getTag() == kRopeLayerTag){
            ropeExtensionValue = drawNodePositionInX - ropeNewPosition.x;
//            log("Force Calculation: %f", drawNodePositionInX - ropeNewPosition.x);
//        }else {
//             ropeExtensionValue = drawNodePositionInX - ropeNewPosition.x;
//        }
    }else {
        drawNodeObj->drawSegment(Vec2(drawNodePositionInX, 0), Vec2(drawNodePositionInX, this->getContentSize().height / 2), 2, Color4F::BLACK);
        
        drawNodeObj->drawSegment(Vec2(drawNodePositionInX, this->getContentSize().height / 2), Vec2(drawNodePositionInX, this->getContentSize().height), 2, Color4F::BLACK);
    }
}

void LPTRope::onExit() {
    BaseLayer::onExit();
}
