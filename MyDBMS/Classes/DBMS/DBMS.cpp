//
//  SUBD.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 19.05.2018.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "DBMS.hpp"

DBMS::DBMS() {
    readDBs();
    _curDB = nullptr;
}

DBMS::~DBMS() {
    for (auto temp : _dbList) {
        delete temp.second;
    }
    _dbList.clear();
}

void DBMS::run() {
    bool check = true;
    printManualTable();
    while (check) {
        int answer = -1;
        
        cout << ">";
        if (_curDB != nullptr) {
            cout << " " << _curDB->getDBName();
        }
        if (_curDB != nullptr && _curDB->_tm.isCurTableSet()) {
            cout << "." << _curDB->_tm.getCurTableName();
        }
        cout << " ";
        
        cin >> answer;
        if (answer == -1) {
            printManualTable();
        } else if (answer == 0) {
            cout << "Bye" << endl;
            check = false;
        } else if (answer == 1) {
            vector<string> dbList = showDBs();
            cout << "Databases:" << endl;
            for (auto dbName : dbList) {
                cout << dbName << endl;
            }
        } else if (answer == 2) {
            string dbName;
            cout << "Enter name of database: ";
            cin >> dbName;
            useDB(dbName);
        } else if (answer == 3) {
            string dbName;
            cout << "Enter name of database: ";
            cin >> dbName;
            createDB(dbName);
        } else if (answer == 4) {
            string dbName;
            cout << "Enter name of database: ";
            cin >> dbName;
            deleteDB(dbName);
        } else if (answer > 4 && _curDB == nullptr) {
            cout << "Please, take database in use" << endl;
        } else if (answer == 5) {
            vector<string> tableList = _curDB->showTables();
            cout << "Tables: " << endl;
            for (auto tableName : tableList) {
                cout << tableName << endl;
            }
        } else if (answer == 6) {
            string curTableName;
            cout << "Enter table name: ";
            cin >> curTableName;
            _curDB->useTable(curTableName);
        } else if (answer == 7) {
            string tableName;
            string primaryKey;
            Header head;
            int countOfCols;
            
            cout << "Enter table name: ";
            cin >> tableName;
            
            cout << "Enter count of columns in table: ";
            cin >> countOfCols;
            
            cout << "Enter list of column headers: " << endl;
            for (int i = 0; i < countOfCols; i++) {
                Column col;
                string colType;
                cout << "Column " << i + 1 << endl;
                cout << "Enter column name: ";
                cin >> col._colName;
                cout << "Enter column type(Int, Double, String, Date): ";
                cin >> colType;
                col.setColTypeFromString(colType);
                head.emplace_back(col);
            }
            
            cout << "Enter name of primary key: ";
            cin >> primaryKey;
            
            _curDB->createTable(tableName, primaryKey, head);
        } else if (answer == 8) {
            string tableName;
            cout << "Enter table name: ";
            cin >> tableName;
            _curDB->removeTable(tableName);
        } else if (answer > 8 && !_curDB->_tm.isCurTableSet()) {
            cout << "Please, take table in use" << endl;
        } else if (answer == 9) {
            vector<string> tableRows = _curDB->_tm.select();
            cout << "Table " << _curDB->_tm.getCurTableName() << ":" << endl;
            vector<string> colNameList = _curDB->_tm.getCurTableColNames();
            drawTable(colNameList, tableRows);
        } else if (answer == 10) {
            string colName;
            string equalTo;
            
            cout << "Condition: what should be the value in the column name to satisfy" << endl;
            cout << "Enter column name: ";
            cin >> colName;
            cout << "Enter equal value: ";
            cin >> equalTo;
            
            vector<string> colNameList = _curDB->_tm.getCurTableColNames();
            vector<string> tableRows = _curDB->_tm.selectWhere(colName, equalTo);
            drawTable(colNameList, tableRows);
        } else if (answer == 11) {
            string colName;
            
            cout << "Enter column name: ";
            cin >> colName;
        
            vector<string> nameOfCol;
            nameOfCol.push_back(colName);
            vector<string> column = _curDB->_tm.selectColumn(colName);
            drawTable(nameOfCol, column);
            
        } else if (answer == 12) {
            vector<string> row;
            vector<string> colNameList = _curDB->_tm.getCurTableColNames();
            cout << "Enter data:" << endl;
            for (int i = 0; i < colNameList.size(); i++) {
                string answer;
                cout << "Enter " << colNameList[i] << ": ";
                cin >> answer;
                row.emplace_back(answer);
            }
            
            _curDB->_tm.insertRow(row);
        } else if (answer == 13) {
            if (_curDB->_tm.deleteRows()) {
                cout << "Success!" << endl;
            } else {
                cout << "Some error, try again" << endl;
            }
        } else if (answer == 14) {
            string colName;
            string equalTo;
            
            cout << "Condition: what should be the value in the column name to satisfy" << endl;
            cout << "Enter column name: ";
            cin >> colName;
            cout << "Enter equal value: ";
            cin >> equalTo;
            
            if (_curDB->_tm.deleteRowWhere(colName, equalTo)) {
                cout << "Success!" << endl;
            } else {
                cout << "Some error, try again" << endl;
            }
        }
        
        cout << endl;
    }
}

void DBMS::readDBs() {
    string path = "/Users/mihailkocetkov/C++/MySUBD/MyDBMS/Databases/";
    
    fstream file(path + "Databases.txt", ios::in);
    
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line, '\n')) {
        _dbList.insert(pair<string, Database*>(line, new Database(line)));
    }
    
    file.close();
}

