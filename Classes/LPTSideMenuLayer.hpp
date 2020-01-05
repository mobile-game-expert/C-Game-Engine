//
//  LPTSideMenuLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 08/10/18.
//

#ifndef LPTSideMenuLayer_hpp
#define LPTSideMenuLayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTSideMenuLayer : public BaseLayer {
public:
    CREATE_FUNC(LPTSideMenuLayer);
    static LPTSideMenuLayer *createLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
#pragma mark CustomMethods
    void setupUI(Size layerSize, std::function<void(Ref*, Widget::TouchEventType)> callback);
    void addButtons(std::function<void(Ref*, Widget::TouchEventType)> callback);
    //Component
    BaseSprite *sideMenuBase;
    
};
#endif /* LPTSideMenuLayer_hpp */
