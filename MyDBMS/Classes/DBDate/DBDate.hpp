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
#include <string>
#include <boost/tokenizer.hpp>

using namespace std;

class DBDate {
    
    // MARK: Date Properties
    int _day;
    int _month;
    int _year;
    static const int _arrDaysNotLeap[13];
    static const int _arrDaysLeap[13];
    
    
public:
    
    // MARK: Date Init
    DBDate() : _day(0), _month(0), _year(0){};
    DBDate(int day, int month, int year) : _day(day), _month(month), _year(year){};
    DBDate(string date);
    DBDate(const DBDate& date) : _day(date._day), _month(date._month), _year(date._year){};
    
    // MARK: Date Methods
    int getDay();
    int getMonth();
    int getYear();
    bool isLeapYear();
    static bool isLeapYear(int year);
    int getDaysInMonth();
    static int getDaysInMonth(int month, int year);
    int getDaysForNowInCurYear();
    string dateToString();
    static string dateToString(const DBDate& date);
    
    // MARK: Date Operators
    bool operator==(const DBDate& date);
    bool operator!=(const DBDate& date);
    bool operator<(const DBDate& date);
    bool operator>(const DBDate& date);
    bool operator<=(const DBDate& date);
    bool operator>=(const DBDate& date);
    DBDate &operator=(const DBDate& date);
    DBDate &operator+=(int days);
    DBDate &operator-=(int days);
    int operator-(const DBDate& date);
    friend ostream &operator<<(ostream &out, const DBDate& date);
};

#endif /* DBDate_hpp */
