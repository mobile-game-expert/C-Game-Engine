//
//  LPTLoginScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 11/09/18.
//

#ifndef LPTLoginScene_hpp
#define LPTLoginScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTServerListener.hpp"

class LPTLoginScene : public BaseLayer, public sdkbox::FacebookListener, public LPTServerDelegate {
public:
    CREATE_FUNC(LPTLoginScene);
    static BaseScene *createScene(LOGOUT_TYPE logoutType);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void addStars();
    void addGameTitle();
    void setUploginSceneUI();
    void setUpSignUpUI();
    void setUpLoginUI();
    void setupForgotPasswordUI();
    void setupCreateUserNameUI();
    void moveGameSelectionScene();
    void facebookLoginProcessStarted();
    static LOGOUT_TYPE logoutType;
private:
    App42Object *tempFBObj;
    string fbUserId, fbEmailID;
    double totalCountOfGuestUsers;
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void submitButtonFunctionality();
    //====
    void authenticateApp42User(const char* email,const char* password);
    void createNewEmailUser(const char* uName, const char* password, const char* emailID);
    void resetPassword(const char* email);
    void createUserName(const char* uName);
    void createGuestCollection();
    //==== App42 callbacks
    void saveIntialPlayerData(const char* uName,const char* emailId, int totalWins, int totalLose, int totalCoins, bool isFBUser, bool isApp42User,bool isGuest);
    void saveFBUserDetails();
    void guestLoginResponse(void *response);
    void authenticateUserResponse(void *response);
    void fetchExistingUserData(void* response);
    void getuserByEmailResponse(void* response);
    void resetPasswordResponse(void* response);
    void createUserNameResponse(void *response);
    void checkForFBLogin(void* response);
    void facebookUserInfoResposne(void* response);
    void fetchCountOfGuestUserResponse(void *response);
    //==== SDKBox Facebook Plugin Listener
    void onLogin(bool isLogin, const std::string& msg);
    void onSharedSuccess(const std::string& message){} ;
    void onSharedFailed(const std::string& message) {};
    void onSharedCancel() {};
    void onAPI(const std::string& key, const std::string& jsonData) ;
    void onPermission(bool isLogin, const std::string& msg) ;
    void onFetchFriends(bool ok, const std::string& msg) ;
    void onRequestInvitableFriends( const FBInvitableFriendsInfo& friends ) ;
    void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg ) ;
    void onInviteFriendsResult( bool result, const std::string& msg ) ;
    void onGetUserInfo( const FBGraphUser& userInfo ) ;
    void signupUI();
    //User Local Session
    void createUserLocalSession(const char* sessionId, const char* usernName,const char* email, LOGIN_STATE state);
    
    //SERVER DELEGATE
    void serverResponseComplete(int tag);
    void serverListenerResponseFaliure(int tag, string message);
    void serverResponseCompletedWithData(picojson::array data);
protected:
};

#endif /* LPTLoginScene_hpp */
