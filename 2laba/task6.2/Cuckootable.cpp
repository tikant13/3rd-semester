#include <iostream>
#include <string>
#include "Cuckootable.h"
#include "Fold.h"
using namespace std;

const int MAX_KICKS = 100; // сколько раз можно выталкивать подряд, потом считаем, что попали в цикл

CuckooTable::CuckooTable(int startCapacity){
    if (startCapacity < 1){
        startCapacity = 1;
    }
    allocate(startCapacity);
}

CuckooTable::~CuckooTable(){
    freeTables();
}

void CuckooTable::allocate(int cap){
    capacity = cap;
    keys1 = new Array;
    vals1 = new Array;
    keys2 = new Array;
    vals2 = new Array;
    for (int i = 0; i < cap; ++i){ // заполняем пустыми ячейками
        keys1->PushBack("");
        vals1->PushBack("");
        keys2->PushBack("");
        vals2->PushBack("");
    }
}

void CuckooTable::freeTables(){
    delete keys1;
    delete vals1;
    delete keys2;
    delete vals2;
    keys1 = vals1 = keys2 = vals2 = nullptr;
}

// h = h * base + символ, в конце остаток от деления на размер таблицы
int CuckooTable::hash(const string& key, int base){
    unsigned long long h = 0;
    for (int i = 0; i < (int)key.size(); ++i){
        h = h * base + (unsigned char)key[i];
    }
    return h % capacity;
}

// Первая хеш-функция - метод свёртки, вторая - полиномиальный хеш.
// Функции должны быть разными, иначе у ключа было бы одно место вместо двух
int CuckooTable::h1(const string& key){
    return foldHash(key) % capacity;
}

int CuckooTable::h2(const string& key){
    return hash(key, 37);
}

// Кладём пару в T1. Если ячейка занята, «выталкиваем» старую пару (как кукушонок яйца из гнезда)
// и пробуем положить её в T2 на её место h2. Если и там занято - выталкиваем оттуда в T1, и так далее
bool CuckooTable::place(string& key, string& value){
    for (int step = 0; step < MAX_KICKS; ++step){
        int i = h1(key);
        if (keys1->At(i).empty()){
            keys1->Set(i, key);
            vals1->Set(i, value);
            return true;
        }
        string oldKey = keys1->At(i); // ячейка занята: занимаем её, а старую пару несём дальше
        string oldValue = vals1->At(i);
        keys1->Set(i, key);
        vals1->Set(i, value);
        key = oldKey;
        value = oldValue;

        int j = h2(key);
        if (keys2->At(j).empty()){
            keys2->Set(j, key);
            vals2->Set(j, value);
            return true;
        }
        oldKey = keys2->At(j);
        oldValue = vals2->At(j);
        keys2->Set(j, key);
        vals2->Set(j, value);
        key = oldKey;
        value = oldValue;
    }
    return false; // зациклились, в key/value осталась пара без места
}

// Перестройка: собираем все пары, создаём таблицы размера newCap и вставляем заново.
// Хеш-функции берут остаток от деления на размер, поэтому с новым размером ключи разложатся по-другому.
// Если и так не получилось - увеличиваем ещё в 2 раза
void CuckooTable::rehash(int newCap, const string& extraKey, const string& extraValue){
    Array allKeys;
    Array allValues;
    for (int i = 0; i < capacity; ++i){
        if (!keys1->At(i).empty()){
            allKeys.PushBack(keys1->At(i));
            allValues.PushBack(vals1->At(i));
        }
        if (!keys2->At(i).empty()){
            allKeys.PushBack(keys2->At(i));
            allValues.PushBack(vals2->At(i));
        }
    }
    if (!extraKey.empty()){
        allKeys.PushBack(extraKey);
        allValues.PushBack(extraValue);
    }
    freeTables();

    int cap = newCap;
    while (true){
        allocate(cap);
        bool ok = true;
        for (int i = 0; i < allKeys.Size(); ++i){
            string k = allKeys.At(i);
            string v = allValues.At(i);
            if (!place(k, v)){
                ok = false;
                break;
            }
        }
        if (ok){
            return;
        }
        freeTables(); // не влезло - пробуем таблицу побольше
        cap *= 2;
    }
}

bool CuckooTable::Insert(const string& key, const string& value){
    if (key.empty()){
        return false; // пустая строка обозначает пустую ячейку
    }
    int i = h1(key);
    if (keys1->At(i) == key){ // ключ уже есть - меняем значение
        vals1->Set(i, value);
        return false;
    }
    int j = h2(key);
    if (keys2->At(j) == key){
        vals2->Set(j, value);
        return false;
    }

    // при заполнении больше 45% кукушка начинает часто зацикливаться, поэтому увеличиваем заранее
    if ((count + 1) * 100 > capacity * 2 * 45){
        rehash(capacity * 2, "", "");
    }

    string k = key;
    string v = value;
    if (!place(k, v)){
        rehash(capacity * 2, k, v); // перестраиваем вместе с парой, оставшейся без места
    }
    ++count;
    return true;
}

bool CuckooTable::Get(const string& key, string& value){
    if (key.empty()){
        return false;
    }
    int i = h1(key);
    if (keys1->At(i) == key){
        value = vals1->At(i);
        return true;
    }
    int j = h2(key);
    if (keys2->At(j) == key){
        value = vals2->At(j);
        return true;
    }
    return false;
}

bool CuckooTable::Remove(const string& key){
    if (key.empty()){
        return false;
    }
    int i = h1(key);
    if (keys1->At(i) == key){
        keys1->Set(i, "");
        vals1->Set(i, "");
        --count;
        return true;
    }
    int j = h2(key);
    if (keys2->At(j) == key){
        keys2->Set(j, "");
        vals2->Set(j, "");
        --count;
        return true;
    }
    return false;
}

int CuckooTable::Size(){
    return count;
}

void CuckooTable::Print(){
    if (count == 0){
        cout << "Пустая" << endl;
        return;
    }
    for (int i = 0; i < capacity; ++i){
        if (!keys1->At(i).empty()){
            cout << "T1[" << i << "] " << keys1->At(i) << "=" << vals1->At(i) << endl;
        }
    }
    for (int i = 0; i < capacity; ++i){
        if (!keys2->At(i).empty()){
            cout << "T2[" << i << "] " << keys2->At(i) << "=" << vals2->At(i) << endl;
        }
    }
}
