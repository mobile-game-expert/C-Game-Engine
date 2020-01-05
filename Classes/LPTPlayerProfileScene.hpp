//
//  LPTPlayerProfileScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 01/10/18.
//

#ifndef LPTPlayerProfileScene_hpp
#define LPTPlayerProfileScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTPlayerProfileScene : public BaseScene {
public:
    CREATE_FUNC(LPTPlayerProfileScene);
    static LPTPlayerProfileScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
private:
};

#endif /* LPTPlayerProfileScene_hpp */
