#pragma once 
#include <iostream>
#include <string>

using namespace std;

class DoublyList {
private:
    struct Node{
        string value;
        Node* next = nullptr;
        Node* prev = nullptr;
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    void clear();
public:
    DoublyList() = default;
    // Объект сам владеет памятью, поверхностное копирование запрещено.
    DoublyList(const DoublyList&) = delete;
    DoublyList& operator=(const DoublyList&) = delete;
    ~DoublyList();
    void pushFront(const string& value); 
    void pushBack(const string& value);
    void pushBefore(const string& tar, const string& value); //  добавить элемент до
    void pushAfter(const string& tar, const string& value); //  добавить элемент после
    void GetHead() const; // прочитать голову
    void GetTail() const; // прочитать хвост
    void GetIndx(const int& indx) const; // прочитать по indx
    void DelVal(const string& value); // удалить по значению 
    void delFront(); // удалить голову
    void delBack();  // удалить хвост
    void delBefore(const string& tar); // удалить элемент перед indx
    void delAfter(const string& indx); // удалить элемент после indx
    void GetVal(const string& value) const; // поиск элемента по значению
    void Print() const;
    int Size() const; // количество элементов
    string At(const int& indx) const; // вернуть элемент по индексу
    int Find(const string& value) const; // вернуть индекс, -1 если не найден
    string ToString() const; // все элементы через пробел — для записи в файл
};
