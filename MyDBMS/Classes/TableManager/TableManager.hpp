//
//  TableManager.hpp
//  MySUBD
//
//  Created by Михаил Кочетков on 29.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#ifndef TableManager_hpp
#define TableManager_hpp

#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "DBTable.hpp"

using namespace std;

class TableManager {
    DBTable *_curTable;
    
    TableManager(const TableManager &tm) {}
    TableManager(TableManager &&tm) {}
public:
    TableManager();
    TableManager(DBTable *curTableName);
    ~TableManager();
    
    string getCurTableName();
    vector<string> getCurTableColNames();
    
    void setCurTable(DBTable *curTable);
    bool isCurTableSet();
    void saveTable(string dbName);
    
    vector<string> select();
    vector<string> selectWhere(string colName, string equalTo);
    vector<string> selectColumn(string colName);
    
    bool insertRow(vector<string> row);
    bool deleteRows();
    bool deleteRowWhere(string colName, string equalTo);
};

#endif /* TableManager_hpp */
