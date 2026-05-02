#ifndef EM_H
#define EM_H

#include <iostream>
#include <vector>
#include<string>
#include <ctime>
#include <fstream>
#include <limits>
#include "BinaryTree.h"
#include "User.h"
#include "Contact.h"
#include "colors.h"
#include "SocialGraph.h"
#include "validateInput.h"

using namespace std;

class EMS {

    BST<User> userBST;
    Contact lastContacted[10];
    std::string settingsArr[2];
    SocialGraph sg;
    int EmailCount;
    int UserCount;

    void loadLastUserId() {
        std::ifstream userFile("users.txt");
        if (!userFile.is_open()) {
            UserCount = 1;
            return;
        }
        std::string line, lastLine = "";
        while (std::getline(userFile, line)) {
            if (!line.empty()) lastLine = line;
        }
        userFile.close();
        if (lastLine.empty()) { UserCount = 1; return; }
        std::string lastId = "";
        for (int i = 0; i < (int)lastLine.size(); i++) {
            if (lastLine[i] == ',') break;
            lastId += lastLine[i];
        }
        try { UserCount = std::stoi(lastId) + 1; }
        catch (...) { UserCount = 1; }
    }

    void loadLastEmailId() {
        std::ifstream emailFile("email.txt");

        if (!emailFile.is_open()) {
            //File doesn't exist yet
            EmailCount = 0;
            return;
        }

        std::string line;
        std::string lastLine = "";

        while (std::getline(emailFile, line)) {
            if (!line.empty()) {
                lastLine = line;
            }
        }

        emailFile.close();

        if (lastLine.empty()) {
            EmailCount = 0;
            return;
        }
        std::string lastId = "";
        for (int i = 0; i < lastLine.size(); i++) {
            if (lastLine[i] == ',') {
                break;
            }
            lastId += lastLine[i];
        }

       
        EmailCount = stoi(lastId) + 1;
       
    }

public:
    std::string* spamWords;
    int spamWordsCount;

    EMS() {
        spamArrayFormation();
        loadUsersFromFile(); 
        loadSettings();
        sg.loadFromFile();
        loadLastEmailId();
        loadLastUserId();
    }

    void loadSettings() {
        settingsArr[0] =
            "Emails are prioritized based on the value (priority - spamScore). "
            "An email with a higher (priority - spamScore) value is treated as more important "
            "and appears earlier in the priority list.\n";

        settingsArr[1] =
            "The spam score of an email increases by 1 for every spam word detected in its content. "
            "Emails with a spam score of 1 or more are automatically marked as spam. "
            "The more spam words an email contains, the higher its spamScore becomes, "
            "which reduces its overall priority.\n";
    }

    void loadUsersFromFile() {
        std::ifstream userFile("users.txt");

        if (!userFile.is_open()) {
            std::cout << RED << "Unale to open users file.\n" << RESET;
            return;
        }

        std::string line;
        while (std::getline(userFile, line)) {
            
            std::vector<std::string> tokens;
            std::string token = "";

            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ',') {
                    tokens.push_back(token);
                    token = "";
                }
                else {
                    token += line[i];
                }
            }
            tokens.push_back(token);

