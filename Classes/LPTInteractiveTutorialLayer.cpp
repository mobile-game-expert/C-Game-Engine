//
//  LPTInteractiveTutorialLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 29/11/18.
//

#include "LPTInteractiveTutorialLayer.hpp"
#include "LPTContactListener.h"
#include "LPTGameBoard.hpp"
#include "LPTRope.hpp"
#include "LPTGameScene.hpp"
#include "LPTDifficultySelection.hpp"
#include "LPTBetSelectionScene.hpp"

#define kCollisionDelayTime             0.001
#define kGameBoardTag                   1000001
#define kStrikerTagBase                 1000002
#define kPointingHandTag                1000003
#define kTutorialDescriptionTag         1000004


#define kInstructionText_1              "Move the disk toward Bungee"
#define kInstructionText_2              "Use the Bungee to launch the disk on your side through the gate"
#define kInstructionText_3              "First one to clear their side from all disks wins"

LPTInteractiveTutorialLayer* LPTInteractiveTutorialLayer::createLayer(int backButtonTag) {
    auto layer = LPTInteractiveTutorialLayer::create();
    layer->initWithTag(backButtonTag);
    return layer;
}

bool LPTInteractiveTutorialLayer::initWithTag(int tag) {
    if (!BaseLayer::init()) {
        return false;
    }
    APP_MANAGER->gameState = INTERACTIVE_TUTORIAL_SCENE;
    backButtonTag = tag;
    setBackgroundSprite(kGamePlayBG);
    return true;
}
void LPTInteractiveTutorialLayer::onEnter() {
    BaseLayer::onEnter();
    locationOfPointingHand = 0;
    this->setupUI();
    
}

void LPTInteractiveTutorialLayer::setupUI() {
    
    this->createPhysicalWorld();
    this->addGameTable();
    this->addStriker();
    this->addRopeInGameScene(kRopeLayerTag, false);
    this->addRopeInGameScene(kOppentRopeLayerTag,true);
    this->addTouchListener();
    this->scheduleTick();
    this->addPointingHand(locationOfPointingHand);
    this->addInformationLabel();
}

#pragma mark addInformationLabel
void LPTInteractiveTutorialLayer::addInformationLabel() {
    Label *statusLabel = createTTFLabel("Move the disk toward Bungee", Vec2(winSize.width * 0.5, winSize.height * 0.5), Vec2::ANCHOR_MIDDLE, this, Color3B::WHITE, kNOUGAT_EXTRABLACK_1, GameUtils::isIphoneX ? 50 : 27);
    statusLabel->setTag(kTutorialDescriptionTag);
    statusLabel->setLocalZOrder(1000);
}

void LPTInteractiveTutorialLayer::updateInstructionText(string text) {
    Label *descriptionLabel = (Label *)this->getChildByTag(kTutorialDescriptionTag);
    descriptionLabel->setString(text);
}
string LPTInteractiveTutorialLayer::getInstructionText() {
    string text = kInstructionText_1;
    
    if(locationOfPointingHand == 0) {
        text = kInstructionText_1;
    }else if (locationOfPointingHand == 1) {
        text = kInstructionText_2;
    }else {
        text = kInstructionText_3;
    }
    return text;
}


#pragma mark POINTING_HAND
void LPTInteractiveTutorialLayer::addPointingHand(int location) {
    BaseSprite *pointingHand = (BaseSprite *)this->getChildByTag(kPointingHandTag);
    if(location != 2 && pointingHand == nullptr) {
        auto pointingHand = BaseSprite::createWithFileName(kPointHandImageName);
        pointingHand->setFlippedX(true);
        pointingHand->setPosition(this->getPointHandFinalLocation());
        pointingHand->setTag(kPointingHandTag);
        pointingHand->setScale(0.45);
        this->addChild(pointingHand, 10000);
        
        pointingHand->runAction(RepeatForever::create(Sequence::create(MoveTo::create(2.0, this->getPointHandInitialLocation()), MoveTo::create(0, this->getPointHandFinalLocation()), NULL)));
    }
}

