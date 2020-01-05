//
//  LPTNotificationPopup.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 15/10/18.
//

#ifndef LPTNotificationPopup_hpp
#define LPTNotificationPopup_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTNotificationPopup : public BaseLayer {
public:
    CREATE_FUNC(LPTNotificationPopup);
    static LPTNotificationPopup* createLayer(LPTOpponent *opponent, string notificationType);
    virtual bool initWithOpponent(LPTOpponent *opponent, string notificationType);
    virtual void onEnter();
    virtual void onExit();
private :
    LPTOpponent *opponent;
    string notificationType;
    LayerColor *baseColorLayer;
    void setupChallengeUI();
    void setupAcceptChallengeUI();
    void setupRejectChallengeUI();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void updateOpponentObject();
};

#endif /* LPTNotificationPopup_hpp */
