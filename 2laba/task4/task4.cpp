#include <iostream>
#include <string>
#include "array.h"

using namespace std;

// Массив хранит строки, поэтому перед сравнением переводим элемент в число
long long num(Array& arr, int i){
    return stoll(arr.At(i));
}

// Меняем местами блоки i и j сразу в обоих массивах, чтобы ширина и высота не разъехались
void swapBlocks(Array& w, Array& h, int i, int j){
    string temp = w.At(i);
    w.Set(i, w.At(j));
    w.Set(j, temp);

    temp = h.At(i);
    h.Set(i, h.At(j));
    h.Set(j, temp);
}

// Просеивание вниз: опускаем элемент i, пока он меньше своих детей
// n - сколько элементов сейчас в куче
void siftDown(Array& w, Array& h, int i, int n){
    while (true){
        int big = i;
        int left = 2 * i + 1; // левый ребёнок
        int right = 2 * i + 2; // правый ребёнок
        if (left < n && num(w, left) > num(w, big)){
            big = left;
        }
        if (right < n && num(w, right) > num(w, big)){
            big = right;
        }
        if (big == i){
            return; // элемент больше обоих детей, он на своём месте
        }
        swapBlocks(w, h, i, big);
        i = big;
    }
}

// Пирамидальная сортировка (heap sort) по ширине за O(N log N)
void heapSort(Array& w, Array& h){
    int n = w.Size();
    // строим кучу: на вершине оказывается самый широкий блок
    for (int i = n / 2 - 1; i >= 0; --i){
        siftDown(w, h, i, n);
    }
    // по одному переносим максимум в конец и восстанавливаем кучу
    for (int end = n - 1; end > 0; --end){
        swapBlocks(w, h, 0, end);
        siftDown(w, h, 0, end);
    }
}

int main(){
    int n;
    if (!(cin >> n) || n < 1 || n > 100000){
        cout << "Ошибка: N должно быть от 1 до 100000" << endl;
        return 1;
    }

    // Шаг 1: читаем блоки, ширины и высоты в двух массивах (блок i = widths[i], heights[i])
    Array widths;
    Array heights;
    for (int i = 0; i < n; ++i){
        long long w, h;
        if (!(cin >> w >> h) || w < 1 || h < 1){
            cout << "Ошибка: ширина и высота должны быть натуральными числами" << endl;
            return 1;
        }
        widths.PushBack(to_string(w));
        heights.PushBack(to_string(h));
    }

    // Шаг 2: сортируем по ширине, чтобы блоки одной ширины стояли рядом
    heapSort(widths, heights);

    // Шаг 3: из каждой группы одинаковой ширины берём самый высокий блок
    long long total = 0; // высота может быть до 100000 * 10^9, в int не влезет
    int i = 0;
    while (i < n){
        long long maxH = num(heights, i);
        int j = i + 1;
        while (j < n && num(widths, j) == num(widths, i)){ // идём по блокам той же ширины
            if (num(heights, j) > maxH){
                maxH = num(heights, j);
            }
            ++j;
        }
        total += maxH;
        i = j; // переходим к следующей ширине
    }

    cout << total << endl;
    return 0;
}