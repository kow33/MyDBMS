//
//  TableManager.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 29.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "TableManager.hpp"

TableManager::TableManager() {
    _curTable = nullptr;
}

TableManager::TableManager(DBTable *curTable) {
    _curTable = curTable;
}

TableManager::~TableManager() {
    _curTable = nullptr;
}

string TableManager::getCurTableName() {
    return _curTable->getTableName();
}

vector<string> TableManager::getCurTableColNames() {
    vector<string> nameList;
    Header tableHead = _curTable->getHead();
    for (int i = 0; i < tableHead.size(); i++) {
        nameList.emplace_back(tableHead[i]._colName);
    }
    
    return nameList;
}

void TableManager::setCurTable(DBTable *curTable) {
    _curTable = curTable;
}

bool TableManager::isCurTableSet() {
    if (_curTable == nullptr) {
        return false;
    }
    return true;
}

void TableManager::saveTable(string dbName) {
    string path = "/Users/mihailkocetkov/C++/MySUBD/MyDBMS/Databases/";
    path += dbName + "/";
    fstream file(path + _curTable->getTableName() + ".csv", ios::out | ios::trunc);
    
    if (!file.is_open()) {
        return;
    }
    
    file << _curTable->getTableName() << "|" << _curTable->getPrimaryKey() << endl;
    
    Header tableHead = _curTable->getHead();
    for (int i = 0; i < tableHead.size(); i++) {
        file << tableHead[i]._colName << "|" << tableHead[i].getStringFromColType();
        if (i != tableHead.size() - 1) {
            file << "|";
        }
    }
    file << endl;
    
    vector<string> stringTable = select();
    for (auto row : stringTable) {
        file << row << endl;
    }
    
    file.close();
}

