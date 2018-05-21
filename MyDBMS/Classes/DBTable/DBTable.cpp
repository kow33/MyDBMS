//
//  DBTable.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 25.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "DBTable.hpp"

DBTable::DBTable() {
    _tableName = "NoName";
    _primaryKey = "Null";
}

DBTable::DBTable(string tableName) {
    _tableName = tableName;
    _primaryKey = "Null";
}

DBTable::DBTable(string tableName, string primaryKey, Header head) {
    _tableName = tableName;
    _primaryKey = primaryKey;
    _head = head;
}

DBTable::DBTable(const DBTable &table) {
    _tableName = table._tableName;
    _primaryKey = table._primaryKey;
    _head = table._head;
    
    for (int i = 0; i < table._data.size(); i++) {
        vector<void*> tempRow;
        for (int j = 0; j < table._data[i].size(); j++) {
            switch (_head[j]._colType) {
                case _NULL:
                    break;
                case _INT:
                    tempRow.push_back(new int(*static_cast<int*>(table._data[i][j])));
                    break;
                case _DOUBLE:
                    tempRow.push_back(new double(*static_cast<double*>(table._data[i][j])));
                    break;
                case _STRING:
                    tempRow.push_back(new string(*static_cast<string*>(table._data[i][j])));
                    break;
                case _DATE:
                    tempRow.push_back(new DBDate(*static_cast<DBDate*>(table._data[i][j])));
                    break;
            }
        }
        _data.push_back(tempRow);
    }
}

DBTable::~DBTable() {
    for (int i = 0; i < _data.size(); i++) {
        for (int j = 0; j < _data[i].size(); j++) {
            switch (_head[j]._colType) {
                case _INT:
                    delete reinterpret_cast<int*>(_data[i][j]);
                    break;
                case _DOUBLE:
                    delete reinterpret_cast<double*>(_data[i][j]);
                    break;
                case _STRING:
                    delete reinterpret_cast<string*>(_data[i][j]);
                    break;
                case _DATE:
                    delete reinterpret_cast<DBDate*>(_data[i][j]);
                    break;
                case _NULL:
                    break;
            }
        }
        _data[i].clear();
    }
    _data.clear();
}

int DBTable::size() {
    return static_cast<int>(_data.size());
}

string DBTable::getTableName() {
    return _tableName;
}

string DBTable::getPrimaryKey() {
    return _primaryKey;
}

Header DBTable::getHead() {
    return _head;
}

void DBTable::setTableName(string tableName) {
    _tableName = tableName;
}

void DBTable::setPrimaryKey(string primaryKey) {
    _primaryKey = primaryKey;
}

void DBTable::setHead(Header head) {
    _head = head;
}

const vector<void*> DBTable::getRow(int ind) {
    return _data[ind];
}

const vector<vector<void*>> DBTable::getRowsWhere(string colName, void *equalTo) {
    int columnIndex = -1;
    DBType columnType = _NULL;
    vector<vector<void*>> selectedRows;
    
    for (int i = 0; i < _head.size(); i++) {
        if (_head[i]._colName == colName) {
            columnIndex = i;
            columnType = _head[i]._colType;
            break;
        }
    }
    
    if (columnIndex == -1) {
        return selectedRows;
    }
    
    for (int i = 0; i < _data.size(); i++) {
        switch (columnType) {
            case _INT:
                if (*static_cast<int*>(_data[i][columnIndex]) == *static_cast<int*>(equalTo)) {
                    selectedRows.push_back(_data[i]);
                }
                break;
            case _DOUBLE:
                if (*static_cast<double*>(_data[i][columnIndex]) == *static_cast<double*>(equalTo)) {
                    selectedRows.push_back(_data[i]);
                }
                break;
            case _STRING:
                if (*static_cast<string*>(_data[i][columnIndex]) == *static_cast<string*>(equalTo)) {
                    selectedRows.push_back(_data[i]);
                }
                break;
            case _DATE:
                if (*static_cast<DBDate*>(_data[i][columnIndex]) == *static_cast<DBDate*>(equalTo)) {
                    selectedRows.push_back(_data[i]);
                }
                break;
                
            default:
                break;
        }
    }
    
    return selectedRows;
}

