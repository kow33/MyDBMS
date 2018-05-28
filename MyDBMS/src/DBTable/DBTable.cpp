//
//  DBTable.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 25.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "DBTable.hpp"

DBTable::DBTable() {
    m_tableName = "NoName";
    m_primaryKey = "Null";
}

DBTable::DBTable(string t_tableName) {
    m_tableName = t_tableName;
    m_primaryKey = "Null";
}

DBTable::DBTable(string t_tableName, string t_primaryKey, Header t_head) {
    m_tableName = t_tableName;
    m_primaryKey = t_primaryKey;
    m_head = t_head;
}

DBTable::DBTable(const DBTable &t_table) {
    m_tableName = t_table.m_tableName;
    m_primaryKey = t_table.m_primaryKey;
    m_head = t_table.m_head;
    
    for (int i = 0; i < t_table.m_data.size(); i++) {
        vector<void*> tempRow;
        for (int j = 0; j < t_table.m_data[i].size(); j++) {
            switch (m_head[j].colType) {
                case DBType::Null:
                    break;
                case DBType::Int:
                    tempRow.push_back(new int(*static_cast<int*>(t_table.m_data[i][j])));
                    break;
                case DBType::Double:
                    tempRow.push_back(new double(*static_cast<double*>(t_table.m_data[i][j])));
                    break;
                case DBType::String:
                    tempRow.push_back(new string(*static_cast<string*>(t_table.m_data[i][j])));
                    break;
                case DBType::Date:
                    tempRow.push_back(new DBDate(*static_cast<DBDate*>(t_table.m_data[i][j])));
                    break;
            }
        }
        m_data.push_back(tempRow);
    }
}

DBTable::~DBTable() {
    for (int i = 0; i < m_data.size(); i++) {
        for (int j = 0; j < m_data[i].size(); j++) {
            switch (m_head[j].colType) {
                case DBType::Int:
                    delete reinterpret_cast<int*>(m_data[i][j]);
                    break;
                case DBType::Double:
                    delete reinterpret_cast<double*>(m_data[i][j]);
                    break;
                case DBType::String:
                    delete reinterpret_cast<string*>(m_data[i][j]);
                    break;
                case DBType::Date:
                    delete reinterpret_cast<DBDate*>(m_data[i][j]);
                    break;
                case DBType::Null:
                    break;
            }
        }
        m_data[i].clear();
    }
    m_data.clear();
}

int DBTable::size() {
    return static_cast<int>(m_data.size());
}

string DBTable::getTableName() {
    return m_tableName;
}

string DBTable::getPrimaryKey() {
    return m_primaryKey;
}

Header DBTable::getHead() {
    return m_head;
}

void DBTable::setTableName(string t_tableName) {
    m_tableName = t_tableName;
}

void DBTable::setPrimaryKey(string t_primaryKey) {
    m_primaryKey = t_primaryKey;
}

void DBTable::setHead(Header t_head) {
    m_head = t_head;
}

const vector<void*> DBTable::getRow(int t_ind) {
    return m_data[t_ind];
}

const vector<vector<void*>> DBTable::getRowsWhere(string t_colName, void *t_equalTo) {
    int columnIndex = -1;
    DBType columnType = DBType::Null;
    vector<vector<void*>> selectedRows;
    
    for (int i = 0; i < m_head.size(); i++) {
        if (m_head[i].colName == t_colName) {
            columnIndex = i;
            columnType = m_head[i].colType;
            break;
        }
    }
    
    if (columnIndex == -1) {
        return selectedRows;
    }
    
    for (int i = 0; i < m_data.size(); i++) {
        switch (columnType) {
            case DBType::Int:
                if (*static_cast<int*>(m_data[i][columnIndex]) == *static_cast<int*>(t_equalTo)) {
                    selectedRows.push_back(m_data[i]);
                }
                break;
            case DBType::Double:
                if (*static_cast<double*>(m_data[i][columnIndex]) == *static_cast<double*>(t_equalTo)) {
                    selectedRows.push_back(m_data[i]);
                }
                break;
            case DBType::String:
                if (*static_cast<string*>(m_data[i][columnIndex]) == *static_cast<string*>(t_equalTo)) {
                    selectedRows.push_back(m_data[i]);
                }
                break;
            case DBType::Date:
                if (*static_cast<DBDate*>(m_data[i][columnIndex]) == *static_cast<DBDate*>(t_equalTo)) {
                    selectedRows.push_back(m_data[i]);
                }
                break;
                
            default:
                break;
        }
    }
    
    return selectedRows;
}

