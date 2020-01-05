//
//  LPTGameSceneRW.cpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 31/10/18.
//

#include "LPTGameSceneRW.hpp"
#include "LPTGameBoard.hpp"
#include "LPTRope.hpp"
#include "LPTContactListener.h"
#include "LPTAIMode.hpp"
#include "LPTGameSelectionScene.hpp"
#include "LPTServerListener.hpp"
#include "LPTPlayerMatchingScene.hpp"
#include "LPTEvents.hpp"
//
#define kRoomID                         "room10001"

//Socket Events
#define EVENT_CONNECT                       "connect"
#define EVENT_DISCONNECT                    "disconnect"
#define EVENT_MESSAGE                       "message"
#define EVENT_ROOMJOINED                    "roomjoined"
#define EVENT_ROOM                          "room"
#define EVENT_ROPE_POSITION                 "updateropeposition"

#define kGameBoardTag                   1001
#define kHUDGameLayer                   1002

#define kMaxPlayerStriker               10
#define kStrikerTagBase                 3000
#define kStrikerTagMax                  3009
#define kSimulatedStrikerTag            3010
#define kResultLayerTag                 3011
#define kResultLayerBgTag               3012

#define kMaximumForce                   1000.0f
#define kMinimumForce                   600.0f

#define kCollisionDelayTime             0.001
#define kCollisionDelayTimeForAI        0.25

#define kStrikerShowingFactor           25

#define kTagSIOSocketClient             "LPT_SIOSocketClient"

#define kUserSelectedSide                PLAY_AREA_LEFT //PLAY_AREA_RIGHT PLAY_AREA_LEFT
#define kWinningAmount                  (GameUtils::getUserBetValueFromSelectedBet(PLAYER->getBetSelected())) * 2

#pragma mark - CreateScene
BaseScene* LPTGameSceneRW::createScene() {
    APP_MANAGER->gameState = GAME_SCENE;
    auto scene = BaseScene::create();
    auto layer = LPTGameSceneRW::create();
    scene->addChild(layer);
    return scene;
}

