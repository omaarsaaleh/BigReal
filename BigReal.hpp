#ifndef BIGREAL_HPP
#define BIGREAL_HPP

#include<iostream>
#include<string>
using namespace std ;

class BigReal{
    private :
        string number;
        bool positive ;
        long long decimalpoint ;
        char add(const char &a ,const char &b , bool &carry ) ;
        void removeRightzeros();
        bool isValidReal (string realNumber) ;
        int toint(char c);
    public :
        BigReal(string n);
        BigReal();
        BigReal operator+ ( BigReal& other) ;
        BigReal operator- ( BigReal& other);
        BigReal(const BigReal& other);
        friend ostream& operator << (ostream& os, const BigReal& num);
        BigReal& operator= (const BigReal &other);
        bool operator< (const BigReal &other);
        bool operator<= (const BigReal &other);
        bool operator> (const BigReal &other);
        bool operator>= (const BigReal &other);
        bool operator == (const BigReal &other);
        void setNum (string realNumber);
        int size();
        int sign();
        
};

#endif
