#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include <string>

class Attachmments {
    std::string name;
    std::string type;
    int sizeInKb;

public:
    Attachmments(std::string n, std::string t, int size);
    void display();
};

#endif
