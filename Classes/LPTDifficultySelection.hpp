//
//  LPTDifficultySelection.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 28/09/18.
//

#ifndef LPTDifficultySelection_hpp
#define LPTDifficultySelection_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include  "LPTHorizontalScrollView.hpp"

class LPTDifficultySelection: public BaseLayer, LPTHorizontalScrollViewDelegate {
public:
    CREATE_FUNC(LPTDifficultySelection);
    static BaseScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void tableCellSelectedWithIndex(int index);
    
private:
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void setupUI();
    void addGameTopBar();

protected:
};

#endif /* LPTDifficultySelection_hpp */
