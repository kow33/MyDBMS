//
//  DBDate.cpp
//  MySUBD
//
//  Created by Михаил Кочетков on 01.03.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#include "DBDate.hpp"

// MARK: - Date Properties
const int DBDate::m_arrDaysNotLeap[] = {
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

const int DBDate::m_arrDaysLeap[] = {
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
DBDate::DBDate(string t_date) {
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
    
    vector<string> arrDate = splitString(t_date);
    
    m_day = stoi(arrDate[0]);
    m_month = stoi(arrDate[1]);
    m_year = stoi(arrDate[2]);
    
}

int DBDate::getDay() {
    return m_day;
}

int DBDate::getMonth() {
    return m_month;
}

int DBDate::getYear() {
    return m_year;
}

bool DBDate::isLeapYear() {
    return isLeapYear(m_year);
}

bool DBDate::isLeapYear(int t_year) {
    if ((t_year % 4 == 0 && t_year % 100 != 0) || (t_year % 400 == 0)) {
        return true;
    } else {
        return false;
    }
}

int DBDate::getDaysInMonth() {
    return getDaysInMonth(m_month, m_year);
}

int DBDate::getDaysInMonth(int t_month, int t_year) {
    if (isLeapYear(t_year)) {
        return m_arrDaysLeap[t_month];
    } else {
        return m_arrDaysNotLeap[t_month];
    }
}

int DBDate::getDaysForNowInCurYear() {
    int sum = m_day - 1;
    
    for (int i = 1; i < m_month; i++) {
        sum += getDaysInMonth(i, m_year);
    }
    
    return sum;
}

string DBDate::dateToString() {
    return dateToString(*this);
}

string DBDate::dateToString(const DBDate &t_date) {
    string strTemp = "";
    
    if (t_date.m_day > 9) {
        strTemp += to_string(t_date.m_day) + ".";
    } else {
        strTemp += "0" + to_string(t_date.m_day) + ".";
    }
    
    if (t_date.m_month > 9) {
        strTemp += to_string(t_date.m_month) + ".";
    } else {
        strTemp += "0" + to_string(t_date.m_month) + ".";
    }
    if (t_date.m_year > 999) {
        strTemp += to_string(t_date.m_year);
    } else if (t_date.m_year > 99) {
        strTemp += "0" + to_string(t_date.m_year);
    } else if (t_date.m_year > 9) {
        strTemp += "00" + to_string(t_date.m_year);
    } else {
        strTemp += "000" + to_string(t_date.m_year);
    }
    
    return strTemp;
}

// MARK: - Date Operators
bool DBDate::operator==(const DBDate &t_date) {
    if ((m_day == t_date.m_day) && (m_month == t_date.m_month) && (m_year == t_date.m_year)) {
        return true;
    } else {
        return false;
    }
}

bool DBDate::operator!=(const DBDate &t_date) {
    return !(*this == t_date);
}

bool DBDate::operator<(const DBDate &t_date) {
    if (m_year < t_date.m_year) {
        return true;
    } else if ((m_year == t_date.m_year) && (m_month < t_date.m_month)) {
        return true;
    } else if ((m_year == t_date.m_year) && (m_month == t_date.m_month) && (m_day < t_date.m_day)) {
        return true;
    } else {
        return false;
    }
}

bool DBDate::operator>(const DBDate &t_date) {
    if (m_year > t_date.m_year) {
        return true;
    } else if ((m_year == t_date.m_year) && (m_month > t_date.m_month)) {
        return true;
    } else if ((m_year == t_date.m_year) && (m_month == t_date.m_month) && (m_day > t_date.m_day)) {
        return true;
    } else {
        return false;
    }
}

bool DBDate::operator<=(const DBDate &t_date) {
    if (*this == t_date) {
        return true;
    } else {
        return (*this < t_date);
    }
}

bool DBDate::operator>=(const DBDate &t_date) {
    if (*this == t_date) {
        return true;
    } else {
        return (*this > t_date);
    }
}

DBDate &DBDate::operator=(const DBDate &t_date) {
    m_day = t_date.m_day;
    m_month = t_date.m_month;
    m_year = t_date.m_year;
    
    return *this;
}

DBDate &DBDate::operator+=(int t_days) {
    while ((m_day + t_days) > (isLeapYear() ? m_arrDaysLeap[m_month] : m_arrDaysNotLeap[m_month])) {
        if (isLeapYear()) {
            t_days -= m_arrDaysLeap[m_month];
            m_month++;
        } else {
            t_days -= m_arrDaysNotLeap[m_month];
            m_month++;
        }
        if (m_month > 12) {
            m_month -= 12;
            m_year++;
        }
    }
    m_day += t_days;
    
    return *this;
}

DBDate &DBDate::operator-=(int t_days) {
    while ((m_day - t_days) < 1) {
        if ((m_month - 1) < 1) {
            m_month += 12;
            m_year--;
        }
        if (isLeapYear()) {
            t_days -= m_arrDaysLeap[m_month - 1];
            m_month--;
        } else {
            t_days -= m_arrDaysNotLeap[m_month - 1];
            m_month --;
        }
    }
    m_day -= t_days;
    return *this;
}

int DBDate::operator-(const DBDate &t_date) {
    bool possitive;
    int res = abs(m_day - t_date.m_day);
    if (*this < t_date) {
        possitive = false;
        for (int i = m_year; i <= t_date.m_year; i++) {
            for (int j = m_month; j < t_date.m_month; j++) {
                if (isLeapYear(i)) {
                    res += m_arrDaysLeap[j];
                } else {
                    res += m_arrDaysNotLeap[j];
                }
            }
        }
    } else {
        possitive = true;
        for (int i = t_date.m_year; i <= m_year; i++) {
            for (int j = t_date.m_month; j < m_month; j++) {
                if (isLeapYear(i)) {
                    res += m_arrDaysLeap[j];
                } else {
                    res += m_arrDaysNotLeap[j];
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

ostream &operator<<(ostream &t_out, const DBDate& t_date) {
    t_out << DBDate::dateToString(t_date);
    return t_out;
}
