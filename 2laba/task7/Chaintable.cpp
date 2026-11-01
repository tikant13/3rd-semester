#include <iostream>
#include <string>
#include "Chaintable.h"
using namespace std;

ChainTable::ChainTable(int startCapacity, bool grow){
    if (startCapacity < 1){
        startCapacity = 1;
    }
    capacity = startCapacity;
    autoGrow = grow;
    table = new linkedList[capacity]; // в каждой корзине пустой список
}

ChainTable::~ChainTable(){
    delete[] table; // деструктор каждого списка сам удалит его узлы
}

// h = h * 31 + символ, в конце берём остаток от деления на число корзин
int ChainTable::hash(const string& key){
    unsigned long long h = 0;
    for (int i = 0; i < (int)key.size(); ++i){
        h = h * 31 + (unsigned char)key[i];
    }
    return h % capacity;
}

// Идём по цепочке корзины b и ищем запись с нужным ключом
int ChainTable::findInBucket(int b, const string& key, string& entry){
    int n = table[b].Size();
    for (int i = 0; i < n; ++i){
        string e = table[b].At(i);
        int tab = e.find('\t');
        if (e.substr(0, tab) == key){
            entry = e;
            return i;
        }
    }
    return -1;
}

bool ChainTable::Insert(const string& key, const string& value){
    if (key.empty()){
        return false;
    }
    int b = hash(key);
    string entry;
    if (findInBucket(b, key, entry) != -1){ // ключ уже есть - меняем значение
        table[b].DelVal(entry);
        table[b].pushFront(key + "\t" + value);
        return false;
    }
    if (table[b].Size() > 0){
        ++collisions; // корзина уже занята другим ключом
    }
    table[b].pushFront(key + "\t" + value); // вставка в начало цепочки за O(1)
    ++count;
    if (autoGrow && count * 4 > capacity * 3){ // заполнено больше 75%
        rehash();
    }
    return true;
}

bool ChainTable::Get(const string& key, string& value){
    string entry;
    if (findInBucket(hash(key), key, entry) == -1){
        return false;
    }
    value = entry.substr(entry.find('\t') + 1);
    return true;
}

bool ChainTable::Remove(const string& key){
    int b = hash(key);
    string entry;
    if (findInBucket(b, key, entry) == -1){
        return false;
    }
    table[b].DelVal(entry);
    --count;
    return true;
}

// Номер корзины зависит от capacity, поэтому после увеличения все записи раскладываем заново
void ChainTable::rehash(){
    ++rehashes;
    int oldCapacity = capacity;
    linkedList* oldTable = table;

    capacity *= 2;
    table = new linkedList[capacity];
    for (int i = 0; i < oldCapacity; ++i){
        int n = oldTable[i].Size();
        for (int j = 0; j < n; ++j){
            string e = oldTable[i].At(0); // берём голову старого списка
            table[hash(e.substr(0, e.find('\t')))].pushFront(e); // кладём в новую корзину
            oldTable[i].delFront(); // и удаляем из старой
        }
    }
    delete[] oldTable;
}

int ChainTable::Size(){
    return count;
}

void ChainTable::Print(){
    if (count == 0){
        cout << "Пустая" << endl;
        return;
    }
    for (int b = 0; b < capacity; ++b){
        int n = table[b].Size();
        if (n == 0){
            continue;
        }
        cout << "[" << b << "]";
        for (int i = 0; i < n; ++i){
            string e = table[b].At(i);
            int tab = e.find('\t');
            cout << " -> " << e.substr(0, tab) << "=" << e.substr(tab + 1);
        }
        cout << endl;
    }
}

int ChainTable::Capacity(){
    return capacity;
}

double ChainTable::LoadFactor(){
    return (double)count / capacity;
}

long long ChainTable::Collisions(){
    return collisions;
}

int ChainTable::Rehashes(){
    return rehashes;
}

int ChainTable::MaxChain(){
    int best = 0;
    for (int b = 0; b < capacity; ++b){
        int n = table[b].Size();
        if (n > best){
            best = n;
        }
    }
    return best;
}

// Чтобы найти k-й элемент цепочки, нужно k сравнений.
// В цепочке длины L это 1 + 2 + ... + L = L(L+1)/2 сравнений на все её элементы
double ChainTable::AvgProbes(){
    if (count == 0){
        return 0;
    }
    long long total = 0;
    for (int b = 0; b < capacity; ++b){
        long long n = table[b].Size();
        total += n * (n + 1) / 2;
    }
    return (double)total / count;
}