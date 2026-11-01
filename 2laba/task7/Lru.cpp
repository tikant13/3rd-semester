#include <iostream>
#include <string>
#include "Lru.h"
using namespace std;

LRUCache::LRUCache(int cap){
    capacity = cap;
}

void LRUCache::touch(const string& key){
    order.DelVal(key); // убираем ключ с его места
    order.pushBack(key); // и ставим в конец как самый свежий
}

void LRUCache::Set(const string& key, const string& value){
    if (capacity <= 0){
        return; // в кэш нулевой ёмкости ничего не положить
    }
    string old;
    if (table.Get(key, old)){ // ключ уже есть - перезаписываем значение
        table.Insert(key, value);
        touch(key);
        return;
    }
    if (table.Size() == capacity){ // кэш полон - удаляем самый старый ключ, он в голове списка
        string oldest = order.At(0);
        order.delFront();
        table.Remove(oldest);
    }
    table.Insert(key, value);
    order.pushBack(key);
}

string LRUCache::Get(const string& key){
    string value;
    if (!table.Get(key, value)){
        return "-1";
    }
    touch(key); // обращение к ключу делает его самым свежим
    return value;
}

void LRUCache::Print(){
    if (table.Size() == 0){
        cout << "Пустой" << endl;
        return;
    }
    for (int i = 0; i < order.Size(); ++i){ // от самого старого к самому свежему
        string key = order.At(i);
        string value;
        table.Get(key, value);
        cout << key << " -> " << value;
        if (i < order.Size() - 1){
            cout << ", ";
        }
    }
    cout << endl;
}
