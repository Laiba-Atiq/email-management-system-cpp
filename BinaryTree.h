#ifndef BINARYTREE_H
#define BINARYTREE_H

#include<iostream>
#include<vector>
#include "colors.h"

using namespace std;

template <class T>
class Bnode {
public:
    T data;
    Bnode* lchild;
    Bnode* rchild;

    Bnode(T d) {
        data = d;
        lchild = rchild = NULL;
    }
};

template <class T>
class BST {

    Bnode<T>* root;

    void inOrderHelper(Bnode<T>* temp) {

        if (!temp) {
            return;
        }
        inOrderHelper(temp->lchild);
        std::cout << temp->data << "\n";
        inOrderHelper(temp->rchild);
    }

    Bnode<T>* Insert(Bnode<T>* temp, T d) {

        if (!temp) {
            Bnode<T>* n = new Bnode<T>(d);
            return n;
        }

        else if (d < temp->data) {
            temp->lchild = Insert(temp->lchild, d);
        }
        else if (d > temp->data) {
            temp->rchild = Insert(temp->rchild, d);
        }
        else {
            return temp;
        }
        return temp;
    }

    void searchByName(Bnode<T>* temp, T d, std::vector<T>& ans) {

        if (!temp)
            return;

        if (temp->data.hasSameName(d)) {
            // Found a match
            ans.push_back(temp->data);

            //search both sides for more matches for same mail
            searchByName(temp->lchild, d, ans);
            searchByName(temp->rchild, d, ans);
        }

        else if (d < temp->data) {
            searchByName(temp->lchild, d, ans);
        }
        else if (d > temp->data) {
            searchByName(temp->rchild, d, ans);
        }
        
    }

    void searchByEmail(Bnode<T>* temp, T d, std::vector<T>& ans) {

        if (!temp)
            return;
        if (temp->data.hasSameEmail(d)) {
            ans.push_back(temp->data);
            return;
        }
       
        searchByEmail(temp->lchild, d, ans);
        
        searchByEmail(temp->rchild, d, ans);
        
    }

    int countNodesHelper(Bnode<T>* temp) {
        if (!temp) {
            return 0;
        }
        return 1 + countNodesHelper(temp->lchild) + countNodesHelper(temp->rchild);
    }

    bool updateHelper(Bnode<T>* temp, T& d, std::string parameter) {
        if (!temp)
            return false;

        if (parameter == "email") {
            if (temp->data.hasSameEmail(d)) {
                temp->data = d;
                return true;
            }
            updateHelper(temp->lchild, d, parameter);
            updateHelper(temp->rchild, d, parameter);
        }
        return false;
    }
public:
    BST() {
        root = NULL;
    }

    void Insert(T d) {
        root = Insert(root, d);
    }

    std::vector<T> Search(T d, std::string parameter) {
        std::vector<T> ans;

        if (parameter == "name") {
            searchByName(root, d, ans);
        }
        else if (parameter == "email") {
            searchByEmail(root, d, ans);
        }
        return ans;

    }

    void inOrder() {
        std::cout << "inOrder display:\n\n";
        inOrderHelper(root);
        std::cout << "\n";
    }

    void del(Bnode<T>* temp) {
        if (!temp) return;

        del(temp->lchild);
        del(temp->rchild);
        delete temp;
    }

    int countNodes() {
        return countNodesHelper(root);
    }

    bool Update(T& d,  std::string parameter) {
        return updateHelper(root, d, parameter);
    }

    ~BST() {
        Bnode<T>* temp = root;
        del(temp);
    }
};
#endif 
