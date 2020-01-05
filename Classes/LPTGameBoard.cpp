//
//  LPTGameBoard.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 19/09/18.
//

#include "LPTGameBoard.hpp"

#define kGameCenterBoardTag             5000
#define kGameBoardCornerTag             5001
#define kGameBoardDeflecterTag          5006
#define kGameBoardSensorTag             5007
#define kActualGameCenterBoardTag       5008

LPTGameBoard* LPTGameBoard::createLayer() {
    auto layer = LPTGameBoard::create();
    layer->init();
    return layer;
}

bool LPTGameBoard::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}

void LPTGameBoard::onEnter() {
    BaseLayer::onEnter();
}

void LPTGameBoard::setUpGameBoardUI(Size layerSize, Vec2 position, string boardImageName, string boardCornerImageName, string boardDivider, b2World *world) {
    this->setContentSize(layerSize);
    this->setPosition(position);
    log("scale value %f", position.x);
    LayerColor *layerColor = LayerColor::create(Color4B(255, 255, 255, 0), layerSize.width, layerSize.height);
    this->addChild(layerColor);
    
    this->addCenterTable(boardImageName, world);
    this->addGameBoardCorners(boardCornerImageName, world);
    this->addDividers(boardDivider, world);
    this->addBoardSensorDetector(world, -1);
    this->addBoardSensorDetector(world, 1);
}

#pragma mark GetCenterTable
BaseSprite* LPTGameBoard::getCenterTable() {
    BaseSprite *baseTable = (BaseSprite*)this->getChildByTag(kGameCenterBoardTag);
    return baseTable;
}

Rect LPTGameBoard::getCenterTableRectInRespectToGameScene(int playerSide) {
    BaseSprite *gameTable = this->getCenterTable();
    
    
    
    float positionInX   =   this->getPosition().x + gameTable->getPosition().x - (gameTable->getContentSize().width * 0.5 * gameTable->getScale());
    float positionInY   =   this->getPosition().y + gameTable->getPosition().y - (gameTable->getContentSize().height * 0.5 * gameTable->getScale());
    float width         =   gameTable->getContentSize().width * 0.475 * gameTable->getScale();
    float height        =   gameTable->getContentSize().height * gameTable->getScale();
    
    if(PLAY_AREA_RIGHT == playerSide) {
        positionInX     =   this->getPosition().x + gameTable->getPosition().x + (gameTable->getContentSize().width * 0.025 * gameTable->getScale()) ;
        positionInY     =   this->getPosition().y + gameTable->getPosition().y - (gameTable->getContentSize().height * 0.5 * gameTable->getScale());
        width           =   gameTable->getContentSize().width * 0.475 * gameTable->getScale();
        height          =   gameTable->getContentSize().height * gameTable->getScale();
    }
    Rect gameTableRect  =   Rect(positionInX, positionInY, width, height);
    return gameTableRect;
}


#pragma mark addingCenterTable
void LPTGameBoard::addCenterTable(string boardImageName, b2World *world) {
    //This center table is used to create bodies and sprite is hidden from the user.
    log("value of boardImageName : %s", boardImageName.c_str());
    BaseSprite *centerTable = BaseLayer::createSprite(boardImageName, kGameCenterBoardTag, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.50), this, 1);
    centerTable->setType(kUserDataTypeCenterTable);
    centerTable->setOpacity(0);
    centerTable->setScale(this->getContentSize().width / centerTable->getContentSize().width);
    
//    centerTable->setScale(this->getContentSize().height * 0.82 / centerTable->getContentSize().height);
//    if(this->getContentSize().width * 0.925 < (centerTable->getContentSize().width * centerTable->getScale())) {
//        centerTable->setScale(this->getContentSize().width * (GameUtils::isIpad ? 0.925 : 0.925) / centerTable->getContentSize().width);
//    }
    
    //This game table is visible to user.  NEED TO CHANGE
