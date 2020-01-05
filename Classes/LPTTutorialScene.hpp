//
//  LPTTutorialScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 11/10/18.
//

#ifndef LPTTutorialScene_hpp
#define LPTTutorialScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include  "LPTHorizontalScrollView.hpp"

class LPTTutorialScene: public BaseScene{
public:
    CREATE_FUNC(LPTTutorialScene);
    static LPTTutorialScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
private :
    void setupUI();
    
};
#endif /* LPTTutorialScene_hpp */