            if (tokens.size() == 6) {
                std::string userId = tokens[0];
                std::string username = tokens[1];
                std::string email = tokens[2];
                std::string password = tokens[3];
                std::string createdDate = tokens[4];
                std::string lastLogin = tokens[5];

                User usr(userId, username, email, password, createdDate, lastLogin);
                userBST.Insert(usr);
            }
            else {
                std::cerr << RED << "Invalid line format in users.txt: " << line << "\n" << RESET;
            }
        }

        userFile.close();
        std::cout << "Users loaded \n";
    }

    bool login(std::string e, std::string p, User& us) {
        User temp("", "", e, "", "", "");
        std::vector<User> ans = userBST.Search(temp, "email");

        if (ans.empty()) {
            return false;
        }

        for (auto i = 0; i < ans.size(); i++) {
            if (ans[i].getPassword() != p) return false;
        }

        us = ans[0];

        time_t now = time(0);
        struct tm tstruct;
        localtime_s(&tstruct, &now);
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
        std::string currentDate = buf;

        us.setLastLogin(currentDate);

        userBST.Update(us, "email");

        updateUserInFile(us);


        return true;
    }

    std::string recoverPassword(std::string e) {

        User temp("", "", e, "", "", "");
        std::vector<User> ans = userBST.Search(temp, "email");

        if (ans.size() == 0) {
            return std::string();
        }
        else {
            return ans[0].getPassword();
        }
    }

    void composeEmail(User& usr) {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
        std::cout << YELLOW << "\n----------------------- Email Composition -----------------------" << RESET;
        std::string id = "", snd = "", recv = "", sub = "", cont = "", recvName = "";
        int p = 0;
        id = to_string(EmailCount);
        EmailCount++;

        std::cout << "\nEmail ID (auto-generated): " << id << "\n";
        std::cout << "Priority: ";
        p = getInteger();
        cin.ignore();
        std::cout << "Reciever: ";
        recv = getString();
        std::cout << "Name of Reciever: ";
        recvName = getString();
        std::cout << "Sunbject: ";
        sub = getString();
        std::cout << "content: ";
        cont = getString();

        Email e(id, usr.getUsername(), recv, sub, cont, p, recvName);

        int choice = 0;
        
        do {
            std::cout << "\n\n1. Send\n";
            std::cout << "2. Save as Draft\n";
            std::cout << "3. Schedule (Queued)\n";
            std::cout << "4. Add Attachment\n";
            std::cout << "5. Back\n";
            std::cout << GREEN << "\nEnter choice: " <<RESET;
            choice = getInteger();

            if (choice == 1) {
                usr.send(e, spamWords, spamWordsCount, sg);

                User reciever("", "", recv, "", "", "");
                std::vector<User> recieverOfSender = userBST.Search(reciever, "email");

                if (recieverOfSender.size() > 0) {
                    recieverOfSender[0].moveToInbox(e, spamWords, spamWordsCount);
                }

                Contact c(recvName, recv);
                usr.addContact(c);
                choice = 5;
            }

            else if (choice == 2) {
                usr.moveToDraft(e, spamWords, spamWordsCount);
                choice = 5;
            }
            else if (choice == 3) {
                usr.scheduleMail(e);
                choice = 5;
            }
            else if (choice == 4) {
                e.addAttachment();
            }
        } while (choice != 5);

            return;
    }

    void FolderManagement(User& usr) {
        int choice = 0;

        std::cout << YELLOW << "\n----------------------- Folder Management -----------------------" << RESET;

        do{
        std::cout << "\n1. Inbox\n";
        std::cout << "2. Sent\n";
        std::cout << "3. Drafts\n";
        std::cout << "4. Spam\n";
        std::cout << "5. Trash\n";
        std::cout << "6. Important\n";
        std::cout << "7. Create New Folder\n";
        std::cout << "8. Empty Trash\n";
        std::cout << "9. Backup\n";
        std::cout << "10. Back\n";

        std::cout << GREEN << "\nEnter choice: " << RESET;
        choice = getInteger();

        if (choice == 1) {
            usr.folderOperations("Inbox", sg);
        }
        else if (choice == 2) {
            usr.folderOperations("Sent", sg);
        }
        else if (choice == 3) {
            usr.folderOperations("Drafts", sg);
        }
        else if (choice == 4) {
            usr.folderOperations("Spam", sg);
        }
        else if (choice == 5) {
            usr.folderOperations("Trash", sg);
        }
        else if (choice == 6) {
            usr.folderOperations("Import", sg);
        }
        else if (choice == 7) {
            usr.createCustomFolder();
        }
        else if (choice == 8) {
            usr.clearTrash();
        }
        else if (choice == 9) {
            usr.BackupOperations();
        }
        } while (choice != 10);

    }

    void searchF(User& usr) {
        int choice = 0;
       
        do {
            std::cout << YELLOW << "\n----------------------- Search -----------------------" << RESET;

            std::cout << "\n1. Quick Search (Contacts BST)\n";
            std::cout << "2. View Recent Contacts:\n";
            std::cout << "3. Back\n";
            std::cout << GREEN << "\nEnter choice: " << RESET;
            choice = getInteger();

            if (choice == 1) {
                //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string query, parameter;
                int opt;

                std::cout << "Search By:\n";
                std::cout << "1. Name\n";
                std::cout << "2. Email\n";
                std::cout << GREEN << "\nEnter option: " << RESET;
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                opt = getInteger();

                if (opt == 1) {
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    parameter = "name";
                    std::cout << "Enter name to search: ";
                    query = getString();
                }
                else if (opt == 2) {
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    parameter = "email";
                    std::cout << "Enter email to search: ";
                    query = getString();
                }
                else {
                    std::cout <<RED << "Invalid option!\n" <<RESET;
                    return;
                }

                try {
                    std::vector<Contact> result = usr.searchContacts(query, parameter);

                    std::cout << "\nResults Found:\n";
                    for (auto& c : result) {
                        std::cout << c << "\n";
                    }
                }
                catch (const runtime_error& e) {
                    std::cout << RED << e.what() << "\n" << RESET;
                }
                }
            else if (choice == 2) {
                usr.viewRecentContacts();
            }

        } while (choice != 3);

    }

    bool validatePassword(std::string p) {
        if (p.length() < 8) return false;

        bool upper = 0, lower = 0, digit = 0, special = 0;

        for (char c : p) {
            if (isupper(c)) upper = true;
            else if (islower(c)) lower = true;
            else if (isdigit(c)) digit = true;
            else special = true;
        }

        return upper && lower && digit && special;
    }

    bool validateEmail(std::string e) {
        int atIndex = -1, dotIndex = -1;

        if (e.size() < 5) return false;

        for (int i = 0; i < e.size(); i++) {
            if (e[i] == '@') {
                if (atIndex != -1 || i == 0) return false;
                atIndex = i;
            }
            else if (e[i] == '.') {
                dotIndex = i;
                if (i == 0 || dotIndex == atIndex - 1 || dotIndex == atIndex + 1 || (i < e.size() - 1 && e[i + 1] == '.'))  return false;
            }
        }
        if (atIndex == -1) return false;

        if (dotIndex + 2 >= e.size()) return false;

        if (dotIndex < atIndex) return false;

        return true;
    }

    void takeUserInfo(std::string& username, std::string& e, std::string& p, std::string& cd) {

        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //clean buffer once

        std::cout << "Enter username: ";
        username = getString();

        do {
            std::cout << "Enter a valid email: ";
            e = getString();
        } while (!validateEmail(e));

        User temp("", "", e, "");
        std::vector<User> ans = userBST.Search(temp, "email");

        while (!ans.empty()) {
            std::cout << RED << "This email id already exists.\n" << RESET;

            do {
                std::cout << "Enter a valid email: ";
                e = getString();
            } while (!validateEmail(e));

            User temp1("", "", e, "");
            ans = userBST.Search(temp1, "email");
        }

        do {
            std::cout << "Enter a valid password: ";
            p= getString();
        } while (!validatePassword(p));

        // current time
        time_t now = time(0);
        struct tm tstruct;
        localtime_s(&tstruct, &now);
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
        cd = buf;
    }

    User addUser() {
        std::string username, e, p, cd;
        takeUserInfo(username, e, p, cd);
        std::string id = std::to_string(UserCount);
        UserCount++;
        std::cout << "Account created. Your User ID is: " << id << "\n";
        User usr(id, username, e, p, cd, cd);
        userBST.Insert(usr);
        return usr;
    }

    void saveUser(User usr) {
        std::ofstream userFile("users.txt", std::ios::app);

        if (!userFile.is_open()) {
            std::cerr << RED << "Error opening users file to save data" << std::endl << RESET;
            return;
        }
        std::string id = usr.getUserId();
        std::string name = usr.getUsername();
        std::string e = usr.getEmail();
        std::string p = usr.getPassword();
        std::string cd = usr.getCreatedDate();
        std::string ld = usr.getLastLoginDate();
        userFile << id << "," << name << "," << e << "," << p << "," << cd << "," << ld << std::endl;

        userFile.close();
    }

    void spamArrayFormation() {
        std::ifstream spamFile("spam_words.txt");
        if (!spamFile.is_open()) {
            std::cerr << RED << "Error opening the spam file to save data" << std::endl << RESET;
            return;
        }

        std::string line;
        std::getline(spamFile, line);
        spamFile.close();

        int count = 1;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                count++;
            }
        }
        spamWordsCount = count;
        spamWords = new std::string[count];
        int startIndex = 0;

        int k = 0; std::string temp;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                temp = line.substr(startIndex, i - startIndex);
                spamWords[k++] = temp;
                startIndex = i + 1;
            }
        }
        temp = line.substr(startIndex, line.size() - startIndex);
        spamWords[k++] = temp;
    }

    void displaySpamWords() {
        for (int i = 0; i < spamWordsCount; i++) {
            std::cout <<"    " << i + 1 << ". " << spamWords[i] << "\n";
        }
        std::cout << "\n";
    }

    void Settings() {
        int choice = 0;
        std::cout << YELLOW << "\n----------------------- Settings -----------------------" << RESET;

        do {

            std::cout << "\n1. Spam Filter Settings\n";
            std::cout << "2. Priority Rules\n";
            std::cout << "3. Back\n";
            std::cout << GREEN << "\nEnter option: " << RESET;
            choice = getInteger();

            if (choice == 2) {
                std::cout << settingsArr[0] << std::endl;
}
            else if (choice == 1) {
                std::cout << settingsArr[1] << std::endl;
            }
        } while (choice != 3);
    }

    void Admin() {
        int choice = 0;
        std::cout << YELLOW << "\n----------------------- Admin Pannel -----------------------" << RESET;

        int totalEmails = 0;
        int spamEmails = 0;
        int readEmails = 0;
        int unreadEmails = 0;
        int scheduledEmails = 0;

        std::ifstream emailFile("email.txt");
        if (emailFile.is_open()) {
            std::string line;
            while (std::getline(emailFile, line)) {
                totalEmails++;

                std::vector<std::string> tokens;
                std::string token = "";
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] == ',') {
                        tokens.push_back(token);
                        token = "";
                    }
                    else {
                        token += line[i];
                    }
                }
                tokens.push_back(token);

                if (tokens.size() >= 10) {
                    if (tokens[9] == "Spam") spamEmails++; //isSpam
                    if (tokens[6] == "true") readEmails++; //isRead
                    else unreadEmails++;
                    if (tokens[9] == "Scheduled") scheduledEmails++;//folder
                }
            }
            emailFile.close();
        }
        else {
            std::cout << RED << "Error opening teh emails file\n" << RESET;
        }


        do {
            std::cout << "\n1. System Statistics\n";
            std::cout << "2. User Management\n";
            std::cout << "3. Spam Analysis\n";
            std::cout << "4. Back\n";
            std::cout << GREEN << "\nEnter option: " << RESET;
            choice = getInteger();

            if (choice == 1) {
                std::cout << BLUE << "USER STATS:\n" << RESET;
                std::cout <<  "   Number of users: " << userBST.countNodes() << "\n\n" ;

                std::cout << BLUE << "SPAM STATS:\n" << RESET;
                std::cout << "   Total number of spam words: " << spamWordsCount << "\n";
                std::cout << "   Spam words:\n"; displaySpamWords();

                std::cout << BLUE << "SYSTEM HEALTH:\n" << RESET;
                std::cout << "   Email System: OPERATIONAL\n";
                std::cout << "   Spam Filter: ACTIVE\n";
                std::cout << "   User Authentication: ACTIVE\n\n";

                std::cout << BLUE << "EMAIL STATISTICS:\n" << RESET;
              
                std::cout << "   Total Emails in System: " << totalEmails << "\n";
                std::cout << "   Spam Emails Detected: " << spamEmails << "\n";
                if (totalEmails > 0) {
                    double spamPercentage = (spamEmails * 100.0) / totalEmails;
                    std::cout << "   Spam Percentage: " << spamPercentage << "%\n";
                }
                std::cout << "   Read Emails: " << readEmails << "\n";
                std::cout << "   Unread Emails: " << unreadEmails << "\n";
                std::cout << "   Scheduled Emails: " << scheduledEmails << "\n";
                std::cout << "\n";
            }
            else if (choice == 2) {
                int opt = 0;
                do {
                    std::cout << "\n1. View total count\n";
                    std::cout << "2. View all users\n";
                    std::cout << "3. Back\n";
                    std::cout << GREEN << "\nEnter option: " << RESET;
                    opt = getInteger();

                    if (opt == 1) {
                        std::cout << BLUE << "Number of users : " << RESET  << userBST.countNodes() << "\n" ;
                    }
                    else if (opt == 2) {
                        userBST.inOrder();
                    }
                } while (opt != 3);
            }
            else if (choice == 3) {
                std::cout << BLUE << "Total number of spam words: " << RESET << spamWordsCount << "\n";
                std::cout << BLUE << "Spam words:\n" << RESET; displaySpamWords();
                std::cout << BLUE << "Spam Emails Detected: " << RESET << spamEmails << "\n";
                if (totalEmails > 0) {
                    double spamPercentage = (spamEmails * 100.0) / totalEmails;
                    std::cout << BLUE  << "Spam Percentage: " << RESET << spamPercentage << "%\n";
                }
            }
        } while (choice != 4);
    }

    void displaySocialGraph() {
        sg.displayGraph();
    }

    void contactOptions() {
        int choice = 0;
        std::cout << YELLOW << "\n----------------------- Contact Management -----------------------" << RESET;


        do {
            std::cout << "\n1. Add Contact\n";
            std::cout << "2. Remove Contact\n";
            std::cout << "3. View Friends\n";
            std::cout << "4. Mutual Connections\n";
            std::cout << "5. Contact Suggestions\n";
            std::cout << "6. Back\n";
            std::cout << GREEN << "\nEnter option: " << RESET;
            choice = getInteger();

            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice == 1) {
                std::string user1, user2;
                std::cout << "Enter the name of user 1: ";
                user1 = getString();
                std::cout << "Enter the name of user 2: ";
                user2 = getString();
                sg.addConnection(user1, user2);
            }
            else if (choice == 2) {
                std::string user1, user2;
                std::cout << "Enter the name of user 1: ";
                user1 = getString();
                std::cout << "Enter the name of user 2: ";
                 user2 = getString();
                sg.removeConnection(user1, user2);
            }
            else if (choice == 3) {
                std::string user1, user2;
                std::cout << "Enter the name of the user: ";
                user1 = getString();
                sg.Friends(user1);
            }
            else if (choice == 4) {
                std::string user1, user2;
                std::cout << "Enter the name of user 1: ";
                user1 = getString();
                std::cout << "Enter the name of user 2: ";
                user2 = getString();
                sg.mutualFriends(user1, user2);
            }
            else if (choice == 5) {
                std::string user1, user2;
                std::cout << "Enter the name of the user: ";
                user1 = getString();
                sg.friendSuggestions(user1);
            }

        } while (choice != 6);       

    }

    void updateUserInFile(const User& usr) {
        std::ifstream inFile("users.txt");
        if (!inFile.is_open()) {
            std::cerr << RED << "Error opening users file!" << RESET << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        bool updated = false;

        while (std::getline(inFile, line)) {
            std::vector<std::string> tokens;
            std::string token = "";

            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ',') {
                    tokens.push_back(token);
                    token = "";
                }
                else {
                    token += line[i];
                }
            }
            tokens.push_back(token);

            // Check if this is the user we want to update
            if (tokens.size() >= 6 && tokens[2] == usr.getEmail()) {
                std::string updatedLine = tokens[0] + "," + tokens[1] + "," +
                    tokens[2] + "," + tokens[3] + "," +
                    tokens[4] + "," + usr.getLastLoginDate();
                lines.push_back(updatedLine);
                updated = true;
            }
            else {
                lines.push_back(line);
            }
        }
        inFile.close();

        //all lines back to file
        if (updated) {
            std::ofstream outFile("users.txt");
            if (!outFile.is_open()) {
                std::cerr << RED << "Error while opening teh user file" << RESET << std::endl;
                return;
            }

            for (const std::string& l : lines) {
                outFile << l << "\n";
            }
            outFile.close();
            std::cout << "\nLast login updated" << std::endl;
        }
    }
    
    ~EMS() {
        delete[] spamWords;
    }
};
#endif