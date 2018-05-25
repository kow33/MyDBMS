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
        string strAns;
        int ans;
        
        cout << ">";
        if (_curDB != nullptr) {
            cout << " " << _curDB->getDBName();
        }
        if (_curDB != nullptr && _curDB->_tm.isCurTableSet()) {
            cout << "." << _curDB->_tm.getCurTableName();
        }
        cout << " ";
        
        cin >> strAns;
        if (strAns == "\\h") {
            printManualTable();
            continue;
        }
        if (strAns == "\\q") {
            cout << "Bye" << endl;
            check = false;
            continue;
        }
        
        ans = stoi(strAns);
        if (ans == 1) {
            try {
                vector<string> dbList = showDBs();
                cout << "Databases:" << endl;
                drawTable(vector<string>(), dbList, left, true);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 2) {
            string dbName;
            cout << "Enter name of database: ";
            cin >> dbName;
            try {
                useDB(dbName);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 3) {
            string dbName;
            cout << "Enter name of database: ";
            cin >> dbName;
            try {
                createDB(dbName);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 4) {
            string dbName;
            cout << "Enter name of database: ";
            cin >> dbName;
            try {
                deleteDB(dbName);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans > 4 && _curDB == nullptr) {
            cerr << "Please, take database in use" << endl;
        } else if (ans == 5) {
            try {
                vector<string> tableList = _curDB->showTables();
                cout << "Tables: " << endl;
                drawTable(vector<string>(), tableList, left, true);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 6) {
            string curTableName;
            cout << "Enter table name: ";
            cin >> curTableName;
            try {
                _curDB->useTable(curTableName);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 7) {
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
            
            try {
                _curDB->createTable(tableName, primaryKey, head);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 8) {
            string tableName;
            cout << "Enter table name: ";
            cin >> tableName;
            try {
                _curDB->removeTable(tableName);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans > 8 && !_curDB->_tm.isCurTableSet()) {
            cerr << "Please, take table in use" << endl;
        } else if (ans == 9) {
            try {
                vector<string> tableRows = _curDB->_tm.select();
                cout << "Table " << _curDB->_tm.getCurTableName() << ":" << endl;
                vector<string> colNameList = _curDB->_tm.getCurTableColNames();
                
                drawTable(colNameList, tableRows);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 10) {
            string colName;
            string equalTo;
            
            cout << "Condition: what should be the value in the column name to satisfy" << endl;
            cout << "Enter column name: ";
            cin >> colName;
            cout << "Enter equal value: ";
            cin >> equalTo;
            
            try {
                vector<string> colNameList = _curDB->_tm.getCurTableColNames();
                vector<string> tableRows = _curDB->_tm.selectWhere(colName, equalTo);
                drawTable(colNameList, tableRows);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 11) {
            string colName;
            
            cout << "Enter column name: ";
            cin >> colName;
        
            vector<string> nameOfCol;
            nameOfCol.push_back(colName);
            try {
                vector<string> nameOfCol;
                nameOfCol.push_back(colName);
                vector<string> column = _curDB->_tm.selectColumn(colName);
                drawTable(nameOfCol, column);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
            
        } else if (ans == 12) {
            vector<string> row;
            vector<string> colNameList = _curDB->_tm.getCurTableColNames();
            cout << "Enter data:" << endl;
            for (int i = 0; i < colNameList.size(); i++) {
                string answer;
                cout << "Enter " << colNameList[i] << ": ";
                cin >> answer;
                row.emplace_back(answer);
            }
            
            try {
                _curDB->_tm.insertRow(row);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 13) {
            _curDB->_tm.deleteRows();
            cout << "Success!" << endl;
        } else if (ans == 14) {
            string colName;
            string equalTo;
            
            cout << "Condition: what should be the value in the column name to satisfy" << endl;
            cout << "Enter column name: ";
            cin >> colName;
            cout << "Enter equal value: ";
            cin >> equalTo;
            
            try {
                _curDB->_tm.deleteRowsWhere(colName, equalTo);
                cout << "Success!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        }
        
        cout << endl;
    }
}

void DBMS::readDBs() {
    string path = databasePath;
    
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
    string path = databasePath;
    
    fstream file(path + "Databases.txt", ios::out | ios::trunc);
    
    for (auto temp : _dbList) {
        file << temp.first << endl;
    }
    
    file.close();
}

vector<string> DBMS::showDBs() {
    vector<string> dbNameList;
    if (_dbList.size() == 0) {
        throw string("No databases yet");
        return dbNameList;
    }
    for (auto temp : _dbList) {
        dbNameList.push_back(temp.first);
    }
    
    return dbNameList;
}

void DBMS::useDB(string dbName) {
    if (_dbList.find(dbName) == _dbList.end()) {
        throw string("database with this name not exist");
        return;
    }
    if (_curDB != nullptr && _curDB->getDBName() == dbName) {
        throw string("database with this name already in use");
        return;
    }
    _curDB = _dbList[dbName];
}

void DBMS::createDB(string dbName) {
    if (_dbList.find(dbName) != _dbList.end()) {
        throw string("database with this name alredy exist");
        return;
    }
    _dbList.insert(pair<string, Database*>(dbName, new Database(dbName)));
    
    string command1 = "mkdir " + databasePath + dbName;
    string command2 = "touch " + databasePath + dbName + "/";
    command2 += "tables.txt";
    system(command1.c_str());
    system(command2.c_str());
    
    saveDBs();
}

void DBMS::deleteDB(string dbName) {
    if (_dbList.find(dbName) == _dbList.end()) {
        throw string("database with this name not exist");
        return;
    }
    if (_curDB != nullptr && _curDB->getDBName() == dbName) {
        _curDB = nullptr;
    }
    delete _dbList[dbName];
    _dbList.erase(dbName);
    
    string command = "rm -R " + databasePath + dbName;
    system(command.c_str());
    
    saveDBs();
}

void printManualTable() {
    vector<string> globalControlTableData = {
        "1 - show list of databases|2 - take database in use",
        "3 - create new database|4 - delete database",
        "\\q - exit|\\h - print help"
    };
    vector<string> databaseControlTableData = {
        "5 - show list of table|6 - take table in use",
        "7 - create new table|8 - delete table"
    };
    vector<string> tableControlTableData = {
        "9 - show all table's rows|10 - show rows with condition",
        "11 - show column of table|12 - insert row in table",
        "13 - clear table|14 - delete rows with condition"
    };
    cout << "Press(Global control):" << endl;
    drawTable(vector<string>(), globalControlTableData, left, true);
    cout << endl;
    
    cout << "Press(Database control if it in use):" << endl;
    drawTable(vector<string>(), databaseControlTableData, left, true);
    cout << endl;
    
    cout << "Press(Table control if it in use):" << endl;
    drawTable(vector<string>(), tableControlTableData, left, true);
    cout << endl;
    
//    cout << "+———————————————————————————————————————————————————————————————————————————————+" << endl;
//    cout << "|\t\t1 - show list of databases.\t\t|\t\t2 - take database in use.\t\t|" << endl;
//    cout << "|\t\t3 - create new database.\t\t|\t\t4 - delete database.\t\t\t|" << endl;
//    cout << "|\t\t\\q - exit. \t\t\t\t\t\t|\t\t\\h - print help.\t\t\t\t|" << endl;
//    cout << "+———————————————————————————————————————————————————————————————————————————————+" << endl;
//    cout << endl;
    
//    cout << "+———————————————————————————————————————————————————————————————————————+" << endl;
//    cout << "|\t\t5 - show list of table.\t\t|\t\t6 - take table in use.\t\t|" << endl;
//    cout << "|\t\t7 - create new table.\t\t|\t\t8 - delete table.\t\t\t|" << endl;
//    cout << "+———————————————————————————————————————————————————————————————————————+" << endl;
//    cout << endl;
    
//    cout << "+———————————————————————————————————————————————————————————————————————————————————————+" << endl;
//    cout << "|\t\t9 - show all table's rows.\t\t|\t\t10 - show rows with condition.\t\t\t|" << endl;
//    cout << "|\t\t11 - show column of table.\t\t|\t\t12 - insert row in table.\t\t\t\t|" << endl;
//    cout << "|\t\t13 - clear table.\t\t\t\t|\t\t14 - delete rows with condition.\t\t|" << endl;
//    cout << "+———————————————————————————————————————————————————————————————————————————————————————+" << endl;
//    cout << endl;
}

void drawTable(vector<string> header, vector<string> data, ios_base &oriented(ios_base &), bool isRowsDelimOn) {
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
    
    auto drawTable = [drawLine, oriented, isRowsDelimOn](vector<string> header, vector<vector<string>> data) {
        int countOfCols;
        bool isHeaderOn = true;
        if (int(header.size()) != 0) {
            countOfCols = int(header.size());
        } else {
            isHeaderOn = false;
            countOfCols = int(data[0].size());
        }
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
        if (isHeaderOn) {
            for (int i = 0; i < countOfCols; i++) {
                if (int(header[i].length()) > maxSizeInCol[i]) {
                    maxSizeInCol[i] = int(header[i].length());
                }
            }
            
            
            drawLine(maxSizeInCol);
            cout << "|";
            for (int i = 0; i < countOfCols; i++) {
                cout << " ";
                cout << setfill(' ') << oriented << setw(maxSizeInCol[i]) << header[i];
                cout << " |";
            }
            cout << endl;
        }

        drawLine(maxSizeInCol);
        for (int i = 0; i < data.size(); i++) {
            cout << "|";
            for (int j = 0; j < countOfCols; j++) {
                cout << " ";
                cout << setfill(' ') << oriented << setw(maxSizeInCol[j]) << data[i][j];
                cout << " |";
            }
            cout << endl;
            if (isRowsDelimOn && i != data.size() - 1) {
                drawLine(maxSizeInCol);
            }
        }
        drawLine(maxSizeInCol);
    };
    
    vector<vector<string>> splitedData;
    
    for (int i = 0; i < data.size(); i++) {
        splitedData.push_back(splitString(data[i]));
    }
    
    drawTable(header, splitedData);
}
