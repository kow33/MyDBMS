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

class Database {
    string _dbName;
    map<string, DBTable*> _tables;
    
    Database() {}
    void readTables();
    void readTable(string tableName);
    void writeTableListFile();
public:
    TableManager _tm;
    
    Database(string dbName);
    ~Database();
    
    string getDBName();
    
    void useTable(string curTableName);
    vector<string> showTables();
    
    void createTable(string tableName, string primaryKey, Header head);
    void removeTable(string tableName);
};

#endif /* Database_hpp */
