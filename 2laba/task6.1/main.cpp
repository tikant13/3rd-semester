#include <iostream>
#include <string>
#include <ctime>
#include "Chaintable.h"
#include "Cuckootable.h"

using namespace std;

// Сколько миллисекунд прошло с момента start
double msSince(clock_t start){
    return (double)(clock() - start) * 1000 / CLOCKS_PER_SEC;
}

// Демонстрация операций
void demo(){
    ChainTable chain(4);
    CuckooTable cuckoo(4);
    string keys[] = {"apple", "banana", "cherry", "grape", "lemon"};
    string vals[] = {"red", "yellow", "red", "green", "yellow"};
    for (int i = 0; i < 5; ++i){
        chain.Insert(keys[i], vals[i]);
        cuckoo.Insert(keys[i], vals[i]);
    }
    chain.Insert("apple", "green"); // обновление значения
    cuckoo.Insert("apple", "green");
    chain.Remove("cherry");
    cuckoo.Remove("cherry");

    string v;
    cout << "Метод цепочек:" << endl;
    if (chain.Get("apple", v)){
        cout << "apple = " << v << endl;
    }
    if (!chain.Get("cherry", v)){
        cout << "cherry удалён" << endl;
    }
    chain.Print();

    cout << "Кукушка:" << endl;
    if (cuckoo.Get("apple", v)){
        cout << "apple = " << v << endl;
    }
    if (!cuckoo.Get("cherry", v)){
        cout << "cherry удалён" << endl;
    }
    cuckoo.Print();
    cout << endl;
}

// Замер метода цепочек: вставка, поиск и удаление n ключей
void testChain(int n){
    ChainTable t;
    string value;

    clock_t start = clock();
    for (int i = 0; i < n; ++i){
        t.Insert("user" + to_string(i), to_string(i));
    }
    double insertTime = msSince(start);

    start = clock();
    for (int i = 0; i < n; ++i){
        t.Get("user" + to_string(i), value);
    }
    double getTime = msSince(start);

    start = clock();
    for (int i = 0; i < n; ++i){
        t.Remove("user" + to_string(i));
    }
    double removeTime = msSince(start);

    cout << "цепочки\t" << n << "\t" << insertTime << "\t" << getTime << "\t" << removeTime << endl;
}

// Тот же замер для кукушки
void testCuckoo(int n){
    CuckooTable t;
    string value;

    clock_t start = clock();
    for (int i = 0; i < n; ++i){
        t.Insert("user" + to_string(i), to_string(i));
    }
    double insertTime = msSince(start);

    start = clock();
    for (int i = 0; i < n; ++i){
        t.Get("user" + to_string(i), value);
    }
    double getTime = msSince(start);

    start = clock();
    for (int i = 0; i < n; ++i){
        t.Remove("user" + to_string(i));
    }
    double removeTime = msSince(start);

    cout << "кукушка\t" << n << "\t" << insertTime << "\t" << getTime << "\t" << removeTime << endl;
}

int main(){
    demo();

    cout << "Время операций, мс:" << endl;
    cout << "метод\tN\tвставка\tпоиск\tудаление" << endl;
    int sizes[] = {1000, 10000, 100000};
    for (int s = 0; s < 3; ++s){
        testChain(sizes[s]);
        testCuckoo(sizes[s]);
    }
    return 0;
}
