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
        _colType = _INT;
    } else if (colType == "Double") {
        _colType = _DOUBLE;
    } else if (colType == "String") {
        _colType = _STRING;
    } else if (colType == "Date") {
        _colType = _DATE;
    } else {
        _colType = _NULL;
    }
}

string Column::getStringFromColType() {
    switch (_colType) {
        case _INT:
            return "Int";
        case _DOUBLE:
            return "Double";
        case _STRING:
            return "String";
        case _DATE:
            return "Date";
        case _NULL:
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
