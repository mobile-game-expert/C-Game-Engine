//
//  LPTHUDLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 18/09/18.
//

#ifndef LPTHUDLayer_hpp
#define LPTHUDLayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTCoinComponent.hpp"
#include "LPTPlayerProfileData.hpp"

class LPTHUDLayerDelegate {
public:
    virtual void gameQuitByUser() = 0;
};

class LPTHUDLayer : public BaseLayer, public LPTPopUpDelegate {
public:
    CREATE_FUNC(LPTHUDLayer);
    static LPTHUDLayer *createLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    LPTHUDLayerDelegate *delegate;
    void setHUDDelegate(LPTHUDLayerDelegate *delegate);
    void setupUI(string player1, string profileImage1, string player2, string profileImage2, UserPlaySide selectedSide);
    BaseSprite* getHUDBaseSprite();
    void addPlayerProfile(string imageName, Vec2 position, Vec2 anchorPoint, int tag);
    void addBackButton();
    void addSideMenuButton();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    LPTPlayerProfileData* createProfilePlayerObjectFromUser(bool fromUser);
    void addChipCoin();
    Label* getCoinStatusLabel();
    void setCoinValue(double coinValue);
    LPTCoinComponent* getCoinBaseSprite();
    void addPlayerNameBar(int tag, int profilePicTag, string playerName, UserPlaySide selectedSide);
    void addTimerBaseSprite();
    Label* getTimerLabel();
    void updateTimerLabel(string timerValue);
    void setPlayerName(string name, int tag);
    void updatePlayerProfile(string imageName, Vec2 anchorPoint, int tag);
    //
    virtual void okButtonAction(Popup_Type type);
    virtual void cancelButtonAction(Popup_Type type);
    
    //SETTINGLAYER
    void addSettingLayer();
    
private:
    
protected:
};
#endif /* LPTHUDLayer_hpp */
