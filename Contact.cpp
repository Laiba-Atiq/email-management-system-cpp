#include "Contact.h"
using namespace std;

Contact::Contact(string n, string e) {
    name = n;
    email = e;
}

string Contact::getName() { return name; }
string Contact::getEmail() { return email; }

ostream& operator<<(ostream& out, Contact& c) {
    out << "Name: " << c.name << "\n" << "Email: " << c.email << "\n";
    return out;
}

bool Contact::operator<(const Contact& other) const {
    if (name < other.name) return true;
    if (name > other.name) return false;
    return email < other.email;
}

bool Contact::operator>(const Contact& other) const {
    if (name > other.name) return true;
    if (name < other.name) return false;
    return email > other.email;
}

bool Contact::operator==(const Contact& other) const {
    return name == other.name && email == other.email;
}

bool Contact::hasSameName(const Contact& other) const {
    return name == other.name;
}

bool Contact::hasSameEmail(const Contact& other) const {
    return email == other.email;
}
