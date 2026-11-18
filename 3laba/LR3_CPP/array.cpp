#include <iostream>
#include <string>
#include "array.h"
#include <fstream>
#include <cstdint>
#include <utility>
using namespace std;

Array::~Array(){
    clear();
}

Array::Array(){
    Create();
}

void Array::Create(){
    clear();
    highlighted = 10;
    data = new string[highlighted];
    isUsed = 0;
}

void Array::increaseArrasy(){
    highlighted *= 2;
    string* newData = new string [highlighted];
    for(int i = 0; i<isUsed; ++i){
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

void Array::PushBack(const string& value){
    if (isUsed == highlighted){
        increaseArrasy();
    }
    data[isUsed] = value;
    ++isUsed;
}

void Array::Print() const{
    for(int i = 0; i<isUsed; ++i){
        cout << data[i]<<" ";
    }
    cout << endl;
}

void Array::Insert(const int& indx, const string& value){
    if (isUsed == highlighted){
        increaseArrasy();
    }
    if (isUsed < indx || indx < 0){
        return;
    }
    for (int i = isUsed; indx < i; --i){
        data[i] = data[i-1];
    }
    data[indx] = value;
    ++isUsed;
}

void Array::clear(){
    delete[] data;
    data = nullptr;
    isUsed = 0;
}

void Array::Length() const{
    cout << "Длинна массива: " << isUsed << endl;
}

void Array::Get(const int& indx) const{
    if (indx < 0 || indx >=isUsed){
        return;
    }
    cout << data[indx]<<endl;
}

void Array::DelIndx(const int& indx){
     if (indx < 0 || indx >= isUsed) {
        return;
    }
    for (int i = indx; i < isUsed - 1; ++i) {
        data[i] = data[i + 1];
    }
    --isUsed;
}

void Array::Set(int indx, const string& val){
    if (indx < 0 || indx >= isUsed){
        return;
    }
    data[indx] = val;
}

int Array::Size() const{
    return isUsed;
}
 
string Array::At(const int& indx) const{
    if (indx < 0 || indx >= isUsed){
        return "";
    }
    return data[indx];
}
 
string Array::ToString() const{
    string res;
    for (int i = 0; i < isUsed; ++i){
        if (i > 0) res += " ";
        res += data[i];
    }
    return res;
}

// Текстовый файл: одна строка массива на одной строке файла.
bool Array::saveToFile(const string& fileName) const {
    for (int i = 0; i < Size(); ++i)
        if (At(i).find_first_of("\r\n") != string::npos) return false;
    ofstream out(fileName);
    if (!out) return false;
    for (int i = 0; i < Size(); ++i) out << At(i) << '\n';
    return static_cast<bool>(out);
}

bool Array::loadFromFile(const string& fileName) {
    ifstream in(fileName);
    if (!in) return false;
    Array loaded;
    string line;
    while (getline(in, line)) loaded.PushBack(line);
    if (in.bad()) return false;
    swap(data, loaded.data);
    swap(isUsed, loaded.isUsed);
    swap(highlighted, loaded.highlighted);
    return true;
}

// Бинарный файл: метка ARR1, количество строк, затем длина и байты каждой строки.
bool Array::saveBinary(const string& fileName) const {
    if (Size() > 100000) return false;
    for (int i = 0; i < Size(); ++i)
        if (data[i].size() > 1024 * 1024) return false;
    ofstream out(fileName, ios::binary);
    if (!out) return false;
    out.write("ARR1", 4);
    uint32_t count = static_cast<uint32_t>(Size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (int i = 0; i < Size(); ++i) {
        const string& value = data[i];
        uint32_t length = static_cast<uint32_t>(value.size());
        out.write(reinterpret_cast<const char*>(&length), sizeof(length));
        out.write(value.data(), length);
    }
    return static_cast<bool>(out);
}

bool Array::loadBinary(const string& fileName) {
    ifstream in(fileName, ios::binary);
    if (!in) return false;
    char header[4];
    in.read(header, 4);
    if (!in || string(header, 4) != "ARR1") return false;
    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!in || count > 100000) return false;
    Array loaded;
    for (uint32_t i = 0; i < count; ++i) {
        uint32_t length = 0;
        in.read(reinterpret_cast<char*>(&length), sizeof(length));
        if (!in || length > 1024 * 1024) return false;
        string value(length, '\0');
        if (length > 0) in.read(&value[0], length);
        if (!in) return false;
        loaded.PushBack(value);
    }
    if (in.peek() != char_traits<char>::eof() || in.bad()) return false;
    swap(data, loaded.data);
    swap(isUsed, loaded.isUsed);
    swap(highlighted, loaded.highlighted);
    return true;
}
