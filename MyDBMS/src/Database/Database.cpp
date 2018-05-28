//
//  Database.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 29.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "Database.hpp"

Database::Database(string t_dbName) {
    m_dbName = t_dbName;
    readTables();
}

Database::~Database() {
    if (tm.isCurTableSet()) {
        tm.saveTable(m_dbName);
    }
    for (auto temp : m_tables) {
        delete temp.second;
    }
    m_tables.clear();
}

string Database::getDBName() {
    return m_dbName;
}

void Database::useTable(string t_curTableName) {
    if (m_tables.find(t_curTableName) == m_tables.end()) {
        throw string("table with this name not exist");
        return;
    }
    if (tm.isCurTableSet() && tm.getCurTableName() == t_curTableName) {
        throw string("table with this name already in use");
        return;
    }
    if (tm.isCurTableSet()) {
        tm.saveTable(m_dbName);
    }
    tm.setCurTable(m_tables[t_curTableName]);
}

vector<string> Database::showTables() {
    vector<string> tableNameList;
    if (m_tables.size() == 0) {
        throw string("No tables yet");
        return tableNameList;
    }
    
    for (auto temp : m_tables) {
        tableNameList.push_back(temp.first);
    }
    
    return tableNameList;
}

void Database::createTable(string t_tableName, string t_primaryKey, Header t_head) {
    if (m_tables.find(t_tableName) != m_tables.end()) {
        throw string("table with this name already exist");
        return;
    }
    m_tables.insert(pair<string, DBTable*>(t_tableName, new DBTable(t_tableName, t_primaryKey, t_head)));
    writeTableListFile();
}

void Database::removeTable(string t_tableName) {
    if (m_tables.find(t_tableName) == m_tables.end()) {
        throw string("table with this name not exist");
        return;
    }
    if (tm.getCurTableName() == t_tableName) {
        tm.setCurTable(nullptr);
    }
    delete m_tables[t_tableName];
    m_tables.erase(t_tableName);
    
    writeTableListFile();
    
    string command = "rm ";
    command += database_path;
    command += m_dbName + "/";
    command += t_tableName + ".csv";
    system(command.c_str());
}

void Database::readTables() {
    string path = database_path;
    path += m_dbName + "/";
    
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

void Database::readTable(string t_tableName) {
    string path = database_path;
    path += m_dbName + "/";
    
    fstream file(path + t_tableName + ".csv", ios::in);
    
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
            col.colType = DBType::Int;
        } else if (colType == "Double") {
            col.colType = DBType::Double;
        } else if (colType == "String") {
            col.colType = DBType::String;
        } else {
            col.colType = DBType::Date;
        }
        tableHead.emplace_back(col);
    }
    
    m_tables.insert(pair<string, DBTable*>(t_tableName, new DBTable(t_tableName, primaryKey, tableHead)));
    useTable(t_tableName);
    
    while (getline(file, line, '\n')) {
        tm.insertRow(splitString(line));
    }
    
    tm.setCurTable(nullptr);
    file.close();
}

void Database::writeTableListFile() {
    vector<string> tableList = showTables();
    string path = database_path;
    path += m_dbName + "/";
    
    fstream file(path + "tables.txt", ios::out | ios::trunc);
    
    if (!file.is_open()) {
        return;
    }
    
    for (int i = 0; i < tableList.size(); i++) {
        file << tableList[i] << endl;
    }
    
    file.close();
}


