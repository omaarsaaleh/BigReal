#include "BigReal.hpp"

using namespace std ;


// set function

void BigReal::setNum (string realNumber) {
    if (!BigReal::isValidReal(realNumber)) {
        throw std::runtime_error("Invalid real number: " + (realNumber));
    }
        
    this->number=realNumber;

    // remove the -ve sign to facilate operations and mark sign as 0 for -ve
    if (this->number[0]=='-'){
        this->positive=0;
        this->number.erase(0, 1);
    }
    // remove the +ve sign to facilate operations and mark sign as 1 for +ve
    else if (this->number[0]=='+'){
        this->positive = 1 ;
        this->number.erase(0, 1);
    }
    
    // mark sign as 1 for +ve if no sign was provided
    else{
        this->positive = 1 ;
    }
    
    // to remove left zeros  ---> 0001 = 1
    if(this->number[0]=='0'){ 
        int i=0;
        while(this->number[i]=='0'){
            i++;
        }
        this->number.erase(0,i);
        
        // in case the number is a zero
        if(this->number.empty() ){this->number="0"; this->positive=1 ;}
    }
    
    // in case the user entered in .1 form make it 0.1 to facilate operations
    if(number[0]=='.'){number="0"+ number ; }

    // save the position of the decimal point if exist (will be used in operations)
    int a = number.find('.');
    if(a!=-1 ){
        decimalpoint=a ;
        removeRightzeros(); // remove right zeros after the decimal point if exist
    }
    
    else{decimalpoint=0;}
}

//----------------------------------------------------------------------------------------------------------------------------------------------

bool BigReal::isValidReal (string realNumber){
    int i=0;
    if(realNumber[i]=='-' || realNumber[i]=='+'){i++;}    
    int numofDecimalpoints=0;
    
    for(; i<realNumber.size(); i++){
        
        //making sure there is nothing but numbers and 1 decimal point
        if( (realNumber[i] < '0' || realNumber[i] > '9') && realNumber[i]!='.'){
            return false;
        }
        
        //making sure that at most there is 1 decimal point
        if(realNumber[i]=='.'){ 
            numofDecimalpoints++;
        }
        if(numofDecimalpoints>1){
            return false;
        }
    }

    return true ;
}

//----------------------------------------------------------------------------------------------------------------------------------------------

// constructors

BigReal::BigReal(){
    this->number="0";
    this->positive = 1;
    this->decimalpoint=0;
}


BigReal::BigReal(string n){
    setNum(n);
}

BigReal::BigReal(const BigReal& other) {
    number = other.number;
    positive = other.positive;
    decimalpoint = other.decimalpoint;
}

//----------------------------------------------------------------------------------------------------------------------------------------------

ostream& operator<<(std::ostream& os, const BigReal& num) {
    if(num.positive){os << num.number;}
    else{os << "-" + num.number ;}
    return os;
}



//----------------------------------------------------------------------------------------------------------------------------------------------

void BigReal::removeRightzeros(){ // to remove right zeros (after decimal point)
    if(!decimalpoint){return;}
    
    int i = number.size()-1 ;
    while(number[i]=='0' && decimalpoint<i){
        i--;
    }
    number.erase(i + 1);
    if(number[i]=='.'){number.erase(i); decimalpoint=0;} // remove the decimal point if no decimal numbers 
    return ; 
}
//----------------------------------------------------------------------------------------------------------------------------------------------

char BigReal::add(const char &a ,const char &b , bool &carry ){
    int sum = (a -'0') + (b -'0'); // convert the numbers into integers and add them
    
    if (carry) {
        sum++; // Add the carry from the previous addition
        carry = false; // Reset carry flag
    }

    if(sum>9){
        sum-=10; 
        carry=true; // set the carry if exist for the next addition
    }
    
    return char(sum + '0'); // return the result
}
//----------------------------------------------------------------------------------------------------------------------------------------------

