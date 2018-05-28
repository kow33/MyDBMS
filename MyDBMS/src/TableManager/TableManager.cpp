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
    string path = databasePath;
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
    
    if (_curTable->size() == 0) {
        throw string("No data in table");
        return selectedRows;
    }
    
    Header tableHead = _curTable->getHead();
    size_t headLen = tableHead.size();
    
    for (int i = 0; i < _curTable->size(); i++) {
        vector<void*> tempRow = _curTable->getRow(i);
        string row = "";
        for (int j = 0; j < headLen; j++) {
            switch (tableHead[j]._colType) {
                case DBType::_NULL:
                    break;
                case DBType::_INT:
                    row += to_string(*static_cast<int*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::_DOUBLE:
                    row += to_string(*static_cast<double*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::_STRING:
                    row += *static_cast<string*>(tempRow[j]);
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::_DATE:
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
    Header tableHead = _curTable->getHead();
    DBType colType = DBType::_NULL;
    size_t headLen = tableHead.size();
    bool isRealCol = false;
    
    for (int i = 0; i < tableHead.size(); i++) {
        if (tableHead[i]._colName == colName) {
            isRealCol = true;
            colType = tableHead[i]._colType;
        }
    }
    
    if (!isRealCol) {
        throw string("column with this name not exist");
        return selectedRows;
    }
    
    void *p_equalTo = nullptr;
    switch (colType) {
        case DBType::_NULL:
            break;
        case DBType::_INT:
            p_equalTo = new int(stoi(equalTo));
            break;
        case DBType::_DOUBLE:
            p_equalTo = new double(stod(equalTo));
            break;
        case DBType::_STRING:
            p_equalTo = new string(equalTo);
            break;
        case DBType::_DATE:
            p_equalTo = new DBDate(equalTo);
            break;
    }
    
    vector<vector<void*>> tempSelectedRows = _curTable->getRowsWhere(colName, p_equalTo);
    
    if (tempSelectedRows.size() == 0) {
        throw string("no row with this parameter");
        return selectedRows;
    }
    
    for (int i = 0; i < tempSelectedRows.size(); i++) {
        vector<void*> tempRow = tempSelectedRows[i];
        string row = "";
        for (int j = 0; j < headLen; j++) {
            switch (tableHead[j]._colType) {
                case DBType::_NULL:
                    break;
                case DBType::_INT:
                    row += to_string(*static_cast<int*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::_DOUBLE:
                    row += to_string(*static_cast<double*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::_STRING:
                    row += *static_cast<string*>(tempRow[j]);
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::_DATE:
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
    Header tableHead = _curTable->getHead();
    DBType colType = DBType::_NULL;
    
    for (int i = 0; i < tableHead.size(); i++) {
        if (tableHead[i]._colName == colName) {
            colType = tableHead[i]._colType;
        }
    }
    
    if (colType == DBType::_NULL) {
        throw string("column with this name not exist");
        return selectedElems;
    }
    
    vector<void*> tempColumn = _curTable->getColumn(colName);
    
    for (int i = 0; i < tempColumn.size(); i++) {
        switch (colType) {
            case DBType::_NULL:
                break;
            case DBType::_INT:
                selectedElems.emplace_back(to_string(*static_cast<int*>(tempColumn[i])));
                break;
            case DBType::_DOUBLE:
                selectedElems.emplace_back(to_string(*static_cast<double*>(tempColumn[i])));
                break;
            case DBType::_STRING:
                selectedElems.emplace_back(*static_cast<string*>(tempColumn[i]));
                break;
            case DBType::_DATE:
                selectedElems.emplace_back(static_cast<DBDate*>(tempColumn[i])->dateToString());
                break;
        }
    }
    
    return selectedElems;
}

void TableManager::insertRow(vector<string> row) {
    Header tableHead = _curTable->getHead();
    
    if (row.size() != tableHead.size()) {
        throw string("incorrect number of row elements");
        return;
    }
    
    vector<void*> tempRow;
    
    for (int i = 0; i < row.size(); i++) {
        switch (tableHead[i]._colType) {
            case DBType::_NULL:
                break;
            case DBType::_INT:
                tempRow.push_back(new int(stoi(row[i])));
                break;
            case DBType::_DOUBLE:
                tempRow.push_back(new double(stod(row[i])));
                break;
            case DBType::_STRING:
                tempRow.push_back(new string(row[i]));
                break;
            case DBType::_DATE:
                tempRow.push_back(new DBDate(row[i]));
                break;
        }
    }
    _curTable->insertRow(tempRow);
}

void TableManager::deleteRows() {
    _curTable->removeRows();
}

void TableManager::deleteRowsWhere(string colName, string equalTo) {
    Header tableHead = _curTable->getHead();
    DBType colType = DBType::_NULL;
    bool isRealCol = false;
    
    for (int i = 0; i < tableHead.size(); i++) {
        if (tableHead[i]._colName == colName) {
            isRealCol = true;
            colType = tableHead[i]._colType;
        }
    }
    
    if (!isRealCol) {
        throw string("column with this name not exist");
        return;
    }
    
    void *p_equalTo = nullptr;
    
    switch (colType) {
        case DBType::_NULL:
            break;
        case DBType::_INT:
            p_equalTo = new int(stoi(equalTo));
            break;
        case DBType::_DOUBLE:
            p_equalTo = new double(stod(equalTo));
            break;
        case DBType::_STRING:
            p_equalTo = new string(equalTo);
            break;
        case DBType::_DATE:
            p_equalTo = new DBDate(equalTo);
            break;
    }

    _curTable->removeRowsWhere(colName, p_equalTo);
}
