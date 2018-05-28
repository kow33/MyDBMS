//
//  Column.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 27.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "Column.hpp"

void Column::setColTypeFromString(string colType) {
    if (colType == "Int") {
        _colType = DBType::_INT;
    } else if (colType == "Double") {
        _colType = DBType::_DOUBLE;
    } else if (colType == "String") {
        _colType = DBType::_STRING;
    } else if (colType == "Date") {
        _colType = DBType::_DATE;
    } else {
        _colType = DBType::_NULL;
    }
}

string Column::getStringFromColType() {
    switch (_colType) {
        case DBType::_INT:
            return "Int";
        case DBType::_DOUBLE:
            return "Double";
        case DBType::_STRING:
            return "String";
        case DBType::_DATE:
            return "Date";
        case DBType::_NULL:
            return "Null";
    }
}

bool Column::operator==(const Column &col) {
    if (_colType == col._colType && _colName == col._colName) {
        return true;
    } else {
        return false;
    }
}

bool Column::operator!=(const Column &col) {
    return !((*this) == col);
}