void LPTInteractiveTutorialLayer::removePointingHand() {
    BaseSprite *pointingHand = (BaseSprite *)this->getChildByTag(kPointingHandTag);
    if(pointingHand) {
        locationOfPointingHand = locationOfPointingHand + 1;
        pointingHand->removeFromParentAndCleanup(true);
        this->updateInstructionText(this->getInstructionText());
    }
}
Vec2 LPTInteractiveTutorialLayer::getPointHandInitialLocation() {
    LPTRope *ropeSegment = (LPTRope *)this->getChildByTag(kRopeLayerTag);
    Vec2 position = Vec2(0, 0);
    if(locationOfPointingHand == 0) {
        position = Vec2(ropeSegment->getPosition().x, ropeSegment->getPosition().y + ropeSegment->getContentSize().height * 0.5);
    }else if (locationOfPointingHand == 1) {
        position = Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5);
    }
    return position;
}

Vec2 LPTInteractiveTutorialLayer::getPointHandFinalLocation() {
    LPTStriker *striker = arrayStriker.at(0);
    Vec2 position = Vec2(0, 0);
    if(locationOfPointingHand == 0) {
        position = Vec2(striker->getPosition().x, striker->getPosition().y);
    }else if (locationOfPointingHand == 1) {
        position = Vec2(this->getContentSize().width * 0.4, this->getContentSize().height * 0.5);
    }
    return position;
}
#pragma mark - TouchListener
void LPTInteractiveTutorialLayer::addTouchListener() {
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(LPTInteractiveTutorialLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(LPTInteractiveTutorialLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(LPTInteractiveTutorialLayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(LPTInteractiveTutorialLayer::onTouchCancelled, this);
    SoundManager::playSound(SCENE_CHANGE_SOUND);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool LPTInteractiveTutorialLayer::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 position = touch->getLocation();
    bool isStrikerFound     =   this->checkForTouchableArea(position);
    selectedStrikerIndex    =   touchedStrikerIndex;
    this->removePointingHand();
    if(selectedStrikerIndex >= 0 && isStrikerFound) {
        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
        striker->isStrikerDragging = true;
    }
    return isStrikerFound;
}
bool LPTInteractiveTutorialLayer::checkForTouchableArea(Vec2 touch) {
    Vec2 touchPoint = touch;
    Rect touchableArea = this->getTableTouchableArea();
    
    //To check for board touchable area
    if(touchableArea.containsPoint(touchPoint)) {
        //Check for striker touched
        for(int strikerCounter = 0; strikerCounter < 1; strikerCounter++) {
            LPTStriker* striker     =   arrayStriker.at(strikerCounter);
            auto strikerPosition    =   striker->getPosition();
            //centerTable->convertToNodeSpace(striker->getPosition());
            
            Rect strikerBoundingBox = Rect(strikerPosition.x - striker->getBoundingBox().size.width,
                                           strikerPosition.y - striker->getBoundingBox().size.height,
                                           striker->getBoundingBox().size.width * 3,
                                           striker->getBoundingBox().size.height * 3);
            
            if(strikerBoundingBox.containsPoint(touchPoint) && striker->isStrikerTouchable == true) {
                touchedStrikerIndex = strikerCounter;
                lastStrikerPosition = striker->getPosition();
                return true;
            }
        }
        return false;
    } else {
        return false;
    }
}
void LPTInteractiveTutorialLayer::onTouchMoved(Touch* touch, Event* event) {
    Vec2 touchPoint = touch->getLocation();
    Rect touchableArea = this->getTableTouchableArea();
    
    if(selectedStrikerIndex >= 0) {
        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
        Vec2 touchPrevLocation = touch->getPreviousLocation();
        
        Vec2 movementDirection = touchPrevLocation - touchPoint;
        if(isStrikerInsideTouchableArea(striker, touchPoint, touchableArea)) {
            striker->getStrikerBody()->SetTransform(b2Vec2((touch->getLocation().x) / kPixelToMeterRatio, (touch->getLocation().y) / kPixelToMeterRatio), 0);
        }else {
            if(checkFeasiblePositionOfStrikerY(touchPoint, touchableArea)) {
                //If left side touch move outside the area.
                float positionInX = touchPoint.x;
                if(touchPoint.x < (touchableArea.origin.x + striker->getContentSize().width * 0.5 * striker->getScale())) {
                    //If left side touch move outside the area.
                    positionInX = touchableArea.origin.x + striker->getContentSize().width * 0.5 * striker->getScale();
                    striker->getStrikerBody()->SetTransform(b2Vec2(positionInX / kPixelToMeterRatio, (touch->getLocation().y) / kPixelToMeterRatio), 0);
                }
                
                if (positionInX > (touchableArea.origin.x + touchableArea.size.width - (striker->getContentSize().width * striker->getScale()))) {
                    // If right side of the touch move outside the area.
                    positionInX = touchableArea.origin.x + touchableArea.size.width - (striker->getContentSize().width * 0.5 * striker->getScale());
                    striker->getStrikerBody()->SetTransform(b2Vec2(positionInX / kPixelToMeterRatio, (touch->getLocation().y) / kPixelToMeterRatio), 0);
                }
            }
            
            if(checkFeasiblePositionOfStrikerX(touchPoint, touchableArea)) {
                float positionInY = touchPoint.y;
                if(touchPoint.y < (touchableArea.origin.y + striker->getContentSize().width * 0.5 * striker->getScale())) {
                    //If touch point moves below the TableArea
                    positionInY = touchableArea.origin.y + (striker->getContentSize().width * 0.5 * striker->getScale());
                    striker->getStrikerBody()->SetTransform(b2Vec2(touchPoint.x / kPixelToMeterRatio, positionInY / kPixelToMeterRatio), 0);
                }
                
                if(touchPoint.y > (touchableArea.origin.y + touchableArea.size.height - (striker->getContentSize().width * 0.5 * striker->getScale()))) {
                    //If touch point moves above the TableArea
                    positionInY = touchableArea.origin.y + touchableArea.size.height - (striker->getContentSize().width * 0.5 * striker->getScale());
                    striker->getStrikerBody()->SetTransform(b2Vec2(touchPoint.x / kPixelToMeterRatio, positionInY / kPixelToMeterRatio), 0);
                }
            }
        }
        striker->isStrikerDragging = true;
    }
}

bool LPTInteractiveTutorialLayer::checkFeasiblePositionOfStrikerY (Vec2 touchPoint, Rect area) {
    auto margin = area.size.height * 0.09;
    if((touchPoint.y > area.origin.y + margin) && (touchPoint.y < (area.origin.y + area.size.height - margin))) {
        return true;
    }
    return false;
}

bool LPTInteractiveTutorialLayer::checkFeasiblePositionOfStrikerX (Vec2 touchPoint, Rect area) {
    auto margin = area.size.width * 0.09;
    if((touchPoint.x > area.origin.x + margin) && (touchPoint.x < (area.origin.x + area.size.width * 0.97))) {
        return true;
    }
    return false;
}


void LPTInteractiveTutorialLayer::onTouchEnded(Touch* touch, Event* event)
{
    Vec2 touchPoint = touch->getLocation();
    this->removePointingHand();
    if(selectedStrikerIndex >= 0) {
        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
        LPTRope *ropeObject = (LPTRope *) this->getChildByTag(kRopeLayerTag);
        log("Value of the rope extension : %f", ropeObject->ropeExtensionValue);
        log("Combined Value : %f", ropeObject->ropeExtensionValue * (GameUtils::isIphoneX ? 400000.0 : 900.0));
        
        striker->getStrikerBody()->SetLinearVelocity(b2Vec2(ropeObject->ropeExtensionValue * (GameUtils::isIphoneX ? 400000.0 : 900.0), 0.0));
        
        auto changeStrikerDragging = CallFunc::create([striker, this](){
            striker->isStrikerDragging = false;
        });
        this->runAction(Sequence::create(DelayTime::create(1.0), changeStrikerDragging, NULL));
    }
}

bool LPTInteractiveTutorialLayer::checkForStrikerPassingArea(Vec2 touch) {
    BaseSprite *centerTable = getCenterTable();
    
    Rect centerTablePassingArea = Rect(centerTable->getParent()->getPosition().x + centerTable->getPosition().x - centerTable->getContentSize().width * 0.1, centerTable->getParent()->getPosition().y + centerTable->getPosition().y - centerTable->getContentSize().height * 0.1, centerTable->getContentSize().width * 0.1, centerTable->getContentSize().height * 0.2);
    
    if(centerTablePassingArea.containsPoint(touch)) {
        return true;
    } else {
        return false;
    }
}

void LPTInteractiveTutorialLayer::onTouchCancelled(Touch* touch, Event* event) {
    onTouchEnded(touch,event);
}

bool LPTInteractiveTutorialLayer::isStrikerInsideTouchableArea(LPTStriker* striker, Vec2 strikerPosition, Rect touchableArea) {
    float strikerBoundingBoxFactor = (GameUtils::isIphoneX?0.5: 0.5);
    auto strikerCorner1 = Vec2(strikerPosition.x - striker->getContentSize().width * striker->getScale()*strikerBoundingBoxFactor,
                               strikerPosition.y - striker->getContentSize().height * striker->getScale()*strikerBoundingBoxFactor);
    auto strikerCorner2 = Vec2(strikerPosition.x + striker->getBoundingBox().size.width * striker->getScale()*strikerBoundingBoxFactor,
                               strikerPosition.y - striker->getContentSize().height * striker->getScale()*strikerBoundingBoxFactor);
    auto strikerCorner3 = Vec2(strikerPosition.x - striker->getContentSize().width * striker->getScale()*strikerBoundingBoxFactor,
                               strikerPosition.y + striker->getContentSize().height * striker->getScale()*strikerBoundingBoxFactor);
    auto strikerCorner4 = Vec2(strikerPosition.x + striker->getBoundingBox().size.width * striker->getScale()*strikerBoundingBoxFactor,
                               strikerPosition.y + striker->getBoundingBox().size.height * striker->getScale()*strikerBoundingBoxFactor);
    if(touchableArea.containsPoint(strikerCorner1) &&
       touchableArea.containsPoint(strikerCorner2) &&
       touchableArea.containsPoint(strikerCorner3) &&
       touchableArea.containsPoint(strikerCorner4) ) {
        return true;
    } else {
        return false;
    }
}

#pragma mark addGameTable
void LPTInteractiveTutorialLayer::addGameTable() {
    Size gameBoardSize = GameUtils::isIphoneX ? Size(1600, 800) : Size(900, 450);
    
    LPTGameBoard *gameBoard = LPTGameBoard::createLayer();
    
    float tablePositionInX = (this->getContentSize().width - gameBoardSize.width) / 2;
    float heightLeft = this->getContentSize().height;
    float tablePositionInY = (heightLeft - gameBoardSize.height) * 0.5;
    
    gameBoard->setUpGameBoardUI(gameBoardSize, Vec2(tablePositionInX, tablePositionInY), kCenterTable_1_ImageName, kBoard_1_CornerImageName, kBoardDividerImageName, world);
    gameBoard->setTag(kGameBoardTag);
    this->addChild(gameBoard, 1);
}
BaseSprite* LPTInteractiveTutorialLayer::getCenterTable() {
    LPTGameBoard *board = (LPTGameBoard*)this->getChildByTag(kGameBoardTag);
    return board->getCenterTable();
}
#pragma mark addStriker
/**
 addStriker: This method will striker in gameScene. There are total 10 striker which are equally divided to each player i.e. 5 each. Striker is physics body object consist of density: 100, restitution: 1.0 and friction: 1.0. Striker zOrder is 4.
 -params:
 -void:
 */

void LPTInteractiveTutorialLayer::addStriker() {
    BaseSprite *centerTable = getCenterTable();
    Vec2 basePositionForPlayer = Vec2((centerTable->getParent()->getPosition().x + centerTable->getPosition().x) -  centerTable->getContentSize().width * 0.1 * centerTable->getScale(), (centerTable->getParent()->getPosition().y + centerTable->getPosition().y) - centerTable->getContentSize().height * 0.44 * centerTable->getScale());
    for (int strikerCounter = 0; strikerCounter < 1; strikerCounter++) {
        LPTStriker *striker = LPTStriker::createStriker();
        float scale = (centerTable->getContentSize().height * centerTable->getScale() * 0.14) / striker->getContentSize().height;
        
        striker->setStrikerProperty(1, basePositionForPlayer, true, scale);
        striker->setTag(kStrikerTagBase);
        this->addChild(striker, 1001);
        striker->addBodyToStriker(b2_dynamicBody,world);
            arrayStriker.push_back(striker);
    }
}

#pragma mark addRopeInGameScene
void LPTInteractiveTutorialLayer::addRopeInGameScene(int tag, bool isBulletTrue) {
    BaseSprite *centerTable = getCenterTable();
    float displacementFactor = 0.475;//GameUtils::isIphoneX ? 0.475 : 0.475;   //rope position according device size
    
    Vec2 ropeInitialPosition =  Vec2(centerTable->getParent()->getPosition().x + centerTable->getPosition().x - centerTable->getContentSize().width * displacementFactor * centerTable->getScale(), centerTable->getParent()->getPosition().y + centerTable->getPosition().y - centerTable->getContentSize().height * 0.5 * centerTable->getScale());
    
    if (tag == kOppentRopeLayerTag) {
        ropeInitialPosition = Vec2(centerTable->getParent()->getPosition().x + centerTable->getPosition().x + centerTable->getContentSize().width * displacementFactor * centerTable->getScale(), centerTable->getParent()->getPosition().y + centerTable->getPosition().y - centerTable->getContentSize().height * 0.5 * centerTable->getScale());
    }
    
    LPTRope *rope = LPTRope::createRope();
    rope->setTag(tag);
    rope->setupUI(Size(centerTable->getContentSize().width * 0.02, centerTable->getContentSize().height * centerTable->getScale()), ropeInitialPosition, world);
    tag == kOppentRopeLayerTag ? rope->setPositionX(rope->getPosition().x -  rope->getContentSize().width):rope->setPositionX(rope->getPosition().x);
    this->addChild(rope, 10);
}

#pragma mark - SetupPhysicsWorld
void LPTInteractiveTutorialLayer::createPhysicalWorld()
{
    world = new b2World(b2Vec2(0.0f, 0.0f));
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);
    //Debug Draw
    
    if (DEBUG_DRAW) {
        debugDraw = new GLESDebugDraw(kPixelToMeterRatio);
        world->SetDebugDraw(debugDraw);
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
        debugDraw->SetFlags(flags);
    }
}

#pragma mark Draw Method
void LPTInteractiveTutorialLayer::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformUpdated)
{
    if(world) {
        Layer::draw(renderer, transform, transformUpdated);
        Director* director = Director::getInstance();
        GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
        director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        world->DrawDebugData();
        director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    }
}
#pragma mark - scheduleTick
void LPTInteractiveTutorialLayer::scheduleTick(){
    schedule(schedule_selector(LPTInteractiveTutorialLayer::tick));
}
#pragma mark Tick Method
void LPTInteractiveTutorialLayer::tick(float dt){
    //Update strikers
    updateTouchableStrikers();
    int velocityIterations = 8;
    int positionIterations = 8;
    world->Step(dt, velocityIterations, positionIterations);
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            BaseSprite *sprite = (BaseSprite*)b->GetUserData();
            Vec2 position = Vec2(0, 0);
            
            sprite->setPosition(Vec2((b->GetPosition().x * kPixelToMeterRatio),
                                     (b->GetPosition().y * kPixelToMeterRatio)));
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
            
            this->changeSelectedStrikerVelocity(b);
        }
    }
    
    //TO CREATE A ROPE AT RUNTIME
    this->changeStatusOfRope();
    this->checkStrikerMovementWhenHitWithRope();
    if(checkForWinningCondition()) {
        delayTimer--;
        if(delayTimer <= 0) {
            this->unschedule(schedule_selector(LPTInteractiveTutorialLayer::tick));
            auto popup = LPTPopUp::showPopUpWith(kGameTutorialCompletionAlert, kGameTutorialCompletionText, false, GAMEQUIT_POPUP);
            popup->setPopUpDelegate(this);
            this->addChild(popup, 5000);
        }
    }
}

