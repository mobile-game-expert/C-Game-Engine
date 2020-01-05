//
//  LPTGameSceneRW.hpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 31/10/18.
//

#ifndef LPTGameSceneRW_hpp
#define LPTGameSceneRW_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTLoginScene.hpp"
#include "LPTStriker.hpp"
#include "LPTRope.hpp"
#include <Box2D/Box2D.h>
#include "GLES-Render.h"
#include "LPTHUDLayer.hpp"
#include "LPTGameResultStatus.hpp"
#include <network/SocketIO.h>

struct b2BodyData {
    // ...other properties
    int bodyId;
};
/*
struct GameSceneUIData {
    int RopeTag;
    UserPlaySide playingArea;
    int indexOfStriker;
};

enum PLAYER_STATE{
    IS_PLAYING,
    HAS_WON,
    HAS_LOST
};
*/
using namespace network;
USING_NS_CC;

class LPTGameSceneRW : public BaseLayer,public LPTGameResultDelegate, public LPTPopUpDelegate, public AppWarp::ConnectionRequestListener, public AppWarp::RoomRequestListener, public AppWarp::ZoneRequestListener, public AppWarp::NotificationListener, public LPTHUDLayerDelegate, public SocketIO::SIODelegate {
public:
    
    CREATE_FUNC(LPTGameSceneRW);
    static BaseScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void addGameTable();
    void addGameHUDLayer();
    void addStriker();
    void changeStatusOfRope();
    void changeStatusOfRopeForOpponent();
    //TouchListener
    void addTouchListener();
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);
    //PhysicsComponent
    b2World *world;
    GLESDebugDraw *debugDraw;
    void createPhysicalWorld();
    bool collisionBetweenRopeAndStrikerSarted = false, createRopeAccordingToBall = false;
    bool collisionBetweenRopeAndStrikerStartedForOpponent = false, createRopeAccordingToBallForOpponent = false;
    void scheduleTick();
    void tick(float dt);
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformUpdated);
    void createElasticRope();
    b2Body* groundBody;
    b2MouseJoint * mouseJoint;
    //==Oppoenet Player UI Updation
    void updateOpponentUI(int strikerTag, Vec2 positionOnRope, b2Vec2 impulse);
    //===
    void updateWorld(const std::string& data, bool shouldTransform);
    SIOClient *soClient;
    void connectToSocket();
    void sendIntialStrikerPositions();
    void sendStrikerPositions(float x, float y, int bodyId);
    void sendUpdateForRopePosition(SIOClient* client);
    void setImpulse(float x, float y, int bodyId);
    void updateStrikerDraggingStatus(int bodyId, string isDragging);
//    void onConnect(SIOClient* client);
//    void onMessage(SIOClient* client, const std::string& data);
//    void onClose(SIOClient* client);
//    void onError(SIOClient* client, const std::string& data);
    //
    /**
     *  @brief Used for network level socket close (not for disconnect from the socket.io server)
     */
    virtual void onClose(cocos2d::network::SIOClient* client);
    /**
     *  @brief Used for network level socket error (not for disconnect from the socket.io server)
     **/
    virtual void onError(cocos2d::network::SIOClient* client, const std::string& data);
    /**
     *  @brief Common function to call on both socket.io disconnect and websocket close
     **/
    void closedSocketAction(cocos2d::network::SIOClient* client);
    /**
     *  @brief Socket.io event handler for custom event "roomjoined"
     **/
    void onRoomJoined(cocos2d::network::SIOClient *client, const std::string& data);
    /**
     *  @brief Socket.io event handler for custom event "user_left"
     **/
    void onRoomLeft(cocos2d::network::SIOClient *client, const std::string& data);
    /**
     *  @brief Socket.io event handler for event "connect"
     **/
    void connect(cocos2d::network::SIOClient* client, const std::string& data);
    /**
     *  @brief Socket.io event handler for event "disconnect"
     **/
    void disconnect(cocos2d::network::SIOClient* client, const std::string& data);
    /**
     *  @brief Socket.io event handler for event "message"
     **/
    void message(cocos2d::network::SIOClient* client, const std::string& data);
