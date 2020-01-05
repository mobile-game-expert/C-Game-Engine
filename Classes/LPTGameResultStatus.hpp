//
//  LPTGameResultStatus.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 01/10/18.
//

#ifndef LPTGameResultStatus_hpp
#define LPTGameResultStatus_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTGameResultDelegate {
public:
    virtual void reMatchButtonAction() = 0;
};

class LPTGameResultStatus : public BaseLayer {
public:
    CREATE_FUNC(LPTGameResultStatus);
    static LPTGameResultStatus *createLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CC_SYNTHESIZE(LPTGameResultDelegate *, delegate, Delegate);
    //
    void setupUI(double coinValue, bool isGameWon, string playerImageName, string opponentPlayerImageName, string playerName, string opponentPlayerName);
    void addCoinsSection(double coinValue);
    void addResultStatus(bool isGameWon);
    void addUserProfile(Vec2 position, Vec2 anchorPoint, int tag, bool isGameWon, string userProfileImageName, string userName);
    void addGameButton(string imageName, int tag, Vec2 position);
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void runCoinAnimation(bool isGameWon);
    void manageSoundEffects() ;
    void hideRematchButton();
private:
    BaseSprite *resultBaseSprite;
};
#endif /* LPTGameResultStatus_hpp */