void LPTInteractiveTutorialLayer::checkStrikerMovementWhenHitWithRope() {
    LPTRope *rope           =   (LPTRope *)this->getChildByTag(kRopeLayerTag);
    LPTRope *opponentRope   =   (LPTRope *)this->getChildByTag(kOppentRopeLayerTag);
    
    for(int strikerCounter = 0; strikerCounter < 1; strikerCounter++) {
        LPTStriker* striker     =   arrayStriker.at(strikerCounter);
        Rect strikerRect    =   striker->getBoundingBox();
        
        
        Rect ropeRect       =   Rect(rope->getPosition().x, rope->getPosition().y, rope->getContentSize().width * 0.5, rope->getContentSize().height);
        Rect opponentRect   =   Rect(opponentRope->getPosition().x + opponentRope->getContentSize().width * 0.5, opponentRope->getPosition().y, opponentRope->getContentSize().width * 0.5, opponentRope->getContentSize().height);

        if(ropeRect.intersectsRect(strikerRect))
        {
            if(!striker->isStrikerDragging && (striker->getPosition().x - striker->spritePreviousPosition.x) < 0) {
                striker->getStrikerBody()->SetLinearVelocity(b2Vec2(-striker->getStrikerBody()->GetLinearVelocity().x * 0.6, striker->getStrikerBody()->GetLinearVelocity().y));
            }else {
                this->addPointingHand(locationOfPointingHand);
            }
        }
        striker->spritePreviousPosition = striker->getPosition();
    }
}
#pragma mark - changeRopeStatus
void LPTInteractiveTutorialLayer::changeStatusOfRope() {
    LPTRope *rope = (LPTRope *)this->getChildByTag(kRopeLayerTag);
    int currentIndex = selectedStrikerIndex;
    rope->updateRopeAccordingToBall = false;
    if (currentIndex >= 0) {
        LPTStriker *striker  = (LPTStriker*) arrayStriker[currentIndex];
        if(striker->isStrikerDragging) {
            rope->updateRopeAccordingToBall = true;
        }
    }
    this->updateRopeTexture(currentIndex, kRopeLayerTag);
}

