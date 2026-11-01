#include <iostream>
#include <string>
#include "Fold.h"
#include "Chaintable.h"
#include "Cuckootable.h"

using namespace std;

int main(){
    ChainTable chain;
    CuckooTable cuckoo;

    cout << "Вводите ключи по одному в строке (пустая строка - конец):" << endl;
    string key;
    int number = 0;
    while (true){
        cout << "Ввод: ";
        if (!getline(cin, key) || key.empty()){
            break;
        }
        string expr;
        long long sum = foldHash(key, expr);
        cout << "Вывод: " << expr << endl;

        ++number;
        chain.Insert(key, to_string(number)); // значение - номер ключа по порядку
        cuckoo.Insert(key, to_string(number));
        cout << "Номер корзины в таблице цепочек: " << sum << " % " << chain.Capacity()
             << " = " << sum % chain.Capacity() << endl;
    }

    if (number > 0){
        cout << endl << "Таблица методом цепочек:" << endl;
        chain.Print();
        cout << "Таблица кукушки:" << endl;
        cuckoo.Print();
    }
    return 0;
}
