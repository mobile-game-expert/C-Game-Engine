//
//  LPTHorizontalScrollView.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 27/09/18.
//

#ifndef LPTHorizontalScrollView_hpp
#define LPTHorizontalScrollView_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTGameSelectionCell.hpp"
using namespace cocos2d::extension;
class LPTHorizontalScrollViewDelegate
{
public:
    virtual void tableCellSelectedWithIndex(int index) = 0;
};

class LPTHorizontalScrollView: public BaseLayer, TableViewDataSource, TableViewDelegate {
public:
    
    int currentSelectedTab = -1;
    int maxNumberOfCellIndicator = 0;
    CREATE_FUNC(LPTHorizontalScrollView);
    static LPTHorizontalScrollView *createLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void setupUI(vector<ValueMap> imageName, Vec2 position, Size contentSize);
    void setSelectionLayerDelegate(LPTHorizontalScrollViewDelegate *delegate);
private:
    
    LPTHorizontalScrollViewDelegate *selectionLayerDelegate;
    
    //
    vector<ValueMap> gameSelectionImagesName;
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    //Method
    void addTableView();
//    void addDataToTableViewCell(LPTGameSelectionCell *cell, Size cellSize, ValueMap cellData);
    void addSelectedIndicator();
    void changeSelectedStatusOfIndicator(int currentSelected);
    // Table Methods and delegates
    TableView *gameSelectionTableView;
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    Size cellSizeForTable(TableView *table);
    virtual void scrollViewDidScroll(extension::ScrollView* view);
protected:
};
#endif /* LPTHorizontalScrollView_hpp */
