#include "array.h"
#include <utility>
#include <iostream>
#include <string>
#include "Chaintable.h"
using namespace std;

ChainTable::ChainTable(int startCapacity){
    if (startCapacity < 1){
        startCapacity = 1;
    }
    capacity = startCapacity;
    table = new linkedList[capacity]; // в каждой корзине пустой список
}

ChainTable::~ChainTable(){
    delete[] table; // деструктор каждого списка сам удалит его узлы
}

// h = h * 31 + символ, в конце берём остаток от деления на число корзин
int ChainTable::hash(const string& key) const{
    unsigned long long h = 0;
    for (int i = 0; i < (int)key.size(); ++i){
        h = h * 31 + (unsigned char)key[i];
    }
    return h % capacity;
}

// Идём по цепочке корзины b и ищем запись с нужным ключом
int ChainTable::findInBucket(int b, const string& key, string& entry) const{
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
    table[b].pushFront(key + "\t" + value); // вставка в начало цепочки за O(1)
    ++count;
    if (count * 4 > capacity * 3){ // заполнено больше 75%
        rehash();
    }
    return true;
}

bool ChainTable::Get(const string& key, string& value) const{
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

int ChainTable::Size() const{
    return count;
}

int ChainTable::Capacity() const{
    return capacity;
}

void ChainTable::Print() const{
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

string ChainTable::ToString() const{
    string res;
    for (int b = 0; b < capacity; ++b){
        for (int i = 0; i < table[b].Size(); ++i){
            string entry = table[b].At(i);
            size_t tab = entry.find('\t');
            if (!res.empty()) res += " ";
            res += entry.substr(0, tab) + " " + entry.substr(tab + 1);
        }
    }
    return res;
}

// Сохраняем элементы в том же порядке, в котором ими пользуется структура.
// Первый элемент — обозначение типа, чтобы не загрузить файл другого класса.
bool ChainTable::saveToFile(const string& fileName) const {
    Array data;
    data.PushBack("H");
    for (int b = 0; b < capacity; ++b) {
        for (int i = 0; i < table[b].Size(); ++i) {
            string entry = table[b].At(i);
            size_t pos = entry.find('	');
            data.PushBack(entry.substr(0, pos));
            data.PushBack(entry.substr(pos + 1));
        }
    }
    return data.saveToFile(fileName);
}

bool ChainTable::loadFromFile(const string& fileName) {
    Array data;
    if (!data.loadFromFile(fileName) || data.Size() < 1 || data.At(0) != "H") return false;
    if ((data.Size() - 1) % 2 != 0) return false;
    ChainTable loaded;
    for (int i = 1; i < data.Size(); i += 2) {
        if (data.At(i).empty() || !loaded.Insert(data.At(i), data.At(i + 1))) return false;
    }
    swap(table, loaded.table);
    swap(capacity, loaded.capacity);
    swap(count, loaded.count);
    return true;
}

bool ChainTable::saveBinary(const string& fileName) const {
    Array data;
    data.PushBack("H");
    for (int b = 0; b < capacity; ++b) {
        for (int i = 0; i < table[b].Size(); ++i) {
            string entry = table[b].At(i);
            size_t pos = entry.find('	');
            data.PushBack(entry.substr(0, pos));
            data.PushBack(entry.substr(pos + 1));
        }
    }
    return data.saveBinary(fileName);
}

bool ChainTable::loadBinary(const string& fileName) {
    Array data;
    if (!data.loadBinary(fileName) || data.Size() < 1 || data.At(0) != "H") return false;
    if ((data.Size() - 1) % 2 != 0) return false;
    ChainTable loaded;
    for (int i = 1; i < data.Size(); i += 2) {
        if (data.At(i).empty() || !loaded.Insert(data.At(i), data.At(i + 1))) return false;
    }
    swap(table, loaded.table);
    swap(capacity, loaded.capacity);
    swap(count, loaded.count);
    return true;
}
