#ifndef USER_H
#define USER_H

#include "EF.h"
#include <iostream>
#include <vector>
#include <limits>
#include "validateInput.h"
#include <queue>
#include "BinaryTree.h"
#include "Linkedlist.h"
#include "Contact.h"
#include"colors.h"
#include "SocialGraph.h"

using namespace std;

class User {
private:
	std::string userId;
	std::string username;
	std::string email;
	std::string password;
	BST<Contact> contacts;
	std::string createdDate;
	std::string LastLogin;
	linkedList<EmailFolder> folders;
	std::queue<Email> scheduledMails;
	Contact contArr[10];
	int contCount;
public:
	User(std::string id = "", std::string name = "", std::string e = "", std::string p = "", std::string cd = "", std::string ld = "") {
		userId = id;
		username = name;
		email = e;
		password = p;
		createdDate = cd;
		LastLogin = ld;
		std::string names[] = { "Inbox", "Sent", "Drafts", "Spam", "Trash", "Import" , "Backup"};

		for (std::string n : names) {
			folders.InsertAtEnd(EmailFolder(n));
		}
		contCount = 0;

	}

	//copy constructor
	User(const User& other)
		: userId(other.userId), username(other.username),
		email(other.email), password(other.password),
		createdDate(other.createdDate), LastLogin(other.LastLogin),
		contCount(other.contCount)  

	{
		std::string names[] = { "Inbox", "Sent", "Drafts", "Spam", "Trash", "Import", "Backup"};
		for (std::string n : names) {
			folders.InsertAtEnd(EmailFolder(n));
		}

		for (int i = 0; i < contCount; i++) {
			contArr[i] = other.contArr[i];
		}
	}

	//assignment operator
	User& operator=(const User& other) {
		if (this != &other) {
			userId = other.userId;
			username = other.username;
			email = other.email;
			password = other.password;
			createdDate = other.createdDate;
			LastLogin = other.LastLogin;

			contCount = other.contCount;
			for (int i = 0; i < contCount; i++) {
				contArr[i] = other.contArr[i];
			}
		}
		return *this;
	}

	std::string getUserId() const { return userId; }
	std::string getUsername() const { return username; }
	std::string getEmail() const { return email; }
	std::string getPassword() const { return password; }
	std::string getCreatedDate() const { return createdDate; }
	std::string getLastLoginDate() const { return LastLogin; }

	void addContact(Contact newContact) {
		contacts.Insert(newContact);

		bool alreadyExists = false;
		int existingIndex = -1;

		for (int i = 0; i < contCount; i++) {
			if (contArr[i] == newContact) {
				alreadyExists = true;
				existingIndex = i;
				break;
			}
		}

		if (alreadyExists) {
			Contact temp = contArr[existingIndex];
			for (int i = existingIndex; i > 0; i--) {
				contArr[i] = contArr[i - 1];
			}
			contArr[0] = temp;
		}
		else {
			if (contCount < 10) {
				//shift everything right and add at front
				for (int i = contCount; i > 0; i--) {
					contArr[i] = contArr[i - 1];
				}
				contArr[0] = newContact;
				contCount++;
			}
			else {
				for (int i = 9; i > 0; i--) {
					contArr[i] = contArr[i - 1];
				}
				contArr[0] = newContact;
			}
		}
	}

	std::vector<Contact> searchContacts(std::string query, std::string parameter) {
		std::vector<Contact> ans;
		if (parameter == "email") {
			Contact temp("", query);
			ans = contacts.Search(temp, parameter);
		}
		else if (parameter == "name") {
			Contact temp(query, "");
			ans = contacts.Search(temp, parameter);
		}

		if (ans.size() > 0) {
			return ans;
		}
		throw std::runtime_error("No such contact found");
	}

	void viewRecentContacts() {
		std::cout << "\nRecent Contacts (Last 10)\n";

		if (contCount == 0) {
			std::cout << RED << "No recent contacts\n" << RESET;
			return;
		}

		for (int i = 0; i < contCount; i++) {
			std::cout << contArr[i] << "\n";
		}
	}

	bool operator<(const User& other) const {
		if (username < other.username) return true;
		if (username > other.username) return false;
		return email < other.email;
	}

