#include <iostream>
#include <string>
#include "Lru.h"

using namespace std;

int main(){
    int cap, q;
    cout << "Введите ёмкость кэша и количество запросов: ";
    if (!(cin >> cap >> q) || cap < 0 || q < 0){
        cout << "Ошибка: нужны два неотрицательных числа" << endl;
        return 1;
    }

    LRUCache cache(cap); // ёмкость задаётся в конструкторе
    string result; // ответы на GET через пробел

    cout << "Введите запросы: ";
    for (int i = 0; i < q; ++i){
        string command;
        if (!(cin >> command)){
            cout << "Ошибка: запросов меньше, чем " << q << endl;
            return 1;
        }
        if (command == "SET"){
            string x, y;
            cin >> x >> y;
            cache.Set(x, y);
        } else if (command == "GET"){
            string x;
            cin >> x;
            if (!result.empty()){
                result += " ";
            }
            result += cache.Get(x);
        } else {
            cout << "Ошибка: неизвестная команда " << command << endl;
            return 1;
        }
    }

    cout << "Вывод: " << result << endl;
    cout << "Кэш в конце (от старого к свежему): ";
    cache.Print();
    return 0;
}