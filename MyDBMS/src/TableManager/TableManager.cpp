//
//  TableManager.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 29.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "TableManager.hpp"

TableManager::TableManager() {
    m_curTable = nullptr;
}

TableManager::TableManager(DBTable *t_curTable) {
    m_curTable = t_curTable;
}

TableManager::~TableManager() {
    m_curTable = nullptr;
}

string TableManager::getCurTableName() {
    return m_curTable->getTableName();
}

vector<string> TableManager::getCurTableColNames() {
    vector<string> nameList;
    Header tableHead = m_curTable->getHead();
    for (int i = 0; i < tableHead.size(); i++) {
        nameList.emplace_back(tableHead[i].colName);
    }
    
    return nameList;
}

void TableManager::setCurTable(DBTable *t_curTable) {
    m_curTable = t_curTable;
}

bool TableManager::isCurTableSet() {
    if (m_curTable == nullptr) {
        return false;
    }
    return true;
}

void TableManager::saveTable(string t_dbName) {
    string path = database_path;
    path += t_dbName + "/";
    fstream file(path + m_curTable->getTableName() + ".csv", ios::out | ios::trunc);
    
    if (!file.is_open()) {
        return;
    }
    
    file << m_curTable->getTableName() << "|" << m_curTable->getPrimaryKey() << endl;
    
    Header tableHead = m_curTable->getHead();
    for (int i = 0; i < tableHead.size(); i++) {
        file << tableHead[i].colName << "|" << tableHead[i].getStringFromColType();
        if (i != tableHead.size() - 1) {
            file << "|";
        }
    }
    file << endl;
    
    try {
        vector<string> stringTable = select();
        for (auto row : stringTable) {
            file << row << endl;
        }
    } catch (string err) {}
    
    file.close();
}

vector<string> TableManager::select() {
    vector<string> selectedRows;
    
    if (m_curTable->size() == 0) {
        throw string("No data in table");
    }
    
    Header tableHead = m_curTable->getHead();
    size_t headLen = tableHead.size();
    
    for (int i = 0; i < m_curTable->size(); i++) {
        vector<void*> tempRow = m_curTable->getRow(i);
        string row = "";
        for (int j = 0; j < headLen; j++) {
            switch (tableHead[j].colType) {
                case DBType::Null:
                    break;
                case DBType::Int:
                    row += to_string(*static_cast<int*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::Double:
                    row += to_string(*static_cast<double*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::String:
                    row += *static_cast<string*>(tempRow[j]);
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::Date:
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

vector<string> TableManager::selectWhere(string t_colName, string t_equalTo) {
    vector<string> selectedRows;
    Header tableHead = m_curTable->getHead();
    DBType colType = DBType::Null;
    size_t headLen = tableHead.size();
    bool isRealCol = false;
    
    for (int i = 0; i < tableHead.size(); i++) {
        if (tableHead[i].colName == t_colName) {
            isRealCol = true;
            colType = tableHead[i].colType;
        }
    }
    
    if (!isRealCol) {
        throw string("column with this name not exist");
    }
    
    void *p_equalTo = nullptr;
    switch (colType) {
        case DBType::Null:
            break;
        case DBType::Int:
            p_equalTo = new int(stoi(t_equalTo));
            break;
        case DBType::Double:
            p_equalTo = new double(stod(t_equalTo));
            break;
        case DBType::String:
            p_equalTo = new string(t_equalTo);
            break;
        case DBType::Date:
            p_equalTo = new DBDate(t_equalTo);
            break;
    }
    
    vector<vector<void*>> tempSelectedRows = m_curTable->getRowsWhere(t_colName, p_equalTo);
    
    if (tempSelectedRows.size() == 0) {
        throw string("no row with this parameter");
    }
    
    for (int i = 0; i < tempSelectedRows.size(); i++) {
        vector<void*> tempRow = tempSelectedRows[i];
        string row = "";
        for (int j = 0; j < headLen; j++) {
            switch (tableHead[j].colType) {
                case DBType::Null:
                    break;
                case DBType::Int:
                    row += to_string(*static_cast<int*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::Double:
                    row += to_string(*static_cast<double*>(tempRow[j]));
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::String:
                    row += *static_cast<string*>(tempRow[j]);
                    if (j != headLen - 1) {
                        row += "|";
                    }
                    break;
                case DBType::Date:
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

vector<string> TableManager::selectColumn(string t_colName) {
    vector<string> selectedElems;
    Header tableHead = m_curTable->getHead();
    DBType colType = DBType::Null;
    
    for (int i = 0; i < tableHead.size(); i++) {
        if (tableHead[i].colName == t_colName) {
            colType = tableHead[i].colType;
        }
    }
    
    if (colType == DBType::Null) {
        throw string("column with this name not exist");
    }
    
    vector<void*> tempColumn = m_curTable->getColumn(t_colName);
    
    for (int i = 0; i < tempColumn.size(); i++) {
        switch (colType) {
            case DBType::Null:
                break;
            case DBType::Int:
                selectedElems.emplace_back(to_string(*static_cast<int*>(tempColumn[i])));
                break;
            case DBType::Double:
                selectedElems.emplace_back(to_string(*static_cast<double*>(tempColumn[i])));
                break;
            case DBType::String:
                selectedElems.emplace_back(*static_cast<string*>(tempColumn[i]));
                break;
            case DBType::Date:
                selectedElems.emplace_back(static_cast<DBDate*>(tempColumn[i])->dateToString());
                break;
        }
    }
    
    return selectedElems;
}

void TableManager::insertRow(vector<string> t_row) {
    Header tableHead = m_curTable->getHead();
    
    if (t_row.size() != tableHead.size()) {
        throw string("incorrect number of row elements");
    }
    
    vector<void*> tempRow;
    
    for (int i = 0; i < t_row.size(); i++) {
        switch (tableHead[i].colType) {
            case DBType::Null:
                break;
            case DBType::Int:
                tempRow.push_back(new int(stoi(t_row[i])));
                break;
            case DBType::Double:
                tempRow.push_back(new double(stod(t_row[i])));
                break;
            case DBType::String:
                tempRow.push_back(new string(t_row[i]));
                break;
            case DBType::Date:
                tempRow.push_back(new DBDate(t_row[i]));
                break;
        }
    }
    m_curTable->insertRow(tempRow);
}

void TableManager::deleteRows() {
    m_curTable->removeRows();
}

void TableManager::deleteRowsWhere(string t_colName, string t_equalTo) {
    Header tableHead = m_curTable->getHead();
    DBType colType = DBType::Null;
    bool isRealCol = false;
    
    for (int i = 0; i < tableHead.size(); i++) {
        if (tableHead[i].colName == t_colName) {
            isRealCol = true;
            colType = tableHead[i].colType;
        }
    }
    
    if (!isRealCol) {
        throw string("column with this name not exist");
        return;
    }
    
    void *p_equalTo = nullptr;
    
    switch (colType) {
        case DBType::Null:
            break;
        case DBType::Int:
            p_equalTo = new int(stoi(t_equalTo));
            break;
        case DBType::Double:
            p_equalTo = new double(stod(t_equalTo));
            break;
        case DBType::String:
            p_equalTo = new string(t_equalTo);
            break;
        case DBType::Date:
            p_equalTo = new DBDate(t_equalTo);
            break;
    }

    m_curTable->removeRowsWhere(t_colName, p_equalTo);
}
