#ifndef MAXHEAP_H
#define MAXHEAP_H

#include <iostream>
#include <vector>
#include "colors.h"

using namespace std;

template<class T>
class maxHeap {

    std::vector<T> arr;      
    std::string mode;        

public:

    maxHeap(std::string m = "priority") {   
        mode = m;
    }

    bool isGreater(const T& a, const T& b) {
        if (mode == "priority") {
            return (a.priority - a.spamScore) > (b.priority - b.spamScore);
        }
        else if (mode == "timestamp") {
            return a.timestamp > b.timestamp;
        }

        return false;
    }

    void insert(T data) {
        arr.push_back(data);
        bubbleUp(arr.size() - 1);
    }

    void buid(T array[], int n) {
        for (int i = 0; i < n; i++)
            insert(array[i]);
    }

    void bubbleUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (isGreater(arr[index], arr[parent])) {
                std::swap(arr[index], arr[parent]);
                index = parent;
            }
            else break;
        }
    }

    void heapifyHelper(int i) {
        int size = arr.size();
        int largest = i;

        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && isGreater(arr[left], arr[largest]))
            largest = left;

        if (right < size && isGreater(arr[right], arr[largest]))
            largest = right;

        if (largest != i) {
            std::swap(arr[i], arr[largest]);
            heapifyHelper(largest);
        }
    }

    void heapify() {
        for (int i = (arr.size() / 2) - 1; i >= 0; i--)
            heapifyHelper(i);
    }

    void display() {
        for (auto& x : arr)
            std::cout << x << " ";
        std::cout << "\n";
    }
   
    void deletee() {
        if (arr.empty()) {
            std::cout << RED << "The heap is already empty" << RESET;
            return;
        }

        arr[0] = arr.back();
        arr.pop_back();

        if (!arr.empty())
            heapifyHelper(0);
    }

    void deleteByValue(T value) {
        if (arr.empty()) {
            std::cout << RED << "Heap is empty\n" << RESET;
            return;
        }

        int index = -1;
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] == value) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            std::cout << "Value not found in heap\n";
            return;
        }

        if (index == arr.size() - 1) {
            arr.pop_back();
            return;
        }

        arr[index] = arr.back();
        arr.pop_back();

        if (index < arr.size()) { // Check if index is still valid
            if (index > 0 && isGreater(arr[index], arr[(index - 1) / 2]))
                bubbleUp(index);
            else
                heapifyHelper(index);
        }
    }

    void heapSort() {
        std::vector<T> backup = arr;
        heapify();

        for (int i = arr.size() - 1; i > 0; i--) {
            std::swap(arr[0], arr[i]);
            int tempSize = i;
            heapifyDownSort(0, tempSize);
        }

        arr = backup;
    }

    void heapifyDownSort(int i, int heapSize) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < heapSize && isGreater(arr[left], arr[largest]))
            largest = left;

        if (right < heapSize && isGreater(arr[right], arr[largest]))
            largest = right;

        if (largest != i) {
            std::swap(arr[i], arr[largest]);
            heapifyDownSort(largest, heapSize);
        }
    }
};

#endif


 