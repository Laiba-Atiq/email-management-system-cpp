#include <iostream>
#include<string>
#include <ctime>
#include<cstdlib>
#include <limits>
#include "EM.h"
#include "colors.h"
#include "validateInput.h"

using namespace std;

void screen1_1(EMS* es);
void screen1_2(EMS* es);
void screen1_3(EMS* es);
void screen1_4();
void screen2(EMS* es, User us);

void Screen1(EMS* es) {
    int choice;

    cout << CYAN;
    cout << "-----------------------------------------------------------------------------------------------------------------------\n";
    cout << "                                             EMAIL MANAGEMENT SYSTEM\n";
    cout << "-----------------------------------------------------------------------------------------------------------------------";
    cout << RESET;

    do {
        cout << "\n\n1. Create Account\n"
            << "2. Login\n"
            << "3. Forgot Password\n"
            << "4. Settings\n"
            << "5. Admin Menu\n"
            << "6. Graph Contacts\n"
            << "7. View Social Graph\n"
            << "8. Exit\n\n";

        cout << GREEN << "Enter Choice: " << RESET;
        choice = getInteger();

        if (choice == 1) {
            screen1_1(es);
        }
        else if (choice == 2) {
            screen1_2(es);
        }
        else if (choice == 3) {
            screen1_3(es);
        }
        else if (choice == 4) {
            es->Settings();
        }
        else if (choice == 5) {
            es->Admin();
        }
        else if (choice == 6) {
            es->contactOptions();
        }
        else if (choice == 7) {
            es->displaySocialGraph();
        }
    } while (choice != 8); 

        screen1_4();    
}

void screen1_1(EMS* es) {

    int choice = 0;
    cout << YELLOW;
    cout << "\n----------- Create New Account -----------\n";
    cout << RESET;

    User usr = es->addUser();

    do {
        cout << "1. Save\n";
        cout << "2. Back\n\n";

        cout << GREEN << "Enter Choice: " << RESET;
        choice = getInteger();



        if (choice == 1) {
            es->saveUser(usr);
            screen2(es, usr);
        }
    } while (choice != 2);
    Screen1(es);
    
}

void screen1_2(EMS* es) {
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int choice;
    string email;
    string password;
    User usr;

    cout << YELLOW << "\n----------------------- Login -----------------------\n" << RESET;

    cout << "Enter Email: " << RESET;
    email = getString();

    cout <<  "Enter Password: " << RESET;
    password = getString();


    
    do {
        cout << "\n1. Login\n";
        cout << "2. Back\n\n";

        cout << GREEN << "Enter Choice: " << RESET;
        choice = getInteger();


        if (choice == 1) {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            bool exists = es->login(email, password, usr);
            while (!exists) {
                cout << RED << "\nIncorrect email or password.\n" << RESET;

                cout << GREEN << "Enter email: " << RESET;
                email = getString();
                cout << GREEN << "Enter Password: " << RESET;
                password = getString();
                exists = es->login(email, password, usr);
            }
            choice = 2;
        }
    } while (choice != 2);
    screen2(es, usr);

}

void screen1_3(EMS* es) {   
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int choice;
    string email;

    cout << YELLOW << "\n----------- Password Recovery -----------\n" << RESET;

    cout << GREEN << "Enter Registered Email: " << RESET;
    email = getString();
    string ans = es->recoverPassword(email);

    while (ans == "") {
        cout << RED << "Incorrect. Enter Registered Email: " << RESET;
         email = getString();
        ans = es->recoverPassword(email);
    }

    cout << "The recovered password is: " << ans << "\n";
    cout << "1. Back\n";
    cout << GREEN << "Enter Choice: " << RESET;
    choice = getInteger();
}

void screen1_4() {
    cout << RED;
    cout << "-----------------------------------------------------------------------------------------------------------------------\n";
    cout << "                                                 EXITING \n";
    cout << "-----------------------------------------------------------------------------------------------------------------------";
    cout << RESET;
}

void screen2(EMS* es, User usr) {
    int choice = 0;
    cout << CYAN;
    cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
    cout << "                                              WELCOME "  << usr.getUsername();
    cout << RESET;

     do {
         cout << "\n\n1. Compose Email\n";
         cout << "2. Folder Management\n";
         cout << "3. Search & Filter\n";
         cout << "4. Logout\n";
         cout << "5. Back\n";
         cout << GREEN << "\nEnter choice: " << RESET;
         choice = getInteger();
         if (choice == 1) {
             es->composeEmail(usr);
         }
         else if (choice == 2) {
             es->FolderManagement(usr);
         }
         else if (choice == 3) {
             es->searchF(usr);
         }
         else if (choice == 4) {
             cout << RED << "\t\t\t\t\t\tLOGGING OUT\n" << RESET;
             break;
         }
     } while (choice != 5);

}

int main() {
    EMS* es = new EMS();
    Screen1(es);
    return 0;
}
