#ifndef EF_H
#define EF_H

#include <iostream>
#include <fstream>
#include "Email.h"
#include "Linkedlist.h"
#include<stack>
#include "MaxHeap.h"
#include "DLL.h"
#include "validateInput.h"
#include "colors.h"
using namespace std;

class EmailFolder {
private:
    std::string folderName;
    DLL<Email> EmailList;
    maxHeap<Email> EmaiMaxHeap;
    std::stack<Email> deletedEmails;

public:
    int count;

    maxHeap<Email> priorityHeap;   //for important emails (priority - spamScore)
    maxHeap<Email> timestampHeap;
    EmailFolder(std::string n = "")
        : count(0), folderName(n),
        EmailList(),
        EmaiMaxHeap(),
        deletedEmails(),
        priorityHeap("priority"),
        timestampHeap("timestamp")
    {}

    std::string getFolderName() {
        return folderName;
    }

    void addEmail(Email newEmail) {
        EmailList.InsertAtEnd(newEmail);
        EmaiMaxHeap.insert(newEmail);
        priorityHeap.insert(newEmail);
        timestampHeap.insert(newEmail);
        count++;

        //write in file
        std::ofstream outFile("email.txt", std::ios::app);

        if (!outFile) {
            std::cerr << RED << "Error opening file!" << std::endl << RESET;
            return;
        }

        struct tm tstruct;
        localtime_s(&tstruct, &newEmail.timestamp);   
        char buf[11];                  //for "YYYY-MM-DD\0"
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
        std::string cdd(buf);

        std::string sp = newEmail.isSpam ? "true" : "false";
        std::string rd = newEmail.isRead ? "true" : "false";

        outFile << newEmail.emailId << "," << newEmail.sender << "," << newEmail.receiver << "," << newEmail.subject << "," << newEmail.content << "," << cdd << "," << rd << "," << sp << "," << newEmail.priority << "," << folderName << "\n"; //"emailId,sender,receiver,subject,content,timestamp,isRead,isSpam,priority,folder\n";

        outFile.close();

    }

