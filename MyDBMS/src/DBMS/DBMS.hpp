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

#include "Database.hpp"

using namespace std;

extern string databasePath;

class DBMS {
    map<string, Database*> _dbList;
    Database *_curDB;
    
public:
    DBMS();
    ~DBMS();
    
    void run();
    
    void readDBs();
    void saveDBs();
    vector<string> showDBs();
    void useDB(string dbName);
    
    void createDB(string dbName);
    void deleteDB(string dbName);
};

void printManualTable();
void drawTable(vector<string> header, vector<string> data, ios_base &oriented(ios_base &) = right,  bool isRowsDelimOn = false);
#endif /* DBMS_hpp */