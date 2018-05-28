//
//  Column.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 27.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "Column.hpp"

void Column::setColTypeFromString(string t_colType) {
    if (t_colType == "Int") {
        colType = DBType::Int;
    } else if (t_colType == "Double") {
        colType = DBType::Double;
    } else if (t_colType == "String") {
        colType = DBType::String;
    } else if (t_colType == "Date") {
        colType = DBType::Date;
    } else {
        colType = DBType::Null;
    }
}

string Column::getStringFromColType() {
    switch (colType) {
        case DBType::Int:
            return "Int";
        case DBType::Double:
            return "Double";
        case DBType::String:
            return "String";
        case DBType::Date:
            return "Date";
        case DBType::Null:
            return "Null";
    }
}

bool Column::operator==(const Column &t_col) {
    if (colType == t_col.colType && colName == t_col.colName) {
        return true;
    } else {
        return false;
    }
}

bool Column::operator!=(const Column &t_col) {
    return !((*this) == t_col);
}
