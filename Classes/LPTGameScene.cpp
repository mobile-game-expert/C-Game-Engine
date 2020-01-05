//
//  LPTGameScene.cpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 13/09/18.
//

#include "LPTGameScene.hpp"
#include "LPTGameBoard.hpp"
#include "LPTRope.hpp"
#include "LPTContactListener.h"
#include "LPTAIMode.hpp"
#include "LPTGameResultStatus.hpp"
#include "LPTGameSelectionScene.hpp"
#include "LPTEvents.hpp"


#define kGameBoardTag                   1001
#define kHUDGameLayer                   1002

#define kMaxPlayerStriker               10
#define kStrikerTagBase                 3000
#define kStrikerTagMax                  3009
#define kMaximumForce                   1000.0f
#define kMinimumForce                   600.0f

#define kCollisionDelayTime             0.001
#define kCollisionDelayTimeForAI        0.25

#define kStrikerShowingFactor           25

#define kUserSelectedSide               PLAY_AREA_LEFT //PLAY_AREA_RIGHT PLAY_AREA_LEFT
#define kWinningAmount                  (GameUtils::getUserBetValueFromSelectedBet(PLAYER->getBetSelected())) * 2

#pragma mark - CreateScene
BaseScene* LPTGameScene::createScene() {
    APP_MANAGER->gameState = GAME_SCENE;
    auto scene = BaseScene::create();
    auto layer = LPTGameScene::create();
    scene->addChild(layer);
    return scene;
}

#pragma mark - SceneMethod
bool LPTGameScene::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    setBackgroundSprite(kGamePlayBG);
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
    warpClientRef->setNotificationListener(this);
    warpClientRef->setRoomRequestListener(this);
    warpClientRef->setZoneRequestListener(this);
    return true;
}

void LPTGameScene::onEnter() {
    BaseLayer::onEnter();
    isResultDeclared = false;
    touchableStrikerCount = 0;
    this->createPhysicalWorld();
    this->setupGameData();
    this->addGameHUDLayer();
    this->addGameTable();
    this->addStriker();
    this->addRopeInGameScene(kRopeLayerTag, false);
    this->addRopeInGameScene(kOppentRopeLayerTag,true);
    startGame();
    this->scheduleTick();
    this->getCenterRectOfTheTable();
    APP_MANAGER->numberOfSPGPerSession = APP_MANAGER->numberOfSPGPerSession + 1;
}

void LPTGameScene::onExit() {
    debugDraw = NULL;
    stopGameTimer();
    LPTEvents::logNumberOfSPG(APP_MANAGER->numberOfSPGPerSession);
    BaseLayer::onExit();
}


