//
//  LPTGameSelectionBoardCell.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 27/09/18.
//

#ifndef LPTGameSelectionBoardCell_hpp
#define LPTGameSelectionBoardCell_hpp

#include <stdio.h>
#include "LPTHeaders.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::extension;

class LPTGameSelectionBoardCell : public TableViewCell
{
public:
    virtual bool initWithSpriteName(string spriteName, int tag, Size cellSize);
    bool isCellEnabled;
    BaseSprite *ticketSprite;
};
#endif /* LPTGameSelectionBoardCell_hpp */