BigReal BigReal::operator+ ( BigReal& other)  {
    // handelling different sign cases
    
    bool neg = false ;
    if(!this->sign() && !other.sign()){ // -ve1 + -ve2 ---> -(+ve1 + +ve2)
        neg=true ;
    }
    else if(this->sign() && !other.sign()){ // +ve1 + -ve2 ---> (+ve1 - +ve2)
        BigReal temp (other.number);
        return this->operator-(temp);
    }
    else if(!this->sign() && other.sign()){ // -ve1 + +ve2 ---> (+ve2 - +ve1)
        BigReal temp (this->number);
        return other.operator-(temp);
    }

    string result = "" ;
    string n1 = this->number , n2 = other.number ;
    int dpoint1 , dpoint2 ;
    
    if(!this->decimalpoint && other.decimalpoint){ // if no decimal point add it to facilate operations
        n1+= ".0"; 
        dpoint1 = n1.size()-2; dpoint2 = other.decimalpoint ;
    } 
    else if(this->decimalpoint && !other.decimalpoint){
        n2+=".0";
        dpoint1 = this->decimalpoint ; dpoint2 = n2.size()-2; 
    }
    else{
        dpoint1 = this->decimalpoint ; dpoint2 = other.decimalpoint ;
    } 
    bool carry = false;

    // Initialize indices for iterating through both numbers
    int i = n1.length()-1 ; // / Start from the end to iterate reversely
    int j = n2.length()-1 ; 
    
    // Calculate the difference in the number of decimal places.
    int d1 = (n1.size() - dpoint1) - (n2.size()-dpoint2) ; // getting the number of the digits after the decimal point 
    int d2 = (n2.size()-dpoint2) - (n1.size() - dpoint1) ; // to add the difference first in case one is taller than other 
                                                                                                     //    1.345 +
    while(this->decimalpoint && d1>0){                                                               //    1.45
        result = add( n1[i] ,'0',carry) + result;                                           //       (5)
        d1--; i-- ;
    }
    while(other.decimalpoint && d2>0){
        result = add( n2[j] ,'0',carry) + result ;
        d2--; j--;
    }

    

    while( i >=0 || j >=0 ){ // iterating until both numbers end


        if( i>=0 && j>=0 ){ // if both did not end yet add them
            result = add(n1[i] , n2[j] , carry) + result ;
        }
        else if(i>=0){ // if only one of them add it with 0 
            result = add( n1[i],'0',carry) + result;
            
        }
        else if(j>=0){
            result = add( n2[j],'0',carry) + result ;
            
        }

        i--; j--; 
        
        // Move to the next digit, and if there's a decimal point, add it to the result.
        if(n1[i]=='.'){ 
            i--; j--;
            result = "." + result ;
        }

    }
    
    // Add the final carry if exist
    if (carry) {
        result = '1' + result ; 
    }
    
    BigReal Result(result);
    if(neg) Result.positive=0 ;
    
    return Result;
   
}

//----------------------------------------------------------------------------------------------------------------------------------------------

int BigReal::toint(char c){
    return c-'0';
}

//----------------------------------------------------------------------------------------------------------------------------------------------

BigReal BigReal:: operator - (BigReal& other){
    // handelling different sign cases

    if(this->sign() && !other.sign()){ // +ve1 - -ve2  ---> (+ve1 + +ve2)
        BigReal temp (other.number);
        return this->operator+(temp);
    }
    else if(!this->sign() && other.sign()){ // -ve1 - +ve2 ---> -(+ve1 + +ve2)

        string n = "-" + other.number ;
        BigReal temp (n);
        return this->operator+(temp);
    }
    else if(!this->sign() && !other.sign()){ //  -ve1 - -ve2 ---> (pos2 - pos1)
        BigReal temp (this->number);
        BigReal temp2 (other.number);
        return temp2-temp;
    }
    else if(this->number==other.number){
        BigReal Result("0");
        return Result;
    }
    
    string result = "" ;
    string bigger , smaller ; int bdec , sdec ;
    bool neg = false ;
    
    // subtract the bigger number from the smaller, put negative after finishing if needed (smaller-bigger)
    if(this->operator<(other)){
        bigger = other.number ; bdec = other.decimalpoint ;
        smaller = this->number ; sdec = this->decimalpoint ;
        neg = true ;
    }
    else{
        bigger = this->number ; bdec = this->decimalpoint ;
        smaller = other.number; sdec = other.decimalpoint ;
    }
    
    // Calculate the difference in the number of decimal places.
    int d1 = (!bdec)? 0 : bigger.size()-bdec-1 ;
    int d2 = (!sdec)? 0 : smaller.size()-sdec-1 ;
    
    // Initialize indices for iterating through both numbers
    int i = bigger.size()-1 ; //  Start from the end to iterate reversely
    int j = smaller.size()-1 ;

    if(d2>d1){ // if other has more digits after the decimal point, pad s1 with zeros to be equal
        if(!d1){
            bigger.push_back('.');
        }
        
        bigger += std::string(d2-d1, '0');
        i = bigger.size()-1;
    }
    
    else if (d1>d2){ // if this has more digits after the decimal point put it in the result (n-0)=n
        int extra = d1-d2 ;
        while(extra>0){ 
            result = bigger[i] + result ;
            i--; extra -- ;
        }
        if(!d2){result = '.' + result; i-- ;} // if other has no decimal point then the digits after the decimal point is done
    }

    while(j>=0){
        bool borrowed = false ;
        int previous ; char c ;    
        
        if(bigger[i]<smaller[j]){ // if we need to borrow
            borrowed = true ;
            
            c = (toint(bigger[i])+10-toint(smaller[j])) + '0';
            result =  c + result ;
            
            previous = i-1 ;
            
            while(borrowed){ // borrowing 
                if(bigger[previous]=='.'){previous--;}
                if(bigger[previous]>'0'){
                    bigger[previous]-='1'-'0';
                    borrowed=false ;
                }
                else{
                    bigger[previous]= '9';
                }
                previous-- ;
            }

        }
        else{ // normal subtraction
            c = bigger[i] - smaller[j] + '0';
            result = c + result ;
        }

        i--; j--;
        if(bigger[i]=='.'){result = '.' + result ; i--; j--;}
    }    

    while(i>=0){ // add the rest of numbers in the bigger one if exist              
            result = bigger[i] + result ;     
            i--;                                 
    }

    BigReal Result(result);
    if(neg) Result.positive=0;
    
    return Result;
}