    Email removeEmail(std::string emailId) {
        Email obj(emailId, "");

        Email temp = EmailList.DeleteByValue(obj);
        EmaiMaxHeap.deleteByValue(obj);
        deletedEmails.push(temp);
        priorityHeap.deleteByValue(temp);
        timestampHeap.deleteByValue(temp);
        count--;


        std::ifstream inFile("email.txt");
        if (!inFile.is_open()) {
            std::cerr << RED << "Error opening email file!" << RESET << std::endl;
            return temp;
        }

        std::vector<std::string> lines;
        std::string line;
        bool removed = false;

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;

            std::string lineEmailId = "";
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ',') {
                    break;
                }
                lineEmailId += line[i];
            }

            if (lineEmailId != emailId) {
                lines.push_back(line);
            }
            else {
                removed = true;
            }
        }
        inFile.close();

        if (removed) {
            std::ofstream outFile("email.txt");
            if (!outFile.is_open()) {
                std::cerr << RED << "Error opening email file for writing!" << RESET << std::endl;
                return temp;
            }

            for (const std::string& l : lines) {
                outFile << l << "\n";
            }
            outFile.close();
            std::cout << "Email removed from file \n";
        }

        return temp;

    }

    std::vector<Email> getEmailsByPriority() {
        std::vector<Email> ans;
        EmaiMaxHeap.heapSort();
        EmaiMaxHeap.display();
        EmaiMaxHeap.heapify();
        return ans;
    }

    void getRecentEmail() {
        Node<Email>* e = EmailList.getTail();
        if (e) {
            std::cout << e->data;
        }
        else {
            std::cout << RED << "The email lsit is empty\n" << RESET;
        }
    }

    void printStatistics() {
        std::cout << "For Folder " << folderName << " :\n";
        std::cout << "Number of email in this folder are: " << count << "\n";

        int spamCount = 0, avPriority = 0;
        for (auto it = EmailList.begin(); it != EmailList.end(); ++it) {
            if ((*it).isSpam) {
                spamCount++;
            }
            avPriority += (*it).priority;
        }
        std::cout << "Total spam messages: " << spamCount << "\n";
        std::cout << "average priority of the folder " << avPriority << "\n";

    }

    void emptyTrash() {
        std::ifstream inFile("email.txt");
        if (!inFile.is_open()) {
            std::cerr << RED << "Error opening email file!" << RESET << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        int deletedCount = 0;

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;

            std::string lineFolder = "";
            int lastComma = -1;
            for (int i = line.size() - 1; i >= 0; i--) {
                if (line[i] == ',') {
                    lastComma = i;
                    break;
                }
            }
            if (lastComma != -1) {
                lineFolder = line.substr(lastComma + 1);
            }

            while (!lineFolder.empty() && (lineFolder.back() == ' ' || lineFolder.back() == '\n' || lineFolder.back() == '\r')) {
                lineFolder.pop_back();
            }

            if (lineFolder != "Trash") {
                lines.push_back(line);
            }
            else {
                deletedCount++;
            }
        }
        inFile.close();

        std::ofstream outFile("email.txt");
        if (!outFile.is_open()) {
            std::cerr << RED << "Error opening email file for writing!" << RESET << std::endl;
            return;
        }

        for (const std::string& l : lines) {
            outFile << l << "\n";
        }
        outFile.close();

        // Clear the stack
        while (!deletedEmails.empty()) {
            deletedEmails.pop();
        }

        std::cout << GREEN << "Trash emptied \n" << RESET;
    }

    void Display() {
        EmailList.DisplayForward();
    }

    /*void markRead(std::string id, char read) {
        for (auto it = EmailList.begin(); it != EmailList.end(); ++it) {
            if ((*it).getID() == id) {
                if (read == 'r') {
                    (*it).markAsRead();
                    std::cout << "Email marked as read\n";
                    return;
                }
                else if (read == 'u') {
                    (*it).markAsUnRead();
                    std::cout << "Email marked as unread\n";
                    return;
                }
            }
        }
        std::cout << RED << "No email with this id\n" << RESET;
    }*/

    void markRead(std::string id, char read) {
        bool found = false;

        for (auto it = EmailList.begin(); it != EmailList.end(); ++it) {
            if ((*it).getID() == id) {
                if (read == 'r') {
                    (*it).markAsRead();
                    std::cout << "Email marked as read\n";
                    found = true;
                }
                else if (read == 'u') {
                    (*it).markAsUnRead();
                    std::cout << "Email marked as unread\n";
                    found = true;
                }
                break;
            }
        }

        if (!found) {
            std::cout << RED << "No email with this id\n" << RESET;
            return;
        }

        std::ifstream inFile("email.txt");
        if (!inFile.is_open()) {
            std::cerr << RED << "Error opening email file!" << RESET << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string line;

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;

            // Extract email ID 
            std::string lineEmailId = "";
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ',') {
                    break;
                }
                lineEmailId += line[i];
            }

            if (lineEmailId == id) {
                std::vector<std::string> fields;
                std::string field = "";
                for (char c : line) {
                    if (c == ',') {
                        fields.push_back(field);
                        field = "";
                    }
                    else {
                        field += c;
                    }
                }
                fields.push_back(field); 

                if (fields.size() >= 10) {
                    fields[6] = (read == 'r') ? "true" : "false";

                    // Reconstruct the line
                    std::string updatedLine = "";
                    for (int i = 0; i < fields.size(); i++) {
                        updatedLine += fields[i];
                        if (i < fields.size() - 1) {
                            updatedLine += ",";
                        }
                    }
                    lines.push_back(updatedLine);
                }
            }
            else {
                lines.push_back(line);
            }
        }
        inFile.close();

        std::ofstream outFile("email.txt");
        if (!outFile.is_open()) {
            std::cerr << RED << "Error opening email file for writing!" << RESET << std::endl;
            return;
        }

        for (const std::string& l : lines) {
            outFile << l << "\n";
        }
        outFile.close();
    }

    int getSpamScore(std::string id) {
        for (auto it = EmailList.begin(); it != EmailList.end(); ++it) {
            if ((*it).getID() == id) {
                return (*it).spamScore;
            }
        }
        //return -1;   
        return -1;
    }

    void setPriority(std::string id, int newPriority) {
        for (auto it = EmailList.begin(); it != EmailList.end(); ++it) {
            if ((*it).getID() == id) {
                (*it).setPriority(newPriority);
                std::cout << "Priority set to " << newPriority << "\n";
                return;
            }
        }
        std::cout << RED << "No email with this id\n" << RESET;
    }

    bool starEmail(std::string id, Email& e) {
        for (auto it = EmailList.begin(); it != EmailList.end(); ++it) {
            if ((*it).getID() == id) {
                (*it).markStar();
                e = (*it);
                return true; 
            }
        }
        return false; 
    }
   
    void clear() {
        EmailList = DLL<Email>();
        EmaiMaxHeap = maxHeap<Email>();
        deletedEmails = std::stack<Email>();
        priorityHeap = maxHeap<Email>();
        timestampHeap = maxHeap<Email>();

        emptyTrash();
        count = 0;
        std::cout << "Trash cleared\n";
    }

    void clearOthers() {
        EmailList = DLL<Email>();
        EmaiMaxHeap = maxHeap<Email>();
        deletedEmails = std::stack<Email>();
        priorityHeap = maxHeap<Email>();
        timestampHeap = maxHeap<Email>();
        count = 0;
    }
};

#endif 
