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

// Небольшая демонстрация операций
void demo(){
    cout << "===== Демонстрация =====" << endl;
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

// Замер метода цепочек на n ключах: вставка, поиск есть/нет, удаление
void testChain(int n){
    ChainTable t;
    string value;
    bool ok = true;

    clock_t start = clock();
    for (int i = 0; i < n; ++i){
        t.Insert("user" + to_string(i), to_string(i));
    }
    double insertTime = msSince(start);

    start = clock();
    for (int i = 0; i < n; ++i){
        if (!t.Get("user" + to_string(i), value) || value != to_string(i)){
            ok = false;
        }
    }
    double hitTime = msSince(start);

    start = clock();
    for (int i = 0; i < n; ++i){
        if (t.Get("none" + to_string(i), value)){
            ok = false;
        }
    }
    double missTime = msSince(start);

    cout << "цепочки\t" << n << "\t" << insertTime << "\t" << hitTime << "\t" << missTime;
    // статистику смотрим до удаления, пока таблица заполнена
    cout << "\t(ячеек " << t.Capacity() << ", заполнение " << t.LoadFactor()
         << ", коллизий " << t.Collisions() << ", перестроек " << t.Rehashes()
         << ", макс. цепочка " << t.MaxChain() << ")";

    start = clock();
    for (int i = 0; i < n; ++i){
        if (!t.Remove("user" + to_string(i))){
            ok = false;
        }
    }
    double removeTime = msSince(start);
    if (t.Size() != 0){
        ok = false;
    }
    cout << "\tудаление " << removeTime << (ok ? "" : "\tОШИБКА") << endl;
}

// Тот же замер для кукушки
void testCuckoo(int n){
    CuckooTable t;
    string value;
    bool ok = true;

    clock_t start = clock();
    for (int i = 0; i < n; ++i){
        t.Insert("user" + to_string(i), to_string(i));
    }
    double insertTime = msSince(start);

    start = clock();
    for (int i = 0; i < n; ++i){
        if (!t.Get("user" + to_string(i), value) || value != to_string(i)){
            ok = false;
        }
    }
    double hitTime = msSince(start);

    start = clock();
    for (int i = 0; i < n; ++i){
        if (t.Get("none" + to_string(i), value)){
            ok = false;
        }
    }
    double missTime = msSince(start);

    cout << "кукушка\t" << n << "\t" << insertTime << "\t" << hitTime << "\t" << missTime;
    cout << "\t(ячеек " << t.Capacity() << ", заполнение " << t.LoadFactor()
         << ", вытеснений " << t.Kicks() << ", перестроек " << t.Rehashes() << ")";

    start = clock();
    for (int i = 0; i < n; ++i){
        if (!t.Remove("user" + to_string(i))){
            ok = false;
        }
    }
    double removeTime = msSince(start);
    if (t.Size() != 0){
        ok = false;
    }
    cout << "\tудаление " << removeTime << (ok ? "" : "\tОШИБКА") << endl;
}

// Эксперимент 1: время операций при росте числа элементов (таблицы сами увеличиваются)
void experimentTime(){
    cout << "===== Эксперимент 1: время операций, мс =====" << endl;
    cout << "метод\tN\tвставка\tпоиск+\tпоиск-" << endl;
    int sizes[] = {1000, 10000, 100000};
    for (int s = 0; s < 3; ++s){
        testChain(sizes[s]);
        testCuckoo(sizes[s]);
    }
    cout << endl;
}

// Эксперимент 2: метод цепочек при фиксированном числе корзин и разном заполнении
void experimentChainLoad(){
    cout << "===== Эксперимент 2: цепочки без увеличения таблицы (1000 корзин) =====" << endl;
    cout << "заполнение\tсравнений\tтеория 1+a/2\tмакс. цепочка" << endl;
    double loads[] = {0.25, 0.5, 0.75, 1, 2, 4};
    for (int l = 0; l < 6; ++l){
        ChainTable t(1000, false);
        int n = loads[l] * 1000;
        for (int i = 0; i < n; ++i){
            t.Insert("user" + to_string(i), "v");
        }
        cout << loads[l] << "\t\t" << t.AvgProbes() << "\t\t" << 1 + loads[l] / 2 << "\t\t" << t.MaxChain() << endl;
    }
    cout << endl;
}

// Эксперимент 3: кукушка при фиксированном размере (2 таблицы по 1000), вставляем до первой неудачи
void experimentCuckooLoad(){
    cout << "===== Эксперимент 3: кукушка без перестройки (2 x 1000 ячеек), 20 опытов =====" << endl;
    const int TRIALS = 20;
    double checkpoints[] = {0.1, 0.2, 0.3, 0.4, 0.45};
    double kicksSum[5] = {0}; // сумма вытеснений на вставку по опытам
    int reached[5] = {0}; // в скольких опытах дошли до этого заполнения
    double failSum = 0;
    double failMin = 1;
    double failMax = 0;

    for (int t = 0; t < TRIALS; ++t){
        CuckooTable cuckoo(1000, false);
        int c = 0; // следующая контрольная точка
        int i = 0;
        while (true){
            cuckoo.Insert("t" + to_string(t) + "_" + to_string(i), "v");
            if (cuckoo.Failed()){
                break;
            }
            ++i;
            while (c < 5 && cuckoo.LoadFactor() >= checkpoints[c]){
                kicksSum[c] += (double)cuckoo.Kicks() / cuckoo.Size();
                ++reached[c];
                ++c;
            }
        }
        double load = cuckoo.LoadFactor();
        failSum += load;
        if (load < failMin){
            failMin = load;
        }
        if (load > failMax){
            failMax = load;
        }
    }

    cout << "заполнение\tвытеснений на вставку\tдошли" << endl;
    for (int c = 0; c < 5; ++c){
        cout << checkpoints[c] << "\t\t";
        if (reached[c] > 0){
            cout << kicksSum[c] / reached[c];
        } else {
            cout << "-";
        }
        cout << "\t\t\t" << reached[c] << "/" << TRIALS << endl;
    }
    cout << "Первая неудачная вставка при заполнении: в среднем " << failSum / TRIALS
         << " (от " << failMin << " до " << failMax << ")" << endl;
}

int main(){
    demo();
    experimentTime();
    experimentChainLoad();
    experimentCuckooLoad();
    return 0;
}