#pragma mark - SceneMethod
bool LPTGameSceneRW::init() {
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

void LPTGameSceneRW::onEnter() {
    BaseLayer::onEnter();
    isResultDeclared = false;
    this->createPhysicalWorld();
    this->setupGameData();
    this->addGameHUDLayer();
    this->addGameTable();
    this->addStriker();
    this->addRopeInGameScene(kRopeLayerTag, false);
    this->addRopeInGameScene(kOppentRopeLayerTag,true);
    startGame();
    connectToSocket();
    this->scheduleTick();
    APP_MANAGER->numberOfMPGPerSession = APP_MANAGER->numberOfMPGPerSession + 1;
}

void LPTGameSceneRW::onExit() {
    debugDraw = NULL;
    if(soClient) {
        soClient->disconnect();
        soClient = NULL;
    }
    stopGameTimer();
    LPTEvents::logNumberOfMPG(APP_MANAGER->numberOfMPGPerSession);
    BaseLayer::onExit();
}


#pragma mark - SetupPhysicsWorld
void LPTGameSceneRW::createPhysicalWorld()
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
void LPTGameSceneRW::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformUpdated)
{
    Layer::draw(renderer, transform, transformUpdated);
    Director* director = Director::getInstance();
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    world->DrawDebugData();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

#pragma mark -Setup Game data
void LPTGameSceneRW::setupGameData() {
    PLAYER->setPlayerGamePlayed(PLAYER->getPlayerGamePlayed() + 1);
    
    if(PLAYER->isAdmin) {
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
    string userPlayingArea = StringUtils::format("%d", userUIData.playingArea);
    auto popup = LPTPopUp::showPopUpWith(kAlertMessageText, atoi(userPlayingArea.c_str())?"You are on Right side":"You are on Left side", false, PLAYER_SIDE);
    popup->setPopUpDelegate(this);
    this->addChild(popup, 5000);
}

#pragma mark - HUDLayer
/*
 addGameHUDLayer: This method will add HUDLayer in GameScene.
 HUDLayer contain Timer, Player profile, Player name, Bet coins and a BackButton.
 HUDLayer zORder will remain to higher than GameTable zOrder.
 */
void LPTGameSceneRW::addGameHUDLayer() {
    LPTHUDLayer *layer = LPTHUDLayer::createLayer();
    layer->setHUDDelegate(this);
    layer->setTag(kHUDGameLayer);
    layer->setPosition(Vec2(0, this->getContentSize().height));
    string opponentName     =   OPPONENT->getUniqueName();
    string opponentImage    =   OPPONENT->getProfilePic();
//    PLAYER->getPlayerUniqueName().c_str()
    layer->setupUI( PLAYER->getPlayerUniqueName(), PLAYER->getPlayerProfilePicture(), opponentName, opponentImage,  userUIData.playingArea);
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
void LPTGameSceneRW::addGameTable() {
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

BaseSprite* LPTGameSceneRW::getCenterTable() {
    LPTGameBoard *board = (LPTGameBoard*)this->getChildByTag(kGameBoardTag);
    return board->getCenterTable();
}

#pragma mark addStriker
/**
 addStriker: This method will striker in gameScene. There are total 10 striker which are equally divided to each player i.e. 5 each. Striker is physics body object consist of density: 100, restitution: 1.0 and friction: 1.0. Striker zOrder is 4.
 -params:
 -void:
 */
void LPTGameSceneRW::addStriker() {
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
        
        
        
        log("value of the bodyID: %d position %f %f", striker->getTag() - kStrikerTagBase, striker->getStrikerBody()->GetPosition().x, striker->getStrikerBody()->GetPosition().y);
        
    }
}

#pragma mark - changeRopeStatus
void LPTGameSceneRW::changeStatusOfRope() {
    LPTRope *rope = (LPTRope *)this->getChildByTag(kRopeLayerTag);
    int currentIndex = userUIData.playingArea == PLAY_AREA_LEFT ? selectedStrikerIndex : opponentStrikerIndex;
    LPTStriker *striker;
    collisionBetweenRopeAndStrikerSarted = true;
    if(collisionBetweenRopeAndStrikerSarted) {
        rope->updateRopeAccordingToBall = false;
        if (currentIndex >= 0) {
            striker  = (LPTStriker*) arrayStriker[currentIndex];
            if (striker && !striker->hasPointingHand()) {
                striker = (LPTStriker*)this->getChildByTag(kSimulatedStrikerTag);
            }
            if(striker && striker->isStrikerDragging) {
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
    this->updateRopeTexture(striker, kRopeLayerTag);
}

void LPTGameSceneRW::changeStatusOfRopeForOpponent() {
    LPTRope *rope = (LPTRope *)this->getChildByTag(kOppentRopeLayerTag);
    int currentIndex = userUIData.playingArea == PLAY_AREA_LEFT ? opponentStrikerIndex : selectedStrikerIndex;
    LPTStriker *striker;
    collisionBetweenRopeAndStrikerStartedForOpponent = true;
    if(collisionBetweenRopeAndStrikerStartedForOpponent) {
        rope->updateRopeAccordingToBall = false;
        if (currentIndex >= 0) {
            striker  = (LPTStriker*) arrayStriker[currentIndex];
            if (striker && !striker->hasPointingHand()) {
                striker = (LPTStriker*)this->getChildByTag(kSimulatedStrikerTag);
            }
            if(striker && striker->isStrikerDragging) {
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
    this->updateRopeTexture(striker, kOppentRopeLayerTag);
}

#pragma mark UPDATE_ROPE_TEXTURE
void LPTGameSceneRW::updateRopeTexture(LPTStriker *striker, int ropeTag) {
    LPTRope *rope = (LPTRope *)this->getChildByTag(ropeTag);
    if(striker) {
//        LPTStriker *striker  = (LPTStriker*) arrayStriker[segment];
        Vec2 ropeNewPosition = Vec2(striker->getPosition().x - striker->getContentSize().width * striker->getScale() * 0.475, striker->getPosition().y);
        if(ropeTag == kOppentRopeLayerTag)
            ropeNewPosition = Vec2(striker->getPosition().x + striker->getContentSize().width * striker->getScale() * 0.475, striker->getPosition().y);
        
        rope->createRopeTexture(0, rope->updateRopeAccordingToBall, ropeNewPosition, striker->isStrikerDragging);
    }else {
        rope->createRopeTexture(-1, rope->updateRopeAccordingToBall, Vec2(0, 0), false);
    }
}

#pragma mark addRopeInGameScene
void LPTGameSceneRW::addRopeInGameScene(int tag, bool isBulletTrue) {
    BaseSprite *centerTable = getCenterTable();
    float displacementFactor = 0.455;//GameUtils::isIphoneX ? 0.475 : 0.475;   //rope position according device size
    
    Vec2 ropeInitialPosition =  Vec2(centerTable->getParent()->getPosition().x + centerTable->getPosition().x - centerTable->getContentSize().width * displacementFactor * centerTable->getScale(), centerTable->getParent()->getPosition().y + centerTable->getPosition().y - centerTable->getContentSize().height * 0.5 * centerTable->getScale());
    
    if (tag == kOppentRopeLayerTag) {
        ropeInitialPosition = Vec2(centerTable->getParent()->getPosition().x + centerTable->getPosition().x + centerTable->getContentSize().width * displacementFactor * centerTable->getScale(), centerTable->getParent()->getPosition().y + centerTable->getPosition().y - centerTable->getContentSize().height * 0.5 * centerTable->getScale());
    }
    
    LPTRope *rope = LPTRope::createRope();
    rope->setTag(tag);
    rope->initialPosition = ropeInitialPosition;
    rope->setupUI(Size(centerTable->getContentSize().width * 0.02, centerTable->getContentSize().height * centerTable->getScale()), ropeInitialPosition, world);
    tag == kOppentRopeLayerTag ? rope->setPositionX(rope->getPosition().x -  rope->getContentSize().width):rope->setPositionX(rope->getPosition().x);
//    rope->createRopeSegment(world, isBulletTrue);
    this->addChild(rope, 10);
    rope->createRopeTexture(selectedStrikerIndex, createRopeAccordingToBall, Vec2(0, 0), false);
//    rope->createRopeJoint(world);
//    rope->changeLinearDamping();
}



#pragma mark Update GameScene UI
void LPTGameSceneRW::updateUI() {
    LPTHUDLayer * layer = (LPTHUDLayer*)this->getChildByTag(kHUDGameLayer);
    layer->setCoinValue(PLAYER->getTotalCoins());
}

#pragma mark - Game Timer Methods
void LPTGameSceneRW::startGame() {
    this->addTouchListener();
    this->schedule(schedule_selector(LPTGameSceneRW::updateGameTimer), 1.0); // Temporary
}

void LPTGameSceneRW::updateGameTimer(float delta) {
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
void LPTGameSceneRW::stopGameTimer() {
    gameTimeInSeconds = 0;
    touchableStrikerCount = 0;
    this->unschedule(schedule_selector(LPTGameSceneRW::updateGameTimer));
//    this->unschedule(schedule_selector(LPTGameSceneRW::playAIMovement));
    this->unschedule(schedule_selector(LPTGameSceneRW::tick));
//    for(int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
//        LPTStriker* striker = arrayStriker.at(strikerCounter);
//        striker->isStrikerTouchable = false;
//    }
    LPTAIMode::sharedInstance()->removeSharedInstance();
}

#pragma mark - scheduleTick
void LPTGameSceneRW::scheduleTick(){
    schedule(schedule_selector(LPTGameSceneRW::tick),0.016);
}
#pragma mark Tick Method
void LPTGameSceneRW::tick(float dt){
    //Update strikers
    if(!isResultDeclared)
        updateTouchableStrikers();
    
    int velocityIterations = 10;
    int positionIterations = 10;
    world->Step(0.01, velocityIterations, positionIterations);
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            BaseSprite *sprite = (BaseSprite*)b->GetUserData();
            Vec2 position = Vec2(0, 0);
            
            if(sprite->userDataType == kUserDataTypeStriker) {
                sprite->setPosition(Vec2((b->GetPosition().x * kPixelToMeterRatio - position.x),
                                         (b->GetPosition().y * kPixelToMeterRatio - position.y)));
                sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
            }
//            this->changeSelectedStrikerVelocity(b);
        }
    }
    /*
    //TO CREATE A ROPE AT RUNTIME
    this->changeStatusOfRope();
    this->changeStatusOfRopeForOpponent();
    
    if( aiDraggableStriker  != NULL && opponentStrikerIndex >= 0) {
        LPTStriker *striker = arrayStriker.at(opponentStrikerIndex);
        striker->getStrikerBody()->SetTransform(b2Vec2(aiDraggableStriker->getPosition().x / kPixelToMeterRatio, (aiDraggableStriker->getPosition().y) / kPixelToMeterRatio), striker->getStrikerBody()->GetAngle());
    }*/
    
    this->changeStatusOfRope();
    this->changeStatusOfRopeForOpponent();
//    this->checkStrikerMovementWhenHitWithRope();
}

void LPTGameSceneRW::checkStrikerMovementWhenHitWithRope() {
    LPTRope *rope           =   (LPTRope *)this->getChildByTag(kRopeLayerTag);
    LPTRope *opponentRope   =   (LPTRope *)this->getChildByTag(kOppentRopeLayerTag);
    
    for(int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
        LPTStriker* striker     =   arrayStriker.at(strikerCounter);
        Rect strikerRect    =   striker->getBoundingBox();
        
        Rect ropeRect       =   Rect(rope->getPosition().x, rope->getPosition().y, rope->getContentSize().width * 0.5, rope->getContentSize().height);
        Rect opponentRect   =   Rect(opponentRope->getPosition().x + opponentRope->getContentSize().width * 0.5, opponentRope->getPosition().y, opponentRope->getContentSize().width * 0.5, opponentRope->getContentSize().height);
        
        if(ropeRect.intersectsRect(strikerRect))
        {
           
        }
        if(opponentRect.intersectsRect(strikerRect)) {
            
        }
        striker->spritePreviousPosition = striker->getPosition();
    }
}

void LPTGameSceneRW::changeSelectedStrikerVelocity(b2Body *body) {
    BaseSprite *sprite = (BaseSprite*)body->GetUserData();
    if(sprite->userDataType == kUserDataTypeStriker) {
        LPTStriker *striker = (LPTStriker*)sprite;
        striker->maintainXVelocity();
        striker->dumpStrikerVelocity();
    }
}


bool LPTGameSceneRW::checkIfStrikerIsRightWay(LPTStriker *striker) {
    if(fabs(striker->getStrikerBody()->GetLinearVelocity().x) > 20) {
        return true;
    } else {
        return false;
    }
}

int LPTGameSceneRW::getStrikerIndexForAI() {
    bool gotIndex = false;
    int indexCheck = 0;
    int selectedIndex = -1;
    while (!gotIndex && arrayOpponentStrikersTag.size() != 0) {
        int strikerRandomIndex = arrayOpponentStrikersTag.at(arc4random()%arrayOpponentStrikersTag.size());
        LPTStriker *striker = arrayStriker.at(strikerRandomIndex);
        bool isRightWay = checkIfStrikerIsRightWay(striker);
        //        log("isRightWay : %d", isRightWay);
        if(striker->isStrikerTouchable == false && !isRightWay) {
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
void LPTGameSceneRW::addTouchListener() {
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(LPTGameSceneRW::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(LPTGameSceneRW::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(LPTGameSceneRW::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(LPTGameSceneRW::onTouchCancelled, this);
    SoundManager::playSound(SCENE_CHANGE_SOUND);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool LPTGameSceneRW::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 position = touch->getLocation();
    bool isStrikerFound     =   this->checkForTouchableArea(position);
    selectedStrikerIndex    =   touchedStrikerIndex;
    if(selectedStrikerIndex >= 0) {
        setImpulse(0.0, 0.0, selectedStrikerIndex);
        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
        sendStrikerPositions(striker->getPositionX(), striker->getPositionY(), selectedStrikerIndex);
        updateStrikerDraggingStatus(selectedStrikerIndex, "0");
        //==
        addSimulatedStriker(striker->getPosition(), striker->getScale());
    }
    return isStrikerFound;
}

void LPTGameSceneRW::onTouchMoved(Touch* touch, Event* event) {
    BaseSprite *centerTable = getCenterTable();
    Vec2 touchPoint = touch->getLocation(); //centerTable->convertToNodeSpace(touch->getLocation());
    Rect touchableArea = this->getTableTouchableArea();
    
    if(selectedStrikerIndex >= 0) {
        LPTStriker *simulatedStriker = (LPTStriker*)this->getChildByTag(kSimulatedStrikerTag);
        if(simulatedStriker){
            simulatedStriker->setVisible(true);
            simulatedStriker->isStrikerDragging = true;
        }
        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
        striker->setVisible(false);
        Vec2 touchPrevLocation = touch->getPreviousLocation(); //centerTable->convertToNodeSpace(touch->getPreviousLocation());
        Vec2 movementDirection = touchPrevLocation - touchPoint;
        if(isStrikerInsideTouchableArea(striker, touchPoint, touchableArea)) {
            if(simulatedStriker)simulatedStriker->setPosition(Vec2(touch->getLocation().x, touch->getLocation().y));
            transformStrikerBody(striker, touch->getLocation().x, touch->getLocation().y,touch, event);
        }else {
            if(checkFeasiblePositionOfStrikerY(touchPoint, touchableArea)) {
                //If left side touch move outside the area.
                float positionInX = touchPoint.x;
                if(touchPoint.x < (touchableArea.origin.x + striker->getContentSize().width * 0.55 * striker->getScale())) {
                      //If left side touch move outside the area.
                    positionInX = touchableArea.origin.x + striker->getContentSize().width * 0.55 * striker->getScale();
                    if(simulatedStriker)simulatedStriker->setPosition(Vec2(positionInX, touch->getLocation().y));
                    transformStrikerBody(striker, positionInX, touch->getLocation().y,touch, event);
                }
                
                if (positionInX > (touchableArea.origin.x + touchableArea.size.width - (striker->getContentSize().width * 0.5 * striker->getScale()))) {
                    // If right side of the touch move outside the area.
                    positionInX = touchableArea.origin.x + touchableArea.size.width - (striker->getContentSize().width * 0.5 * striker->getScale());
                    if(simulatedStriker)simulatedStriker->setPosition(Vec2(positionInX, touch->getLocation().y));
                    transformStrikerBody(striker, positionInX, touch->getLocation().y,touch, event);
                }
             
            }
            
            if(checkFeasiblePositionOfStrikerX(touchPoint, touchableArea)) {
                float positionInY = touchPoint.y;
                if(touchPoint.y < (touchableArea.origin.y + striker->getContentSize().width * 0.5 * striker->getScale())) {
                    //If touch point moves below the TableArea
                    positionInY = touchableArea.origin.y + (striker->getContentSize().width * 0.5 * striker->getScale());
                    if(simulatedStriker)simulatedStriker->setPosition(Vec2(touchPoint.x, positionInY));
                    transformStrikerBody(striker, touchPoint.x, positionInY,touch, event);
                }
                
                if(touchPoint.y > (touchableArea.origin.y + touchableArea.size.height - (striker->getContentSize().width * 0.5 * striker->getScale()))) {
                    //If touch point moves above the TableArea
                    positionInY = touchableArea.origin.y + touchableArea.size.height - (striker->getContentSize().width * 0.5 * striker->getScale());
                    if(simulatedStriker)simulatedStriker->setPosition(Vec2(touchPoint.x, positionInY));
                    transformStrikerBody(striker, touchPoint.x, positionInY,touch, event);
                }

            }
        }
        striker->isStrikerDragging = true;
    }
}

void LPTGameSceneRW::onTouchEnded(Touch* touch, Event* event)
{
    Vec2 touchPoint = touch->getLocation();
    if(selectedStrikerIndex >= 0) {
        LPTStriker *striker = arrayStriker.at(selectedStrikerIndex);
        LPTRope *ropeObject = (LPTRope *) this->getChildByTag(userUIData.RopeTag);
        //        log("## TOUCH _END ROPE POSITION : %f", ropeObject->ropeExtensionValue);
        updateStrikerDraggingStatus(selectedStrikerIndex, "0");
        if(striker->isStrikerDragging || collisionBetweenRopeAndStrikerSarted){
            log("striker pos x = %f, rope x pos = %f",striker->getPositionX(),ropeObject->initialPosition.x);
            if (userUIData.playingArea == PLAY_AREA_LEFT && striker->getPositionX() <= ropeObject->initialPosition.x + (striker->getContentSize().width * 0.5)) {
                sendStrikerPositions(striker->getPositionX() + striker->getContentSize().width*0.4, striker->getPositionY(), selectedStrikerIndex);
            }else if(userUIData.playingArea == PLAY_AREA_RIGHT && striker->getPositionX() >= ropeObject->initialPosition.x - (striker->getContentSize().width * 0.5)) {
                sendStrikerPositions(striker->getPositionX() - striker->getContentSize().width*0.4, striker->getPositionY(), selectedStrikerIndex);
            }
            setImpulse((ropeObject->ropeExtensionValue * 5.0), 0.0, selectedStrikerIndex);
            auto endSimulation = CallFunc::create([striker, this](){
                this->removeChildByTag(kSimulatedStrikerTag);
                striker->setVisible(true);
            });
            this->runAction(Sequence::create(DelayTime::create(0.4), endSimulation, NULL));
            //            selectedStrikerIndex = -1;
            //            touchedStrikerIndex = -1;
            auto changeStrikerDragging = CallFunc::create([striker, this](){
                striker->isStrikerDragging = false;
                collisionBetweenRopeAndStrikerSarted = false;
            });
            this->runAction(Sequence::create(DelayTime::create(1.0), changeStrikerDragging, NULL));
        }
    }
}

void LPTGameSceneRW::onTouchCancelled(Touch* touch, Event* event) {
    onTouchEnded(touch,event);
}

#pragma mark Striker movement related methods
LPTStriker* LPTGameSceneRW::addSimulatedStriker(Vec2 position, float scale){
    if (!this->getChildByTag(kSimulatedStrikerTag)) {
        LPTStriker *simulatedStriker = LPTStriker::createStriker();
        simulatedStriker->setPosition(position);
        simulatedStriker->setTag(kSimulatedStrikerTag);
        simulatedStriker->setScale(scale);
        simulatedStriker->isStrikerDragging = false;
        this->addChild(simulatedStriker, 100);
        simulatedStriker->setVisible(false);
        return simulatedStriker;
    }
    return NULL;
}

void LPTGameSceneRW::transformStrikerBody(LPTStriker *striker, float xPos, float yPos,Touch* touch, Event* event) {
    if(PLAYER->isAdmin) {
        float peakX = this->getTableTouchableArea().origin.x + this->getTableTouchableArea().size.width*0.7;
//        (GameUtils::isIphone || GameUtils::isIpad )?(peakX = peakX * 0.8) : (peakX = peakX);
        if(striker->getStrikerBody()->GetPosition().x > peakX/kPixelToMeterRatio) {
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
    }else {
        float peakX = this->getTableTouchableArea().origin.x + this->getTableTouchableArea().size.width*0.3;
        if(striker->getStrikerBody()->GetPosition().x < peakX/kPixelToMeterRatio) {
            if(touch->getPreviousLocation().x < xPos) {
                if(xPos / kPixelToMeterRatio < striker->getStrikerBody()->GetPosition().x) {
                    striker->getStrikerBody()->SetTransform(b2Vec2(striker->getStrikerBody()->GetPosition().x, yPos / kPixelToMeterRatio), true);
                }else {
                    striker->getStrikerBody()->SetTransform(b2Vec2(xPos / kPixelToMeterRatio, yPos / kPixelToMeterRatio), true);
                }
            }else {
                striker->getStrikerBody()->SetTransform(b2Vec2(striker->getStrikerBody()->GetPosition().x, yPos / kPixelToMeterRatio), true);
            }
            
        }else if(xPos <= peakX) {
            striker->getStrikerBody()->SetTransform(b2Vec2(peakX / kPixelToMeterRatio, yPos / kPixelToMeterRatio), true);
        }else {
            striker->getStrikerBody()->SetTransform(b2Vec2(xPos / kPixelToMeterRatio, yPos / kPixelToMeterRatio), true);
        }
    }
    sendStrikerPositions(striker->getStrikerBody()->GetPosition().x * kPixelToMeterRatio, striker->getStrikerBody()->GetPosition().y * kPixelToMeterRatio, selectedStrikerIndex);
    updateStrikerDraggingStatus(selectedStrikerIndex, "1");
    
}

bool LPTGameSceneRW::checkForTouchableArea(Vec2 touch) {
    Vec2 touchPoint = touch; //centerTable->convertToNodeSpace(touch);
    Rect touchableArea = this->getTableTouchableArea();
    //To check for board touchable area
    if(touchableArea.containsPoint(touchPoint)) {
        //Check for striker touched
        for(int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
            LPTStriker* striker     =   arrayStriker.at(strikerCounter);
            auto strikerPosition    =   striker->getPosition();
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

Rect LPTGameSceneRW::getTableTouchableArea() {
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

bool LPTGameSceneRW::checkForStrikerPassingArea(Vec2 touch) {
    BaseSprite *centerTable = getCenterTable();

    Rect centerTablePassingArea = Rect(centerTable->getParent()->getPosition().x + centerTable->getPosition().x - centerTable->getContentSize().width * 0.1, centerTable->getParent()->getPosition().y + centerTable->getPosition().y - centerTable->getContentSize().height * 0.1, centerTable->getContentSize().width * 0.1, centerTable->getContentSize().height * 0.2);
    
    if(centerTablePassingArea.containsPoint(touch)) {
        return true;
    } else {
        return false;
    }
}


bool LPTGameSceneRW::checkFeasiblePositionOfStrikerY (Vec2 touchPoint, Rect area) {
    auto margin = area.size.height * 0.0;
    if((touchPoint.y > area.origin.y + margin) && (touchPoint.y < (area.origin.y + area.size.height - margin))) {
        return true;
    }
    return false;
}

bool LPTGameSceneRW::checkFeasiblePositionOfStrikerX (Vec2 touchPoint, Rect area) {
    auto margin = area.size.width * 0.0;
    if((touchPoint.x > area.origin.x + margin) && (touchPoint.x < (area.origin.x + area.size.width * 0.97))) {
        return true;
    }
    return false;
}

#pragma mark - ButtonCallback
void LPTGameSceneRW::buttonCallback(Ref* sender, Widget::TouchEventType type) {
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

PLAYER_STATE LPTGameSceneRW::checkForWinningCondition() {
    if(arrayOpponentStrikersTag.size() == 0) {
        return HAS_LOST;
    } else if (arrayOpponentStrikersTag.size() == kMaxPlayerStriker) {
        return HAS_WON;
    } else {
        return IS_PLAYING;
    }
}

void LPTGameSceneRW::updateTouchableStrikers() {
    touchableStrikerCount = 0;
    BaseSprite *centerTable = getCenterTable();
    Rect userPlayingArea = this->getTableTouchableArea();
    arrayOpponentStrikersTag.clear();
    //Check for striker
    
    for(int strikerCounter = 0; strikerCounter < kMaxPlayerStriker; strikerCounter++) {
        LPTStriker* striker = arrayStriker.at(strikerCounter);
        Vec2 strikerPosition =  striker->getPosition(); //centerTable->convertToNodeSpace(striker->getPosition());
        float strikerBoundingBoxFactor = (GameUtils::isIphoneX ? 0.5 : 0.5);
        auto strikerCorner1 = Vec2(strikerPosition.x - striker->getContentSize().width * striker->getScale() * strikerBoundingBoxFactor,
                                   strikerPosition.y - striker->getContentSize().height * striker->getScale()*strikerBoundingBoxFactor);
        auto strikerCorner2 = Vec2(strikerPosition.x + striker->getBoundingBox().size.width * striker->getScale()*strikerBoundingBoxFactor,
                                   strikerPosition.y - striker->getContentSize().height * striker->getScale()*strikerBoundingBoxFactor);
        auto strikerCorner3 = Vec2(strikerPosition.x - striker->getContentSize().width * striker->getScale()*strikerBoundingBoxFactor,
                                   strikerPosition.y + striker->getContentSize().height * striker->getScale()*strikerBoundingBoxFactor);
        auto strikerCorner4 = Vec2(strikerPosition.x + striker->getBoundingBox().size.width * striker->getScale()*strikerBoundingBoxFactor,
                        
                                   strikerPosition.y + striker->getBoundingBox().size.height * striker->getScale()*strikerBoundingBoxFactor);
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

bool LPTGameSceneRW::isStrikerInsideTouchableArea(LPTStriker* striker, Vec2 strikerPosition, Rect touchableArea) {
    float strikerBoundingBoxFactor = (GameUtils::isIphoneX ? 0.5 : 0.5);
    auto strikerCorner1 = Vec2(strikerPosition.x - striker->getContentSize().width * striker->getScale() * strikerBoundingBoxFactor,
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

void LPTGameSceneRW::addResultScreen(bool hasWon, bool shouldHideRematch) {
    if (!isResultDeclared) {
        isResultDeclared = true;
        //---
        if (hasWon) {
            LPTEvents::logTableWonByUser(PLAYER->getBetSelected());
        }else {
            LPTEvents::logTableLooseByUser(PLAYER->getBetSelected());
        }
        //---
        LayerColor *base = LayerColor::create(Color4B(0, 0, 0, 200), this->getContentSize().width, this->getContentSize().height);
        base->setTag(kResultLayerBgTag);
        this->addChild(base,9999);
        //PLAYER->getPlayerUniqueName().c_str()
        LPTGameResultStatus *result = LPTGameResultStatus::createLayer();
        result->setupUI(kWinningAmount, hasWon, PLAYER->getPlayerProfilePicture(), OPPONENT->getProfilePic(), PLAYER->getPlayerUniqueName(), OPPONENT->getUniqueName());
        result->setDelegate(this);
        result->setTag(kResultLayerTag);
        this->addChild(result, 10000);
        result->runAction(Sequence::create(EaseBounceIn::create(ScaleTo::create(0, 0)), EaseBounceOut::create(ScaleTo::create(0.5, result->getScale())), NULL));
        if(shouldHideRematch){result->hideRematchButton();}
        
        //UserXP Result
        int userXP  =  gameTimeInSeconds > 180 ? (20 * GameUtils::getRandomIntegerNumber(3, 6)) : (10 * GameUtils::getRandomIntegerNumber(1, 4));
        //        int userXP  =  PLAYER->getPlayerGameWon() > 0 ?  (PLAYER->getPlayerGameWon() * GameUtils::getRandomIntegerNumber(5, 10)) : GameUtils::getRandomIntegerNumber(15, 30);
        if(hasWon) {
            PLAYER->setPlayerGameWon(PLAYER->getPlayerGameWon() + 1);
            //            userXP  = getUserXP();
            userXP = gameTimeInSeconds > 180 ? (20 * GameUtils::getRandomIntegerNumber(4, 6)) : (30 * GameUtils::getRandomIntegerNumber(4, 8));
            PLAYER->setTotalCoins(PLAYER->getTotalCoins() + kWinningAmount);
        }
        //
        userXP = userXP + PLAYER->getPlayerGameXP();
        if(userXP > GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel())) {
            userXP = userXP - GameUtils::getPlayerMaxXP(PLAYER->getPlayerGameLevel());
            PLAYER->setPlayerGameLevel(PLAYER->getPlayerGameLevel() + 1);
        }
        PLAYER->setPlayerGameXP(userXP);
        //GameXP.....
        LPTServerListener::getInstance()->updateUserGameData(PLAYER->getPlayerGameXP(), PLAYER->getTotalCoins(), PLAYER->getPlayerGameWon(), PLAYER->getPlayerGamePlayed(), PLAYER->getPlayerGameLevel(), PLAYER->getPlayerAuthToken(), "0", kGameOverRequest);
        //Log user Level and XP
        LPTEvents::logUserLevel(PLAYER->getPlayerGameLevel());
        LPTEvents::logUserXP(userXP);
    }
}

void LPTGameSceneRW::removeResultScreen(){
    if (this->getChildByTag(kResultLayerTag)) {
        this->getChildByTag(kResultLayerBgTag)->removeFromParentAndCleanup(true);
        this->getChildByTag(kResultLayerTag)->removeFromParentAndCleanup(true);
    }
}

void LPTGameSceneRW::reMatchButtonAction() {
    if(soClient) {
        soClient->disconnect();
        soClient = NULL;
    }
    //===Delete Room
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    if (PLAYER->isAdmin) {
        warpClientRef->deleteRoom(PLAYER->getRoomId());
    }else {
        warpClientRef->leaveRoom(PLAYER->getRoomId());
    }
    if (PLAYER->isAdmin) {
        PLAYER->roomName = PLAYER->getPlayerUniqueName();
        PLAYER->roompassword = PLAYER->getPlayerUniqueName();
    }else {
        PLAYER->roomName = OPPONENT->getUniqueName();
        PLAYER->roompassword = OPPONENT->getUniqueName();
    }
    Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0, LPTPlayerMatchingScene::createScene()));
}

#pragma mark - Game popup callabck
void LPTGameSceneRW::okButtonAction(Popup_Type type) {
    switch (type) {
        case LOGOUT_POPUP:
            break;
        case GAMEQUIT_POPUP:
            addResultScreen(false,true);
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

void LPTGameSceneRW::cancelButtonAction(Popup_Type type) {
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

#pragma mark - App42 call
void LPTGameSceneRW::updateUserDocumentForGameResult() {
    

//    const char* key = FIELD_UNAME;
//    const char* value = LPTPlayer::getPlayerUniqueName().c_str();
//    App42Object *obj = PLAYER->getApp42ObjectForUpdation();
    
//    StorageService *storageService = App42API::BuildStorageService();
//    storageService->UpdateDocumentByKeyValue(APPWARP_DB_NAME, COLLECTION_USER, key, value, obj, app42callback(LPTGameSceneRW::updateUserDocumentForGameResultResponse, this));
}

void LPTGameSceneRW::updateUserDocumentForGameResultResponse(void *response) {
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
void LPTGameSceneRW::onConnectDone(int res, int code){};
void LPTGameSceneRW::onDisconnectDone(int res){};

#pragma mark RoomRequestListener
void LPTGameSceneRW::onLeaveRoomDone (AppWarp::room revent){};

#pragma mark ZoneRequestListener
void LPTGameSceneRW::onDeleteRoomDone (AppWarp::room revent){};
void LPTGameSceneRW::onGetAllRoomsDone (AppWarp::liveresult res){};
void LPTGameSceneRW::onGetOnlineUsersDone (AppWarp::liveresult res){};
void LPTGameSceneRW::onGetLiveUserInfoDone (AppWarp::liveuser uevent){};
void LPTGameSceneRW::onGetMatchedRoomsDone(AppWarp::matchedroom mevent){};

#pragma mark NotificationListener
void LPTGameSceneRW::onUserLeftRoom(AppWarp::room rData, std::string user) {
    if(user != PLAYER->getPlayerUniqueName()) {
        //Opponent
        addResultScreen(true,true);
    }else {
        
    }
};

void LPTGameSceneRW::onChatReceived(AppWarp::chat chatevent){
    if (chatevent.sender != PLAYER->getPlayerUniqueName()) {
        //===Opponent
        picojson::value v;
        picojson::parse(v, chatevent.chat.c_str());
        picojson::object object = v.get<picojson::object>();
        int tag = object["StrikerTag"].get<double>();
        Vec2 posOnRope = Vec2(object["PositionXOnRope"].get<double>(), object["PositionYOnRope"].get<double>());
        b2Vec2 impulse = b2Vec2(object["ImpulseX"].get<double>(), object["ImpulseY"].get<double>());
        //        updateOpponentUI(tag, posOnRope, impulse);
    } else {
        //===Myself
    }
};

void LPTGameSceneRW::onUpdatePeersReceived(AppWarp::byte update[], int len, bool isUDP){};

#pragma mark - Game result UI and DB changes
void LPTGameSceneRW::playerWinGameUpdation() {
    LPTHUDLayer * layer = (LPTHUDLayer*)this->getChildByTag(kHUDGameLayer);
    LPTEvents::logUserGameLength(true, layer->getTimerLabel()->getString());
    this->addResultScreen(true,false);
    auto stopGame = CallFunc::create([this](){
        this->stopGameTimer();
        
    });
    this->runAction(Sequence::create(DelayTime::create(0.3), stopGame, NULL));
}

void LPTGameSceneRW::playerLoseGameUpdation() {
    LPTHUDLayer * layer = (LPTHUDLayer*)this->getChildByTag(kHUDGameLayer);
    LPTEvents::logUserGameLength(false, layer->getTimerLabel()->getString());
    addResultScreen(false,false);
    auto stopGame = CallFunc::create([this](){
        this->stopGameTimer();
    });
    this->runAction(Sequence::create(DelayTime::create(0.3), stopGame, NULL));
}

#pragma mark - LPTHUDLayerDelegate
void LPTGameSceneRW::gameQuitByUser(){
    addResultScreen(false,true);
    auto stopGame = CallFunc::create([this](){
        this->stopGameTimer();
    });
    this->runAction(Sequence::create(DelayTime::create(0.3), stopGame, NULL));
}

#pragma mark - Update b2World

void LPTGameSceneRW::updateWorld(const std::string& data, bool shouldTransform) {
    picojson::value v;
    picojson::parse(v, data.c_str());
    picojson::object object = v.get<picojson::object>();
    picojson::object data1          = object["d"].get<picojson::object>();
    string bid                      = object["bid"].get<string>();
    for (int strikerCounter = 0; strikerCounter < arrayStriker.size(); strikerCounter++) {
        LPTStriker *striker  = (LPTStriker*) arrayStriker[strikerCounter];
        b2Body *body = striker->getStrikerBody();
        int bid = (striker->getTag() - kStrikerTagBase);
        if (!data1.empty()) {
            if (data1[__String::createWithFormat("%d",bid)->getCString()].is<picojson::object>()) {
                picojson::object realData       = data1[__String::createWithFormat("%d",bid)->getCString()].get<picojson::object>();
                //                    double angle    = realData["a"].get<double>();
                double aVel     = realData["av"].get<double>();
                picojson::object realDataPos       = realData["p"].get<picojson::object>();
                double xPos    = realDataPos["x"].get<double>();
                double yPos     = realDataPos["y"].get<double>();
                picojson::object realDataVelocity       = realData["lv"].get<picojson::object>();
                double xVelocity    = realDataVelocity["x"].get<double>();
                double yVelocity     = realDataVelocity["y"].get<double>();
                log("x velocity = %f, y velocity = %f",xVelocity,yVelocity);
                //==Drag status
                string dragStatus = realData["isdragging"].get<string>();
                body->SetAwake(true);
                striker->isStrikerDragging = atoi(dragStatus.c_str());
                
                if(striker->isStrikerDragging && !striker->isStrikerTouchable) {
                    opponentStrikerIndex = strikerCounter;
                    body->SetTransform(body->GetPosition(), 0.0);
                    striker->addPointingHand();
                }else {
                    striker->removePointingHand();
                }
                
                if (shouldTransform) {
                    striker->velocity = b2Vec2(xVelocity, yVelocity);
                    body->SetTransform(b2Vec2((xPos * (GameUtils::isIphoneX ? 1.777 : 1.0)) + (tablePositionInX / kPixelToMeterRatio), (yPos * (GameUtils::isIphoneX ? 1.777 : 1.0)) + (GameUtils::isIphoneX ? 1.8 : 1.0) + (tablePositionInY / kPixelToMeterRatio)), body->GetAngle());
                }
                //                    body->SetLinearVelocity(b2Vec2(xVelocity, -yVelocity));
                    body->SetAngularVelocity(aVel);
            }
        }
    }
}

#pragma mark - SIOClient
void LPTGameSceneRW::connectToSocket() {
//    soClient = SocketIO::connect(url, *this);
    soClient = SocketIO::connect("http://100.24.187.148:8008", *this);// http://192.168.2.55:8008 Live https://lpt-remote-box2d.herokuapp.com // http://35.153.215.232:8008 (m5.large Frimus acc) //http://100.24.187.148:8008 (m5.large Michal acc) //http://18.210.178.127:8008 (m4.16xlarge)
    soClient->setTag(kTagSIOSocketClient);
    //register event callbacks using the CC_CALLBACK_2() macro and passing the instance of the target class
    soClient->on(EVENT_ROOMJOINED, CC_CALLBACK_2(LPTGameSceneRW::onRoomJoined, this));
    soClient->on(EVENT_MESSAGE, CC_CALLBACK_2(LPTGameSceneRW::message, this));
    soClient->on(EVENT_CONNECT, CC_CALLBACK_2(LPTGameSceneRW::connect, this));
    soClient->on(EVENT_DISCONNECT, CC_CALLBACK_2(LPTGameSceneRW::disconnect, this));
}

void LPTGameSceneRW::sendIntialStrikerPositions() {
    for (int strikerCounter = 0; strikerCounter < arrayStriker.size(); strikerCounter++) {
        LPTStriker *striker  = (LPTStriker*) arrayStriker[strikerCounter];
        int bid = (striker->getTag() - kStrikerTagBase);
        sendStrikerPositions(striker->getPositionX(), striker->getPositionY(), bid);
    }
}

void LPTGameSceneRW::sendUpdateForRopePosition(SIOClient* client) {
    client->emit(EVENT_ROPE_POSITION, __String::createWithFormat("%d",GameUtils::getDeviceType())->getCString());
}

void LPTGameSceneRW::sendStrikerPositions(float x, float y, int bodyId) {
    
//    log("######## bodyID: %d  #########", bodyId);
    
    x = (x  - tablePositionInX) / (GameUtils::isIphoneX ? 1.77 : 1);
    y = ((y - tablePositionInY - (GameUtils::isIphoneX ? (1.8 * kPixelToMeterRatio) : kPixelToMeterRatio)) / (GameUtils::isIphoneX ? 1.77 : 1));
    
    if(soClient) {
        soClient->send(__String::createWithFormat("{\"m\":\"updatePosition\",\"roomId\":\"%s\",\"xPos\":%f,\"yPos\":%f,\"bid\":%d}",PLAYER->getRoomId().c_str(),x,y, bodyId)->getCString());
    }
}

void LPTGameSceneRW::setImpulse(float x, float y, int bodyId) {
    if(soClient){
        soClient->send(__String::createWithFormat("{\"m\":\"setImpulse\",\"roomId\":\"%s\",\"x\":%f,\"y\":%f,\"bid\":%d}",PLAYER->getRoomId().c_str(),x,y,bodyId)->getCString());
    }
}

void LPTGameSceneRW::updateStrikerDraggingStatus(int bodyId, string isDragging) {
    if(soClient) {
        soClient->send(__String::createWithFormat("{\"m\":\"updateDragStatus\",\"roomId\":\"%s\",\"bid\":%d,\"isdragging\":\"%s\"}",PLAYER->getRoomId().c_str(), bodyId, isDragging.c_str())->getCString());
    }
}
#pragma mark - SIOClient Delegate
void LPTGameSceneRW::onClose(SIOClient* client) {
    soClient = NULL;
    CCLOG("SIODelegate onClose called");

    auto popup = LPTPopUp::showPopUpWith(kAlertErrorText, kDisconnectAlertText, false, GAMEQUIT_POPUP);
    popup->setPopUpDelegate(this);
    this->addChild(popup, 5000);
}

void LPTGameSceneRW::onError(SIOClient* client, const std::string& data) {
    soClient = NULL;
    CCLOG("SIODelegate onError Message: %s", data.c_str());
}

//test event callback handlers, these will be registered with socket.io
void LPTGameSceneRW::onRoomJoined(SIOClient *client, const std::string& data) {
//    CCLOGINFO("onRoomJoined::testevent called with data: %s", data.c_str());
        sendIntialStrikerPositions();
    //Update Rope body over remote physics in case iPhoneX
    if (GameUtils::isIphoneX) {
//        sendUpdateForRopePosition(client);
    }
}
void LPTGameSceneRW::onRoomLeft(SIOClient *client, const std::string& data) {
        CCLOGINFO("onRoomLeft::testevent called with data: %s", data.c_str());
//    MessageBox("User left the room", "Error");
}

// onMessage is no longer a required override from the delegate class
// 'message' events and handlers are now registered in the same way that other events are
void LPTGameSceneRW::message(network::SIOClient* client, const std::string& data)
{
//    CCLOGINFO("SocketIOTest::message received: %s", data.c_str());
    if (GameUtils::findSubstrInString(data,"world-update")) {
        updateWorld(data,true);
    }else if(GameUtils::findSubstrInString(data,"user_left")) {
        onRoomLeft(client, data);
    }
    
}

void LPTGameSceneRW::connect(network::SIOClient* client, const std::string& data)
{
    CCLOGINFO("SocketIOTest::connect called");
    client->emit(EVENT_ROOM, PLAYER->getRoomId());
}

void LPTGameSceneRW::disconnect(network::SIOClient* client, const std::string& data)
{
    this->closedSocketAction(client);
}

void LPTGameSceneRW::closedSocketAction(network::SIOClient* client)
{
    //set the local pointer to nullptr or connect to another client
    //the client object will be released on its own after this method completes
    if (client == soClient) {
        soClient = nullptr;
    }
}

double LPTGameSceneRW::getUserXP() {
    double gamePlayed = PLAYER->getPlayerGameWon();
    if(gamePlayed < 10) {
        gamePlayed =  gamePlayed * GameUtils::getRandomIntegerNumber(50, 75);
    }else if(gamePlayed < 100) {
        gamePlayed =  gamePlayed * GameUtils::getRandomIntegerNumber(10, 15);
    }else if(gamePlayed < 1000) {
        gamePlayed =  gamePlayed * GameUtils::getRandomIntegerNumber(6, 15);
    }else {
        gamePlayed =  gamePlayed * GameUtils::getRandomIntegerNumber(1, 3) * 0.2;
    }
    return gamePlayed;
}
