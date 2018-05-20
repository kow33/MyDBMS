//
//  Database.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 29.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "Database.hpp"

Database::Database(string dbName) {
    _dbName = dbName;
    readTables();
}

Database::~Database() {
    if (_tm.isCurTableSet()) {
        _tm.saveTable(_dbName);
    }
    for (auto temp : _tables) {
        delete temp.second;
    }
    _tables.clear();
}

string Database::getDBName() {
    return _dbName;
}

void Database::useTable(string curTableName) {
    if (_tm.isCurTableSet()) {
        _tm.saveTable(_dbName);
    }
    _tm.setCurTable(_tables[curTableName]);
}

vector<string> Database::showTables() {
    vector<string> tableNameList;
    if (_tables.size() == 0) {
        return tableNameList;
    }
    
    for (auto temp : _tables) {
        tableNameList.push_back(temp.first);
    }
    
    return tableNameList;
}

void Database::createTable(string tableName, string primaryKey, Header head) {
    _tables.insert(pair<string, DBTable*>(tableName, new DBTable(tableName, primaryKey, head)));
    writeTableListFile();
}

void Database::removeTable(string tableName) {
    if (_tm.getCurTableName() == tableName) {
        _tm.setCurTable(nullptr);
    }
    delete _tables[tableName];
    _tables.erase(tableName);
    
    writeTableListFile();
    
    string command = "rm /Users/mihailkocetkov/C++/MySUBD/MySUBD/Databases/";
    command += _dbName + "/";
    command += tableName + ".csv";
    system(command.c_str());
}

void Database::readTables() {
    string path = "/Users/mihailkocetkov/C++/MySUBD/MySUBD/Databases/";
    path += _dbName + "/";
    
    fstream file(path + "tables.txt", ios::in);
    if (!file.is_open()) {
        return;
    }
    string tableName;
    while (getline(file, tableName, '\n')) {
        readTable(tableName);
    }
    file.close();
}

void Database::readTable(string tableName) {
    string path = "/Users/mihailkocetkov/C++/MySUBD/MySUBD/Databases/";
    path += _dbName + "/";
    
    fstream file(path + tableName + ".csv", ios::in);
    
    if (!file.is_open()) {
        return;
    }
    
    auto splitString = [](string str) -> vector<string> {
        vector<string> splitedStr;
        string tempStr = "";
        
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '|') {
                splitedStr.push_back(tempStr);
                tempStr = "";
                continue;
            }
            tempStr += str[i];
        }
        splitedStr.push_back(tempStr);
        
        return splitedStr;
    };
    
    string line;
    
    getline(file, line, '\n');
    vector<string> splitedLine = splitString(line);
    string primaryKey = splitedLine[1];
    
    getline(file, line, '\n');
    splitedLine = splitString(line);
    vector<Column> tableHead;
    for (int i = 0; i < splitedLine.size(); i += 2) {
        Column col(splitedLine[i]);
        string colType = splitedLine[i+1];
        if (colType == "Int") {
            col._colType = _INT;
        } else if (colType == "Double") {
            col._colType = _DOUBLE;
        } else if (colType == "String") {
            col._colType = _STRING;
        } else {
            col._colType = _DATE;
        }
        tableHead.emplace_back(col);
    }
    
    _tables.insert(pair<string, DBTable*>(tableName, new DBTable(tableName, primaryKey, tableHead)));
    useTable(tableName);
    
    while (getline(file, line, '\n')) {
        _tm.insertRow(splitString(line));
    }
    
    _tm.setCurTable(nullptr);
    file.close();
}

void Database::writeTableListFile() {
    vector<string> tableList = showTables();
    string path = "/Users/mihailkocetkov/C++/MySUBD/MySUBD/Databases/";
    path += _dbName + "/";
    
    fstream file(path + "tables.txt", ios::out | ios::trunc);
    
    if (!file.is_open()) {
        return;
    }
    
    for (int i = 0; i < tableList.size(); i++) {
        file << tableList[i] << endl;
    }
    
    file.close();
}


