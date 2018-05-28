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

extern string database_path;

class TableManager {
    DBTable *m_curTable;
    
    TableManager(const TableManager &t_tm) = delete;
    TableManager(TableManager &&t_tm) = delete;
public:
    TableManager();
    TableManager(DBTable *t_curTableName);
    ~TableManager();
    
    string getCurTableName();
    vector<string> getCurTableColNames();
    
    void setCurTable(DBTable *t_curTable);
    bool isCurTableSet();
    void saveTable(string t_dbName);
    
    vector<string> select();
    vector<string> selectWhere(string t_colName, string t_equalTo);
    vector<string> selectColumn(string t_colName);
    
    void insertRow(vector<string> t_row);
    void deleteRows();
    void deleteRowsWhere(string t_colName, string t_equalTo);
};

#endif /* TableManager_hpp */
