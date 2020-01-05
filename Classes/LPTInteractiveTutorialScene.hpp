//
//  LPTInteractiveTutorialScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 29/11/18.
//

#ifndef LPTInteractiveTutorialScene_hpp
#define LPTInteractiveTutorialScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTInteractiveTutorialScene: public BaseScene{
public:
    CREATE_FUNC(LPTInteractiveTutorialScene);
    static LPTInteractiveTutorialScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    private :
    void setupUI();
    
};
#endif /* LPTInteractiveTutorialScene_hpp */