//    string boardName = "gameBoard_1.png";
//    log(UserDefault::getInstance()->getIntegerForKey(kAIMode));
//    if(PLAYER->getBetSelected() == BET_2)
//        boardName = "gameBoard_2.png";
//    else if(PLAYER->getBetSelected() == BET_3)
//        boardName = "gameBoard_3.png";
    
    string boardName = getBoardName();
    BaseSprite *actualCenterTable = BaseLayer::createSprite(boardName, kActualGameCenterBoardTag, Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.50), this, 1);
    actualCenterTable->setScale(centerTable->getScale());
    actualCenterTable->setOpacity(255);
    boardScale = centerTable->getScale();
}

string LPTGameBoard::getBoardName() {
    switch (PLAYER->getBetSelected()) {
        case BET_1:
            return kGameBoard1;
            break;
        case BET_2:
            return kGameBoard2;
            break;
        case BET_3:
            return kGameBoard3;
            break;
        case BET_4:
            return kGameBoard4;
            break;
        case BET_5:
            return kGameBoard5;
            break;
        case BET_6:
            return kGameBoard6;
            break;
        case BET_7:
            return kGameBoard7;
            break;
        case BET_8:
            return kGameBoard8;
            break;
        default:
            return kGameBoard1;
            break;
    }
}

void LPTGameBoard::addGameBoardCorners(string ImageName, b2World *world) {
    int cornerTag           =   kGameBoardCornerTag;
    BaseSprite *centerTable =   this->getCenterTable();
    
    this->gameCorner(ImageName + kCorner_1_Extension, Vec2(centerTable->getPosition().x, centerTable->getPosition().y + (centerTable->getContentSize().height * 0.5 * centerTable->getScale())), cornerTag++, world, Vec2(0, 1)); //TOP

    this->gameCorner(ImageName + kCorner_2_Extension, Vec2(centerTable->getPosition().x, centerTable->getPosition().y - (centerTable->getContentSize().height * 0.5 * centerTable->getScale())), cornerTag++, world, Vec2(0, -1)); //Bottom
    
    this->gameCorner(ImageName + kCorner_3_Extension, Vec2(centerTable->getPosition().x - (centerTable->getContentSize().width * 0.5 * centerTable->getScale()), centerTable->getPosition().y), cornerTag++, world, Vec2(-1, 0));
    this->gameCorner(ImageName + kCorner_4_Extension, Vec2(centerTable->getPosition().x + (centerTable->getContentSize().width * 0.5 * centerTable->getScale()), centerTable->getPosition().y), cornerTag, world, Vec2(1, 0));
}

void LPTGameBoard::gameCorner(string imageName, Vec2 position, int tag, b2World *world, Vec2 displacementFactor) {
    BaseSprite *tableCorner = BaseLayer::createSprite(imageName, tag, position, this, 1);
    tableCorner->setType(kUserDataTypeWall);
    tableCorner->setScale(boardScale);
    tableCorner->setPosition(Vec2(tableCorner->getPosition().x + (displacementFactor.x * tableCorner->getContentSize().width * boardScale * 0.5), tableCorner->getPosition().y + (displacementFactor.y * tableCorner->getContentSize().height * boardScale * 0.5)));
    //Adjusting tableWidth * 0.5 to position
    tableCorner->setOpacity(0);
    this->createPhysicsBody(tableCorner->getContentSize(), 1, 0.0, 1.0, world, false, tableCorner, Wall_Collision_Bit, 1);
}

#pragma mark GameCenterDeflection
void LPTGameBoard::addDividers(string imageName, b2World *world) {
    int dividerTag          =   kGameBoardDeflecterTag;
    BaseSprite *centerTable =   this->getCenterTable();
    
    this->addDivider(imageName + kCorner_1_Extension, Vec2(Vec2(centerTable->getPosition().x, centerTable->getPosition().y + (centerTable->getContentSize().height * 0.5 * centerTable->getScale()))), Vec2(0.5, 1), dividerTag++, world);
    
    this->addDivider(imageName + kCorner_2_Extension, Vec2(Vec2(centerTable->getPosition().x, centerTable->getPosition().y - (centerTable->getContentSize().height * 0.5 * centerTable->getScale()))), Vec2(0.5, 0), dividerTag, world);
}

