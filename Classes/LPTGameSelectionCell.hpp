//
//  LPTGameSelectionCell.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 28/09/18.
//

#ifndef LPTGameSelectionCell_hpp
#define LPTGameSelectionCell_hpp

#include <stdio.h>
#include "LPTHeaders.h"

using namespace cocos2d::extension;

class LPTGameSelectionCell : public TableViewCell
{
public:
    virtual bool initWithSpriteName(ValueMap cellData, Size cellSize, int tag);
    BaseSprite *ticketSprite;
    void addBetSelectionComponent(string coinBaseImageName, double coinPrice, string tableName);
};
#endif /* LPTGameSelectionCell_hpp */