#pragma mark - SetupPhysicsWorld
void LPTGameScene::createPhysicalWorld()
{
    world = new b2World(b2Vec2(0.0f, 0.0f));
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);
    LPTContactListener *contactListener = new LPTContactListener(this);
    world->SetContactListener(contactListener);
    
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
void LPTGameScene::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformUpdated)
{
    Layer::draw(renderer, transform, transformUpdated);
    Director* director = Director::getInstance();
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    world->DrawDebugData();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

#pragma mark -Setup Game data
void LPTGameScene::setupGameData() {
    PLAYER->setPlayerGamePlayed(PLAYER->getPlayerGamePlayed() + 1);
    
    if(kUserSelectedSide == PLAY_AREA_LEFT) {
        //User Data
        userUIData.playingArea = PLAY_AREA_LEFT;
        userUIData.RopeTag = kRopeLayerTag;
        
        //Opponent Data
        opponentUIData.playingArea = PLAY_AREA_RIGHT;
        opponentUIData.RopeTag = kOppentRopeLayerTag;
    } else {
        //User Data
        userUIData.playingArea = PLAY_AREA_RIGHT;
        userUIData.RopeTag = kOppentRopeLayerTag;
        
        //Opponent Data
        opponentUIData.playingArea = PLAY_AREA_LEFT;
        opponentUIData.RopeTag = kRopeLayerTag;
    }
}

#pragma mark - HUDLayer
/*
 addGameHUDLayer: This method will add HUDLayer in GameScene.
 HUDLayer contain Timer, Player profile, Player name, Bet coins and a BackButton.
 HUDLayer zORder will remain to higher than GameTable zOrder.
 */
void LPTGameScene::addGameHUDLayer() {
    LPTHUDLayer *layer = LPTHUDLayer::createLayer();
    layer->setHUDDelegate(this);
    layer->setTag(kHUDGameLayer);
    layer->setPosition(Vec2(0, this->getContentSize().height));
    string opponentName = OPPONENT->getUniqueName();
    string opponentImage =OPPONENT->getProfilePic();
    layer->setupUI(PLAYER->getPlayerUniqueName(), PLAYER->getPlayerProfilePicture(), opponentName, opponentImage, kUserSelectedSide);
    layer->setCoinValue(kWinningAmount);
    layer->setAnchorPoint(Vec2(0, 1));
    layer->setTag(kHUDGameLayer);
    this->addChild(layer, 1000);
}

#pragma mark GameTable
/**
 addGameTable: This method will add gameTable in gameScene. GameTable is custom table which consist table carpet, edges of the table, and deflector of the table (which divides table into two equal parts). Table is a physics body.
 -params:
 -void:
 */
//void LPTGameScene::addGameTable() {
//    LPTHUDLayer *layer = (LPTHUDLayer *)this->getChildByTag(kHUDGameLayer);
//    LPTGameBoard *gameBoard = LPTGameBoard::createLayer();
//    gameBoard->setUpGameBoardUI(Size(this->getContentSize().width, this->getContentSize().height - layer->getContentSize().height), Vec2(0, 0), kCenterTable_1_ImageName, kBoard_1_CornerImageName, kBoardDividerImageName, world);
//    gameBoard->setTag(kGameBoardTag);
//    this->addChild(gameBoard, 1);
//}

void LPTGameScene::addGameTable() {
    Size gameBoardSize = GameUtils::isIphoneX ? Size(1600, 800) : Size(900, 450);
    LPTHUDLayer *layer = (LPTHUDLayer *)this->getChildByTag(kHUDGameLayer);
    LPTGameBoard *gameBoard = LPTGameBoard::createLayer();
    
    tablePositionInX = (this->getContentSize().width - gameBoardSize.width) / 2;
    float heightLeft = this->getContentSize().height - layer->getContentSize().height;
    tablePositionInY = (heightLeft - gameBoardSize.height) * 0.5;
    
    log("Position: %f == %f", tablePositionInX, tablePositionInY);
    gameBoard->setUpGameBoardUI(gameBoardSize, Vec2(tablePositionInX, tablePositionInY), kCenterTable_1_ImageName, kBoard_1_CornerImageName, kBoardDividerImageName, world);
    gameBoard->setTag(kGameBoardTag);
    this->addChild(gameBoard, 1);
}




BaseSprite* LPTGameScene::getCenterTable() {
    LPTGameBoard *board = (LPTGameBoard*)this->getChildByTag(kGameBoardTag);
    return board->getCenterTable();
}

#pragma mark addStriker
/**
 addStriker: This method will striker in gameScene. There are total 10 striker which are equally divided to each player i.e. 5 each. Striker is physics body object consist of density: 100, restitution: 1.0 and friction: 1.0. Striker zOrder is 4.
 -params:
 -void:
 */

void LPTGameScene::addStriker() {
    BaseSprite *centerTable = getCenterTable();
    Vec2 basePositionForPlayer = Vec2((centerTable->getParent()->getPosition().x + centerTable->getPosition().x) -  centerTable->getContentSize().width * 0.1 * centerTable->getScale(), (centerTable->getParent()->getPosition().y + centerTable->getPosition().y) - centerTable->getContentSize().height * 0.44 * centerTable->getScale());
    int signMultiplier = 1;
    bool isPlayerStriker = userUIData.playingArea == PLAY_AREA_LEFT ? true : false;
    int opponentTagMedian = 4;
    for (int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
        LPTStriker *striker = LPTStriker::createStriker();
        //This Code line will change Player Striker to Opposition Striker along with striker property
        if(strikerCounter % 5 == 0 && strikerCounter > 0) {
            isPlayerStriker = !isPlayerStriker;
            signMultiplier = -1;
            basePositionForPlayer = Vec2((centerTable->getParent()->getPosition().x + centerTable->getPosition().x) +  centerTable->getContentSize().width * 0.04 * centerTable->getScale(), (centerTable->getParent()->getPosition().y + centerTable->getPosition().y) + centerTable->getContentSize().height * 0.44 * centerTable->getScale());
            opponentTagMedian = -1;
        }
        opponentTagMedian += 1;
        //Striker property
        float scale = (centerTable->getContentSize().height * centerTable->getScale() * 0.14) / striker->getContentSize().height;
        striker->setStrikerProperty(1, basePositionForPlayer, isPlayerStriker, scale);
        //        PLAYER->isAdmin ? striker->setTag(kStrikerTagBase + strikerCounter) : striker->setTag(kStrikerTagBase + opponentTagMedian);
        striker->setTag(kStrikerTagBase + strikerCounter);
        this->addChild(striker, 11);
        striker->addBodyToStriker(b2_dynamicBody,world);
        //==
        //        b2BodyData *bodyData = new b2BodyData;
        //        bodyData->bodyId = strikerCounter;
        //        striker->getStrikerBody()->SetUserData(bodyData);
        //==
        arrayStriker.push_back(striker);
        basePositionForPlayer.x = basePositionForPlayer.x - (striker->getContentSize().width * striker->getScale() * signMultiplier);
    }
}



//void LPTGameScene::addStriker() {
//    BaseSprite *centerTable = getCenterTable();
//    Vec2 basePositionForPlayer = Vec2(centerTable->getPosition().x -  centerTable->getContentSize().width * 0.04 * centerTable->getScale(), centerTable->getPosition().y - centerTable->getContentSize().height * 0.45 * centerTable->getScale());
//    int signMultiplier = 1;
//    bool isPlayerStriker = userUIData.playingArea == PLAY_AREA_LEFT ? true : false;
//    int opponentTagMedian = 4;
//    for (int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
//
//        LPTStriker *striker = LPTStriker::createStriker();
//        //This Code line will change Player Striker to Opposition Striker along with striker property
//        if(strikerCounter % 5 == 0 && strikerCounter > 0) {
//            isPlayerStriker = !isPlayerStriker;
//            signMultiplier = -1;
//            basePositionForPlayer = Vec2(centerTable->getPosition().x +  centerTable->getContentSize().width * 0.04 * centerTable->getScale(), centerTable->getPosition().y + centerTable->getContentSize().height * 0.45 * centerTable->getScale());
//            opponentTagMedian = -1;
//        }
//        opponentTagMedian += 1;
//        //Striker property
//        float scale = (centerTable->getContentSize().height * centerTable->getScale() * 0.14) / striker->getContentSize().height;
//        striker->setStrikerProperty(1, basePositionForPlayer, isPlayerStriker, scale);
//        PLAYER->isAdmin ? striker->setTag(kStrikerTagBase + strikerCounter) : striker->setTag(kStrikerTagBase + opponentTagMedian);
//        this->addChild(striker, 11);
//
//        striker->addBodyToStriker(b2_dynamicBody,world);
//        arrayStriker.push_back(striker);
//        basePositionForPlayer.x = basePositionForPlayer.x - (striker->getContentSize().width * striker->getScale() * signMultiplier);
//    }
//}

#pragma mark - changeRopeStatus
void LPTGameScene::changeStatusOfRope() {
    LPTRope *rope = (LPTRope *)this->getChildByTag(kRopeLayerTag);
    int currentIndex = userUIData.playingArea == PLAY_AREA_LEFT ? selectedStrikerIndex : opponentStrikerIndex;
    if(collisionBetweenRopeAndStrikerSarted) {
        rope->updateRopeAccordingToBall = false;
        if (currentIndex >= 0) {
            LPTStriker *striker  = (LPTStriker*) arrayStriker[currentIndex];
            if(striker->isStrikerDragging) {
                rope->updateRopeAccordingToBall = true;
            }else {
                collisionBetweenRopeAndStrikerSarted = false;
            }
        }else {
            collisionBetweenRopeAndStrikerSarted = false;
        }
    }else {
        rope->updateRopeAccordingToBall = false;
    }
    this->updateRopeTexture(currentIndex, kRopeLayerTag);
}

void LPTGameScene::changeStatusOfRopeForOpponent() {
    LPTRope *rope = (LPTRope *)this->getChildByTag(kOppentRopeLayerTag);
    int currentIndex = userUIData.playingArea == PLAY_AREA_LEFT ? opponentStrikerIndex : selectedStrikerIndex;
    if(collisionBetweenRopeAndStrikerStartedForOpponent) {
        rope->updateRopeAccordingToBall = false;
        if (currentIndex >= 0) {
            LPTStriker *striker  = (LPTStriker*) arrayStriker[currentIndex];
            if(striker->isStrikerDragging) {
                rope->updateRopeAccordingToBall = true;
            }else {
                collisionBetweenRopeAndStrikerStartedForOpponent = false;
            }
        }else {
            collisionBetweenRopeAndStrikerStartedForOpponent = false;
        }
    }else {
        rope->updateRopeAccordingToBall = false;
    }
    this->updateRopeTexture(currentIndex, kOppentRopeLayerTag);
}

#pragma mark UPDATE_ROPE_TEXTURE
void LPTGameScene::updateRopeTexture(int segment, int ropeTag) {
    LPTRope *rope = (LPTRope *)this->getChildByTag(ropeTag);
    if(segment >= 0) {
        LPTStriker *striker  = (LPTStriker*) arrayStriker[segment];
        Vec2 ropeNewPosition = Vec2(striker->getPosition().x - striker->getContentSize().width * striker->getScale() * 0.475, striker->getPosition().y);
        if(ropeTag == kOppentRopeLayerTag)
            ropeNewPosition = Vec2(striker->getPosition().x + striker->getContentSize().width * striker->getScale() * 0.475, striker->getPosition().y);
        
        rope->createRopeTexture(segment, rope->updateRopeAccordingToBall, ropeNewPosition, striker->isStrikerDragging);
    }else {
        rope->createRopeTexture(segment, rope->updateRopeAccordingToBall, Vec2(0, 0), false);
    }
}

#pragma mark addRopeInGameScene
void LPTGameScene::addRopeInGameScene(int tag, bool isBulletTrue) {
    BaseSprite *centerTable = getCenterTable();
    float displacementFactor = 0.475;//GameUtils::isIphoneX ? 0.475 : 0.475;   //rope position according device size
    
    Vec2 ropeInitialPosition =  Vec2(centerTable->getParent()->getPosition().x + centerTable->getPosition().x - centerTable->getContentSize().width * displacementFactor * centerTable->getScale(), centerTable->getParent()->getPosition().y + centerTable->getPosition().y - centerTable->getContentSize().height * 0.5 * centerTable->getScale());

    if (tag == kOppentRopeLayerTag) {
        ropeInitialPosition = Vec2(centerTable->getParent()->getPosition().x + centerTable->getPosition().x + centerTable->getContentSize().width * displacementFactor * centerTable->getScale(), centerTable->getParent()->getPosition().y + centerTable->getPosition().y - centerTable->getContentSize().height * 0.5 * centerTable->getScale());
    }
    
    LPTRope *rope = LPTRope::createRope();
    rope->setTag(tag);
    rope->initialPosition = ropeInitialPosition;
    rope->setupUI(Size(centerTable->getContentSize().width * 0.02, centerTable->getContentSize().height * centerTable->getScale()), ropeInitialPosition, world);
    tag == kOppentRopeLayerTag?rope->setPositionX(rope->getPosition().x -  rope->getContentSize().width):rope->setPositionX(rope->getPosition().x);
    rope->createRopeSegment(world, isBulletTrue);
    this->addChild(rope, 10);
    rope->createRopeTexture(selectedStrikerIndex, createRopeAccordingToBall, Vec2(0, 0), false);
    rope->createRopeJoint(world);
    rope->changeLinearDamping();
}



#pragma mark Update GameScene UI
void LPTGameScene::updateUI() {
    LPTHUDLayer * layer = (LPTHUDLayer*)this->getChildByTag(kHUDGameLayer);
    layer->setCoinValue(PLAYER->getTotalCoins());
}

#pragma mark - Game Timer Methods
void LPTGameScene::startGame() {
    this->addTouchListener();
    this->schedule(schedule_selector(LPTGameScene::updateGameTimer), 1.0); // Temporary
    
    //Check if AI mode selected or not
    if(PLAYER->getGameSelected() == GAME_WITH_COMPUTER) { //It will change according to selection
        UserPlaySide opponentSide = kUserSelectedSide == PLAY_AREA_LEFT ? PLAY_AREA_RIGHT : PLAY_AREA_LEFT;
        float seconds = LPTAIMode::sharedInstance(opponentSide)->nextMoveTime();
        this->scheduleOnce(schedule_selector(LPTGameScene::playAIMovement),seconds);
    }
}

void LPTGameScene::updateGameTimer(float delta) {
    gameTimeInSeconds++;
    int minutes = gameTimeInSeconds / 60;
    int seconds = gameTimeInSeconds % 60;
    
    string displayTime = StringUtils::format("%02d : %02d",minutes, seconds);
    
    LPTHUDLayer * layer = (LPTHUDLayer*)this->getChildByTag(kHUDGameLayer);
    layer->updateTimerLabel(displayTime);
}
/**
 stopGameTimer : This method will unschedule the game timer and remove physics bodies from the striker to stop their movement.
 */
void LPTGameScene::stopGameTimer() {
    gameTimeInSeconds = 0;
    touchableStrikerCount = 0;
    this->unschedule(schedule_selector(LPTGameScene::updateGameTimer));
    this->unschedule(schedule_selector(LPTGameScene::playAIMovement));
    this->unschedule(schedule_selector(LPTGameScene::tick));
    for(int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
        LPTStriker* striker = arrayStriker.at(strikerCounter);
        striker->isStrikerTouchable = false;
    }
    LPTAIMode::sharedInstance()->removeSharedInstance();
}

#pragma mark - scheduleTick
void LPTGameScene::scheduleTick(){
    schedule(schedule_selector(LPTGameScene::tick));
}
#pragma mark Tick Method
void LPTGameScene::tick(float dt){
    //Update strikers
    if(!isResultDeclared)
        updateTouchableStrikers();
    
    int velocityIterations = 8;
    int positionIterations = 8;
    world->Step(dt, velocityIterations, positionIterations);
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            BaseSprite *sprite = (BaseSprite*)b->GetUserData();
            Vec2 position = Vec2(0, 0);
            
            //This will add to normalise the position
//            if(sprite->getType() == kUserDataTypeRope) {
//                LPTRope *rope = (LPTRope *)this->getChildByTag(kRopeLayerTag);
//                if(rope != NULL)
//                    position = Vec2(rope->getPosition().x, rope->getPosition().y);
//            }
            sprite->setPosition(Vec2((b->GetPosition().x * kPixelToMeterRatio - position.x),
                                     (b->GetPosition().y * kPixelToMeterRatio - position.y)));
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
            
            this->changeSelectedStrikerVelocity(b);
        }
    }
    
    //TO CREATE A ROPE AT RUNTIME
    this->changeStatusOfRope();
    this->changeStatusOfRopeForOpponent();
    
    if( aiDraggableStriker  != NULL && opponentStrikerIndex >= 0) {
        LPTStriker *striker = arrayStriker.at(opponentStrikerIndex);
        striker->getStrikerBody()->SetTransform(b2Vec2(aiDraggableStriker->getPosition().x / kPixelToMeterRatio, (aiDraggableStriker->getPosition().y) / kPixelToMeterRatio), striker->getStrikerBody()->GetAngle());
    }
}

