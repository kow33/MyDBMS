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
    m_curDB = nullptr;
}

DBMS::~DBMS() {
    for (auto temp : m_dbList) {
        delete temp.second;
    }
    m_dbList.clear();
}

void DBMS::run() {
    bool check = true;
    printManualTable();
    while (check) {
        string strAns;
        int ans;
        
        cout << ">";
        if (m_curDB != nullptr) {
            cout << " " << m_curDB->getDBName();
        }
        if (m_curDB != nullptr && m_curDB->tm.isCurTableSet()) {
            cout << "." << m_curDB->tm.getCurTableName();
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
        } else if (ans > 4 && m_curDB == nullptr) {
            cerr << "Please, take database in use" << endl;
        } else if (ans == 5) {
            try {
                vector<string> tableList = m_curDB->showTables();
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
                m_curDB->useTable(curTableName);
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
                cin >> col.colName;
                cout << "Enter column type(Int, Double, String, Date): ";
                cin >> colType;
                col.setColTypeFromString(colType);
                head.emplace_back(col);
            }
            
            cout << "Enter name of primary key: ";
            cin >> primaryKey;
            
            try {
                m_curDB->createTable(tableName, primaryKey, head);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 8) {
            string tableName;
            cout << "Enter table name: ";
            cin >> tableName;
            try {
                m_curDB->removeTable(tableName);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans > 8 && !m_curDB->tm.isCurTableSet()) {
            cerr << "Please, take table in use" << endl;
        } else if (ans == 9) {
            try {
                vector<string> tableRows = m_curDB->tm.select();
                cout << "Table " << m_curDB->tm.getCurTableName() << ":" << endl;
                vector<string> colNameList = m_curDB->tm.getCurTableColNames();
                
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
                vector<string> colNameList = m_curDB->tm.getCurTableColNames();
                vector<string> tableRows = m_curDB->tm.selectWhere(colName, equalTo);
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
                vector<string> column = m_curDB->tm.selectColumn(colName);
                drawTable(nameOfCol, column);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
            
        } else if (ans == 12) {
            vector<string> row;
            vector<string> colNameList = m_curDB->tm.getCurTableColNames();
            cout << "Enter data:" << endl;
            for (int i = 0; i < colNameList.size(); i++) {
                string answer;
                cout << "Enter " << colNameList[i] << ": ";
                cin >> answer;
                row.emplace_back(answer);
            }
            
            try {
                m_curDB->tm.insertRow(row);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 13) {
            m_curDB->tm.deleteRows();
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
                m_curDB->tm.deleteRowsWhere(colName, equalTo);
                cout << "Success!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        }
        
        cout << endl;
    }
}

void DBMS::readDBs() {
    string path = database_path;
    
    fstream file(path + "Databases.txt", ios::in);
    
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line, '\n')) {
        m_dbList.insert(pair<string, Database*>(line, new Database(line)));
    }
    
    file.close();
}

void DBMS::saveDBs() {
    string path = database_path;
    
    fstream file(path + "Databases.txt", ios::out | ios::trunc);
    
    for (auto temp : m_dbList) {
        file << temp.first << endl;
    }
    
    file.close();
}

vector<string> DBMS::showDBs() {
    vector<string> dbNameList;
    if (m_dbList.size() == 0) {
        throw string("No databases yet");
    }
    for (auto temp : m_dbList) {
        dbNameList.push_back(temp.first);
    }
    
    return dbNameList;
}

void DBMS::useDB(string t_dbName) {
    if (m_dbList.find(t_dbName) == m_dbList.end()) {
        throw string("database with this name not exist");
    }
    if (m_curDB != nullptr && m_curDB->getDBName() == t_dbName) {
        throw string("database with this name already in use");
    }
    m_curDB = m_dbList[t_dbName];
}

void DBMS::createDB(string t_dbName) {
    if (m_dbList.find(t_dbName) != m_dbList.end()) {
        throw string("database with this name alredy exist");
    }
    m_dbList.insert(pair<string, Database*>(t_dbName, new Database(t_dbName)));
    
    string command1 = "mkdir " + database_path + t_dbName;
    string command2 = "touch " + database_path + t_dbName + "/";
    command2 += "tables.txt";
    system(command1.c_str());
    system(command2.c_str());
    
    saveDBs();
}

void DBMS::deleteDB(string t_dbName) {
    if (m_dbList.find(t_dbName) == m_dbList.end()) {
        throw string("database with this name not exist");
    }
    if (m_curDB != nullptr && m_curDB->getDBName() == t_dbName) {
        m_curDB = nullptr;
    }
    delete m_dbList[t_dbName];
    m_dbList.erase(t_dbName);
    
    string command = "rm -R " + database_path + t_dbName;
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

void drawTable(vector<string> t_header, vector<string> t_data, ios_base &t_oriented(ios_base &), bool t_isRowsDelimOn) {
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
    
    auto drawTable = [drawLine, t_oriented, t_isRowsDelimOn](vector<string> header, vector<vector<string>> data) {
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
                cout << setfill(' ') << t_oriented << setw(maxSizeInCol[i]) << header[i];
                cout << " |";
            }
            cout << endl;
        }

        drawLine(maxSizeInCol);
        for (int i = 0; i < data.size(); i++) {
            cout << "|";
            for (int j = 0; j < countOfCols; j++) {
                cout << " ";
                cout << setfill(' ') << t_oriented << setw(maxSizeInCol[j]) << data[i][j];
                cout << " |";
            }
            cout << endl;
            if (t_isRowsDelimOn && i != data.size() - 1) {
                drawLine(maxSizeInCol);
            }
        }
        drawLine(maxSizeInCol);
    };
    
    vector<vector<string>> splitedData;
    
    for (int i = 0; i < t_data.size(); i++) {
        splitedData.push_back(splitString(t_data[i]));
    }
    
    drawTable(t_header, splitedData);
}

void DBMS::test() {
    vector<queue<string>> commands;
    
    commands.emplace_back(queue<string>());
    commands.back().push("\\h");
    
    // command 1
    commands.emplace_back(queue<string>());
    commands.back().push("1");
    
    //errors
    commands.emplace_back(queue<string>());
    commands.back().push("666");
    
    //command 3
    commands.emplace_back(queue<string>());
    commands.back().push("3");
    commands.back().push("test");
    
    commands.emplace_back(queue<string>());
    commands.back().push("3");
    commands.back().push("_test");
    
    // command 1
    commands.emplace_back(queue<string>());
    commands.back().push("1");
    
    //command 4
    commands.emplace_back(queue<string>());
    commands.back().push("4");
    commands.back().push("_test");
    
    commands.emplace_back(queue<string>());
    commands.back().push("4");
    commands.back().push("test3");
    
    // command 1
    commands.emplace_back(queue<string>());
    commands.back().push("1");
    
    //command 2
    commands.emplace_back(queue<string>());
    commands.back().push("2");
    commands.back().push("_test_");
    
    commands.emplace_back(queue<string>());
    commands.back().push("2");
    commands.back().push("test");
    
    //command 5
    commands.emplace_back(queue<string>());
    commands.back().push("5");
    
    //errors
    commands.emplace_back(queue<string>());
    commands.back().push("100");
    
    // command 7
    commands.emplace_back(queue<string>());
    commands.back().push("7");
    commands.back().push("Books");
    commands.back().push("3");
    commands.back().push("Test");
    commands.back().push("Int");
    commands.back().push("Column");
    commands.back().push("String");
    commands.back().push("Example");
    commands.back().push("Date");
    commands.back().push("Test");
    
    commands.emplace_back(queue<string>());
    commands.back().push("7");
    commands.back().push("test_table");
    commands.back().push("3");
    commands.back().push("Test");
    commands.back().push("Int");
    commands.back().push("Column");
    commands.back().push("String");
    commands.back().push("Example");
    commands.back().push("Date");
    commands.back().push("Test");
    
    //command 5
    commands.emplace_back(queue<string>());
    commands.back().push("5");
    
    //command 8
    commands.emplace_back(queue<string>());
    commands.back().push("8");
    commands.back().push("Boo");
    
    commands.emplace_back(queue<string>());
    commands.back().push("8");
    commands.back().push("test_table");
    
    //command 5
    commands.emplace_back(queue<string>());
    commands.back().push("5");
    
    //command 6
    commands.emplace_back(queue<string>());
    commands.back().push("6");
    commands.back().push("Book");
    
    commands.emplace_back(queue<string>());
    commands.back().push("6");
    commands.back().push("Books");
    
    // command 9
    commands.emplace_back(queue<string>());
    commands.back().push("9");
    
    // command 10
    commands.emplace_back(queue<string>());
    commands.back().push("10");
    commands.back().push("Book");
    commands.back().push("Linal");
    
    commands.emplace_back(queue<string>());
    commands.back().push("10");
    commands.back().push("BookID");
    commands.back().push("14");
    
    commands.emplace_back(queue<string>());
    commands.back().push("10");
    commands.back().push("Number");
    commands.back().push("0");
    
    // command 11
    commands.emplace_back(queue<string>());
    commands.back().push("11");
    commands.back().push("ID");
    
    commands.emplace_back(queue<string>());
    commands.back().push("11");
    commands.back().push("Title");
    
    // command 12
    commands.emplace_back(queue<string>());
    commands.back().push("12");
    commands.back().push("100");
    commands.back().push("Pushkin");
    commands.back().push("EvgenyOnegin");
    commands.back().push("5");
    commands.back().push("3");
    
    // command 9
    commands.emplace_back(queue<string>());
    commands.back().push("9");
    
    // command 14
    commands.emplace_back(queue<string>());
    commands.back().push("14");
    commands.back().push("Books");
    commands.back().push("Linal");
    
    commands.emplace_back(queue<string>());
    commands.back().push("14");
    commands.back().push("Number");
    commands.back().push("0");
    
    // command 13
    commands.emplace_back(queue<string>());
    commands.back().push("13");
    
    // command 9
    commands.emplace_back(queue<string>());
    commands.back().push("9");
    
    commands.emplace_back(queue<string>());
    commands.back().push("\\q");
    
    for (auto & command : commands) {
        string strAns = command.front();
        command.pop();
        int ans;
        
        cout << ">";
        if (m_curDB != nullptr) {
            cout << " " << m_curDB->getDBName();
        }
        if (m_curDB != nullptr && m_curDB->tm.isCurTableSet()) {
            cout << "." << m_curDB->tm.getCurTableName();
        }
        cout << " " << strAns << endl;
        
        if (strAns == "\\h") {
            printManualTable();
            continue;
        }
        
        if (strAns == "\\q") {
            cout << "Bye" << endl;
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
            string dbName = command.front();
            command.pop();
            cout << "Enter name of database: " << dbName << endl;
            try {
                useDB(dbName);
                cout << "Success!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 3) {
            string dbName = command.front();
            command.pop();
            cout << "Enter name of database: " << dbName << endl;
            try {
                createDB(dbName);
                cout << "Database with name " << dbName << " created!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 4) {
            string dbName = command.front();
            command.pop();
            cout << "Enter name of database: " << dbName << endl;
            try {
                deleteDB(dbName);
                cout << "Database with name " << dbName << " deleted!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans > 4 && m_curDB == nullptr) {
            cerr << "Please, take database in use" << endl;
        } else if (ans == 5) {
            try {
                vector<string> tableList = m_curDB->showTables();
                cout << "Tables: " << endl;
                drawTable(vector<string>(), tableList, left, true);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 6) {
            string curTableName = command.front();
            command.pop();
            cout << "Enter table name: " << curTableName << endl;
            try {
                m_curDB->useTable(curTableName);
                cout << "Success!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 7) {
            string tableName = command.front();
            command.pop();
            int countOfCols = stoi(command.front());
            command.pop();
            string primaryKey;
            Header head;
            
            cout << "Enter table name: " << tableName << endl;
            
            cout << "Enter count of columns in table: " << countOfCols << endl;
            
            cout << "Enter list of column headers: " << endl;
            for (int i = 0; i < countOfCols; i++) {
                Column col;
                string colType;
                cout << "Column " << i + 1 << endl;
                col.colName = command.front();
                command.pop();
                cout << "Enter column name: " << col.colName << endl;
                colType = command.front();
                command.pop();
                cout << "Enter column type(Int, Double, String, Date): " << colType << endl;
                col.setColTypeFromString(colType);
                head.emplace_back(col);
            }
            
            primaryKey = command.front();
            command.pop();
            cout << "Enter name of primary key: " << primaryKey << endl;
            
            try {
                m_curDB->createTable(tableName, primaryKey, head);
                cout << "Table with name " << tableName << " created!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 8) {
            string tableName = command.front();
            command.pop();
            cout << "Enter table name: " << tableName << endl;
            try {
                m_curDB->removeTable(tableName);
                cout << "Table with name " << tableName << " deleted!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans > 8 && !m_curDB->tm.isCurTableSet()) {
            cerr << "Please, take table in use" << endl;
        } else if (ans == 9) {
            try {
                vector<string> tableRows = m_curDB->tm.select();
                cout << "Table " << m_curDB->tm.getCurTableName() << ":" << endl;
                vector<string> colNameList = m_curDB->tm.getCurTableColNames();
                
                drawTable(colNameList, tableRows);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 10) {
            string colName = command.front();
            command.pop();
            string equalTo = command.front();
            command.pop();
            
            cout << "Condition: what should be the value in the column name to satisfy" << endl;
            cout << "Enter column name: " << colName << endl;
            cout << "Enter equal value: " << equalTo << endl;
            
            try {
                vector<string> colNameList = m_curDB->tm.getCurTableColNames();
                vector<string> tableRows = m_curDB->tm.selectWhere(colName, equalTo);
                drawTable(colNameList, tableRows);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 11) {
            string colName = command.front();
            command.pop();
            
            cout << "Enter column name: " << colName << endl;
            
            vector<string> nameOfCol;
            nameOfCol.push_back(colName);
            try {
                vector<string> nameOfCol;
                nameOfCol.push_back(colName);
                vector<string> column = m_curDB->tm.selectColumn(colName);
                drawTable(nameOfCol, column);
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
            
        } else if (ans == 12) {
            vector<string> row;
            vector<string> colNameList = m_curDB->tm.getCurTableColNames();
            cout << "Enter data:" << endl;
            for (int i = 0; i < colNameList.size(); i++) {
                string answer = command.front();
                command.pop();
                cout << "Enter " << colNameList[i] << ": " << answer << endl;
                row.emplace_back(answer);
            }
            
            try {
                m_curDB->tm.insertRow(row);
                cout << "Success!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        } else if (ans == 13) {
            m_curDB->tm.deleteRows();
            cout << "Success!" << endl;
        } else if (ans == 14) {
            string colName = command.front();
            command.pop();
            string equalTo = command.front();
            command.pop();
            
            cout << "Condition: what should be the value in the column name to satisfy" << endl;
            cout << "Enter column name: " << colName << endl;
            cout << "Enter equal value: " << equalTo << endl;
            
            try {
                m_curDB->tm.deleteRowsWhere(colName, equalTo);
                cout << "Success!" << endl;
            } catch (string err) {
                cerr << "Error: " << err << endl;
            }
        }
        
        cout << endl;
    }
}
