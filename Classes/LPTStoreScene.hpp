//
//  LPTStoreScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 17/09/18.
//

#ifndef LPTStoreScene_hpp
#define LPTStoreScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTStoreScene : public BaseScene {
public:
    CREATE_FUNC(LPTStoreScene);
    static BaseScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void setupUI();
protected:
};
#endif /* LPTStoreScene_hpp */
