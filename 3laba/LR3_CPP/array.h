#pragma once
#include <iostream>
#include <string>
using namespace std;

class Array {
private:
    string* data = nullptr;
    int isUsed = 0; // записанно элементов
    int highlighted = 10; // под сколько элементов выделенно
    void clear();
    void increaseArrasy(); // увеличиваем если переполнен
public:
    // Объект сам владеет памятью, поверхностное копирование запрещено.
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;
    ~Array();
    Array();
    void Create();
    void Print() const;
    void PushBack(const string& value);
    void Insert(const int& indx, const string& value); // добавление по индексу 
    void Get(const int& indx) const; // получение по индексу
    void DelIndx(const int& indx); // удаление по индексу 
    void Length() const; // длина массива
    void Set(int indx, const string& value); // замена  
    int Size() const; // количество элементов (без вывода)
    string At(const int& indx) const; // вернуть элемент по индексу (без вывода)
    string ToString() const; // все элементы через пробел — для записи в файл
    // Сохранение и восстановление структуры из файла.
    bool saveToFile(const std::string& fileName) const;
    bool loadFromFile(const std::string& fileName);
    bool saveBinary(const std::string& fileName) const;
    bool loadBinary(const std::string& fileName);
};
