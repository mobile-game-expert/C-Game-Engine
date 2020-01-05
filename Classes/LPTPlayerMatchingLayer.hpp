//
//  LPTPlayerMatchingLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 04/10/18.
//

#ifndef LPTPlayerMatchingLayer_hpp
#define LPTPlayerMatchingLayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTPlayerMatchingLayer : public BaseLayer {
public:
    CREATE_FUNC(LPTPlayerMatchingLayer);
    static LPTPlayerMatchingLayer *createLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    //
    void setupUI(double coinValue, bool isGameWon, string opponentPlayerImageName, string playerImageName, string playerName, string opponentPlayerName);
    void setupUIForPlayerMatching(double coinValue, bool isGameWon, string opponentPlayerImageName, string playerImageName, string playerName, string opponentPlayerName);
    void addCoinsSection(double coinValue);
    void addVSTitle();
    void addUserProfile(Vec2 position, Vec2 anchorPoint, int tag, bool isGameWon, string userProfileImageName, string userName);
    void addUserCoins(double coinValue, Vec2 anchorPoint, bool isOpponent, int tag);
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void runCoinAnimationForUser(bool fromUser);
    void opponentSearchingAnimation();
    void stopSearchingAnimation();
    void updateOpponentProfile(string imageName);
    void updateCoinsAfterBet(double coins, int tag);
    void updateOpponentUniqueName(string name) ;
    void updateBetAmount(double amount);
    void manageSoundEffects();
    void updatePlayerAppwarpStatus(string text);
private:
    BaseSprite *matchBaseSprite;
};
#endif /* LPTPlayerMatchingLayer_hpp */
