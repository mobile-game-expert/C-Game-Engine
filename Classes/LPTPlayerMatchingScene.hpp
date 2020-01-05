//
//  LPTPlayerMatchingScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 17/09/18.
//

#ifndef LPTPlayerMatchingScene_hpp
#define LPTPlayerMatchingScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTServerListener.hpp"

class LPTPlayerMatchingScene : public BaseLayer, public AppWarp::ConnectionRequestListener, public AppWarp::RoomRequestListener, public AppWarp::ZoneRequestListener, public AppWarp::NotificationListener, public LPTServerDelegate {
public:
    CREATE_FUNC(LPTPlayerMatchingScene);
    static BaseScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
private:
    BetSelected userBet;
    void setupAppwarp();
    void addGameTopBar();
    void setupUI();
    void addScheduleAutomaticSearchStop();
    void stopSearch(float delta);
    void addPlayerMatchingLayer();
    void moveToGameScene(float delta);
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    //App42 Calls
    void updateUserDocumentForCoinsResponse(void *response);
    void getOpponentDetailsFromUserName(string userName);
    void fetchExistingUserData(void* response);;
    //AppaWarp Calls
    void getRoomWithProperties(std::map<std::string, std::string> tableProperty);
    void scheduleRecover();
    void unscheduleRecover();
    void recover(float dt);
    //ConnectionRequestListener
    void onConnectDone(int res, int code);
    void onDisconnectDone(int res);
    //RoomRequestListener
    void onSubscribeRoomDone(AppWarp::room revent);
    void onJoinRoomDone(AppWarp::room revent);
    void onLeaveRoomDone (AppWarp::room revent);
    void onGetLiveRoomInfoDone(AppWarp::liveroom revent);
    void onUpdatePropertyDone(AppWarp::liveroom revent);
    //ZoneRequestListener
    void onCreateRoomDone (AppWarp::room revent);
    void onDeleteRoomDone (AppWarp::room revent);
    void onGetAllRoomsDone (AppWarp::liveresult res);
    void onGetOnlineUsersDone (AppWarp::liveresult res);
    void onGetLiveUserInfoDone (AppWarp::liveuser uevent);
    void onGetMatchedRoomsDone(AppWarp::matchedroom mevent);
    //NotificationListener
    void onRoomCreated(AppWarp::room rData);
    void onUserLeftRoom(AppWarp::room rData, std::string user);
    void onUserJoinedRoom(AppWarp::room rData, std::string user);
    void onChatReceived(AppWarp::chat chatevent);
    void onUpdatePeersReceived(AppWarp::byte update[], int len, bool isUDP);
    //ServerCalls
    void serverResponseComplete(int tag);
    void serverListenerResponseFaliure(int tag, string message);
    void serverResponseCompletedWithData(picojson::array data);
protected:
};
#endif /* LPTPlayerMatchingScene_hpp */