const vector<void*> DBTable::getColumn(string t_colName) {
    vector<void*> tempColumn;
    int columnIndex = -1;
    for (int i = 0; i < m_head.size(); i++) {
        if (t_colName == m_head[i].colName) {
            columnIndex = i;
        }
    }
    
    if (columnIndex == -1) {
        return tempColumn;
    }
    
    switch (m_head[columnIndex].colType) {
        case DBType::Int:
            for (int i = 0; i < m_data.size(); i++) {
                tempColumn.push_back(static_cast<int*>(m_data[i][columnIndex]));
            }
            break;
        case DBType::Double:
            for (int i = 0; i < m_data.size(); i++) {
                tempColumn.push_back(static_cast<double*>(m_data[i][columnIndex]));
            }
            break;
        case DBType::String:
            for (int i = 0; i < m_data.size(); i++) {
                tempColumn.push_back(static_cast<string*>(m_data[i][columnIndex]));
            }
            break;
        case DBType::Date:
            for (int i = 0; i < m_data.size(); i++) {
                tempColumn.push_back(static_cast<DBDate*>(m_data[i][columnIndex]));
            }
            break;
        case DBType::Null:
            break;
    }
    
    return tempColumn;
}

void DBTable::insertRow(vector<void*> &t_row) {
    m_data.push_back(t_row);
}

void DBTable::removeRows() {
    for (int i = 0; i < m_data.size(); i++) {
        for (int j = 0; j < m_data[0].size(); j++) {
            switch (m_head[j].colType) {
                case DBType::Int:
                    delete reinterpret_cast<int*>(m_data[i][j]);
                    break;
                case DBType::Double:
                    delete reinterpret_cast<double*>(m_data[i][j]);
                    break;
                case DBType::String:
                    delete reinterpret_cast<string*>(m_data[i][j]);
                    break;
                case DBType::Date:
                    delete reinterpret_cast<DBDate*>(m_data[i][j]);
                    break;
                case DBType::Null:
                    break;
            }
        }
    }
    for (int i = 0; i < m_data.size(); i++) {
        m_data[i].clear();
    }
    m_data.clear();
}

void DBTable::removeRow(int t_ind) {
    for (int i = 0; i < m_data[t_ind].size(); i++) {
        switch (m_head[i].colType) {
            case DBType::Int:
                delete reinterpret_cast<int*>(m_data[t_ind][i]);
                break;
            case DBType::Double:
                delete reinterpret_cast<double*>(m_data[t_ind][i]);
                break;
            case DBType::String:
                delete reinterpret_cast<string*>(m_data[t_ind][i]);
                break;
            case DBType::Date:
                delete reinterpret_cast<DBDate*>(m_data[t_ind][i]);
                break;
            case DBType::Null:
                break;
        }
    }
    
    for (auto iter = m_data.end() - 1; iter != m_data.begin() - 1; iter--) {
        if (*iter == m_data[t_ind]) {
            m_data.erase(iter);
        }
    }
}

void DBTable::removeRowsWhere(string t_colName, void *t_equalTo) {
    int columnIndex = -1;
    DBType columnType = DBType::Null;
    
    for (int i = 0; i < m_head.size(); i++) {
        if (m_head[i].colName == t_colName) {
            columnIndex = i;
            columnType = m_head[i].colType;
            break;
        }
    }
    
    for (int i = int(m_data.size()) - 1; i >= 0; i--) {
        switch (columnType) {
            case DBType::Int:
                if (*static_cast<int*>(m_data[i][columnIndex]) == *static_cast<int*>(t_equalTo)) {
                    removeRow(i);
                }
                break;
            case DBType::Double:
                if (*static_cast<double*>(m_data[i][columnIndex]) == *static_cast<double*>(t_equalTo)) {
                    removeRow(i);
                }
                break;
            case DBType::String:
                if (*static_cast<string*>(m_data[i][columnIndex]) == *static_cast<string*>(t_equalTo)) {
                    removeRow(i);
                }
                break;
            case DBType::Date:
                if (*static_cast<DBDate*>(m_data[i][columnIndex]) == *static_cast<DBDate*>(t_equalTo)) {
                    removeRow(i);
                }
                break;
                
            default:
                break;
        }
    }
}

