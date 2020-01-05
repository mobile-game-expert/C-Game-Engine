//
//  LPTSearchFriendCell.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 09/10/18.
//

#ifndef LPTSearchFriendCell_hpp
#define LPTSearchFriendCell_hpp

#include <stdio.h>
#include "LPTHeaders.h"

using namespace cocos2d::extension;

class LPTSearchFriendCellDelegate {
public:
    virtual void challengeButtonPressed(Button *sender,bool alreadyChallenged) = 0;
};

class LPTSearchFriendCell : public TableViewCell
{
public:
    virtual bool initWithFriendData(FriendData cellData, Size cellSize, int tag);
    void setCellDelegate(LPTSearchFriendCellDelegate *delegate);
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
private:
    LPTSearchFriendCellDelegate *delegate;
    void setupCell(FriendData cellData, Size cellSize, int tag);
};
#endif /* LPTSearchFriendCell_hpp */