vector<string> TableManager::select() {
    vector<string> selectedRows;
    if (_curTable == nullptr) {
        selectedRows.push_back("No table in use");
        return selectedRows;
    }
    Header tableHead = _curTable->getHead();
    size_t headLen = tableHead.size();
    
    for (int i = 0; i < _curTable->size(); i++) {
        vector<void*> tempRow = _curTable->getRow(i);
        string row = "";
        for (int j = 0; j < headLen; j++) {
            switch (tableHead[j]._colType) {
                case _NULL:
                    break;
                case _INT:
                    row += to_string(*static_cast<int*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case _DOUBLE:
                    row += to_string(*static_cast<double*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case _STRING:
                    row += *static_cast<string*>(tempRow[j]);
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case _DATE:
                    row += static_cast<DBDate*>(tempRow[j])->dateToString();
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
            }
        }
        selectedRows.push_back(row);
    }
    
    return selectedRows;
}

vector<string> TableManager::selectWhere(string colName, string equalTo) {
    vector<string> selectedRows;
    if (_curTable == nullptr) {
        selectedRows.push_back("No table in use");
        return selectedRows;
    }
    Header tableHead = _curTable->getHead();
    DBType colType = _NULL;
    size_t headLen = tableHead.size();
    bool isRealCol = false;
    
    for (int i = 0; i < tableHead.size(); i++) {
        if (tableHead[i]._colName == colName) {
            isRealCol = true;
            colType = tableHead[i]._colType;
        }
    }
    
    if (!isRealCol) {
        selectedRows.push_back("Error column name");
        return selectedRows;
    }
    
    void *p_equalTo = nullptr;
    switch (colType) {
        case _NULL:
            break;
        case _INT:
            p_equalTo = new int(stoi(equalTo));
            break;
        case _DOUBLE:
            p_equalTo = new double(stod(equalTo));
            break;
        case _STRING:
            p_equalTo = new string(equalTo);
            break;
        case _DATE:
            p_equalTo = new DBDate(equalTo);
            break;
    }
    
    vector<vector<void*>> tempSelectedRows = _curTable->getRowsWhere(colName, p_equalTo);
    
    if (tempSelectedRows.size() == 0) {
        selectedRows.push_back("No row with this parametr");
        return selectedRows;
    }
    
    for (int i = 0; i < tempSelectedRows.size(); i++) {
        vector<void*> tempRow = tempSelectedRows[i];
        string row = "";
        for (int j = 0; j < headLen; j++) {
            switch (tableHead[j]._colType) {
                case _NULL:
                    break;
                case _INT:
                    row += to_string(*static_cast<int*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case _DOUBLE:
                    row += to_string(*static_cast<double*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case _STRING:
                    row += *static_cast<string*>(tempRow[j]);
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case _DATE:
                    row += static_cast<DBDate*>(tempRow[j])->dateToString();
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
            }
        }
        selectedRows.push_back(row);
    }
    
    return selectedRows;
}

vector<string> TableManager::selectColumn(string colName) {
    vector<string> selectedElems;
    if (_curTable == nullptr) {
        selectedElems.push_back("No table in use");
        return selectedElems;
    }
    
    Header tableHead = _curTable->getHead();
    DBType colType = _NULL;
    
    for (int i = 0; i < tableHead.size(); i++) {
        if (tableHead[i]._colName == colName) {
            colType = tableHead[i]._colType;
        }
    }
    
    if (colType == _NULL) {
        selectedElems.push_back("Error column name");
        return selectedElems;
    }
    
    vector<void*> tempColumn = _curTable->getColumn(colName);
    
    for (int i = 0; i < tempColumn.size(); i++) {
        switch (colType) {
            case _NULL:
                break;
            case _INT:
                selectedElems.emplace_back(to_string(*static_cast<int*>(tempColumn[i])));
                break;
            case _DOUBLE:
                selectedElems.emplace_back(to_string(*static_cast<double*>(tempColumn[i])));
                break;
            case _STRING:
                selectedElems.emplace_back(*static_cast<string*>(tempColumn[i]));
                break;
            case _DATE:
                selectedElems.emplace_back(static_cast<DBDate*>(tempColumn[i])->dateToString());
                break;
        }
    }
    
    return selectedElems;
}

bool TableManager::insertRow(vector<string> row) {
    if (_curTable == nullptr) {
        return false;
    }

    Header tableHead = _curTable->getHead();
    
    if (row.size() != tableHead.size()) {
        return false;
    }
    
    vector<void*> tempRow;
    
    for (int i = 0; i < row.size(); i++) {
        switch (tableHead[i]._colType) {
            case _NULL:
                break;
            case _INT:
                tempRow.push_back(new int(stoi(row[i])));
                break;
            case _DOUBLE:
                tempRow.push_back(new double(stod(row[i])));
                break;
            case _STRING:
                tempRow.push_back(new string(row[i]));
                break;
            case _DATE:
                tempRow.push_back(new DBDate(row[i]));
                break;
        }
    }
    _curTable->insertRow(tempRow);
    
    return true;
}

bool TableManager::deleteRows() {
    if (_curTable == nullptr) {
        return false;
    }
    _curTable->removeRows();
    
    return true;
}

bool TableManager::deleteRowWhere(string colName, string equalTo) {
    if (_curTable == nullptr) {
        return false;
    }
    Header tableHead = _curTable->getHead();
    DBType colType = _NULL;
    bool isRealCol = false;
    
    for (int i = 0; i < tableHead.size(); i++) {
        if (tableHead[i]._colName == colName) {
            isRealCol = true;
            colType = tableHead[i]._colType;
        }
    }
    
    if (!isRealCol) {
        return false;
    }
    
    void *p_equalTo = nullptr;
    
    switch (colType) {
        case _NULL:
            break;
        case _INT:
            p_equalTo = new int(stoi(equalTo));
            break;
        case _DOUBLE:
            p_equalTo = new double(stod(equalTo));
            break;
        case _STRING:
            p_equalTo = new string(equalTo);
            break;
        case _DATE:
            p_equalTo = new DBDate(equalTo);
            break;
    }

    _curTable->removeRowsWhere(colName, p_equalTo);
    
    return true;
}
