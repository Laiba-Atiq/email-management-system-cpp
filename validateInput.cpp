#include <iostream>
#include <string>
#include "colors.h"
using namespace std;
#include "validateInput.h"

char getCharacter() {
    string input;
    cin >> input;

    if (input.size() == 1 && isalpha(input[0]))  //only one english alphabet allowed
        return input[0];
    else {
        cout << RED << "Invalid input! Please enter a single alphabetic character.\n" << RESET;
        return getCharacter();
    }
}

int getInteger() {
    string input;
    cin >> input;

    bool isValid = true;

    //checking all characters are digits
    for (char c : input) {
        if (!isdigit(c)) {
            isValid = false;
            break;
        }
    }

    if (isValid && !input.empty())
        return stoi(input); //convert string to int
    else {
        cout << RED << "Invalid input!Please enter digits only\n" << RESET;
        return getInteger();
    }
}

float getFloat() {
    string input;
    cin >> input;

    bool isValid = true;
    int dotCount = 0;

    for (char c : input) {
        if (c == '.') dotCount++;
        else if (!isdigit(c)) {
            isValid = false;
            break;
        }
    }

    if (isValid && dotCount <= 1 && !input.empty())
        return stof(input);
    else {
        cout << RED << "Invalid input!Please enter a valid positive number.\n" << RESET;
        return getFloat();
    }
}

string getString() {

    string input;
    getline(cin, input);
    // bool valid = false;

    //for (char c : input) {
    //    if (!isalpha(c) && c != ' ') {
    //        cout << "Invalid input! Please enter a valid string.\n";
    //        return getString();
    //    }
    //}
    return input;
}