	bool operator>(const User& other) const {
		if (username > other.username) return true;
		if (username < other.username) return false;
		return email > other.email;
	}

	bool operator==(const User& other) const {
		return username == other.username && email == other.email;
	}

	friend ostream& operator<<(ostream& out, const User& other) {
		out << "Name: " << other.getUsername() << "\n";
		out << "Email: " << other.getEmail() << "\n";
		return out;
	}

	bool hasSameName(const User& other) const {
		return username == other.username;
	}

	bool hasSameEmail(const User& other) const {
		return email == other.email;
	}

	void createCustomFolder() {
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::string Fname;
		std::cout << "Enter the name of the folder you want to add: ";
		Fname = getString();

		// Check if folder already exists
		for (auto it = folders.begin(); it != folders.end(); ++it) {
			if ((*it).getFolderName() == Fname) {
				std::cout << RED << "A folder with this name already exists\n" << RESET;
				return;
			}
		}

		EmailFolder temp(Fname);
		folders.InsertAtEnd(temp);
		std::cout << "Folder '" << Fname << "' created\n";
	}

	void send(Email& e, std::string spamWords[], int count, SocialGraph& sg) {
		for (auto it = folders.begin(); it != folders.end(); ++it) {
			if ((*it).getFolderName() == "Sent") {
				(*it).addEmail(e);
				std::cout <<"Sent\n" ;
				break;  
			}
		}
		if (e.containsSpamWords(spamWords, count)) {
			for (auto it = folders.begin(); it != folders.end(); ++it) {
				if ((*it).getFolderName() == "Spam") {
					e.isSpam = true;
					(*it).addEmail(e);
					std::cout << "Added to spam\n";
					break;
				}
			}
		}
		int connectionStrength = std::max(1, 10 - e.spamScore);  // High spam = low strength
		sg.addConnection(e.sender, e.receiverName, e.spamScore);
	}

	void moveToInbox(Email& e, std::string spamWords[], int count) {
		for (auto it = folders.begin(); it != folders.end(); ++it) {
			if ((*it).getFolderName() == "Inbox") {
				(*it).addEmail(e);
				std::cout << "Recieved by : " << username << "\n";
				break;
			}
		}
		if (e.containsSpamWords(spamWords, count)) {
			for (auto it = folders.begin(); it != folders.end(); ++it) {
				if ((*it).getFolderName() == "Spam") {
					(*it).addEmail(e);
					std::cout << "Added to spam\n";
					return;
				}
			}
		}
	}

	void moveToDraft(Email& e, std::string spamWords[], int count) {
		for (auto it = folders.begin(); it != folders.end(); ++it) {
			if ((*it).getFolderName() == "Drafts") {
				(*it).addEmail(e);
				std::cout << "Email moved in the draft of : " << username << "\n";
				break;
			}
		}
		if (e.containsSpamWords(spamWords, count)) {
			for (auto it = folders.begin(); it != folders.end(); ++it) {
				if ((*it).getFolderName() == "Spam") {
					(*it).addEmail(e);
					std::cout << "Added to spam\n";
					return;
				}
			}
		}
	}

	void scheduleMail(Email& e) {
		std::string date, timeStr;
		std::cout << "Enter date (YYYY-MM-DD): ";
		cin >> date;
		std::cout << "Enter time (HH:MM): ";
		cin >> timeStr;

		int year, month, day, hour, minute;
		sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
		sscanf(timeStr.c_str(), "%d:%d", &hour, &minute);


		tm scheduleTm = {};
		scheduleTm.tm_year = year - 1900;
		scheduleTm.tm_mon = month - 1;
		scheduleTm.tm_mday = day;
		scheduleTm.tm_hour = hour;
		scheduleTm.tm_min = minute;
		scheduleTm.tm_sec = 0;

		time_t scheduledTime = mktime(&scheduleTm);

		e.timestamp = scheduledTime;

		scheduledMails.push(e);
		std::cout << "Email scheduled \n";

		//writing in file:
		std::ofstream outFile("email.txt", std::ios::app);

		if (!outFile) {
			std::cerr << "Error opening file!" << std::endl;
			return;
		}

		struct tm tstruct;
		localtime_s(&tstruct, &e.timestamp); 
		char buf[11]; 
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
		std::string dateOnly(buf);

		std::string sp = e.isSpam ? "true" : "false";
		std::string rd = e.isRead ? "true" : "false";

		// Write in required pattern
		outFile << e.emailId << ","
			<< e.sender << ","
			<< e.receiver << ","
			<< e.subject << ","
			<< e.content << ","
			<< dateOnly << ","   
			<< rd << ","
			<< sp << ","
			<< e.priority << ","
			<< "Scheduled"        
			<< "\n";

		outFile.close();
	}

