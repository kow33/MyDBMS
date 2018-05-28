//
//  DBDate.h
//  MySUBD
//
//  Created by Михаил Кочетков on 25.04.18.
//  Copyright © 2018 veseluha.ru. All rights reserved.
//

#ifndef DBDate_hpp
#define DBDate_hpp

#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class DBDate {
    
    // MARK: Date Properties
    int m_day;
    int m_month;
    int m_year;
    static const int m_arrDaysNotLeap[13];
    static const int m_arrDaysLeap[13];
    
    
public:
    
    // MARK: Date Init
    DBDate() : m_day(0), m_month(0), m_year(0) {};
    DBDate(int t_day, int t_month, int t_year) : m_day(t_day), m_month(t_month), m_year(t_year) {};
    DBDate(string t_date);
    DBDate(const DBDate &t_date) : m_day(t_date.m_day), m_month(t_date.m_month), m_year(t_date.m_year) {};
    
    // MARK: Date Methods
    int getDay();
    int getMonth();
    int getYear();
    bool isLeapYear();
    static bool isLeapYear(int t_year);
    int getDaysInMonth();
    static int getDaysInMonth(int t_month, int t_year);
    int getDaysForNowInCurYear();
    string dateToString();
    static string dateToString(const DBDate &t_date);
    
    // MARK: Date Operators
    bool operator==(const DBDate& t_date);
    bool operator!=(const DBDate& t_date);
    bool operator<(const DBDate& t_date);
    bool operator>(const DBDate& t_date);
    bool operator<=(const DBDate& t_date);
    bool operator>=(const DBDate& t_date);
    DBDate &operator=(const DBDate& t_date);
    DBDate &operator+=(int t_days);
    DBDate &operator-=(int t_days);
    int operator-(const DBDate& t_date);
    friend ostream &operator<<(ostream &t_out, const DBDate& t_date);
};

#endif /* DBDate_hpp */
