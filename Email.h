#ifndef EMAIL_H
#define EMAIL_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <ctime>
#include <cstdlib>
#include "attachment.h"
#include "validateInput.h"
#include "colors.h"

class Email {
public:
    std::string emailId;
    std::string sender;
    std::string receiver;
    std::string receiverName;
    std::string subject;
    std::string content;
    time_t timestamp;
    bool isRead;
    bool isSpam;
    int priority;
    std::vector<Attachmments> at;
    int spamScore;
    bool isStar;

    Email(std::string id = "", std::string snd = "", std::string recv = "",
          std::string sub = "", std::string cont = "", int p = 0, std::string rcvName = "")
    {
        emailId = id;
        sender = snd;
        receiver = recv;
        receiverName = rcvName;
        subject = sub;
        content = cont;
        timestamp = time(0);
        isRead = false;
        isSpam = false;
        priority = p;
        spamScore = 0;
        isStar = false;
    }

    std::string getID() { return emailId; }
    void markStar() { isStar = true; }

    void addAttachment() {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string name, type;
        int sizeInKb;
        std::cout << "Type of attachment: ";
        type = getString();
        std::cout << "Name of attachment: ";
        name = getString();
        sizeInKb = rand() % 10240 + 1;
        Attachmments atch(name, type, sizeInKb);
        at.push_back(atch);
    }

    bool containsSpamWords(std::string spamWords[], int count) {
        for (int i = 0; i < count; i++) {
            size_t pos = content.find(spamWords[i]);
            if (pos != std::string::npos) {
                isSpam = true;
                spamScore++;
            }
        }
        return isSpam;
    }

    void markAsRead()   { isRead = true; }
    void markAsUnRead() { isRead = false; }
    void setPriority(int p) { priority = p; }

    bool operator==(const Email& other) const { return emailId == other.emailId; }
    bool operator<(const Email& other) const  { return (priority - spamScore) < (other.priority - other.spamScore); }
    bool operator>(const Email& other) const  { return (priority - spamScore) > (other.priority - other.spamScore); }
    bool operator()(const Email& a, const Email& b) const { return a.timestamp < b.timestamp; }

    friend std::ostream& operator<<(std::ostream& out, const Email& other) {
        out << "Email ID: "  << other.emailId  << "\n";
        out << "Sender: "    << other.sender    << "\n";
        out << "Receiver: "  << other.receiver  << "\n";
        out << "Subject: "   << other.subject   << "\n";
        out << "Priority: "  << other.priority  << "\n";
        return out;
    }
};

#endif
