//
//  Column.hpp
//  MySUBD
//
//  Created by Михаил Кочетков on 27.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#ifndef Column_hpp
#define Column_hpp

#pragma once

#include "Enums.hpp"

#include <string>

using namespace std;

struct Column {
    string _colName;
    DBType _colType;
    
    Column() {}
    Column(string colName) : _colName(colName) {}
    Column(const Column& col) : _colName(col._colName), _colType(col._colType) {}
    
    void setColTypeFromString(string colType);
    string getStringFromColType();
    
    bool operator==(const Column& col);
    bool operator!=(const Column& col);
};

#endif /* Column_hpp */
