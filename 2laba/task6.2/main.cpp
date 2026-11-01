#include <iostream>
#include <string>
#include <cstdlib>
#include "Fold.h"
#include "Chaintable.h"
#include "Cuckootable.h"

using namespace std;

// Проверка на 10000 случайных 9-значных ключах: всё ли находится и как распределились ключи
void testRandom(){
    const int N = 10000;
    ChainTable chain;
    CuckooTable cuckoo;
    string* keys = new string[N];
    srand(1);
    for (int i = 0; i < N; ++i){
        long long num = 100000000 + (long long)(rand() % 30000) * 30000 + rand() % 30000; // 9-значное число
        keys[i] = to_string(num);
        chain.Insert(keys[i], to_string(i));
        cuckoo.Insert(keys[i], to_string(i));
    }

    int found = 0;
    string value;
    for (int i = 0; i < N; ++i){
        if (chain.Get(keys[i], value) && cuckoo.Get(keys[i], value)){
            ++found;
        }
    }

    cout << "Проверка на " << N << " случайных 9-значных ключах:" << endl;
    cout << "найдено в обеих таблицах: " << found << " из " << N << endl;
    cout << "цепочки: корзин " << chain.Capacity() << ", заполнение " << chain.LoadFactor()
         << ", макс. цепочка " << chain.MaxChain() << ", сравнений при поиске " << chain.AvgProbes() << endl;
    cout << "кукушка: ячеек " << cuckoo.Capacity() << ", заполнение " << cuckoo.LoadFactor()
         << ", вытеснений " << cuckoo.Kicks() << ", перестроек " << cuckoo.Rehashes() << endl;
    delete[] keys;
}

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

    cout << endl;
    testRandom();
    return 0;
}