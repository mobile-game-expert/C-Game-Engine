//
//  LPTInteractiveTutorialLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 29/11/18.
//

#ifndef LPTInteractiveTutorialLayer_hpp
#define LPTInteractiveTutorialLayer_hpp


#include <stdio.h>
#include "LPTHeaders.h"
#include "ui/CocosGUI.h"
#include <Box2D/Box2D.h>
#include "GLES-Render.h"
#include "LPTStriker.hpp"

using namespace cocos2d::ui;

class LPTInteractiveTutorialLayer : public BaseLayer, public LPTPopUpDelegate {
public:
    CREATE_FUNC(LPTInteractiveTutorialLayer);
    static LPTInteractiveTutorialLayer* createLayer(int backButtonTag);
    virtual bool initWithTag(int backButtonTag);
    virtual void onEnter();
    virtual void onExit();
    
private:
    
    int locationOfPointingHand = 0;
    int selectedStrikerIndex = -1, delayTimer = 100;
    int touchedStrikerIndex = -1, touchableStrikerCount;
    Vec2 lastStrikerPosition;
    bool collisionBetweenRopeAndStrikerSarted = false;
    int backButtonTag;
    LPTStriker *aiDraggableStriker;
    vector<LPTStriker*> arrayStriker;
    EventListenerTouchOneByOne *touchListener;
    
    b2World *world;
    GLESDebugDraw *debugDraw;
    
    void addTouchListener();
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);
    
    void addInformationLabel();
    void updateInstructionText(string text);
    string getInstructionText();
    
    void setupUI();
    virtual void okButtonAction(Popup_Type type);
    virtual void cancelButtonAction(Popup_Type type);
    BaseSprite* getCenterTable();
    void addGameTable();
    void createPhysicalWorld();
    void addStriker();
    void addRopeInGameScene(int tag, bool isBulletTrue);
    void updateTouchableStrikers();
    Rect getTableTouchableArea();
    void changeSelectedStrikerVelocity(b2Body *body);
    void changeStatusOfRope();
    void updateRopeTexture(int segment, int ropeTag);
    bool checkForTouchableArea(Vec2 touch);
    bool isStrikerInsideTouchableArea(LPTStriker* striker, Vec2 strikerPosition, Rect touchableArea);
    bool checkFeasiblePositionOfStrikerY (Vec2 touchPoint, Rect area);
    bool checkFeasiblePositionOfStrikerX (Vec2 touchPoint, Rect area);
    bool checkForStrikerPassingArea(Vec2 touch);
    void checkStrikerMovementWhenHitWithRope();
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformUpdated);
    void scheduleTick();
    bool checkForWinningCondition();
    void addPointingHand(int location);
    void removePointingHand();
    Vec2 getPointHandInitialLocation();
    Vec2 getPointHandFinalLocation();
    void tick(float dt);
};
#endif /* LPTInteractiveTutorialLayer_hpp */
