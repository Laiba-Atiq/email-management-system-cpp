#ifndef CONTACT_H
#define CONTACT_H

#include <iostream>
#include <string>

class Contact {
    std::string name;
    std::string email;

public:
    Contact(std::string n = "", std::string e = "");
    std::string getName();
    std::string getEmail();

    friend std::ostream& operator<<(std::ostream& out, Contact& c);
    bool operator<(const Contact& other) const;
    bool operator>(const Contact& other) const;
    bool operator==(const Contact& other) const;
    bool hasSameName(const Contact& other) const;
    bool hasSameEmail(const Contact& other) const;
};

#endif