void DBMS::saveDBs() {
    string path = "/Users/mihailkocetkov/C++/MySUBD/MyDBMS/Databases/";
    
    fstream file(path + "Databases.txt", ios::out | ios::trunc);
    
    for (auto temp : _dbList) {
        file << temp.first << endl;
    }
    
    file.close();
}

vector<string> DBMS::showDBs() {
    vector<string> dbNameList;
    for (auto temp : _dbList) {
        dbNameList.push_back(temp.first);
    }
    
    return dbNameList;
}

void DBMS::useDB(string dbName) {
    if (_dbList.find(dbName) == _dbList.end() || (_curDB != nullptr && _curDB->getDBName() == dbName)) {
        return;
    }
    _curDB = _dbList[dbName];
}

void DBMS::createDB(string dbName) {
    if (_dbList.find(dbName) != _dbList.end()) {
        return;
    }
    _dbList.insert(pair<string, Database*>(dbName, new Database(dbName)));
    
    string command1 = "mkdir /Users/mihailkocetkov/C++/MySUBD/MyDBMS/Databases/" + dbName;
    string command2 = "touch /Users/mihailkocetkov/C++/MySUBD/MyDBMS/Databases/" + dbName + "/";
    command2 += "tables.txt";
    system(command1.c_str());
    system(command2.c_str());
    
    saveDBs();
}

void DBMS::deleteDB(string dbName) {
    if (_curDB != nullptr && _curDB->getDBName() == dbName) {
        _curDB = nullptr;
    }
    delete _dbList[dbName];
    _dbList.erase(dbName);
    
    string command = "rm -R /Users/mihailkocetkov/C++/MySUBD/MyDBMS/Databases/" + dbName;
    system(command.c_str());
    
    saveDBs();
}

void printManualTable() {
    cout << "Press(Global control):" << endl;
    cout << "+———————————————————————————————————————————————————————————————————————————————+" << endl;
    cout << "|\t\t1 - show list of databases.\t\t|\t\t2 - take database in use.\t\t|" << endl;
    cout << "|\t\t3 - create new database.\t\t|\t\t4 - delete database.\t\t\t|" << endl;
    cout << "|\t\t0 - exit. \t\t\t\t\t\t|\t\t-1 - print help.\t\t\t\t|" << endl;
    cout << "+———————————————————————————————————————————————————————————————————————————————+" << endl;
    cout << endl;
    
    cout << "Press(Database control if it in use):" << endl;
    cout << "+———————————————————————————————————————————————————————————————————————+" << endl;
    cout << "|\t\t5 - show list of table.\t\t|\t\t6 - take table in use.\t\t|" << endl;
    cout << "|\t\t7 - create new table.\t\t|\t\t8 - delete table.\t\t\t|" << endl;
    cout << "+———————————————————————————————————————————————————————————————————————+" << endl;
    cout << endl;
    
    cout << "Press(Table control if it in use):" << endl;
    cout << "+———————————————————————————————————————————————————————————————————————————————————————+" << endl;
    cout << "|\t\t9 - show all table's rows.\t\t|\t\t10 - show rows with condition.\t\t\t|" << endl;
    cout << "|\t\t11 - show column of table.\t\t|\t\t12 - insert row in table.\t\t\t\t|" << endl;
    cout << "|\t\t13 - clear table.\t\t\t\t|\t\t14 - delete rows with condition.\t\t|" << endl;
    cout << "+———————————————————————————————————————————————————————————————————————————————————————+" << endl;
    cout << endl;
}

void drawTable(vector<string> header, vector<string> data) {
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
    
    auto drawLine = [](vector<int> maxColWidth) {
        cout << "+—";
        for (int i = 0; i < maxColWidth.size(); i++) {
            for (int j = 0; j < maxColWidth[i] ; j++) {
                cout << "—";
            }
            if (i != maxColWidth.size() - 1) {
                cout << "—+—";
            }
        }
        cout << "—+";
        cout << endl;
    };
    
    auto drawTable = [drawLine](vector<string> header, vector<vector<string>> data) {
        int countOfCols = int(header.size());
        vector<int> maxSizeInCol;
        for (int i = 0; i < countOfCols; i++) {
            int len = -1;
            for (int j = 0; j < data.size(); j++) {
                if (int(data[j][i].length()) > len) {
                    len = int(data[j][i].length());
                }
            }
            maxSizeInCol.emplace_back(len);
        }
        for (int i = 0; i < countOfCols; i++) {
            if (int(header[i].length()) > maxSizeInCol[i]) {
                maxSizeInCol[i] = int(header[i].length());
            }
        }
        
        drawLine(maxSizeInCol);
        cout << "|";
        for (int i = 0; i < countOfCols; i++) {
            cout << " ";
            cout << setfill(' ') << setw(maxSizeInCol[i]) << header[i];
            cout << " |";
        }
        cout << endl;
        drawLine(maxSizeInCol);
        for (int i = 0; i < data.size(); i++) {
            cout << "|";
            for (int j = 0; j < countOfCols; j++) {
                cout << " ";
                cout << setfill(' ') << setw(maxSizeInCol[j]) << data[i][j];
                cout << " |";
            }
            cout << endl;
        }
        drawLine(maxSizeInCol);
    };
    
    vector<vector<string>> splitedData;
    
    for (int i = 0; i < data.size(); i++) {
        splitedData.push_back(splitString(data[i]));
    }
    
    drawTable(header, splitedData);
}
