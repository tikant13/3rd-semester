#pragma once
#include <iostream>
#include <string>
#include "array.h"
using namespace std;

// Хеширование кукушки: две таблицы и две хеш-функции.
// Ключ может лежать только в T1[h1(key)] или в T2[h2(key)], поэтому поиск - максимум 2 проверки.
// Таблицы - массивы из ЛР1, пустая ячейка - пустая строка
struct CuckooTable {
private:
    Array* keys1 = nullptr; // первая таблица: ключи
    Array* vals1 = nullptr; // первая таблица: значения
    Array* keys2 = nullptr; // вторая таблица: ключи
    Array* vals2 = nullptr; // вторая таблица: значения
    int capacity = 0; // размер каждой из двух таблиц
    int count = 0; // количество пар ключ-значение
    bool autoGrow = true; // перестраивать ли таблицу при неудаче
    bool failed = false; // вставка не удалась (только при autoGrow = false)
    long long kicks = 0; // сколько раз выталкивали ключ из ячейки
    int rehashes = 0; // сколько раз перестраивали таблицу
    int hash(const string& key, int base); // полиномиальный хеш с основанием base
    int h1(const string& key); // первая хеш-функция
    int h2(const string& key); // вторая хеш-функция
    void allocate(int cap); // создать две пустые таблицы размера cap
    void freeTables();
    bool place(string& key, string& value); // вставка с выталкиванием, при неудаче в key/value остаётся лишняя пара
    void rehash(int newCap, const string& extraKey, const string& extraValue); // перестройка с увеличением размера
public:
    CuckooTable(int startCapacity = 8, bool grow = true);
    ~CuckooTable();
    bool Insert(const string& key, const string& value); // true - новый ключ, false - обновили значение
    bool Get(const string& key, string& value); // false, если ключа нет
    bool Remove(const string& key); // false, если ключа нет
    int Size();
    void Print();
    // статистика для анализа
    int Capacity(); // общее число ячеек в обеих таблицах
    double LoadFactor(); // коэффициент заполнения = элементов / ячеек
    long long Kicks();
    int Rehashes();
    bool Failed();
};