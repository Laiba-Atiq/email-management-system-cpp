#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<iostream>
#include "colors.h"

using namespace std;

template <typename T>
class node {
public:
    node<T>* next;
    T data;

    node(T d) {
        next = nullptr;
        this->data = d;
    }
};

template <typename T>
class linkedList {

    node<T>* head;
    node<T>* tail;

    bool isEmpty() {
        return head == NULL && tail == NULL;
    }
    int size() {
        node<T>* temp = head;
        int i = 0;
        while (temp != NULL) {
            temp = temp->next;
            i++;
        }
        return i;
    }

public:

    linkedList() {
        head = tail = NULL;
    }
    void InserAtbeginning(T data) {

        node<T>* n = new node<T>(data);

        if (isEmpty()) {
            head = tail = n;
            return;
        }

        n->next = head;
        head = n;
    }

    void InsertAtEnd(T data) {

        node<T>* n = new node<T>(data);

        if (isEmpty()) {
            head = tail = n;
            return;
        }

        tail->next = n;
        tail = n;
    }

    void InsertAtPosition(int pos, T data) {
        int sizee = size();
        if (pos <= 0 || pos > size() + 1) {
            std::cerr << "Invalid index";
            return;
        }
        if (pos == 0) {
            InserAtbeginning(data);
            return;
        }
        if (pos == size() + 1) {
            InsertAtEnd(data);
            return;
        }

        node<T>* n = new node<T>(data);

        int i = 1;
        node<T>* temp = head;
        while (i < pos - 1) {
            temp = temp->next;
            i++;
        }
        node<T>* temp1 = temp->next;
        temp->next = n;
        n->next = temp1;

    }

    void DeleteFromBeginning() {

        if (isEmpty()) {
            std::cerr << RED << "The list is already empty" << RESET;
            return;
        }

        else if (head->next == NULL) {
            delete head;
            head = tail = NULL;
            return;
        }

        node<T>* temp = head->next;
        delete head;
        head = temp;
    }

    void deleteFromEnd() {

        if (isEmpty()) {
            std::cerr << RED << "The list is already empty" << RESET;
            return;
        }

        else if (head->next == NULL) {
            delete head;
            head = tail = NULL;
            return;
        }

        node<T>* temp = head;
        while (temp->next->next != NULL) {
            temp = temp->next;
        }

        delete tail;
        tail = temp;
        tail->next = NULL;
    }

    void DeleteByValue(T value) {

        if (isEmpty()) {
            std::cerr << RED "The list is already empty" << RESET;
            return;
        }

        else if (value == head->data) {
            DeleteFromBeginning();
            return;
        }

        else if (value == tail->data) {
            deleteFromEnd();
            return;
        }

        node<T>* prev = head;
        node<T>* temp = head->next;

        while (temp != NULL) {

            if (temp->data == value) {
                prev->next = temp->next;
                delete temp;
                return;
            }
            temp = temp->next;
            prev = prev->next;
        }

        std::cerr << "The value to be deleted is not found in the list";

    }

    int search(T val) {

        if (isEmpty()) {
            std::cerr << RED << "The list is empty" << RESET;
            return -1;
        }

        node<T>* temp = head;
        int pos = 1;
        while (temp != NULL) {
            if (temp->data == val) {
                return pos;
            }
            pos++;
            temp = temp->next;
        }

        return -1;
    }

    void display() {
        if (isEmpty()) {
            std::cerr << RED << "The list is empty" << RESET;
            return;
        }

        node<T>* temp = head;
        int i = 0;

        while (temp != NULL) {
            i++;
            std::cout << i << " " << temp->data << "\n";
            temp = temp->next;
        }
        std::cout << "\n";
    }

    class Iterator {

        node<T>* current;

    public:
        Iterator(node<T>* n = nullptr) :current(n) {}

        bool operator!=(const Iterator& other) {
            return current != other.current;
        }

        Iterator& operator++() {
            current = current->next;
            return*this;
        }

        T& operator*() {
            return current->data;
        }
    };

    Iterator begin() {
        return Iterator(head);
    }
    Iterator end() {
        return Iterator(nullptr);
    }

    ~linkedList() {
        node<T>* temp = head;
        while (temp != NULL) {
            node<T>* next = temp->next;
            delete temp;
            temp = next;
        }
        head = tail = NULL;
    }

};


#endif 