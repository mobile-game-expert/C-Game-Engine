//
//  LPTGameSelectionScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 24/09/18.
//

#ifndef LPTGameSelectionScene_hpp
#define LPTGameSelectionScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include  "LPTHorizontalScrollView.hpp"
#include "LPTNotificationPopup.hpp"
#include "LPTServerListener.hpp"


class LPTGameSelectionScene: public BaseLayer,public LPTHorizontalScrollViewDelegate, public LPTPopUpDelegate, public AppWarp::ConnectionRequestListener, public AppWarp::RoomRequestListener, public AppWarp::ZoneRequestListener, public AppWarp::NotificationListener, public LPTServerDelegate {
public:
    CREATE_FUNC(LPTGameSelectionScene);
    static BaseScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void tableCellSelectedWithIndex(int index);
    
private:
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void setupUI();
    void checkForValidSession();
    void fetchExistingUserData(void* response);
//    void logoutUser();
    void invalidateSessionResponse(void *response);
    void addTopBarInGameSelectionScene();
    
    virtual void okButtonAction(Popup_Type type);
    virtual void cancelButtonAction(Popup_Type type);
    
    //--ask user Age
    void addUserAgePanel();
    void removeUserAgePanel();
protected:
    //ConnectionRequestListener
    void onConnectDone(int res, int code){};
    void onDisconnectDone(int res){};
    //RoomRequestListener
    void onSubscribeRoomDone(AppWarp::room revent){};
    void onJoinRoomDone(AppWarp::room revent){};
    void onLeaveRoomDone (AppWarp::room revent);
    void onGetLiveRoomInfoDone(AppWarp::liveroom revent){};
    void onUpdatePropertyDone(AppWarp::liveroom revent){};
    //ZoneRequestListener
    void onCreateRoomDone (AppWarp::room revent){};
    void onDeleteRoomDone (AppWarp::room revent);
    void onGetAllRoomsDone (AppWarp::liveresult res){};
    void onGetOnlineUsersDone (AppWarp::liveresult res){};
    void onGetLiveUserInfoDone (AppWarp::liveuser uevent){};
    void onGetMatchedRoomsDone(AppWarp::matchedroom mevent){};
    //NotificationListener
    void onRoomCreated(AppWarp::room rData){};
    void onUserLeftRoom(AppWarp::room rData, std::string user){};
    void onUserJoinedRoom(AppWarp::room rData, std::string user){};
    void onChatReceived(AppWarp::chat chatevent){};
    void onUpdatePeersReceived(AppWarp::byte update[], int len, bool isUDP){};
    
    //SERVER DELEGATE
    void serverResponseComplete(int tag);
    void serverListenerResponseFaliure(int tag, string message);
    void serverResponseCompletedWithData(picojson::array data);
    void updateTopBarUI();
};

#endif /* LPTGameSelectionScene_hpp */
