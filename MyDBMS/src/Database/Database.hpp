//
//  Database.hpp
//  MySUBD
//
//  Created by Михаил Кочетков on 29.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#ifndef Database_hpp
#define Database_hpp

#pragma once

#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "DBTable.hpp"
#include "TableManager.hpp"
#include "Column.hpp"

using namespace std;

extern string database_path;

class Database {
    string m_dbName;
    map<string, DBTable*> m_tables;
    
    Database() {}
    void readTables();
    void readTable(string t_tableName);
    void writeTableListFile();
public:
    TableManager tm;
    
    Database(string t_dbName);
    ~Database();
    
    string getDBName();
    
    void useTable(string t_curTableName);
    vector<string> showTables();
    
    void createTable(string t_tableName, string t_primaryKey, Header t_head);
    void removeTable(string t_tableName);
};

#endif /* Database_hpp */