const vector<void*> DBTable::getColumn(string colName) {
    vector<void*> tempColumn;
    int columnIndex = -1;
    for (int i = 0; i < _head.size(); i++) {
        if (colName == _head[i]._colName) {
            columnIndex = i;
        }
    }
    
    if (columnIndex == -1) {
        return tempColumn;
    }
    
    switch (_head[columnIndex]._colType) {
        case _INT:
            for (int i = 0; i < _data.size(); i++) {
                tempColumn.push_back(static_cast<int*>(_data[i][columnIndex]));
            }
            break;
        case _DOUBLE:
            for (int i = 0; i < _data.size(); i++) {
                tempColumn.push_back(static_cast<double*>(_data[i][columnIndex]));
            }
            break;
        case _STRING:
            for (int i = 0; i < _data.size(); i++) {
                tempColumn.push_back(static_cast<string*>(_data[i][columnIndex]));
            }
            break;
        case _DATE:
            for (int i = 0; i < _data.size(); i++) {
                tempColumn.push_back(static_cast<DBDate*>(_data[i][columnIndex]));
            }
            break;
        case _NULL:
            break;
    }
    
    return tempColumn;
}

void DBTable::insertRow(vector<void*> &row) {
    _data.push_back(row);
}

void DBTable::removeRows() {
    for (int i = 0; i < _data.size(); i++) {
        for (int j = 0; j < _data[0].size(); j++) {
            switch (_head[j]._colType) {
                case _INT:
                    delete reinterpret_cast<int*>(_data[i][j]);
                    break;
                case _DOUBLE:
                    delete reinterpret_cast<double*>(_data[i][j]);
                    break;
                case _STRING:
                    delete reinterpret_cast<string*>(_data[i][j]);
                    break;
                case _DATE:
                    delete reinterpret_cast<DBDate*>(_data[i][j]);
                    break;
                case _NULL:
                    break;
            }
        }
    }
    for (int i = 0; i < _data.size(); i++) {
        _data[i].clear();
    }
    _data.clear();
}

void DBTable::removeRow(int ind) {
    for (int i = 0; i < _data[ind].size(); i++) {
        switch (_head[i]._colType) {
            case _INT:
                delete reinterpret_cast<int*>(_data[ind][i]);
                break;
            case _DOUBLE:
                delete reinterpret_cast<double*>(_data[ind][i]);
                break;
            case _STRING:
                delete reinterpret_cast<string*>(_data[ind][i]);
                break;
            case _DATE:
                delete reinterpret_cast<DBDate*>(_data[ind][i]);
                break;
            case _NULL:
                break;
        }
    }
    
    for (auto iter = _data.end() - 1; iter != _data.begin() - 1; iter--) {
        if (*iter == _data[ind]) {
            _data.erase(iter);
        }
    }
}

void DBTable::removeRowsWhere(string colName, void *equalTo) {
    int columnIndex = -1;
    DBType columnType = _NULL;
    
    for (int i = 0; i < _head.size(); i++) {
        if (_head[i]._colName == colName) {
            columnIndex = i;
            columnType = _head[i]._colType;
            break;
        }
    }
    
    for (int i = 0; i < _data.size(); i++) {
        switch (columnType) {
            case _INT:
                if (*static_cast<int*>(_data[i][columnIndex]) == *static_cast<int*>(equalTo)) {
                    removeRow(i);
                }
                break;
            case _DOUBLE:
                if (*static_cast<double*>(_data[i][columnIndex]) == *static_cast<double*>(equalTo)) {
                    removeRow(i);
                }
                break;
            case _STRING:
                if (*static_cast<string*>(_data[i][columnIndex]) == *static_cast<string*>(equalTo)) {
                    removeRow(i);
                }
                break;
            case _DATE:
                if (*static_cast<DBDate*>(_data[i][columnIndex]) == *static_cast<DBDate*>(equalTo)) {
                    removeRow(i);
                }
                break;
                
            default:
                break;
        }
    }
}