//----------------------------------------------------------------------------------------------------------------------------------------------


BigReal& BigReal::operator=(const BigReal& other) {
    
    if (this == &other) {  // Handle self-assignment
        return *this;
    }
    // Perform the assignment
    
    this->number = other.number;
    this->positive = other.positive;
    this->decimalpoint = other.decimalpoint;
    
    return *this;
}

//----------------------------------------------------------------------------------------------------------------------------------------------

//comparing operators

bool BigReal::operator == (const BigReal &other){
    return (this->positive==other.positive) && this->number==other.number ;
}

bool BigReal::operator > (const BigReal &other){
    if(this->operator==(other)){
        return false ;
    }
    
    else if(this->positive > other.positive){ // if +ve and other is -ve then surely greater
        return true ;
    }
    
    else if(this->positive < other.positive){ // if -ve and other is +ve then surely smaller
        return false ;
    }
    
    else{ // if both have the same sign
        
        //The number of the whole digits in each number (number of numbers before the decimal point if exist else the whole integer). (954).08
        long long nums1 = (this->decimalpoint)? this->decimalpoint : this->number.size();
        long long nums2 = (other.decimalpoint)? other.decimalpoint : other.number.size();
        
        if(nums1>nums2){ // if this has more digits before the decimal point than surely bigger if +ve or smaller if -ve 
            return this->positive ; // true if the sign is positive - false if ther sign is negative
        }
        else if(nums1<nums2){
            return !this->positive ;
        }
        else{ 
            // if they are of the same number of numbers 
            //comparing digit by digit
            // the comparison is done based on the lexicographical (dictionary) order. 
            //This means that the ASCII values of the characters in the strings are compared character by character from left to right
            
            for(int i=0 ; i<nums1 ; i++){
                if(this->number[i]>other.number[i]){
                    return this->positive ;
                }
                else if(this->number[i]<other.number[i]){
                    return !this-> positive ;
                }
            }
            
            // if equal we will comapare the decimal digit by digit if exist
            
            for(int i=nums1+1 ; i<this->number.size() || i<other.number.size() ; ){
                if(this->number[i]>other.number[i]){
                    return this-> positive ;
                }
                else if(this->number[i]<other.number[i]){
                    return !this->positive ;
                }
                i++;
                
                // if one ends before the other and all the previous digits are equal then the one ends is the smaller one and vice versa
                
                if(i<this->number.size() && i>=other.number.size()){
                    return this->positive ;
                }
                else if(i>=this->number.size() && i<other.number.size()){
                    return !this->positive ;
                }
            }
            return false ; 

            
        }
        
    }
    
}

bool BigReal::operator< (const BigReal &other) {
    return !this->operator>(other) && !this->operator==(other);
}

bool BigReal::operator<= (const BigReal &other) {
    return !this->operator>(other);
}

bool BigReal::operator>= (const BigReal &other) {
    return !this->operator<(other);
}

//----------------------------------------------------------------------------------------------------------------------------------------------

int BigReal::sign(){
    return this->positive;
}

//----------------------------------------------------------------------------------------------------------------------------------------------


int BigReal::size(){
    return (decimalpoint)? this->number.size()-1 :this->number.size();
}

//----------------------------------------------------------------------------------------------------------------------------------------------

