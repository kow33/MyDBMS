//
//  SUBD.hpp
//  MySUBD
//
//  Created by Михаил Кочетков on 19.05.2018.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#ifndef DBMS_hpp
#define DBMS_hpp

#pragma once

#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>

#include "Database.hpp"

using namespace std;

extern string database_path;

class DBMS {
    map<string, Database*> m_dbList;
    Database *m_curDB;
    
public:
    DBMS();
    ~DBMS();
    
    void run();
    void test();
    
    void readDBs();
    void saveDBs();
    vector<string> showDBs();
    void useDB(string t_dbName);
    
    void createDB(string t_dbName);
    void deleteDB(string t_dbName);
};

void printManualTable();
void drawTable(vector<string> t_header, vector<string> t_data, ios_base &t_oriented(ios_base &) = right,  bool t_isRowsDelimOn = false);
#endif /* DBMS_hpp */
