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
    string _tableName;
    string _primaryKey;
    Header _head;
    vector<vector<void*>> _data;
    
public:
    DBTable();
    DBTable(string tableName);
    DBTable(string tableName, string primaryKey, Header head);
    DBTable(const DBTable &table);
    ~DBTable();
    
    int size();
    string getTableName();
    string getPrimaryKey();
    Header getHead();
    
    void setTableName(string tableName);
    void setPrimaryKey(string primaryKey);
    void setHead(Header head);
    
    const vector<void*> getRow(int ind);
    const vector<vector<void*>> getRowsWhere(string colName, void *equalTo);
    const vector<void*> getColumn(string colName);
    
    void insertRow(vector<void*> &row);
    void removeRows();
    void removeRow(int ind);
    void removeRowsWhere(string colName, void *equalTo);
};

#endif /* DBTable_hpp */