	void folderOperations(std::string f, SocialGraph& sg) {
		int choice = 0;
		std::cout << YELLOW << "\n----------------------- " << f << " ---------------------- - " << RESET;


		do {
			std::cout << "\n1. View Emails\n";
			std::cout << "2. Sort By:\n";
			std::cout << "3. Move Email\n";
			std::cout << "4. Mark Read/Unread\n";
			std::cout << "5. Delete Email\n";
			std::cout << "6. Get Spam Score\n";
			std::cout << "7. Set Priority\n";
			std::cout << "8. Star Important Emails\n";
			std::cout << "9. Back\n";
			std::cout << "-------------------------------------\n";
			std::cout << GREEN << "\nEnter choice: " << RESET;
			choice = getInteger();


			if (choice == 1) {
				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == f) {
						(*it).Display();
						break;
					}
				}
			}

			else if (choice == 2) {
				char option;
				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == f) {
						std::cout << "\t a. Priority\n";
						std::cout << "\t b. Date\n";
						option = getCharacter();

						if (option == 'a') {
							(*it).priorityHeap.heapSort();
							(*it).priorityHeap.display();
							(*it).priorityHeap.heapify();
						}
						else if (option == 'b') {
							(*it).timestampHeap.heapSort();
							(*it).timestampHeap.display();
							(*it).timestampHeap.heapify();
						}
						break;
					}
				}
			}

			else if (choice == 3) {
				bool folderEmpty = true;
				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == f) {
						if ((*it).count == 0) {
							std::cout << RED << "This folder is empty. No emails to move.\n" << RESET;
							folderEmpty = true;
						}
						else {
							folderEmpty = false;
						}
						break;
					}
				}
				if (folderEmpty) {
					continue;
				}

				std::string emailId;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\nEnter Email ID to move: ";
				emailId = getString();

				Email emailToMove;
				bool found = false;
				std::string destFolder = "";  

				try {
					for (auto it = folders.begin(); it != folders.end(); ++it) {
						if ((*it).getFolderName() == f) {
							emailToMove = (*it).removeEmail(emailId);
							found = true;
							break;
						}
					}

					std::cout << "\nAvailable folders:\n";
					int folderNum = 1;
					std::vector<std::string> folderNames;

					for (auto it = folders.begin(); it != folders.end(); ++it) {
						std::string folderName = (*it).getFolderName();
						if (folderName != f) {
							std::cout << folderNum++ << ". " << folderName << "\n";
							folderNames.push_back(folderName);
						}
					}

					std::cout << "\nSelect destination folder: ";
					int destChoice;
					destChoice = getInteger();

					if (destChoice < 1 || destChoice > folderNames.size()) {
						std::cout << RED << "Invalid choice!\n" << RESET;
						for (auto it = folders.begin(); it != folders.end(); ++it) {
							if ((*it).getFolderName() == f) {
								(*it).addEmail(emailToMove);
								std::cout << "Email restored to " << f << "\n";
								break;
							}
						}
						continue;
					}

					destFolder = folderNames[destChoice - 1];
					for (auto it = folders.begin(); it != folders.end(); ++it) {
						if ((*it).getFolderName() == destFolder) {
							(*it).addEmail(emailToMove);
							std::cout << "Email moved from " << f << " to " << destFolder << "\n";
							break;
						}
					}

					char opt = '\0';
					std::cout << GREEN << "Enter U to undo and any other character to continue\n" << RESET;
					opt = getCharacter();

					if (opt == 'U' || opt == 'u') {
						for (auto it = folders.begin(); it != folders.end(); ++it) {
							if ((*it).getFolderName() == destFolder) {
								try {
									Email restoredEmail = (*it).removeEmail(emailId);

									for (auto origIt = folders.begin(); origIt != folders.end(); ++origIt) {
										if ((*origIt).getFolderName() == f) {
											(*origIt).addEmail(restoredEmail);
											std::cout << "Email restored to " << f << "\n";
											break;
										}
									}
								}
								catch (const std::runtime_error& e) {
									std::cout << RED << "Failed to undo: " << e.what() << "\n" << RESET;
								}
								break;
							}
						}
					}
				}
				catch (const std::runtime_error& e) {
					std::cout << RED << e.what() << "\n" << RESET;
				}
			}

			else if (choice == 4) {
				std::string emailId;
				std::cout << "Enter Email ID: ";
				cin >> emailId;

				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == f) {

						std::cout << "Enter r to mark as read and u to mark as unread? ";
						char readChoice;
						readChoice = getCharacter();
						(*it).markRead(emailId, readChoice);
						break;
					}
				}
			}

			else if (choice == 5) {
				std::string emailId;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Enter Email ID to delete: ";
				emailId = getString();

				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == f) {

						if ((*it).count == 0) {
							std::cout << RED << "The folder is empty\n" << RESET;
							break;
						}

						try {
							Email deletedEmail = (*it).removeEmail(emailId);

							sg.removeConnection(deletedEmail.receiver, deletedEmail.sender);
							// Move to Trash
							for (auto itt = folders.begin(); itt != folders.end(); ++itt) {
								if ((*itt).getFolderName() == "Trash") {
									(*itt).addEmail(deletedEmail);
									std::cout << "Email moved to Trash\n";
									break;
								}
							}

							for (auto itt = folders.begin(); itt != folders.end(); ++itt) {
								if ((*itt).getFolderName() == "Backup") {
									(*itt).addEmail(deletedEmail);
									std::cout << "Email moved to Backup\n";
									break;
								}
							}

							char opt = '\0';
							std::cout << GREEN << "Enter U to undo and any other character to continue\n" << RESET;
							opt = getCharacter();

							if (opt == 'U' || opt == 'u') {

								for (auto itt = folders.begin(); itt != folders.end(); ++itt) {
									if ((*itt).getFolderName() == "Trash") {
										try {
											(*itt).removeEmail(emailId);
										}
										catch (const std::runtime_error& e) {
											std::cout << RED  << e.what() << "\n" << RESET;
										}
										break;
									}
								}


								for (auto itt = folders.begin(); itt != folders.end(); ++itt) {
									if ((*itt).getFolderName() == "Backup") {
										try {
											Email restoredEmail = (*itt).removeEmail(emailId);

											for (auto origIt = folders.begin(); origIt != folders.end(); ++origIt) {
												if ((*origIt).getFolderName() == f) {
													(*origIt).addEmail(restoredEmail);
													std::cout << "Email restored to " << f << "\n";
													break;
												}
											}
										}
										catch (const std::runtime_error& e) {
											std::cout << RED << "Failed to undo: " << e.what() << "\n" << RESET;
										}
										break;
									}
								}
							}
						}
						catch (const std::runtime_error& e) {
							std::cout << RED << e.what() << "\n" << RESET;
						}
						break;
					}
				}
			}

			else if (choice == 6) {
				std::string emailId;
				std::cout << "Enter Email ID: ";
				emailId = getString();

				int score = 0;

				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == f) {
						score = (*it).getSpamScore(emailId);
						break;
					}
				}
				if (score == -1) {
					std::cout << RED << "The folder does not have email with this id\n" << RESET;
				}
				else
				std::cout << "Spam Score: " << score << "\n";
			}

			else if (choice == 7) {
				std::string emailId;
				int newPriority;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				std::cout << "Enter Email ID: ";
				emailId = getString();
				std::cout << "Enter new priority ";
				newPriority = getInteger();

				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == f) {
						(*it).setPriority(emailId, newPriority);
						break;
					}
				}
			}

			else if (choice == 8) {
				std::string emailId;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				std::cout << "Enter Email ID to star: ";
				emailId = getString();

				bool found = false;
				Email toMove;

				// Star the email first
				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == f) {
						found = (*it).starEmail(emailId, toMove);
						break;
					}
				}

				if (!found) {
					std::cout << RED << "Email not found in " << f << "\n" << RESET;
					return;
				}

				std::cout << "Email starred \n";

				// Add to Important folder
				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == "Import") { 
						(*it).addEmail(toMove);
						std::cout << "Email moved to Important folder!\n";
						break;
					}
				}
			}
				
		} while (choice != 9);
	}

	void clearTrash() {
		for (auto it = folders.begin(); it != folders.end(); ++it) {
			if ((*it).getFolderName() == "Trash") {
				(*it).clear();
				return;
			}
		}
	}

	void setLastLogin(std::string date) {
		LastLogin = date;
	}

	void BackupOperations() {
		int choice = 0;

		std::cout << CYAN << "\n----------------------- Backup -----------------------" << RESET;

		do {
			std::cout << "\n1. View Backup Emails\n";
			std::cout << "2. Clear Backup\n";
			std::cout << "3. Back\n";
			std::cout << "-------------------------------------\n";
			std::cout << GREEN << "\nEnter choice: " << RESET;

			choice = getInteger();

			if (choice == 1) {
				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == "Backup") {				
						(*it).Display();				
					}
				}
			}
			else if (choice == 2) {
				
				for (auto it = folders.begin(); it != folders.end(); ++it) {
					if ((*it).getFolderName() == "Backup") {
						(*it).clearOthers();
						std::cout << "Backup folder cleared\n";
						break;
					}
				}
				
			}

		} while (choice != 3);
	}
};
#endif 



