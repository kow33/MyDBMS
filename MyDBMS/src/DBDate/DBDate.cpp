//
//  DBDate.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 01.03.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "DBDate.hpp"

// MARK: - Date Properties
const int DBDate::_arrDaysNotLeap[] = {
    0,
    31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
};

const int DBDate::_arrDaysLeap[] = {
    0,
    31,
    29,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
};

// MARK: - Date Methods
DBDate::DBDate(string date) {
    auto splitString = [](string str) -> vector<string> {
        vector<string> splitedStr;
        string tempStr = "";
        
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '.') {
                splitedStr.push_back(tempStr);
                tempStr = "";
                continue;
            }
            tempStr += str[i];
        }
        splitedStr.push_back(tempStr);
        
        return splitedStr;
    };
    
    vector<string> arrDate = splitString(date);
    
    _day = stoi(arrDate[0]);
    _month = stoi(arrDate[1]);
    _year = stoi(arrDate[2]);
    
}

int DBDate::getDay() {
    return _day;
}

int DBDate::getMonth() {
    return _month;
}

int DBDate::getYear() {
    return _year;
}

bool DBDate::isLeapYear() {
    return isLeapYear(_year);
}

bool DBDate::isLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        return true;
    } else {
        return false;
    }
}

int DBDate::getDaysInMonth() {
    return getDaysInMonth(_month, _year);
}

int DBDate::getDaysInMonth(int month, int year) {
    if (isLeapYear(year)) {
        return _arrDaysLeap[month];
    } else {
        return _arrDaysNotLeap[month];
    }
}

int DBDate::getDaysForNowInCurYear() {
    int sum = _day - 1;
    
    for (int i = 1; i < _month; i++) {
        sum += getDaysInMonth(i, _year);
    }
    
    return sum;
}

string DBDate::dateToString() {
    return dateToString(*this);
}

string DBDate::dateToString(const DBDate &date) {
    string strTemp = "";
    
    if (date._day > 9) {
        strTemp += to_string(date._day) + ".";
    } else {
        strTemp += "0" + to_string(date._day) + ".";
    }
    
    if (date._month > 9) {
        strTemp += to_string(date._month) + ".";
    } else {
        strTemp += "0" + to_string(date._month) + ".";
    }
    if (date._year > 999) {
        strTemp += to_string(date._year);
    } else if (date._year > 99) {
        strTemp += "0" + to_string(date._year);
    } else if (date._year > 9) {
        strTemp += "00" + to_string(date._year);
    } else {
        strTemp += "000" + to_string(date._year);
    }
    
    return strTemp;
}

// MARK: - Date Operators
bool DBDate::operator==(const DBDate &date) {
    if ((_day == date._day) && (_month == date._month) && (_year == date._year)) {
        return true;
    } else {
        return false;
    }
}

bool DBDate::operator!=(const DBDate &date) {
    return !(*this == date);
}

bool DBDate::operator<(const DBDate &date) {
    if (_year < date._year) {
        return true;
    } else if ((_year == date._year) && (_month < date._month)) {
        return true;
    } else if ((_year == date._year) && (_month == date._month) && (_day < date._day)) {
        return true;
    } else {
        return false;
    }
}

bool DBDate::operator>(const DBDate &date) {
    if (_year > date._year) {
        return true;
    } else if ((_year == date._year) && (_month > date._month)) {
        return true;
    } else if ((_year == date._year) && (_month == date._month) && (_day > date._day)) {
        return true;
    } else {
        return false;
    }
}

bool DBDate::operator<=(const DBDate &date) {
    if (*this == date) {
        return true;
    } else {
        return (*this < date);
    }
}

bool DBDate::operator>=(const DBDate &date) {
    if (*this == date) {
        return true;
    } else {
        return (*this > date);
    }
}

DBDate &DBDate::operator=(const DBDate &date) {
    _day = date._day;
    _month = date._month;
    _year = date._year;
    
    return *this;
}

DBDate &DBDate::operator+=(int days) {
    while ((_day + days) > (isLeapYear() ? _arrDaysLeap[_month] : _arrDaysNotLeap[_month])) {
        if (isLeapYear()) {
            days -= _arrDaysLeap[_month];
            _month++;
        } else {
            days -= _arrDaysNotLeap[_month];
            _month++;
        }
        if (_month > 12) {
            _month -= 12;
            _year++;
        }
    }
    _day += days;
    
    return *this;
}

DBDate &DBDate::operator-=(int days) {
    while ((_day - days) < 1) {
        if ((_month - 1) < 1) {
            _month += 12;
            _year--;
        }
        if (isLeapYear()) {
            days -= _arrDaysLeap[_month - 1];
            _month--;
        } else {
            days -= _arrDaysNotLeap[_month - 1];
            _month --;
        }
    }
    _day -= days;
    return *this;
}

int DBDate::operator-(const DBDate &date) {
    bool possitive;
    int res = abs(_day - date._day);
    if (*this < date) {
        possitive = false;
        for (int i = _year; i <= date._year; i++) {
            for (int j = _month; j < date._month; j++) {
                if (isLeapYear(i)) {
                    res += _arrDaysLeap[j];
                } else {
                    res += _arrDaysNotLeap[j];
                }
            }
        }
    } else {
        possitive = true;
        for (int i = date._year; i <= _year; i++) {
            for (int j = date._month; j < _month; j++) {
                if (isLeapYear(i)) {
                    res += _arrDaysLeap[j];
                } else {
                    res += _arrDaysNotLeap[j];
                }
            }
        }
    }
    
    if (possitive) {
        return res;
    } else {
        return (-res);
    }
}

ostream &operator<<(ostream &out, const DBDate& date) {
    out << DBDate::dateToString(date);
    return out;
}
