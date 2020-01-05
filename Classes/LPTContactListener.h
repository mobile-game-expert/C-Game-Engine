//
//  LPTContactListener.h
//  LePasseTrappe
//
//  Created by Vinod on 22/09/18.
//  Copyright (c) 2015 ChicMic. All rights reserved.
//

#ifndef __LePasseTrappe__LPTContactListener__
#define __LePasseTrappe__LPTContactListener__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "LPTHeaders.h"

class LPTGameScene;
class LPTGameSceneRW;

class LPTContactListener: public b2ContactListener
{
private:
    
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
public:
    LPTGameScene *gameSceneObject;
    LPTGameSceneRW *gameSceneObject_rw;
    LPTContactListener(BaseLayer *obj);
};


#endif /* defined(__LePasseTrappe__LPTContactListener__) */
