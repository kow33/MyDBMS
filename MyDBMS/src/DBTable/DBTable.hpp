//
//  DBTable.hpp
//  MySUBD
//
//  Created by Михаил Кочетков on 25.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#ifndef DBTable_hpp
#define DBTable_hpp

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Column.hpp"
#include "DBDate.hpp"
#include "Enums.hpp"

using namespace std;

typedef vector<Column> Header;

class DBTable {
    string m_tableName;
    string m_primaryKey;
    Header m_head;
    vector<vector<void*>> m_data;
    
public:
    DBTable();
    DBTable(string t_tableName);
    DBTable(string t_tableName, string t_primaryKey, Header t_head);
    DBTable(const DBTable &t_table);
    ~DBTable();
    
    int size();
    string getTableName();
    string getPrimaryKey();
    Header getHead();
    
    void setTableName(string t_tableName);
    void setPrimaryKey(string t_primaryKey);
    void setHead(Header t_head);
    
    const vector<void*> getRow(int t_ind);
    const vector<vector<void*>> getRowsWhere(string t_colName, void *t_equalTo);
    const vector<void*> getColumn(string t_colName);
    
    void insertRow(vector<void*> &t_row);
    void removeRows();
    void removeRow(int t_ind);
    void removeRowsWhere(string t_colName, void *t_equalTo);
};

#endif /* DBTable_hpp */
