//
//  LPTTopBar.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 28/09/18.
//

#ifndef LPTTopBar_hpp
#define LPTTopBar_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTTopBar : public BaseLayer, public LPTPopUpDelegate {
public:
    CREATE_FUNC(LPTTopBar);
    static LPTTopBar *createLayer(string imageName);
    virtual bool initWithImageName(string imageName);
    virtual void onEnter();
    virtual void onExit();
    void setupUI(string player1, string profileImage1, string player2, string profileImage2);
    BaseSprite* getTopBarBaseSprite();
    void addPlayerProfile(string imageName, Vec2 position, Vec2 anchorPoint, int tag);
    void addBackButton();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void addChipCoin();
    void addProgressBar(double, double);
    Label* getCoinStatusLabel();
    void setCoinValue(double coinValue);
    BaseSprite* getCoinBaseSprite();
    void addPlayerNameBar( int tag, int profilePicTag, string playerName);
    void addTimerBaseSprite();
    Label* getTimerLabel();
    void updateTimerLabel(string timerValue);
//    void invalidateSessionResponse(void *response);
     void invalidateSessionResponse();
//    void logoutUser();
    
    void setPlayerName(string name, int tag);
    void updatePlayerProfile(string imageName);
    void profilePictureCallback(Ref* sender, Widget::TouchEventType type);
    //TOPBAR
    void addGameSelectionSceneTopBarUI(string player1, double coinValue);
    void addSettingButton(Vec2 position, int tag, Vec2 anchorPoint);
    void addCoinChipBase(double coinValue);
    void updateCoinText(double coinCount);
    void addUserProfileTopBar(int backButtonTag, string screenText, std::function<void(Ref*, Widget::TouchEventType)> callback);
    void sceneStatusBar(string screenName);
    void removeUserXPBar();
    //Popup callback
    virtual void okButtonAction(Popup_Type type);
    virtual void cancelButtonAction(Popup_Type type);
private:
protected:
};

#endif /* LPTTopBar_hpp */
