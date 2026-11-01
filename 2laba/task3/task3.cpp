#include <iostream>
#include <string>
#include <vector>
#include "set.h"

using namespace std;

// Разбирает строку вида {{1, 2, 3}, {2, 3, 4}} на подмножества
// Возвращает false, если строка записана неправильно
bool parse(const string& str, vector<vector<string>>& groups){
    int depth = 0; // уровень вложенности скобок
    string num; // сюда набираем цифры текущего числа
    for (int i = 0; i < (int)str.size(); ++i){
        char c = str[i];
        if (c >= '0' && c <= '9'){
            if (depth != 2){
                return false; // число стоит не внутри подмножества
            }
            num += c;
        } else if (c == '{' || c == '}' || c == ',' || c == ' '){
            if (!num.empty()){ // число закончилось, кладём его в текущее подмножество
                groups.back().push_back(num);
                num = "";
            }
            if (c == '{'){
                ++depth;
                if (depth > 2){
                    return false;
                }
                if (depth == 2){
                    groups.push_back(vector<string>()); // началось новое подмножество
                }
            } else if (c == '}'){
                --depth;
                if (depth < 0){
                    return false;
                }
            }
        } else {
            return false; // посторонний символ
        }
    }
    return depth == 0;
}

// Печатает подмножество в виде {2, 3, 4}
void printGroup(const vector<string>& group){
    cout << "{";
    for (int i = 0; i < (int)group.size(); ++i){
        if (i > 0){
            cout << ", ";
        }
        cout << group[i];
    }
    cout << "}";
}

int main(){
    string str;
    cout << "Введите подмножества: ";
    getline(cin, str);

    vector<vector<string>> groups;
    if (!parse(str, groups)){
        cout << "Ошибка: неверный формат, пример: {{1, 2, 3}, {2, 3, 4}}" << endl;
        return 1;
    }
    int k = groups.size(); // количество подмножеств
    if (k < 2){
        cout << "Ошибка: нужно хотя бы два подмножества" << endl;
        return 1;
    }

    // Шаг 1: каждое подмножество кладём в множество из задания 2
    Set* sets = new Set[k];
    vector<vector<string>> elems(k); // элементы без повторов в порядке ввода, нужны для перебора и вывода
    for (int i = 0; i < k; ++i){
        for (int j = 0; j < (int)groups[i].size(); ++j){
            if (sets[i].add(groups[i][j])){ // add вернёт false, если такой элемент уже был
                elems[i].push_back(groups[i][j]);
            }
        }
    }

    // Шаг 2: перебираем все пары подмножеств и считаем общие элементы
    int best = -1; // максимальное количество общих элементов
    int bestA = 0;
    int bestB = 1;
    for (int a = 0; a < k; ++a){
        for (int b = a + 1; b < k; ++b){
            // идём по меньшему множеству и ищем его элементы в большем
            int small = a;
            int big = b;
            if (elems[a].size() > elems[b].size()){
                small = b;
                big = a;
            }
            int common = 0;
            for (int i = 0; i < (int)elems[small].size(); ++i){
                if (sets[big].at(elems[small][i])){ // проверка за O(1)
                    ++common;
                }
            }
            if (common >= best){ // >= чтобы при равенстве брать последнюю пару, как в примере
                best = common;
                bestA = a;
                bestB = b;
            }
        }
    }

    // Шаг 3: выводим ответ
    if (best == 0){
        cout << "Ни у одной пары подмножеств нет общих элементов" << endl;
        delete[] sets;
        return 0;
    }

    vector<string> common; // сами общие элементы
    for (int i = 0; i < (int)elems[bestA].size(); ++i){
        if (sets[bestB].at(elems[bestA][i])){
            common.push_back(elems[bestA][i]);
        }
    }

    cout << "{";
    printGroup(elems[bestA]);
    cout << ", ";
    printGroup(elems[bestB]);
    cout << "}, количество общих элементов: " << best;
    if (best == 1){
        cout << " (элемент " << common[0] << ")" << endl;
    } else {
        cout << " (элементы ";
        for (int i = 0; i < (int)common.size(); ++i){
            if (i == (int)common.size() - 1){
                cout << " и ";
            } else if (i > 0){
                cout << ", ";
            }
            cout << common[i];
        }
        cout << ")" << endl;
    }

    delete[] sets;
    return 0;
}