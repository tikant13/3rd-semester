#pragma once
#include <iostream>
#include <string>
#include "linkedList.h"
using namespace std;

// Хеш-таблица методом цепочек: массив корзин, каждая корзина - односвязный список.
// В списке хранится запись "ключ<TAB>значение"
class ChainTable {
private:
    linkedList* table = nullptr; // массив корзин
    int capacity = 0; // количество корзин
    int count = 0; // количество пар ключ-значение
    int hash(const string& key) const; // номер корзины
    int findInBucket(int b, const string& key, string& entry) const; // позиция ключа в цепочке, -1 если нет
    void rehash(); // увеличение таблицы в 2 раза
public:
    // Объект сам владеет памятью, поверхностное копирование запрещено.
    ChainTable(const ChainTable&) = delete;
    ChainTable& operator=(const ChainTable&) = delete;
    ChainTable(int startCapacity = 8);
    ~ChainTable();
    bool Insert(const string& key, const string& value); // true - новый ключ, false - обновили значение
    bool Get(const string& key, string& value) const; // false, если ключа нет
    bool Remove(const string& key); // false, если ключа нет
    int Size() const; // количество элементов
    int Capacity() const; // количество корзин
    string ToString() const; // пары ключ значение для записи в файл
    void Print() const;
};
