#ifndef USERENVIRONMENT_H
#define USERENVIRONMENT_H

#include <iostream>
#include <string>
#include <vector>
#include "User.h"

class userEnvironmemt {
    User us;
public:
    Email createEmail() {
        std::string id, snd, recv, sub, cont;
        int p;

        std::cout << "To: ";
        std::getline(std::cin, recv);

        std::cout << "Subject: ";
        std::getline(std::cin, sub);

        std::cout << "Content: ";
        std::getline(std::cin, cont);

        std::cout << "Priority: ";
        std::cin >> p;
        std::cin.ignore();

        return Email(id, snd, recv, sub, cont, p);
    }
};

#endif
