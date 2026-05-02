#ifndef SOCIALGRAPH_H
#define SOCIALGRAPH_H

#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include<fstream>
#include<ctime>
#include <cstdlib>
#include "colors.h"

using namespace std;

class Connection {
public:
    std::string FEmail;
    int CStrength;
    std::string CDate;

    Connection(std::string email = "", int strength = 0, std::string date = "") {
        FEmail = email;
        CStrength = strength;
        CDate = date;
        srand(time(0));
    }
};

class SocialGraph {
private:
    std::unordered_map<std::string, std::vector<Connection>> adjList;

public:
    void loadFromFile() {
        std::ifstream file("social_graph.txt");
        if (!file.is_open()) {
            std::cerr << "Error opening the graph file.\n";
            return;
        }

        std::string line;
        while (std::getline(file, line)) {

            std::vector<std::string> tokens;
            std::string temp = "";

            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ',') {
                    tokens.push_back(temp);
                    temp = "";
                }
                else {
                    temp += line[i];
                }
            }
            tokens.push_back(temp);

            if (tokens.size() == 4) {
                std::string user1 = tokens[0];
                std::string user2 = tokens[1];
                int strength = stoi(tokens[2]);
                std::string date = tokens[3];

                // undirected graph
                adjList[user1].push_back(Connection(user2, strength, date));
                adjList[user2].push_back(Connection(user1, strength, date));
            }
        }

        file.close();
        std::cout << "Social graph loaded\n";
    }

    double getSpamProbability(int strength) {
        return (10 - strength) * 10.0;
    }

    void Friends(std::string& userEmail) {
        std::cout << "Friends of " << userEmail << ":\n";

        if (adjList.find(userEmail) == adjList.end() ||
            adjList[userEmail].empty()) {
            std::cout << RED << "No friends found.\n" << RESET;
            return;
        }

        std::vector<Connection>& friends = adjList[userEmail];

        std::cout << "Total Friends: " << friends.size() << "\n\n";

        for (int i = 0; i < friends.size(); i++) {

            Connection conn = friends[i];
            double spamProb = getSpamProbability(conn.CStrength);

            std::cout << (i + 1) << ". " << conn.FEmail << "\n";
            std::cout << "   Connection Strength: " << conn.CStrength << "/10\n";
            std::cout << "   Connected Date: " << conn.CDate << "\n";
            std::cout << "   Spam Probability: " << spamProb << "%\n";

            if (spamProb >= 70.0) {
                std::cout << "   High spam probability!\n";
            }
            else if (spamProb >= 40.0) {
                std::cout << "   Moderate spam risk.\n";
            }
            std::cout << "\n";
        }
    }

    void mutualFriends(std::string& user1, const std::string& user2) {
        std::cout << "Mutual Connections between " << user1
            << " and " << user2 << "\n";

        if (adjList.find(user1) == adjList.end() ||
            adjList.find(user2) == adjList.end()) {
            std::cout << "One or both users not found in graph.\n";
            return;
        }

        std::unordered_set<std::string> user1Friends;
        for (int i = 0; i < adjList[user1].size(); i++) {
            user1Friends.insert(adjList[user1][i].FEmail);
        }

        std::vector<std::string> mutuals;

        for (int i = 0; i < adjList[user2].size(); i++) {
            std::string candidate = adjList[user2][i].FEmail;

            if (user1Friends.count(candidate)) {
                mutuals.push_back(candidate);
            }
        }

        if (mutuals.empty()) {
            std::cout << RED << "No mutual friends found.\n" << RESET;
            return;
        }

        // std::cout << "Total Mutual Friends: " << mutuals.size() << "\n\n";
        for (int i = 0; i < mutuals.size(); i++) {
            std::cout << (i + 1) << ". " << mutuals[i] << "\n";
        }
    }

    void friendSuggestions(std::string& userEmail) {

        if (adjList.find(userEmail) == adjList.end()) {
            std::cout << "User not found.\n";
            return;
        }

        std::vector<Connection> directFriends = adjList[userEmail];
        std::vector<std::string> direct;

        for (int i = 0; i < directFriends.size(); i++) {
            direct.push_back(directFriends[i].FEmail);
        }

        std::vector<std::string> suggestions;

        for (int i = 0; i < directFriends.size(); i++) {

            std::vector<Connection> secondLevel = adjList[directFriends[i].FEmail];

            for (int j = 0; j < secondLevel.size(); j++) {
                std::string candidate = secondLevel[j].FEmail;

                if (candidate == userEmail) continue;

                bool isDirect = false;
                for (int k = 0; k < direct.size(); k++) {
                    if (direct[k] == candidate) {
                        isDirect = true;
                        break;
                    }
                }
                if (isDirect) continue;

                bool alreadyAdded = false;
                for (int k = 0; k < suggestions.size(); k++) {
                    if (suggestions[k] == candidate) {
                        alreadyAdded = true;
                        break;
                    }
                }
                if (!alreadyAdded) {
                    suggestions.push_back(candidate);
                }
            }
        }

        if (suggestions.empty()) {
            std::cout << RED << "No suggestions for this user.\n" << RESET;
            return;
        }

        std::cout << "Friend Suggestions for " << userEmail << ":\n";
        for (int i = 0; i < suggestions.size(); i++) {
            std::cout << i + 1 << ". " << suggestions[i] << "\n";
        }
    }

    void displayGraph() {

        std::cout << YELLOW << "\n----------------------- Social Graph -----------------------\n" << RESET;

        if (adjList.empty()) {
            std::cout << RED << "Graph is empty.\n" << RESET;
            return;
        }

        std::unordered_map<std::string, std::vector<Connection>>::iterator it;

        for (it = adjList.begin(); it != adjList.end(); ++it) {

            std::string user = it->first;
            std::vector<Connection> friends = it->second;

            std::cout << user << " -> [";

            for (int i = 0; i < friends.size(); i++) {
                std::cout << friends[i].FEmail;
                if (i < friends.size() - 1) std::cout << ", ";
            }

            std::cout << "]\n";
        }

        std::cout << "\n";
    }

   /* void addConnection(std::string user1, std::string user2) {

        bool existsIn1 = false;
        srand(time(0));          
        int st = (rand() % 10) + 1;   

        for (int i = 0; i < adjList[user1].size(); i++) {
            if (adjList[user1][i].FEmail == user2) {
                existsIn1 = true;
                break;
            }
        }

        time_t now = time(0);
        struct tm tstruct;
        localtime_s(&tstruct, &now);
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
        std::string cd = buf;

        if (!existsIn1) {
            adjList[user1].push_back(Connection(user2, st, cd));
        }

        bool existsIn2 = false;

        for (int i = 0; i < adjList[user2].size(); i++) {
            if (adjList[user2][i].FEmail == user1) {
                existsIn2 = true;
                break;
            }
        }

        if (!existsIn2) {
            adjList[user2].push_back(Connection(user1, st, cd));
        }

        std::cout << "Connection added between " << user1
            << " and " << user2 << "\n";
    }*/

  /*  int getNum() { 
        srand(time(0));
        int st = (rand() % 10) + 1;
        return st;
    }*/
    void addConnection(std::string user1, std::string user2, int st = -1) {
        bool existsIn1 = false;
        if (st == -1) {
            st = (rand() % 10) + 1;
        }

        for (int i = 0; i < adjList[user1].size(); i++) {
            if (adjList[user1][i].FEmail == user2) {
                existsIn1 = true;
                break;
            }
        }

        time_t now = time(0);
        struct tm tstruct;
        localtime_s(&tstruct, &now);
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
        std::string cd = buf;

        if (!existsIn1) {
            adjList[user1].push_back(Connection(user2, st, cd));
        }

        bool existsIn2 = false;
        for (int i = 0; i < adjList[user2].size(); i++) {
            if (adjList[user2][i].FEmail == user1) {
                existsIn2 = true;
                break;
            }
        }

        if (!existsIn2) {
            adjList[user2].push_back(Connection(user1, st, cd));
        }

        if (!existsIn1 && !existsIn2) {
            std::ofstream outFile("social_graph.txt", std::ios::app);
            if (!outFile.is_open()) {
                std::cerr << RED << "Error opening social_graph.txt for writing!" << RESET << std::endl;
                return;
            }

            outFile << user1 << "," << user2 << "," << st << "," << cd << "\n";
            outFile.close();

            std::cout << "Connection added between " << user1 << " and " << user2 << "\n";
        }
        else {
            std::cout << "Connection already exists between " << user1 << " and " << user2 << "\n";
        }
    }

    void removeConnection(std::string user1, std::string user2) {
        // Remove from adjList
        std::vector<Connection>& list1 = adjList[user1];
        for (int i = 0; i < list1.size(); i++) {
            if (list1[i].FEmail == user2) {
                list1.erase(list1.begin() + i);
                break;
            }
        }

        std::vector<Connection>& list2 = adjList[user2];
        for (int i = 0; i < list2.size(); i++) {
            if (list2[i].FEmail == user1) {
                list2.erase(list2.begin() + i);
                break;
            }
        }
        std::ifstream inFile("social_graph.txt");
        if (!inFile.is_open()) {
            std::cerr << RED << "Error opening social_graph.txt for reading!" << RESET << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        bool removed = false;

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;

            std::vector<std::string> tokens;
            std::string temp = "";
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ',') {
                    tokens.push_back(temp);
                    temp = "";
                }
                else {
                    temp += line[i];
                }
            }
            tokens.push_back(temp);

            if (tokens.size() >= 2) {
                std::string fileUser1 = tokens[0];
                std::string fileUser2 = tokens[1];

                if ((fileUser1 == user1 && fileUser2 == user2) ||
                    (fileUser1 == user2 && fileUser2 == user1)) {
                    removed = true;
                    continue;  // Skip this line 
                }
            }

            lines.push_back(line);
        }
        inFile.close();

        if (removed) {
            std::ofstream outFile("social_graph.txt");
            if (!outFile.is_open()) {
                std::cerr << RED << "Error opening social_graph.txt for writing!" << RESET << std::endl;
                return;
            }

            for (const std::string& l : lines) {
                outFile << l << "\n";
            }
            outFile.close();

            std::cout << "Connection removed between " << user1 << " and " << user2 << "\n";
        }
        else {
            std::cout << "Connection not found between " << user1 << " and " << user2 << "\n";
        }
    }
};

#endif


