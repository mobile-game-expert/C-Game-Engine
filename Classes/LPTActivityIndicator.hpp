//
//  LPTActivityIndicator.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 14/09/18.
//

#ifndef LPTActivityIndicator_hpp
#define LPTActivityIndicator_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <cocos-ext.h>
#include "BaseLayer.hpp"


using namespace cocos2d;
using namespace ui;

class LPTActivityIndicator :BaseLayer {
public:    
    static void startAnimating(Node *parent,string msg = "");
    static void stopAnimating(Node *parent);
};

#endif /* LPTActivityIndicator_hpp */
