//
//  LPTBetSelectionScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 28/09/18.
//

#ifndef LPTBetSelectionScene_hpp
#define LPTBetSelectionScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTHorizontalScrollView.hpp"


class LPTBetSelectionScene: public BaseLayer, LPTHorizontalScrollViewDelegate ,public  LPTPopUpDelegate, public AppWarp::ZoneRequestListener, public AppWarp::RoomRequestListener{
public:
    CREATE_FUNC(LPTBetSelectionScene);
    static BaseScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void tableCellSelectedWithIndex(int index);
    
private:
    vector<string> arrayBets;
    void setupBetsData();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void setupUI();
    void addGameTopBar();
    virtual void okButtonAction(Popup_Type type);
    virtual void cancelButtonAction(Popup_Type type);
    void moveToNextScene();
protected:
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
};
#endif /* LPTBetSelectionScene_hpp */
