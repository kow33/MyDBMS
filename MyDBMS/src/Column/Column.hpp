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
    string colName;
    DBType colType;
    
    Column() {}
    Column(string t_colName) : colName(t_colName) {}
    Column(const Column& t_col) : colName(t_col.colName), colType(t_col.colType) {}
    
    void setColTypeFromString(string t_colType);
    string getStringFromColType();
    
    bool operator==(const Column& t_col);
    bool operator!=(const Column& t_col);
};

#endif /* Column_hpp */