void LPTGameScene::changeSelectedStrikerVelocity(b2Body *body) {
    BaseSprite *sprite = (BaseSprite*)body->GetUserData();
    if(sprite->userDataType == kUserDataTypeStriker) {
        LPTStriker *striker = (LPTStriker*)sprite;
        striker->maintainXVelocity();
        striker->dumpStrikerVelocity();
        striker->getStrikerBody()->SetLinearVelocity(b2Vec2(striker->getStrikerBody()->GetLinearVelocity().x, striker->getStrikerBody()->GetLinearVelocity().y * 0.9)); // * 0.4
    }
}

#pragma mark - Opponent
/**
 * Update Opponent Player UI
 *
 */
void LPTGameScene::updateOpponentUI(int strikerIndex, Vec2 positionOnRope, b2Vec2 impulse) {
    /*
    for (int i = 0; i < arrayStriker.size(); i++) {
        LPTStriker *striker = arrayStriker.at(i);
        if (striker->getTag() == strikerTag) {
            opponentStrikerIndex = i;
            break;
        }
    }*/
    if(aiDraggableStriker == NULL){
        opponentStrikerIndex = getStrikerIndexForAI();
        if (opponentStrikerIndex >= 0 ) {
            LPTStriker *striker = arrayStriker.at(opponentStrikerIndex);
            striker->removeBodyFromStriker();
            striker->isStrikerDragging = true;
            striker->playedByMe = true; 
            striker->addBodyToStriker(b2_staticBody, world);
            striker->addPointingHand();
            //Striker sprite which moves the physics body
            aiDraggableStriker = LPTStriker::create();
            aiDraggableStriker->setPosition( striker->getPosition());
            aiDraggableStriker->setOpacity(255);
            this->getCenterTable()->addChild(aiDraggableStriker,100);
            
            
            
            LPTRope *rope = (LPTRope *)this->getChildByTag(opponentUIData.RopeTag);
            int direction = kUserSelectedSide == PLAY_AREA_LEFT ? 1 : -1;
            Vec2 ropePosition = Vec2(rope->getPosition().x + direction * rope->getContentSize().width * 0.5,rope->getPositionY());
            Vec2 movementPosition = Vec2(ropePosition.x - LPTAIMode::sharedInstance()->aiModeProperties.positionOnRope.x,
                                         (ropePosition.y + positionOnRope.y));
            
            auto removeStriker = CallFunc::create([this, striker, rope, impulse](){
                if(aiDraggableStriker != NULL) {
                    this->aiDraggableStriker->removeFromParentAndCleanup(true);
                    this->aiDraggableStriker = NULL;
                    striker->removeBodyFromStriker();
                    striker->removePointingHand();
                    striker->addBodyToStriker(b2_dynamicBody, world); // Properties set to 0.5
                    collisionBetweenRopeAndStrikerStartedForOpponent = false;
                    rope->updateRopeAccordingToBall = false;
                    auto mass = striker->getStrikerBody()->GetMass();
                    striker->getStrikerBody()->ApplyLinearImpulseToCenter(b2Vec2(-mass * impulse.x, impulse.y), true);
                    striker->isStrikerDragging = false;
                }
            });
            auto seq = Sequence::create(MoveTo::create(1, movementPosition), DelayTime::create(kCollisionDelayTimeForAI), removeStriker, NULL);
            aiDraggableStriker->runAction(seq);
        }
    }
}
 

