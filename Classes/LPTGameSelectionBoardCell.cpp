//
//  LPTGameSelectionBoardCell.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 27/09/18.
//

#include "LPTGameSelectionBoardCell.hpp"

bool LPTGameSelectionBoardCell::initWithSpriteName(string imageName, int tag, Size cellSize)
{
    if (!TableViewCell::init())
    {
        return false;
    }
    isCellEnabled       =   false;
    
//    ticketSprite = BaseLayer::createSprite(imageName, tag, Vec2(cellSize.width * 0.5, cellSize.height * 0.5), this);
//    ticketSprite->setScale(cellSize.width * 0.9 / ticketSprite->getContentSize().width);
    
    return true;
}