//void send(Email& e, std::string spamWords[], int count) {
//	bool isSpamEmail = e.containsSpamWords(spamWords, count);
//
//	// Add to Sent folder
//	for (auto it = folders.begin(); it != folders.end(); ++it) {
//		if ((*it).getFolderName() == "Sent") {
//			(*it).addEmail(e);
//			std::cout << "Sent\n";
//			break;
//		}
//	}
//
//	// If spam, also add to Spam folder
//	if (isSpamEmail) {
//		for (auto it = folders.begin(); it != folders.end(); ++it) {
//			if ((*it).getFolderName() == "Spam") {
//				(*it).addEmail(e);
//				std::cout << "Added to spam\n";
//
//				// Update the file to mark as spam
//				std::ifstream inFile("email.txt");
//				if (!inFile.is_open()) {
//					std::cerr << RED << "Error opening email file!" << RESET << std::endl;
//					return;
//				}
//
//				std::vector<std::string> lines;
//				std::string line;
//
//				while (std::getline(inFile, line)) {
//					if (line.empty()) continue;
//
//					// Extract email ID
//					std::string lineEmailId = "";
//					for (int i = 0; i < line.size(); i++) {
//						if (line[i] == ',') {
//							break;
//						}
//						lineEmailId += line[i];
//					}
//
//					if (lineEmailId == e.emailId) {
//						// Parse the line
//						std::vector<std::string> fields;
//						std::string field = "";
//						for (char c : line) {
//							if (c == ',') {
//								fields.push_back(field);
//								field = "";
//							}
//							else {
//								field += c;
//							}
//						}
//						fields.push_back(field);
//
//						// Update isSpam field (index 7) and folder (index 9)
//						if (fields.size() >= 10) {
//							fields[7] = "true";  // Mark as spam
//							fields[9] = "Spam";  // Update folder
//
//							// Reconstruct the line
//							std::string updatedLine = "";
//							for (int i = 0; i < fields.size(); i++) {
//								updatedLine += fields[i];
//								if (i < fields.size() - 1) {
//									updatedLine += ",";
//								}
//							}
//							lines.push_back(updatedLine);
//						}
//					}
//					else {
//						lines.push_back(line);
//					}
//				}
//				inFile.close();
//
//				// Write back to file
//				std::ofstream outFile("email.txt");
//				if (!outFile.is_open()) {
//					std::cerr << RED << "Error opening email file for writing!" << RESET << std::endl;
//					return;
//				}
//
//				for (const std::string& l : lines) {
//					outFile << l << "\n";
//				}
//				outFile.close();
//
//				return;
//			}
//		}
//	}
//}
