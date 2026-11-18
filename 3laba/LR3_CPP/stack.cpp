#include "array.h"
#include <utility>
#include <iostream>
#include "stack.h"

Stack::~Stack(){
    clear();
}

void Stack::push(const std::string& value){
    Node* newTop = new Node;
    newTop->value = value;
    newTop->next = top;
    top = newTop;
}

void Stack::pop(){
    if (top == nullptr){
        std::cout << "Error!" << std::endl;
        return;
    }
    Node* newTop = top->next;
    delete top;
    top = newTop;
}

void Stack::peek() const{
    if (top == nullptr){
        std::cout << "Пуст" << std::endl;
        return;
    }
    std::cout << top->value << std::endl;
}

void Stack::Print() const {
    if (top == nullptr) {
        std::cout << "Пустой" << std::endl;
        return;
    }
    Node* temp = top;
    while (temp != nullptr) {
        std::cout << temp->value << std::endl;
        temp = temp->next;
    }
}

void Stack::clear(){
        while (top != nullptr) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

int Stack::Size() const{
    int count = 0;
    Node* temp = top;
    while (temp != nullptr){
        ++count;
        temp = temp->next;
    }
    return count;
}

std::string Stack::ToString() const{
    std::string res;
    Node* temp = top;
    while (temp != nullptr){
        if (temp == top){
            res = temp->value;
        } else {
            res = temp->value + " " + res;
        }
        temp = temp->next;
    }
    return res;
}

using namespace std;

// Сохраняем элементы в том же порядке, в котором ими пользуется структура.
// Первый элемент — обозначение типа, чтобы не загрузить файл другого класса.
bool Stack::saveToFile(const string& fileName) const {
    Array data;
    data.PushBack("S");
    for (Node* node = top; node != nullptr; node = node->next) data.PushBack(node->value);
    return data.saveToFile(fileName);
}

bool Stack::loadFromFile(const string& fileName) {
    Array data;
    if (!data.loadFromFile(fileName) || data.Size() < 1 || data.At(0) != "S") return false;
    Stack loaded;
    for (int i = data.Size() - 1; i >= 1; --i) loaded.push(data.At(i));
    swap(top, loaded.top);
    return true;
}

bool Stack::saveBinary(const string& fileName) const {
    Array data;
    data.PushBack("S");
    for (Node* node = top; node != nullptr; node = node->next) data.PushBack(node->value);
    return data.saveBinary(fileName);
}

bool Stack::loadBinary(const string& fileName) {
    Array data;
    if (!data.loadBinary(fileName) || data.Size() < 1 || data.At(0) != "S") return false;
    Stack loaded;
    for (int i = data.Size() - 1; i >= 1; --i) loaded.push(data.At(i));
    swap(top, loaded.top);
    return true;
}
