//
//  LPTSearchFriendsScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 08/10/18.
//

#ifndef LPTSearchFriendsScene_hpp
#define LPTSearchFriendsScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTSearchFriendCell.hpp"
#include "LPTServerListener.hpp"

using namespace cocos2d::extension;

class LPTSearchFriendsScene : public BaseLayer, TableViewDataSource, TableViewDelegate, LPTSearchFriendCellDelegate, public LPTPopUpDelegate, public EditBoxDelegate , public AppWarp::ZoneRequestListener, public LPTServerDelegate {
public:
    
    CREATE_FUNC(LPTSearchFriendsScene);
    static BaseScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
private:
    
    TableView *friendListTableView;
    vector<FriendData> arrayFriends;
    vector<FriendData> arraySearchResult;
    vector<string> arrayOnlineUsers;
    Size cellSize;
    
    void setupData();
    void setupUI();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    
    Size cellSizeForTable(TableView *table);
    //Table Methods
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    //Challenge button callback from cell
    virtual void challengeButtonPressed(Button *sender,bool alreadyChallenged);
    void sendChallangeNotification(string userName);
    void updateCompleteSearchArray(FriendData challengedUser);
    //Search username
    void searchFriendWithUsername(string username);
    //App42 calls
    void fetchAllUsers();
    void fetchAllUsersResponse(void *response);
    void getTotalUsersCount();
    void getTotalUsersCountResponse(void* response);
    //Popup callback
    virtual void okButtonAction(Popup_Type type);
    virtual void cancelButtonAction(Popup_Type type);
    //EditBox Delegate
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    //Push notification callback
    void onPushRequestCompleted( void *response);
    //ZoneRequestListener
    void onCreateRoomDone (AppWarp::room revent) {};
    void onDeleteRoomDone (AppWarp::room revent) {};
    void onGetAllRoomsDone (AppWarp::liveresult res) {};
    void onGetOnlineUsersDone (AppWarp::liveresult res);
    void onGetLiveUserInfoDone (AppWarp::liveuser uevent) {};
    void onGetMatchedRoomsDone(AppWarp::matchedroom mevent) {};
    //Online status
    //
    void serverResponseComplete(int tag);
    void serverListenerResponseFaliure(int tag, string message);
    void serverResponseCompletedWithData(picojson::array data);
    
};
#endif /* LPTSearchFriendsScene_hpp */
