#include <iostream>
#include <string>
#include "set.h"
using namespace std;

Set::Set(){
    table = new linkedList[capacity]; // в каждой корзине пустой список
}

Set::~Set(){
    delete[] table; // деструктор каждого списка сам удалит его узлы
}

// Полиномиальный хеш строки: h = h * 31 + символ, в конце берём остаток от деления на capacity
int Set::hash(const string& value){
    unsigned long long h = 0;
    for (int i = 0; i < (int)value.size(); ++i){
        h = h * 31 + (unsigned char)value[i];
    }
    return h % capacity;
}

// Увеличиваем таблицу в 2 раза. Номер корзины зависит от capacity,
// поэтому все элементы нужно разложить по новым корзинам заново
void Set::rehash(){
    int oldCapacity = capacity;
    linkedList* oldTable = table;

    capacity *= 2;
    table = new linkedList[capacity];

    for (int i = 0; i < oldCapacity; ++i){
        int k = oldTable[i].Size();
        for (int j = 0; j < k; ++j){
            string value = oldTable[i].At(0); // берём голову старого списка
            table[hash(value)].pushFront(value); // кладём в новую корзину
            oldTable[i].delFront(); // и удаляем из старой
        }
    }
    delete[] oldTable;
}

bool Set::add(const string& value){
    if (at(value)){
        return false; // во множестве нет повторов
    }
    table[hash(value)].pushFront(value); // вставка в начало списка за O(1)
    ++count;
    if (count * 4 > capacity * 3){ // заполнено больше 75%
        rehash();
    }
    return true;
}

bool Set::del(const string& value){
    if (!at(value)){
        return false;
    }
    table[hash(value)].DelVal(value); // удаление из списка по значению
    --count;
    return true;
}

// Ищем только в одной корзине: Find из ЛР1 вернёт -1, если элемента нет
bool Set::at(const string& value){
    return table[hash(value)].Find(value) != -1;
}

int Set::Size(){
    return count;
}

void Set::Print(){
    if (count == 0){
        cout << "Пустое" << endl;
        return;
    }
    cout << ToString() << endl;
}

// Склеиваем ToString всех непустых корзин
string Set::ToString(){
    string res;
    for (int i = 0; i < capacity; ++i){
        string part = table[i].ToString();
        if (!part.empty()){
            if (!res.empty()){
                res += " ";
            }
            res += part;
        }
    }
    return res;
}