private:
    int signValue = 0;
    float selectedStrikerPreviousPosition = 0.0;
    int selectedStrikerIndex = -1;
    int opponentStrikerIndex;
    int gameTimeInSeconds, touchedStrikerIndex = -1, touchableStrikerCount;
    int opponentRopeTag;
    GameSceneUIData userUIData, opponentUIData;
    Vec2 lastStrikerPosition;
    vector<LPTStriker*> arrayStriker;
    vector<int> arrayOpponentStrikersTag;
    PhysicsWorld *physicsWorld;
    EventListenerTouchOneByOne *touchListener;
    LPTStriker *aiDraggableStriker;
    //Methods
    BaseSprite* getCenterTable();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void logoutUser();
    void updateUI();
    void addRopeInGameScene(int tag, bool isBulletTrue);
    void updateRopeTexture(LPTStriker *striker, int ropeTextureTag);
    void changeSelectedStrikerVelocity(b2Body *body);
    //Timer
    void startGame();
    void updateGameTimer(float delta);
    void stopGameTimer();
    //AI game play, Single Player
    void opponentHitStriker();
    //Check for user touch
    LPTStriker* addSimulatedStriker(Vec2 position, float scale);
    void transformStrikerBody(LPTStriker *striker, float xPos, float yPos,Touch* touch, Event* event);
    bool checkForTouchableArea(Vec2 touch);
    bool checkFeasiblePositionOfStrikerX (Vec2 touchPoint, Rect area);
    bool checkFeasiblePositionOfStrikerY (Vec2 touchPoint, Rect area);
    Rect getTableTouchableArea();
    bool checkForStrikerPassingArea(Vec2 touch);
    bool isStrikerInsideTouchableArea(LPTStriker* striker, Vec2 strikerPosition, Rect touchableArea);
    //Check for winning condition
    void updateTouchableStrikers();
    PLAYER_STATE checkForWinningCondition();
    bool checkIfStrikerIsRightWay(LPTStriker *striker) ;
    int getStrikerIndexForAI();
    void playAIMovement(float delta);
    void setupGameData();
    //GameResult
    void addResultScreen(bool hasWon,bool shouldHideRematch);
    void removeResultScreen();
    virtual void reMatchButtonAction();
    virtual void okButtonAction(Popup_Type type);
    virtual void cancelButtonAction(Popup_Type type);
    //App42 calls
    void updateUserDocumentForGameResult();
    void updateUserDocumentForGameResultResponse(void *response);
protected:
    //ConnectionRequestListener
    void onConnectDone(int res, int code);
    void onDisconnectDone(int res);
    //RoomRequestListener
    void onSubscribeRoomDone(AppWarp::room revent){};
    void onJoinRoomDone(AppWarp::room revent){};
    void onLeaveRoomDone (AppWarp::room revent);
    void onGetLiveRoomInfoDone(AppWarp::liveroom revent){};
    void onUpdatePropertyDone(AppWarp::liveroom revent){};
    //ZoneRequestListener
    void onCreateRoomDone (AppWarp::room revent){};
    void onDeleteRoomDone (AppWarp::room revent);
    void onGetAllRoomsDone (AppWarp::liveresult res);
    void onGetOnlineUsersDone (AppWarp::liveresult res);
    void onGetLiveUserInfoDone (AppWarp::liveuser uevent);
    void onGetMatchedRoomsDone(AppWarp::matchedroom mevent);
    //NotificationListener
    void onRoomCreated(AppWarp::room rData){};
    void onUserLeftRoom(AppWarp::room rData, std::string user);
    void onUserJoinedRoom(AppWarp::room rData, std::string user){};
    void onChatReceived(AppWarp::chat chatevent);
    void onUpdatePeersReceived(AppWarp::byte update[], int len, bool isUDP);
    //LPTHUDLayerDelegate
    virtual void gameQuitByUser();
    //Game win and lose changes
    void playerWinGameUpdation();
    void playerLoseGameUpdation();
    
    float tablePositionInX, tablePositionInY;
    bool createLayer = false;
    
    //CheckStriker Movement When Hit with Rope
    void checkStrikerMovementWhenHitWithRope();
    double getUserXP();
    int strikerCollisionCounter = 0;
    bool isResultDeclared = false;
};

#endif /* LPTGameSceneRW_hpp */
