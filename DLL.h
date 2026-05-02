//#ifndef DLL_H
//#define DLL_H
//
//#include <iostream>
//#include "Email.h"
//using namespace std;
//
//template <class T>
//class Node {
//public:
//    T data;
//    Node* next;
//    Node* prev;
//
//    Node(T d) : data(d), next(NULL), prev(NULL) {}
//
//    ~Node() {
//        next = prev = NULL;
//    }
//
//};
//
//template <class T>
//class DLL {
//
//    Node<T>* head;
//    Node<T>* tail;
//
//    bool isEmpty() {
//        return head == NULL;
//    }
//    int size() {
//        Node<T>* temp = head;
//        int i = 0;
//        while (temp != NULL) {
//            temp = temp->next;
//            i++;
//        }
//        return i;
//    }
//
//public:
//    DLL() {
//        head = NULL;
//        tail = NULL;
//    }
//
//    void InserAtbeginning(T data) {
//
//        Node<T>* n = new Node<T>(data);
//
//        if (isEmpty()) {
//            head = tail = n;
//            return;
//        }
//
//        n->next = head;
//        head->prev = n;
//        head = n;
//    }
//
//    void InsertAtEnd(T data) {
//
//        Node<T>* n = new Node<T>(data);
//
//        if (isEmpty()) {
//            head = tail = n;
//            return;
//        }
//
//        tail->next = n;
//        n->prev = tail;
//        tail = n;
//    }
//
//    void InsertAtPosition(int pos, T data) {
//        int sizee = size();
//        if (pos <= 0 || pos > size() + 1) {
//            std::cerr << "Invalid index";
//            return;
//        }
//        if (pos == 0) {
//            InserAtbeginning(data);
//            return;
//        }
//        if (pos == size()) {
//            InsertAtEnd(data);
//            return;
//        }
//
//        Node<T>* n = new Node<T>(data);
//
//        int i = 1;
//        Node<T>* temp = head;
//        while (i < pos - 1) {
//            temp = temp->next;
//            i++;
//        }
//        Node<T>* temp1 = temp->next;
//
//        temp->next = n;
//        n->prev = temp;
//
//        n->next = temp1;
//        temp1->prev = n;
//
//    }
//
//    void DisplayForward() {
//        if (isEmpty()) {
//            std::cout << "The list is emty and there is nothing to be displayed\n";
//            return;
//        }
//        Node<T>* temp = head;
//        int i = 0;
//        while (temp != NULL) {
//            i++;
//            std::cout << i << " " << temp->data << "\n";
//            temp = temp->next;
//        }
//        std::cout << "NULL\n";
//    }
//
//    void DisplayBackward() {
//        if (isEmpty()) {
//            std::cout << "The list is empty and there is nothing to be displayed\n";
//            return;
//        }
//        Node<T>* temp = tail;
//        while (temp != NULL) {
//            std::cout << temp->data << " -> ";
//            temp = temp->prev;
//        }
//        std::cout << "NULL\n";
//    }
//
//    T DeleteFromBeginning() {
//
//        if (isEmpty()) {
//            std::cerr << "The list is already empty";
//            return T();
//        }
//
//        else if (head->next == NULL) {
//            T val = head->data;
//            delete head;
//            head = tail = NULL;
//            return val;
//        }
//
//        Node<T>* temp = head->next;
//        T val = head->data;
//        delete head;
//        head = temp;
//        temp->prev = NULL;
//        return val;
//    }
//
//    T deleteFromEnd() {
//
//        if (isEmpty()) {
//            std::cerr << "The list is already empty";
//            return T();
//        }
//
//        else if (head->next == NULL) {
//            T val = head->data;
//            delete head;
//            head = tail = NULL;
//            return val;
//        }
//
//        Node<T>* temp = tail->prev;
//        T val = tail->data;
//        delete tail;
//        tail = temp;
//        temp->next = NULL;
//        return val;
//    }
//
//    T DeleteByValue(T value) {
//
//        if (isEmpty()) {
//            std::cerr << "The list is already empty";
//            return T();
//        }
//
//        else if (value == head->data) {
//            return DeleteFromBeginning();
//        }
//
//        else if (value == tail->data) {
//            return deleteFromEnd();
//        }
//
//        Node<T>* temp = search(value);
//
//
//        if (temp) {
//            temp->prev->next = temp->next;
//            temp->next->prev = temp->prev;
//
//            T val = temp->data;
//            delete temp;
//            return val;
//        }
//
//        std::cerr << "The value to be deleted is not found in the list";
//        return T();
//
//    }
//
//    Node<T>* search(T val) {
//
//        if (isEmpty()) {
//            std::cerr << "The list is empty";
//            return nullptr;
//        }
//
//        Node<T>* temp = head;
//        int pos = 0;
//        while (temp != NULL) {
//            if (temp->data == val) {
//                std::cout << val << " is found at position : " << pos << "\n";
//                return temp;
//            }
//            pos++;
//            temp = temp->next;
//        }
//
//        std::cout << val << " isn't found in the list\n";
//    }
//
//    Node<T>* getTail() {
//        return tail;
//    }
//
//    class iterator {
//        Node<T>* ptr;
//    public:
//        iterator(Node<T>* p = nullptr) : ptr(p) {}
//
//        T& operator*() { return ptr->data; }
//
//        iterator& operator++() {
//            if (ptr) ptr = ptr->next;
//            return *this;
//        }
//
//        iterator& operator--() {
//            if (ptr) ptr = ptr->prev;
//            return *this;
//        }
//
//        bool operator==(const iterator& other) const {
//            return ptr == other.ptr;
//        }
//        bool operator!=(const iterator& other) const {
//            return ptr != other.ptr;
//        }
//    };
//
//    iterator begin() { return iterator(head); }
//    iterator end() { return iterator(nullptr); }
//
//    ~DLL() {
//        if (head != nullptr) {
//            Node<T>* temp = head;
//            while (temp != nullptr) {
//                Node<T>* n = temp->next;
//                delete temp;
//                temp = n;
//            }
//
//        }
//    }
//};
//
//#endif 

