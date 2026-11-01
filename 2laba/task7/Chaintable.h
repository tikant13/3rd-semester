#pragma once
#include <iostream>
#include <string>
#include "linkedList.h"
using namespace std;

// Хеш-таблица методом цепочек: массив корзин, каждая корзина - односвязный список
// В списке хранится запись "ключ<TAB>значение"
struct ChainTable {
private:
    linkedList* table = nullptr; // массив корзин
    int capacity = 0; // количество корзин
    int count = 0; // количество пар ключ-значение
    bool autoGrow = true; // увеличивать ли таблицу при заполнении
    long long collisions = 0; // сколько раз новый ключ попал в непустую корзину
    int rehashes = 0; // сколько раз увеличивали таблицу
    int hash(const string& key); // номер корзины
    int findInBucket(int b, const string& key, string& entry); // позиция ключа в цепочке, -1 если нет
    void rehash(); // увеличение таблицы в 2 раза
public:
    ChainTable(int startCapacity = 8, bool grow = true);
    ~ChainTable();
    bool Insert(const string& key, const string& value); // true - новый ключ, false - обновили значение
    bool Get(const string& key, string& value); // false, если ключа нет
    bool Remove(const string& key); // false, если ключа нет
    int Size();
    void Print();
    // статистика для анализа
    int Capacity();
    double LoadFactor(); // коэффициент заполнения = элементов / корзин
    long long Collisions();
    int Rehashes();
    int MaxChain(); // длина самой длинной цепочки
    double AvgProbes(); // среднее число сравнений при успешном поиске
};