#pragma mark UPDATE_ROPE_TEXTURE
void LPTInteractiveTutorialLayer::updateRopeTexture(int segment, int ropeTag) {
    LPTRope *rope = (LPTRope *)this->getChildByTag(ropeTag);
    if(segment >= 0) {
        LPTStriker *striker  = (LPTStriker*) arrayStriker[segment];
        Vec2 ropeNewPosition = Vec2(striker->getPosition().x - striker->getContentSize().width * striker->getScale() * 0.475, striker->getPosition().y);
        rope->createRopeTexture(segment, rope->updateRopeAccordingToBall, ropeNewPosition, striker->isStrikerDragging);
    }else {
        rope->createRopeTexture(segment, rope->updateRopeAccordingToBall, Vec2(0, 0), false);
    }
}
#pragma mark updateTouchableStrikers
void LPTInteractiveTutorialLayer::updateTouchableStrikers() {
    Rect userPlayingArea = this->getTableTouchableArea();
    
    userPlayingArea = Rect(userPlayingArea.origin.x, userPlayingArea.origin.y, userPlayingArea.size.width * 1.05, userPlayingArea.size.height);
    
    touchableStrikerCount = 0;
    for(int strikerCounter = 0; strikerCounter < 1; strikerCounter++) {
        LPTStriker* striker = (LPTStriker*) arrayStriker[strikerCounter];
        
        Vec2 strikerPostion = striker->getPosition();
        
        auto strikerCorner1 = strikerPostion;
        auto strikerCorner2 = Vec2(strikerPostion.x + striker->getBoundingBox().size.width,
                                   strikerPostion.y);
        auto strikerCorner3 =Vec2(strikerPostion.x,
                                  strikerPostion.y + striker->getBoundingBox().size.height);
        auto strikerCorner4 = Vec2(strikerPostion.x + striker->getBoundingBox().size.width,
                                   strikerPostion.y + striker->getBoundingBox().size.height);
        if(userPlayingArea.containsPoint(strikerCorner1) ||
           userPlayingArea.containsPoint(strikerCorner2) ||
           userPlayingArea.containsPoint(strikerCorner3) ||
           userPlayingArea.containsPoint(strikerCorner4) ) {
            striker->isStrikerTouchable = true;
            touchableStrikerCount++;
        } else {
            striker->isStrikerTouchable = false;
        }
    }
}
#pragma mark checkForWinningCondition
bool LPTInteractiveTutorialLayer::checkForWinningCondition() {
    if(touchableStrikerCount == 0) {
        return true;
    }
    return false;
}
#pragma mark TableTouchableArea
Rect LPTInteractiveTutorialLayer::getTableTouchableArea() {
    BaseSprite *centerTable = getCenterTable();
    Vec2 centerTablePostion = centerTable->convertToNodeSpace(centerTable->getPosition());
    return  ((LPTGameBoard *)centerTable->getParent())->getCenterTableRectInRespectToGameScene(PLAY_AREA_LEFT);
}