#ifndef DLL_H
#define DLL_H

#include <iostream>
#include <stdexcept>
#include "colors.h"
#include "Email.h"
using namespace std;

template <class T>
class Node {
public:
    T data;
    Node* next;
    Node* prev;

    Node(T d) : data(d), next(NULL), prev(NULL) {}

    ~Node() {
        next = prev = NULL;
    }
};

template <class T>
class DLL {

    Node<T>* head;
    Node<T>* tail;

    bool isEmpty() {
        return head == NULL;
    }

    int size() {
        Node<T>* temp = head;
        int i = 0;
        while (temp != NULL) {
            temp = temp->next;
            i++;
        }
        return i;
    }

public:
    DLL() {
        head = NULL;
        tail = NULL;
    }

    void InserAtbeginning(T data) {
        Node<T>* n = new Node<T>(data);

        if (isEmpty()) {
            head = tail = n;
            return;
        }

        n->next = head;
        head->prev = n;
        head = n;
    }

    void InsertAtEnd(T data) {
        Node<T>* n = new Node<T>(data);

        if (isEmpty()) {
            head = tail = n;
            return;
        }

        tail->next = n;
        n->prev = tail;
        tail = n;
    }

    void InsertAtPosition(int pos, T data) {
        int sizee = size();
        if (pos <= 0 || pos > size() + 1) {
            throw std::out_of_range("Invalid index");
        }
        if (pos == 0) {
            InserAtbeginning(data);
            return;
        }
        if (pos == size()) {
            InsertAtEnd(data);
            return;
        }

        Node<T>* n = new Node<T>(data);

        int i = 1;
        Node<T>* temp = head;
        while (i < pos - 1) {
            temp = temp->next;
            i++;
        }
        Node<T>* temp1 = temp->next;

        temp->next = n;
        n->prev = temp;

        n->next = temp1;
        temp1->prev = n;
    }

    void DisplayForward() {
        if (isEmpty()) {
            std::cout << RED << "The list is empty and there is nothing to be displayed\n" << RESET;
            return;
        }
        Node<T>* temp = head;
        int i = 0;
        while (temp != NULL) {
            i++;
            std::cout << i << " " << temp->data << "\n";
            temp = temp->next;
        }
    }

    void DisplayBackward() {
        if (isEmpty()) {
            std::cout << RED << "The list is empty and there is nothing to be displayed\n" << RESET;
            return;
        }
        Node<T>* temp = tail;
        while (temp != NULL) {
            std::cout << temp->data << " -> ";
            temp = temp->prev;
        }
    }

    T DeleteFromBeginning() {
        if (isEmpty()) {
            throw std::runtime_error("The list is already empty");
        }

        else if (head->next == NULL) {
            T val = head->data;
            delete head;
            head = tail = NULL;
            return val;
        }

        Node<T>* temp = head->next;
        T val = head->data;
        delete head;
        head = temp;
        temp->prev = NULL;
        return val;
    }

    T deleteFromEnd() {
        if (isEmpty()) {
            throw std::runtime_error("The list is already empty");
        }

        else if (head->next == NULL) {
            T val = head->data;
            delete head;
            head = tail = NULL;
            return val;
        }

        Node<T>* temp = tail->prev;
        T val = tail->data;
        delete tail;
        tail = temp;
        temp->next = NULL;
        return val;
    }

    T DeleteByValue(T value) {
        if (isEmpty()) {
            throw std::runtime_error("The list is already empty");
        }

        else if (value == head->data) {
            return DeleteFromBeginning();
        }

        else if (value == tail->data) {
            return deleteFromEnd();
        }

        Node<T>* temp = head;
        while (temp != NULL) {
            if (temp->data == value) {
                // Found it - now safely delete
                if (temp->prev) {
                    temp->prev->next = temp->next;
                }
                if (temp->next) {
                    temp->next->prev = temp->prev;
                }

                T val = temp->data;
                delete temp;
                return val;
            }
            temp = temp->next;
        }

        // Value not found - throw exception
        throw std::runtime_error("The value to be deleted is not found in the list");
    }

    Node<T>* search(T val) {
        if (isEmpty()) {
            throw std::runtime_error("The list is empty");
        }

        Node<T>* temp = head;
        int pos = 0;
        while (temp != NULL) {
            if (temp->data == val) {
                std::cout << val << " is found at position : " << pos << "\n";
                return temp;
            }
            pos++;
            temp = temp->next;
        }

        // Not found - throw exception
        throw std::runtime_error("Value not found in the list");
    }

    Node<T>* getTail() {
        return tail;
    }

    class iterator {
        Node<T>* ptr;
    public:
        iterator(Node<T>* p = nullptr) : ptr(p) {}

        T& operator*() { return ptr->data; }

        iterator& operator++() {
            if (ptr) ptr = ptr->next;
            return *this;
        }

        iterator& operator--() {
            if (ptr) ptr = ptr->prev;
            return *this;
        }

        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    ~DLL() {
        if (head != nullptr) {
            Node<T>* temp = head;
            while (temp != nullptr) {
                Node<T>* n = temp->next;
                delete temp;
                temp = n;
            }
        }
    }
};

#endif