#pragma mark - AI in case on single player
void LPTGameScene::playAIMovement(float delta) {

    LPTAIMode::sharedInstance()->setupAIProperties();
    if(aiDraggableStriker == NULL) {
        opponentStrikerIndex = getStrikerIndexForAI();
        if (opponentStrikerIndex >= 0) {
            LPTStriker *striker = arrayStriker.at(opponentStrikerIndex);
            bool ballIsInCenter = getCenterRectOfTheTable().intersectsRect(striker->getBoundingBox());
//            containsPoint(striker->getPosition());
            
            if(!ballIsInCenter) {
                striker->removeBodyFromStriker();
                striker->isStrikerDragging = true;
                striker->playedByMe = false;
                striker->addBodyToStriker(b2_staticBody, world);
                striker->addPointingHand();
                //Striker sprite which moves the physics body
                aiDraggableStriker = LPTStriker::create();
                aiDraggableStriker->setPosition( striker->getPosition());
                aiDraggableStriker->setOpacity(255);
                this->getCenterTable()->addChild(aiDraggableStriker,100);
                
                LPTRope *rope = (LPTRope *)this->getChildByTag(opponentUIData.RopeTag);
                int direction = kUserSelectedSide == PLAY_AREA_LEFT ? 1 : -1;
                Vec2 ropePosition = Vec2(rope->getPosition().x + direction * rope->getContentSize().width * 0.475, rope->getPositionY());
                Vec2 movementPosition = Vec2(ropePosition.x - LPTAIMode::sharedInstance()->aiModeProperties.positionOnRope.x,
                                             (ropePosition.y + rope->getContentSize().height * 0.5) + LPTAIMode::sharedInstance()->aiModeProperties.positionOnRope.y);
                
                //changing status of the status...
                //            auto strikerDraggingStatus = CallFunc::create([this, striker](){
                //                striker->isStrikerDragging = false;
                //            });
                //
                auto removeStriker = CallFunc::create([this, striker, rope](){
                    if(aiDraggableStriker != NULL) {
                        this->aiDraggableStriker->removeFromParentAndCleanup(true);
                        this->aiDraggableStriker = NULL;
                        striker->removeBodyFromStriker();
                        striker->removePointingHand();
                        striker->addBodyToStriker(b2_dynamicBody, world); // Properties set to 0.5
                        collisionBetweenRopeAndStrikerStartedForOpponent = false;
                        rope->updateRopeAccordingToBall = false;
                        auto mass = striker->getStrikerBody()->GetMass();
                        log("Striker tag Enabled : %d", striker->getTag());
                        if (LPTAIMode::sharedInstance()->aiModeProperties.direction == -1) {
                            striker->getStrikerBody()->SetTransform(b2Vec2(((rope->initialPosition.x - striker->getContentSize().width*0.4) / kPixelToMeterRatio), striker->getStrikerBody()->GetPosition().y), striker->getStrikerBody()->GetAngle());
                        }else{
                            striker->getStrikerBody()->SetTransform(b2Vec2(((rope->initialPosition.x + striker->getContentSize().width*0.4) / kPixelToMeterRatio), striker->getStrikerBody()->GetPosition().y), striker->getStrikerBody()->GetAngle());
                        }
                        striker->getStrikerBody()->ApplyLinearImpulseToCenter(b2Vec2(-mass * LPTAIMode::sharedInstance()->aiModeProperties.impluse.x, striker->getStrikerBody()->GetLinearVelocity().y * 0.0), true);
                        //                    striker->isStrikerDragging = false;
                        striker->runAction(Sequence::create(DelayTime::create(1.5), CallFunc::create([this, striker](){
                            striker->isStrikerDragging = false;
                            log("Striker tag disabled : %d", striker->getTag());
                        }), NULL));
                    }
                });
                //
                GAME_MODE mode = (GAME_MODE)UserDefault::getInstance()->getIntegerForKey(kAIMode, BEGINNER);
                auto seq = Sequence::create(MoveTo::create(2.0 / (mode + 1), movementPosition), DelayTime::create(kCollisionDelayTimeForAI), removeStriker, NULL);
                aiDraggableStriker->runAction(seq);
            }
        }
    }
    float seconds = (float)LPTAIMode::sharedInstance()->nextMoveTime();
    this->scheduleOnce(schedule_selector(LPTGameScene::playAIMovement),seconds);
}

bool LPTGameScene::checkIfStrikerIsRightWay(LPTStriker *striker) {
    if(fabs(striker->getStrikerBody()->GetLinearVelocity().x) > 20) {
        return true;
    } else {
        return false;
    }
}
#pragma mark RECT
/*
 This method will return the rect of the hole in the table.
 We need this RECT as we have to restrict AI player to drag the ball from the center of the table.
 */
Rect LPTGameScene::getCenterRectOfTheTable() {
    BaseSprite *centerTable = getCenterTable();
    
    float width     =   centerTable->getContentSize().width * 0.003;
    float height    = 	centerTable->getContentSize().height * 0.015;
    float posX      =   this->getContentSize().width * 0.5 - width * 0.5;
    float posY      =   centerTable->getParent()->getPosition().y + centerTable->getContentSize().height * 0.575 * centerTable->getScale();
    
//    LayerColor *layer = LayerColor::create(Color4B::WHITE, width, height);
//    layer->setPosition(Vec2(posX, posY));
//    layer->setOpacity(100);
//    this->addChild(layer, 2000);
    
    return Rect(posX, posY, width, height);
}

