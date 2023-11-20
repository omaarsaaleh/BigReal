#include<iostream>
#include<cassert>
#include"BigReal.cpp"
using namespace std ;

void testComparisonOperators() {
    BigReal num1("123.456");
    BigReal num2("76.544");
    BigReal num3("123.456");

    assert(num2 < num1);
    assert(num1 > num2);
    assert(num1 == num3);
    assert(num1 >= num3);
    assert(num1 <= num3);
    assert(num1 >= num2);
    assert(num2 <= num1);

    // Negative numbers
    BigReal negNum1("-123.456");
    BigReal negNum2("-76.544");
    assert(negNum1 < negNum2);
    assert(negNum2 > negNum1);
    assert(negNum1 <= negNum2);
    assert(negNum2 >= negNum1);

    // Mixed positive and negative numbers
    assert(negNum1 < num1);
    assert(num1 > negNum1);
    assert(negNum1 <= num1);
    assert(num1 >= negNum1);

    // Zero comparison
    BigReal zero("0.0");
    assert(zero < num1);
    assert(num1 > zero);
    assert(zero == BigReal("0.0"));
    assert(zero <= num1);
    assert(num1 >= zero);

    // Harder cases
    // Very large numbers
    BigReal veryLarge1("12345678901234567890.123456789");
    BigReal veryLarge2("9876543210987654321.987654321");
    assert(veryLarge1 > veryLarge2);
    assert(veryLarge2 < veryLarge1);
    assert(veryLarge1 >= veryLarge2);
    assert(veryLarge2 <= veryLarge1);

    // Very small numbers
    BigReal verySmall1("0.000000000123456789");
    BigReal verySmall2("0.000000000987654321");
    assert(verySmall1 < verySmall2);
    assert(verySmall2 > verySmall1);
    assert(verySmall1 <= verySmall2);
    assert(verySmall2 >= verySmall1);

    // Mixed large and small numbers
    assert(veryLarge1 > verySmall1);
    assert(verySmall1 < veryLarge1);
    assert(veryLarge1 >= verySmall1);
    assert(verySmall1 <= veryLarge1);

    // Mixed positive and negative large numbers
    BigReal negVeryLarge("-12345678901234567890.123456789");
    assert(veryLarge1 > negVeryLarge);
    assert(negVeryLarge < veryLarge1);
    assert(veryLarge1 >= negVeryLarge);
    assert(negVeryLarge <= veryLarge1);

    // Equal large numbers
    BigReal equalLarge1("12345678901234567890.123456789");
    BigReal equalLarge2("12345678901234567890.123456789");
    assert(equalLarge1 == equalLarge2);
    assert(equalLarge1 >= equalLarge2);
    assert(equalLarge1 <= equalLarge2);
}


void testAddition() {
    // Simple positive numbers
    BigReal num1("123.456");
    BigReal num2("76.544");
    assert(num1 + num2 == BigReal("200.000"));

    // Large numbers
    BigReal large1("999999999999.999");
    BigReal large2("0.001");
    assert(large1 + large2 == BigReal("1000000000000.000"));

    // Positive and negative numbers
    BigReal negNum("-50.25");
    assert(num1 + negNum == BigReal("73.206"));

    // Negative numbers
    BigReal negNum1("-123.456");
    BigReal negNum2("-76.544");   
    assert(negNum1 + negNum2 == BigReal("-200.000"));

    // Adding zeros
    BigReal zero("0.0");
    assert(num1 + zero == BigReal("123.456"));
}

void testSubtraction() {
    // Simple positive numbers
    BigReal num1("123.456");
    BigReal num2("76.544");
    assert(num1 - num2 == BigReal("46.912"));

    // Large numbers
    BigReal large1("1000000000000.000");
    BigReal large2("999999999999.999");
    assert(large1 - large2 == BigReal("0.001"));

    // Positive and negative numbers
    BigReal negNum("-50.25");
    assert(num1 - negNum == BigReal("173.706"));

    // Negative numbers
    BigReal negNum1("-123.456");
    BigReal negNum2("-76.544");
    assert(negNum1 - negNum2 == BigReal("-46.912"));

    // Subtracting zeros
    BigReal zero("0.0");
    assert(num1 - zero == BigReal("123.456"));
    
    // Zero minus number
    assert(zero - num1 == BigReal("-123.456"));

    // Harder cases
    // Very large numbers
    BigReal veryLarge1("12345678901234567890.123456789");
    BigReal veryLarge2("9876543210987654321.987654321");
    assert(veryLarge1 - veryLarge2 == BigReal("2469135690246913568.135802468"));

    // Very small numbers
    BigReal verySmall1("0.000000000123456789");
    BigReal verySmall2("0.000000000987654321");
    assert(verySmall1 - verySmall2 == BigReal("-0.000000000864197532"));

    // Mixed large and small numbers
    assert(veryLarge1 - verySmall1 == BigReal("12345678901234567890.123456788876543211"));

    // Mixed positive and negative large numbers
    BigReal negVeryLarge("-12345678901234567890.123456789");
    assert(veryLarge1 - negVeryLarge == BigReal("24691357802469135780.246913578"));
}

int main() {
    
    testComparisonOperators();
    testAddition();
    testSubtraction();

    cout << "\033[32mAll tests passed!\033[0m" << '\n';

    
    return 0;
}
