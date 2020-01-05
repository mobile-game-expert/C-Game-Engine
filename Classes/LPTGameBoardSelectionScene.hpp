//
//  LPTGameBoardSelectionScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 05/10/18.
//

#ifndef LPTGameBoardSelectionScene_hpp
#define LPTGameBoardSelectionScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTGameBoardSelectionScene : public BaseLayer {
public:
    CREATE_FUNC(LPTGameBoardSelectionScene);
    static BaseScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void setupUI();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    
private:
};
#endif /* LPTGameBoardSelectionScene_hpp */
