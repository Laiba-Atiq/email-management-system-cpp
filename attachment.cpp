#include "attachment.h"
#include <iostream>
using namespace std;

Attachmments::Attachmments(string n, string t, int size) {
    name = n;
    type = t;
    sizeInKb = size;
}

void Attachmments::display() {
    cout << "Name: " << name << ", Type: " << type << ", Size: " << sizeInKb << " KB\n";
}