#pragma mark StrikerIndex
int LPTGameScene::getStrikerIndexForAI() {
    bool gotIndex = false;
    int indexCheck = 0;
    int selectedIndex = -1;
    while (!gotIndex && arrayOpponentStrikersTag.size() != 0) {
        int strikerRandomIndex = arrayOpponentStrikersTag.at(arc4random()%arrayOpponentStrikersTag.size());
        LPTStriker *striker = arrayStriker.at(strikerRandomIndex);
        bool isRightWay = checkIfStrikerIsRightWay(striker);
        
       
//        if(ballIsInCenter)
//            log("Hi");
        /*
         ballIsInCenter variable detect whether ball is in center of the table or not. If this found true we will not send the index of that ball for dragging.
        */
        if(striker->isStrikerTouchable == false && !striker->isStrikerDragging && !isRightWay) {
            gotIndex = true;
            selectedIndex = strikerRandomIndex;
            break;
        }
        if (indexCheck == 5) {
            gotIndex = true;
        }
        indexCheck++;
    }
    return selectedIndex;
}
#pragma mark - TouchListener
void LPTGameScene::addTouchListener() {
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(LPTGameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(LPTGameScene::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(LPTGameScene::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(LPTGameScene::onTouchCancelled, this);
    SoundManager::playSound(SCENE_CHANGE_SOUND);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool LPTGameScene::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 position = touch->getLocation();
    bool isStrikerFound     =   this->checkForTouchableArea(position);
    selectedStrikerIndex    =   touchedStrikerIndex;
    
    if(selectedStrikerIndex >= 0 && isStrikerFound) {
        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
        striker->removeBodyFromStriker();
        striker->isStrikerDragging = true;
        striker->isStrikerHitFromCenter = false;
        striker->addBodyToStriker(b2_staticBody, world);
    }
    return isStrikerFound;
}

void LPTGameScene::onTouchMoved(Touch* touch, Event* event) {
    BaseSprite *centerTable = getCenterTable();
    Vec2 touchPoint = touch->getLocation(); //centerTable->convertToNodeSpace(touch->getLocation());
    Rect touchableArea = this->getTableTouchableArea();
    
    if(selectedStrikerIndex >= 0) {
        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
        Vec2 touchPrevLocation = touch->getPreviousLocation(); //centerTable->convertToNodeSpace(touch->getPreviousLocation());
        Vec2 movementDirection = touchPrevLocation - touchPoint;
        if(isStrikerInsideTouchableArea(striker, touchPoint, touchableArea)) {//touchableArea.containsPoint(touchPoint)
//            striker->getStrikerBody()->SetTransform(b2Vec2((touch->getLocation().x) / kPixelToMeterRatio, (touch->getLocation().y) / kPixelToMeterRatio), 0);
            transformStrikerBody(striker, touch->getLocation().x, touch->getLocation().y, touch, event);
        }else {
            if(checkFeasiblePositionOfStrikerY(touchPoint, touchableArea)) {
                //If left side touch move outside the area.
                float positionInX = touchPoint.x;
                if(touchPoint.x < (touchableArea.origin.x + striker->getContentSize().width * 0.5 * striker->getScale())) {
                    //If left side touch move outside the area.
                    positionInX = touchableArea.origin.x + striker->getContentSize().width * 0.5 * striker->getScale();
//                    striker->getStrikerBody()->SetTransform(b2Vec2(positionInX / kPixelToMeterRatio, (touch->getLocation().y) / kPixelToMeterRatio), 0);
                    transformStrikerBody(striker, positionInX, touch->getLocation().y, touch, event);
                }
                
                if (positionInX > (touchableArea.origin.x + touchableArea.size.width - (striker->getContentSize().width * striker->getScale()))) {
                    // If right side of the touch move outside the area.
                    positionInX = touchableArea.origin.x + touchableArea.size.width - (striker->getContentSize().width * 0.5 * striker->getScale());
//                    striker->getStrikerBody()->SetTransform(b2Vec2(positionInX / kPixelToMeterRatio, (touch->getLocation().y) / kPixelToMeterRatio), 0);
                    transformStrikerBody(striker, positionInX, touch->getLocation().y, touch, event);
                }
            }
            
            if(checkFeasiblePositionOfStrikerX(touchPoint, touchableArea)) {
                float positionInY = touchPoint.y;
                if(touchPoint.y < (touchableArea.origin.y + striker->getContentSize().width * 0.5 * striker->getScale())) {
                    //If touch point moves below the TableArea
                    positionInY = touchableArea.origin.y + (striker->getContentSize().width * 0.5 * striker->getScale());
//                    striker->getStrikerBody()->SetTransform(b2Vec2(touchPoint.x / kPixelToMeterRatio, positionInY / kPixelToMeterRatio), 0);
                    transformStrikerBody(striker, touchPoint.x, positionInY, touch, event);
                }
                
                if(touchPoint.y > (touchableArea.origin.y + touchableArea.size.height - (striker->getContentSize().width * 0.5 * striker->getScale()))) {
                    //If touch point moves above the TableArea
                    positionInY = touchableArea.origin.y + touchableArea.size.height - (striker->getContentSize().width * 0.5 * striker->getScale());
//                    striker->getStrikerBody()->SetTransform(b2Vec2(touchPoint.x / kPixelToMeterRatio, positionInY / kPixelToMeterRatio), 0);
                    transformStrikerBody(striker, touchPoint.x, positionInY, touch, event);
                }
            }
        }
        striker->isStrikerDragging = true;
    }
}


//void LPTGameScene::onTouchMoved(Touch* touch, Event* event) {
//    BaseSprite *centerTable = getCenterTable();
//    Vec2 touchPoint = touch->getLocation(); // centerTable->convertToNodeSpace(touch->getLocation());
//    Rect touchableArea = this->getTableTouchableArea();
//    if(selectedStrikerIndex >= 0) {
//        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
//        Vec2 touchPrevLocation = touch->getPreviousLocation(); //centerTable->convertToNodeSpace(touch->getPreviousLocation());
//        Vec2 movementDirection = touchPrevLocation - touchPoint;
//        if(isStrikerInsideTouchableArea(striker, touchPoint, touchableArea)) {//touchableArea.containsPoint(touchPoint)
//            striker->getStrikerBody()->SetTransform(b2Vec2((touch->getLocation().x) / kPixelToMeterRatio, (touch->getLocation().y) / kPixelToMeterRatio), 0);
////            transformStrikerBody(striker, touch->getLocation().x, touch->getLocation().y, touch, event);
//        } else {
//            if(checkFeasiblePositionOfStrikerY(touchPoint, touchableArea)) {
//                //This signValue will adjust the striker position to the end of board when player touches moves outside the gameboard.
//                int signValue = 1;
//                if(kUserSelectedSide == PLAY_AREA_RIGHT)
//                    signValue = -1;
//
//                //This will check if user touch location is behind the board then only we have to stretch the rope at max slant value.
//                float positionInX = striker->getPositionX();
//                if (touchPoint.x < touchableArea.origin.x && kUserSelectedSide == PLAY_AREA_LEFT) {
//                    positionInX = centerTable->getPositionX() - (centerTable->getContentSize().width * 0.5 * centerTable->getScale() * signValue) + (striker->getContentSize().width * 0.52 * striker->getScale() * signValue);
//                }
//                striker->getStrikerBody()->SetTransform(b2Vec2(positionInX / kPixelToMeterRatio, (touch->getLocation().y) / kPixelToMeterRatio), true);
////                transformStrikerBody(striker, positionInX, touch->getLocation().y,touch, event);
//            }
//            if(checkFeasiblePositionOfStrikerX(touchPoint, touchableArea)) {
//                //===
//                float positionInY = striker->getPositionY();
//                float positionInX = touch->getLocation().x;
//                if (positionInX < (touchableArea.origin.x + (striker->getContentSize().width * striker->getScale()))) {
//                    positionInX = touchableArea.origin.x + (striker->getContentSize().width * striker->getScale());
//                }
//                if ((touchPoint.y - (striker->getContentSize().height * 0.5 * striker->getScale())) < touchableArea.origin.y) {
//                    positionInY = centerTable->getPositionY() - (centerTable->getContentSize().height * 0.5 * centerTable->getScale()) + (striker->getContentSize().height * 0.5 * striker->getScale());
//                }else if(touchPoint.y > centerTable->getContentSize().height) {
//                    positionInY = centerTable->getPositionY() + (centerTable->getContentSize().height * 0.5 * centerTable->getScale()) - (striker->getContentSize().height * 0.5 * striker->getScale());
//                }
//                striker->getStrikerBody()->SetTransform(b2Vec2(positionInX / kPixelToMeterRatio, positionInY / kPixelToMeterRatio), true);
////                transformStrikerBody(striker, positionInX, positionInY, touch, event);
//            }
//        }
//
//    }
//}

void LPTGameScene::onTouchEnded(Touch* touch, Event* event)
{
    BaseSprite *centerTable = getCenterTable();
    Vec2 touchPoint = touch->getLocation();//centerTable->convertToNodeSpace(touch->getLocation());
    
    if(selectedStrikerIndex >= 0) {
        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
        striker->removeBodyFromStriker();
        striker->addBodyToStriker(b2_dynamicBody, world);        
        LPTRope *ropeObject = (LPTRope *) this->getChildByTag(userUIData.RopeTag);
        log("## TOUCH _END ROPE POSITION : %f", ropeObject->ropeExtensionValue);
        //=== Set y velocity reducer
        int direction = kUserSelectedSide == PLAY_AREA_LEFT ? 1 : -1;
        Vec2 ropePosition = Vec2(ropeObject->getPosition().x + direction * ropeObject->getContentSize().width * 0.5, ropeObject->getPositionY());
        float maxR = (ropePosition.y + ropeObject->getContentSize().height * 0.6);
        float minR = (ropePosition.y + ropeObject->getContentSize().height * 0.4);
        
        if (striker->getPosition().y > minR && striker->getPosition().y < maxR) {
            striker->isStrikerHitFromCenter = true;
        }else {
            striker->isStrikerHitFromCenter = false;
        }
        //===
        auto hitStriker = CallFunc::create([ropeObject, striker, this]() {
            ropeObject->resetAllRopeSegmentToInitialPosition();
//            collisionBetweenRopeAndStrikerSarted = false;
            ropeObject->updateRopeAccordingToBall = false;
            selectedStrikerIndex = -1;
            touchedStrikerIndex = -1;
            
            //ADD FORCE AT TOUCH END TO MOVE STRIKER FASTER....
            if(striker->getStrikerBody()->GetLinearVelocity().x != 0 && striker->getStrikerBody()->GetLinearVelocity().y != 0 && striker->isStrikerDragging) {
                float ratio = striker->getStrikerBody()->GetLinearVelocity().y / striker->getStrikerBody()->GetLinearVelocity().x;
                float additionalForce = GameUtils::isIphoneX ? 10000 : 14.0;
                auto force = Vec2(striker->getStrikerBody()->GetLinearVelocity().x + additionalForce, striker->getStrikerBody()->GetLinearVelocity().y + (additionalForce  * ratio));
                striker->getStrikerBody()->ApplyLinearImpulseToCenter(b2Vec2(force.x,  0.0), true);
                selectedStrikerPreviousPosition  = striker->getPosition().x;
            }else {
                striker->getStrikerBody()->ApplyLinearImpulseToCenter(b2Vec2(striker->getStrikerBody()->GetLinearVelocity().x * (GameUtils::isIphoneX?1700:13.0),  striker->getStrikerBody()->GetLinearVelocity().y), true);
            }
            striker->isStrikerDragging = false;
            striker->playedByMe = true;
        });
        if (striker->getPositionX() < ropePosition.x) {
            this->runAction(Sequence::create(DelayTime::create(kCollisionDelayTime), hitStriker, NULL));
        }
    }
}

void LPTGameScene::onTouchCancelled(Touch* touch, Event* event) {
    onTouchEnded(touch,event);
}

#pragma mark Striker movement related methods
void LPTGameScene::transformStrikerBody(LPTStriker *striker, float xPos, float yPos,Touch* touch, Event* event) {
    float peakX = this->getTableTouchableArea().origin.x + this->getTableTouchableArea().size.width*0.7;
    (GameUtils::isIphone || GameUtils::isIpad )?(peakX = peakX * 0.8) : (peakX = peakX);
    if(striker->getStrikerBody()->GetPosition().x > peakX / kPixelToMeterRatio) {
        if(touch->getPreviousLocation().x > xPos) {
            if(xPos / kPixelToMeterRatio > striker->getStrikerBody()->GetPosition().x) {
                striker->getStrikerBody()->SetTransform(b2Vec2(striker->getStrikerBody()->GetPosition().x, yPos / kPixelToMeterRatio), true);
            }else {
                striker->getStrikerBody()->SetTransform(b2Vec2(xPos / kPixelToMeterRatio, yPos / kPixelToMeterRatio), true);
            }
        }else {
            striker->getStrikerBody()->SetTransform(b2Vec2(striker->getStrikerBody()->GetPosition().x, yPos / kPixelToMeterRatio), true);
        }
        
    }else if(xPos >= peakX) {
        striker->getStrikerBody()->SetTransform(b2Vec2(peakX / kPixelToMeterRatio, yPos / kPixelToMeterRatio), true);
    }else {
        striker->getStrikerBody()->SetTransform(b2Vec2(xPos / kPixelToMeterRatio, yPos / kPixelToMeterRatio), true);
    }    
}

bool LPTGameScene::checkForTouchableArea(Vec2 touch) {
    BaseSprite *centerTable = getCenterTable();
    Vec2 touchPoint = touch; //centerTable->convertToNodeSpace(touch);
    //    Vec2 centerTablePostion = centerTable->convertToNodeSpace(centerTable->getPosition());
    Rect touchableArea = this->getTableTouchableArea();
    
    //To check for board touchable area
    if(touchableArea.containsPoint(touchPoint)) {
        //Check for striker touched
        for(int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
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

//bool LPTGameScene::checkForTouchableArea(Vec2 touch) {
//    BaseSprite *centerTable = getCenterTable();
//    Vec2 touchPoint = centerTable->convertToNodeSpace(touch);
//    Vec2 centerTablePostion = centerTable->convertToNodeSpace(centerTable->getPosition());
//    Rect touchableArea = this->getTableTouchableArea();
//
//    //To check for board touchable area
//    if(touchableArea.containsPoint(touchPoint)) {
//        //Check for striker touched
//        for(int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
//            LPTStriker* striker = arrayStriker.at(strikerCounter);
//            auto strikerPosition = centerTable->convertToNodeSpace(striker->getPosition());
//            Rect strikerBoundingBox = Rect(strikerPosition.x - striker->getBoundingBox().size.width,
//                                           strikerPosition.y - striker->getBoundingBox().size.height,
//                                           striker->getBoundingBox().size.width * 3,
//                                           striker->getBoundingBox().size.height * 3);
//            if(strikerBoundingBox.containsPoint(touchPoint) && striker->isStrikerTouchable == true) {
//                touchedStrikerIndex = strikerCounter;
//                lastStrikerPosition = striker->getPosition();
//                return true;
//            }
//        }
//        return false;
//    } else {
//        return false;
//    }
//}

//Rect LPTGameScene::getTableTouchableArea() {
//    BaseSprite *centerTable = getCenterTable();
//    Vec2 centerTablePostion = centerTable->convertToNodeSpace(centerTable->getPosition());
//    Rect touchableArea;
//    if(kUserSelectedSide == PLAY_AREA_LEFT) {
//        touchableArea = Rect(centerTablePostion.x - centerTable->getContentSize().width * 0.5,
//                             centerTablePostion.y - centerTable->getContentSize().height * 0.5,
//                             centerTable->getContentSize().width * 0.475,
//                             centerTable->getContentSize().height);
//    } else {
//        touchableArea = Rect(centerTablePostion.x + centerTable->getContentSize().width * 0.025,
//                             centerTablePostion.y - centerTable->getContentSize().height * 0.5,
//                             centerTable->getContentSize().width * 0.46,
//                             centerTable->getContentSize().height);
//    }
//    return touchableArea;
//}

Rect LPTGameScene::getTableTouchableArea() {
    BaseSprite *centerTable = getCenterTable();
    Rect touchableArea;
    
    if( userUIData.playingArea == PLAY_AREA_LEFT) {
        touchableArea = ((LPTGameBoard *)centerTable->getParent())->getCenterTableRectInRespectToGameScene(PLAY_AREA_LEFT);
        //
        //        if(!createLayer) {
        //            createLayer = true;
        //            LayerColor *layerColor = LayerColor::create(Color4B(0, 0, 0, 70), touchableArea.size.width, touchableArea.size.height);
        //            layerColor->setPosition(Vec2(touchableArea.origin.x, touchableArea.origin.y));
        //            this->addChild(layerColor,9999);
        //        }
        //        //(GameUtils::isIphoneX ? 0.5 : 0.5)
        
    } else {
        touchableArea = ((LPTGameBoard *)centerTable->getParent())->getCenterTableRectInRespectToGameScene(PLAY_AREA_RIGHT);
        //        if(!createLayer) {
        //            createLayer = true;
        //            LayerColor *layerColor = LayerColor::create(Color4B(0, 0, 0, 70), touchableArea.size.width, touchableArea.size.height);
        //            layerColor->setPosition(Vec2(touchableArea.origin.x, touchableArea.origin.y));
        //            this->addChild(layerColor,9999);
        //        }
    }
    return touchableArea;
}


bool LPTGameScene::checkForStrikerPassingArea(Vec2 touch) {
    BaseSprite *centerTable = getCenterTable();
    
    Rect centerTablePassingArea = Rect(centerTable->getParent()->getPosition().x + centerTable->getPosition().x - centerTable->getContentSize().width * 0.1, centerTable->getParent()->getPosition().y + centerTable->getPosition().y - centerTable->getContentSize().height * 0.1, centerTable->getContentSize().width * 0.1, centerTable->getContentSize().height * 0.2);
    
    if(centerTablePassingArea.containsPoint(touch)) {
        return true;
    } else {
        return false;
    }
}


bool LPTGameScene::checkFeasiblePositionOfStrikerY (Vec2 touchPoint, Rect area) {
    auto margin = area.size.height * 0.09;
    if((touchPoint.y > area.origin.y + margin) && (touchPoint.y < (area.origin.y + area.size.height - margin))) {
        return true;
    }
    return false;
}

bool LPTGameScene::checkFeasiblePositionOfStrikerX (Vec2 touchPoint, Rect area) {
    auto margin = area.size.width * 0.09;
    if((touchPoint.x > area.origin.x + margin) && (touchPoint.x < (area.origin.x + area.size.width * 0.97))) {
        return true;
    }
    return false;
}
//bool LPTGameScene::checkForStrikerPassingArea(Vec2 touch) {
//    BaseSprite *centerTable = getCenterTable();
//    Vec2 centerTablePosition = centerTable->convertToNodeSpace(centerTable->getPosition());
//    Rect centerTablePassingArea = Rect(centerTablePosition.x - centerTable->getContentSize().width * 0.1,
//                                       centerTablePosition.y- centerTable->getContentSize().height * 0.1,
//                                       centerTable->getContentSize().width * 0.1,
//                                       centerTable->getContentSize().height * 0.2);
//    if(centerTablePassingArea.containsPoint(touch)) {
//        return true;
//    } else {
//        return false;
//    }
//}
//
//
//bool LPTGameScene::checkFeasiblePositionOfStrikerY (Vec2 touchPoint, Rect area) {
//    auto margin = area.size.height * 0.12;
//    if((touchPoint.y > area.origin.y + margin) && (touchPoint.y < (area.origin.y + area.size.height - margin))) {
//        return true;
//    }
//    return false;
//}
//
//bool LPTGameScene::checkFeasiblePositionOfStrikerX (Vec2 touchPoint, Rect area) {
//    auto margin = area.size.width * 0.1;
//    if((touchPoint.x > area.origin.x + margin) && (touchPoint.x < (area.origin.x + area.size.width*0.97))) {
//        return true;
//    }
//    return false;
//}

#pragma mark - ButtonCallback
void LPTGameScene::buttonCallback(Ref* sender, Widget::TouchEventType type) {
    Button *button = (Button*)sender;
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::CANCELED:
            break;
        case Widget::TouchEventType::ENDED:
        {
            SoundManager::playSound(BUTTON_TAP_SOUND);
            switch (button->getTag()) {
                default:
                    break;
            }
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
}

#pragma mark - Checking for win
/**
 checkForWinningCondition : This method will check if player has won or not.
 - returns : PLAYER_STATE
 */

PLAYER_STATE LPTGameScene::checkForWinningCondition() {
    if(touchableStrikerCount == kMaxPlayerStriker) {
        return HAS_LOST;
    } else if (touchableStrikerCount == 0) {
        return HAS_WON;
    } else {
        return IS_PLAYING;
    }
}

void LPTGameScene::updateTouchableStrikers() {
    touchableStrikerCount = 0;
    BaseSprite *centerTable = getCenterTable();
    Rect userPlayingArea = this->getTableTouchableArea();
    arrayOpponentStrikersTag.clear();
    //Check for striker
    for(int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
        LPTStriker* striker = arrayStriker.at(strikerCounter);
        Vec2 strikerPostion = striker->getPosition();//centerTable->convertToNodeSpace(striker->getPosition());
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
            arrayOpponentStrikersTag.push_back(strikerCounter);
            striker->isStrikerTouchable = false;
        }
    }
    
    if(checkForWinningCondition() == HAS_WON) {
        playerWinGameUpdation();
    } else  if (checkForWinningCondition() == HAS_LOST) {
        playerLoseGameUpdation();
    }
}

bool LPTGameScene::isStrikerInsideTouchableArea(LPTStriker* striker, Vec2 strikerPosition, Rect touchableArea) {
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

#pragma mark - AddGameResultScreen
void LPTGameScene::addResultScreen(bool hasWon) {
    isResultDeclared = true;
    LayerColor *base = LayerColor::create(Color4B(0, 0, 0, 200), this->getContentSize().width, this->getContentSize().height);
    this->addChild(base,9999);
    LPTGameResultStatus *result = LPTGameResultStatus::createLayer();
    //PLAYER->getPlayerUniqueName().c_str()
    result->setupUI(kWinningAmount, hasWon, PLAYER->getPlayerProfilePicture(), OPPONENT->getProfilePic(), PLAYER->getPlayerUniqueName(), OPPONENT->getUniqueName());
    this->addChild(result, 10000);
    result->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, result->getScale())), NULL));
}

#pragma mark - Game popup callabck
void LPTGameScene::okButtonAction(Popup_Type type) {
    switch (type) {
        case LOGOUT_POPUP:
            
            break;
        case GAMEQUIT_POPUP:
            
            break;
        case NORMAL_POPUP:
            SoundManager::playSound(SCENE_CHANGE_SOUND);
            Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, LPTGameSelectionScene::createScene()));
            break;
        default:
            
            break;
    }
    LPTPopUp * popUp = (LPTPopUp *)this->getChildByTag((int)type);
    if(popUp != NULL) {
        popUp->removeFromParentAndCleanup(true);
    }
}

void LPTGameScene::cancelButtonAction(Popup_Type type) {
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

void LPTGameScene::updateUserDocumentForGameResultResponse(void *response) {
    App42StorageResponse *storageResponse = (App42StorageResponse*)response;
    printf("\ncode=%d",storageResponse->getCode());
    printf("\nResponse Body=%s",storageResponse->getBody().c_str());
    if (storageResponse->isSuccess) {
        printf("Puchase Success");
    } else {
        printf("\nerrordetails:%s",storageResponse->errorDetails.c_str());
        printf("\nerrorMessage:%s",storageResponse->errorMessage.c_str());
        printf("\nappErrorCode:%d",storageResponse->appErrorCode);
        printf("\nhttpErrorCode:%d",storageResponse->httpErrorCode);
    }
}

#pragma mark ConnectionRequestListener
void LPTGameScene::onConnectDone(int res, int code){};
void LPTGameScene::onDisconnectDone(int res){};

#pragma mark RoomRequestListener
void LPTGameScene::onLeaveRoomDone (AppWarp::room revent){};

#pragma mark ZoneRequestListener
void LPTGameScene::onDeleteRoomDone (AppWarp::room revent){};
void LPTGameScene::onGetAllRoomsDone (AppWarp::liveresult res){};
void LPTGameScene::onGetOnlineUsersDone (AppWarp::liveresult res){};
void LPTGameScene::onGetLiveUserInfoDone (AppWarp::liveuser uevent){};
void LPTGameScene::onGetMatchedRoomsDone(AppWarp::matchedroom mevent){};

#pragma mark NotificationListener
void LPTGameScene::onUserLeftRoom(AppWarp::room rData, std::string user) {
    if(user != PLAYER->getPlayerUniqueName()) {
        //Opponent
        addResultScreen(true);
    }else {
        
    }
};

void LPTGameScene::onChatReceived(AppWarp::chat chatevent){
    if (chatevent.sender != PLAYER->getPlayerUniqueName()) {
        //===Opponent
        picojson::value v;
        picojson::parse(v, chatevent.chat.c_str());
        picojson::object object = v.get<picojson::object>();
        int tag = object["StrikerTag"].get<double>();
        Vec2 posOnRope = Vec2(object["PositionXOnRope"].get<double>(), object["PositionYOnRope"].get<double>());
        b2Vec2 impulse = b2Vec2(object["ImpulseX"].get<double>(), object["ImpulseY"].get<double>());
        updateOpponentUI(tag, posOnRope, impulse);
    } else {
        //===Myself
    }
};

void LPTGameScene::onUpdatePeersReceived(AppWarp::byte update[], int len, bool isUDP){};

#pragma mark - Game result UI and DB changes
void LPTGameScene::playerWinGameUpdation() {
    LPTHUDLayer * layer = (LPTHUDLayer*)this->getChildByTag(kHUDGameLayer);
    LPTEvents::logUserGameLength(true, layer->getTimerLabel()->getString());
    int wins = PLAYER->getPlayerGameWon() + 1;
    PLAYER->setPlayerGameWon(wins);
//    PLAYER->setTotalCoins(PLAYER->getTotalCoins() + kWinningAmount);
    this->addResultScreen(true);
    
    //SETTING PLAYER LEVEL & XP
    int userXP              =   gameTimeInSeconds > 180 ? (20 * GameUtils::getRandomIntegerNumber(4, 6)) : (30 * GameUtils::getRandomIntegerNumber(4, 8));
    
    userXP = userXP + PLAYER->getPlayerGameXP();
    if(userXP > GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel())) {
        userXP = userXP - GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel());
        PLAYER->setPlayerGameLevel(PLAYER->getPlayerGameLevel() + 1);
    }
    PLAYER->setPlayerGameXP(userXP);
    //
    //
    //
    //
    log("PLAYER GAME XP: %f", PLAYER->getPlayerGameXP());
    LPTServerListener::getInstance()->updateUserGameData(PLAYER->getPlayerGameXP(), PLAYER->getTotalCoins(), PLAYER->getPlayerGameWon(), PLAYER->getPlayerGamePlayed(), PLAYER->getPlayerGameLevel(), PLAYER->getPlayerAuthToken(), "true", kGameOverRequest);
    
    auto stopGame = CallFunc::create([this](){
        this->stopGameTimer();
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.3), stopGame, NULL));
    
    //Log user Level and XP
    LPTEvents::logUserLevel(PLAYER->getPlayerGameLevel());
    LPTEvents::logUserXP(userXP);
}

