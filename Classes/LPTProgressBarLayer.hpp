//
//  LPTProgressBarLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 04/12/18.
//

#ifndef LPTProgressBarLayer_hpp
#define LPTProgressBarLayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class LPTProgressBarLayer : public BaseLayer {
public:
    CREATE_FUNC(LPTProgressBarLayer);
    static LPTProgressBarLayer* createLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void setupUI(double currentXP, double totalXP, float scale);
private:
    
};
#endif /* LPTProgressBarLayer_hpp */