void LPTGameBoard::addDivider(string imageName, Vec2 position, Vec2 anchorPoint, int tag, b2World *world) {
//    BaseSprite *divider = BaseLayer::createSprite(imageName, tag, position, this, 1);
//    //    divider->setOpacity(100);
    BaseSprite *centerTable = this->getCenterTable();
//    divider->setScale((centerTable->getContentSize().height * 0.43 * centerTable->getScale()) / divider->getContentSize().height);
//    if(anchorPoint.y == 1) {
//        divider->setPositionY(divider->getPosition().y - (divider->getContentSize().height * 0.5 * divider->getScale()));
//    }else if(anchorPoint.y == 0) {
//        divider->setPositionY(divider->getPosition().y + (divider->getContentSize().height * 0.5 * divider->getScale()));
//    }
    //temp Divider is just for creating shorter bodies
    BaseSprite *dividerForBodies = BaseLayer::createSprite(imageName, tag, position, this, 1);
    dividerForBodies->setType(kUserDataTypeDeflector);
    dividerForBodies->setScale((centerTable->getContentSize().height * 0.4 * centerTable->getScale()) / dividerForBodies->getContentSize().height);
    dividerForBodies->setOpacity(0);
    
    if(anchorPoint.y == 1) {
        dividerForBodies->setPositionY(dividerForBodies->getPosition().y - (dividerForBodies->getContentSize().height * 0.5 * dividerForBodies->getScale()));
    }else if(anchorPoint.y == 0) {
        dividerForBodies->setPositionY(dividerForBodies->getPosition().y + (dividerForBodies->getContentSize().height * 0.5 * dividerForBodies->getScale()));
    }
    this->createPhysicsBody(dividerForBodies->getContentSize(), 1, 0.0, 1.0, world, false, dividerForBodies, Wall_Collision_Bit, 1);
}

#pragma mark BoardSensor
void LPTGameBoard::addBoardSensorDetector(b2World *world, int sign) {
    BaseSprite *centerTable =   this->getCenterTable();
    float displacement      =   0.1;
    
//    if(UserDefault::getInstance()->getIntegerForKey(kAIMode) == BEGINNER)
//        displacement = 0.123;
    
    BaseSprite *sensor = BaseLayer::createSprite(kGameSensorImageName, kGameBoardSensorTag, Vec2(centerTable->getPosition().x, centerTable->getPosition().y + (centerTable->getContentSize().height * displacement * centerTable->getScale() * sign)), this, 1);
    sensor->setType(kUserDataTypeSensor);
    sensor->setOpacity(0);
    sensor->setScale((centerTable->getContentSize().height * 0.045 * centerTable->getScale()) / sensor->getContentSize().height);
    this->createPhysicsBody(sensor->getContentSize(), 1, 0.5, 1.0, world, false, sensor, Wall_Collision_Bit, 1, true);
}

#pragma mark AddingPhysicsBody
void LPTGameBoard::createPhysicsBody(Size bodySize, float density, float restitution, float friction, b2World *world, bool isSensor, BaseSprite *userData, int categoryBit, int groundIndex, bool isBodyCircular) {
    log("Position: %f", this->getPosition().x);
    Vec2 updatedPosition = Vec2(userData->getPosition().x + this->getPosition().x, userData->getPosition().y + this->getPosition().y);
    b2Body *wallBody = LPTPhysicsComponent::createBodyDefinition(b2_staticBody, updatedPosition, userData, world);
    
    b2FixtureDef wallFixtureDefinition;
    
    if(isBodyCircular) {
        b2CircleShape wallBodyShape;
        wallBodyShape.m_radius = (bodySize.width * userData->getScale() * 0.5) / kPixelToMeterRatio;
        wallFixtureDefinition  = LPTPhysicsComponent::createFixtureDefinition(&wallBodyShape, restitution, density, friction, isSensor, categoryBit, groundIndex);
    }else {
        b2PolygonShape wallBodyShape;
        wallBodyShape.SetAsBox((bodySize.width * userData->getScale() * 0.5) / kPixelToMeterRatio, (bodySize.height * userData->getScale() * 0.5) / kPixelToMeterRatio);
        wallFixtureDefinition  = LPTPhysicsComponent::createFixtureDefinition(&wallBodyShape, restitution, density, friction, isSensor, categoryBit, groundIndex);
    }
    wallBody->CreateFixture(&wallFixtureDefinition);
}

void LPTGameBoard::onExit() {
    BaseLayer::onExit();
}