void LPTGameScene::playerLoseGameUpdation() {
    LPTHUDLayer * layer = (LPTHUDLayer*)this->getChildByTag(kHUDGameLayer);
    LPTEvents::logUserGameLength(false, layer->getTimerLabel()->getString());
    
    int userXP              =   gameTimeInSeconds > 180 ? (20 * GameUtils::getRandomIntegerNumber(3, 6)) : (10 * GameUtils::getRandomIntegerNumber(1, 4));
    userXP = userXP + PLAYER->getPlayerGameXP();
    if(userXP > GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel())) {
        userXP = userXP - GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel());
        PLAYER->setPlayerGameLevel(PLAYER->getPlayerGameLevel() + 1);
    }
    PLAYER->setPlayerGameXP(userXP);
    
    LPTServerListener::getInstance()->updateUserGameData(PLAYER->getPlayerGameXP(), PLAYER->getTotalCoins(), PLAYER->getPlayerGameWon(), PLAYER->getPlayerGamePlayed(), PLAYER->getPlayerGameLevel(), PLAYER->getPlayerAuthToken(), "true", kGameOverRequest);
    
    //UserXP...
    addResultScreen(false);
    auto stopGame = CallFunc::create([this](){
        this->stopGameTimer();
    });
    this->runAction(Sequence::create(DelayTime::create(0.3), stopGame, NULL));
    //Log user Level and XP
    LPTEvents::logUserLevel(PLAYER->getPlayerGameLevel());
    LPTEvents::logUserXP(userXP);
}

#pragma mark - LPTHUDLayerDelegate
void LPTGameScene::gameQuitByUser(){
    playerLoseGameUpdation();
}
