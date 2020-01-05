//
//  LPTCoinComponent.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 03/10/18.
//

#ifndef LPTCoinComponent_hpp
#define LPTCoinComponent_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTCoinComponent : public BaseSprite {
public:    
    CREATE_FUNC(LPTCoinComponent);
    static LPTCoinComponent *createCoinComponent(string coinBaseImageName, Vec2 position, int tag, Vec2 anchorPoint);
    virtual bool initWithSpriteName(string coinBaseImageName, Vec2 position, int tag, Vec2 anchorPoint);
    void setGameComponentUI(double chipCoinValue);
    void setGameResultUI();
    void setGameSelectionUI(std::function<void(Ref*, Widget::TouchEventType)> callback, int buyButtonTag);
    void updateCoinText(double coinValue);
    virtual void onEnter();
    virtual void onExit();
    
    //Button Press Component
    
    
private:
protected:
};

#endif /* LPTCoinComponent_hpp */