#pragma adjusting Velocity
void LPTInteractiveTutorialLayer::changeSelectedStrikerVelocity(b2Body *body) {
    BaseSprite *sprite = (BaseSprite*)body->GetUserData();
    if(sprite->userDataType == kUserDataTypeStriker) {
        LPTStriker *striker = (LPTStriker*)sprite;
        striker->getStrikerBody()->SetLinearVelocity(b2Vec2(striker->getStrikerBody()->GetLinearVelocity().x * 0.95, 0));
        striker->getStrikerBody()->SetAngularDamping(0.7);
//        striker->dumpStrikerVelocity();
    }
}
#pragma mark - Game popup callabck
void LPTInteractiveTutorialLayer::okButtonAction(Popup_Type type) {
    switch (type) {
        case LOGOUT_POPUP:
            
            break;
        case GAMEQUIT_POPUP:
            APP_MANAGER->setIsTutorialShown(true);
            
            if(backButtonTag != kBackToGameSceneFromTutorialButtonTag) {
                if(PLAYER->getGameSelected() == GAME_WITH_COMPUTER) {
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3,  LPTDifficultySelection::createScene()));
                }else {
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3,  LPTBetSelectionScene::createScene()));
                }
            }
            break;
        default:
            break;
    }
    LPTPopUp * popUp = (LPTPopUp *)this->getChildByTag((int)type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
    
    if(backButtonTag == kBackToGameSceneFromTutorialButtonTag)
        this->removeFromParentAndCleanup(true);
}

void LPTInteractiveTutorialLayer::cancelButtonAction(Popup_Type type) {
    switch (type) {
        case LOGOUT_POPUP:
            
            break;
        case GAMEQUIT_POPUP:

            break;
        case NORMAL_POPUP:
            break;
        default:
            
            break;
    }
    LPTPopUp * popUp = (LPTPopUp *)this->getChildByTag(type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

void LPTInteractiveTutorialLayer::onExit() {
    BaseLayer::onExit();
}
