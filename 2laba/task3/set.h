#pragma once
#include <iostream>
#include <string>
#include "linkedList.h"
using namespace std;

// Множество на хеш-таблице: массив корзин, каждая корзина это односвязный список
struct Set {
private:
    linkedList* table = nullptr; // массив корзин
    int capacity = 8; // количество корзин
    int count = 0; // количество элементов
    int hash(const string& value); // номер корзины для элемента
    void rehash(); // увеличение таблицы в 2 раза
public:
    Set();
    ~Set();
    bool add(const string& value); // false, если элемент уже был
    bool del(const string& value); // false, если элемента не было
    bool at(const string& value); // есть ли элемент во множестве
    int Size(); // количество элементов
    void Print();
    string ToString(); // все элементы через пробел для записи в файл
};