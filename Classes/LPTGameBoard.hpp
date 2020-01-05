//
//  LPTGameBoard.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 19/09/18.
//

#ifndef LPTGameBoard_hpp
#define LPTGameBoard_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTGameBoard : public BaseLayer {
public:
    CREATE_FUNC(LPTGameBoard);
    static LPTGameBoard *createLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    string getBoardName();
    //Variable
    float boardScale =  1;
    //Methods
    void setUpGameBoardUI(Size layerSize, Vec2 position, string boardImageName, string boardCornerImageName, string boardDivider, b2World *world);
    void addCenterTable(string boardImageName, b2World *world);
    void addGameBoardCorners(string ImageName, b2World *world);
    void gameCorner(string imageName, Vec2 position, int tag, b2World *world, Vec2 displacementFactor);
    BaseSprite* getCenterTable();
    void createPhysicsBody(Size bodySize, float density, float restitution, float friction, b2World *world, bool isSensor, BaseSprite *userData, int categoryBit, int groundIndex, bool isBodyCircular = false);
    void addDivider(string imageName, Vec2 position, Vec2 anchorPoint, int tag, b2World *world);
    void addDividers(string imageName, b2World *world);
    void addBoardSensorDetector(b2World *world, int sign);
    Rect getCenterTableRectInRespectToGameScene(int playerSide);
};
#endif /* LPTGameBoard_hpp */
