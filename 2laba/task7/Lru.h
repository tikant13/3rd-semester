#pragma once
#include <iostream>
#include <string>
#include "Chaintable.h"
#include "doublyList.h"
using namespace std;

// LRU кэш (Least Recently Used): при переполнении удаляется элемент,
// к которому дольше всего не обращались
struct LRUCache {
private:
    int capacity = 0; // ёмкость кэша
    ChainTable table; // хеш-таблица из задания 6: ключ -> значение
    DoublyList order; // ключи по времени обращения: голова - самый старый, хвост - самый свежий
    void touch(const string& key); // к ключу обратились - переносим его в хвост
public:
    LRUCache(int cap);
    void Set(const string& key, const string& value);
    string Get(const string& key); // значение или "-1", если ключа нет
    void Print();
};
