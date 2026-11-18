#include "array.h"
#include <utility>
#include <iostream>
#include "Queue.h"

Queue::~Queue(){
    clear();
}

void Queue::push(const std::string& value){
    Node* newTail = new Node;
    newTail->value = value;
    if (tail == nullptr){
        head = tail = newTail;
        return;
    }
    tail->next = newTail;
    tail = newTail;
}

void Queue::pop(){
    if (head == nullptr){
        std::cout << "Error!" << std::endl;
        return;
    }
    Node* newHead = head->next;
    delete head;
    head = newHead;
    if (head == nullptr){
        tail = nullptr; // удалили последний элемент
    }
}

void Queue::peek() const{
    if (head == nullptr){
        std::cout << "Пуста" << std::endl;
        return;
    }
    std::cout << head->value << std::endl;
}

void Queue::Print() const{
    if (head == nullptr){
        std::cout << "Пустая" << std::endl;
        return;
    }
    Node* temp = head;
    while (temp != nullptr){
        std::cout << temp->value << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

void Queue::clear(){
    while (head != nullptr){
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
}

int Queue::Size() const{
    int count = 0;
    Node* temp = head;
    while (temp != nullptr){
        ++count;
        temp = temp->next;
    }
    return count;
}

std::string Queue::ToString() const{
    std::string res;
    Node* temp = head;
    while (temp != nullptr){
        if (temp != head){
            res += " ";
        }
        res += temp->value;
        temp = temp->next;
    }
    return res;
}

using namespace std;

// Сохраняем элементы в том же порядке, в котором ими пользуется структура.
// Первый элемент — обозначение типа, чтобы не загрузить файл другого класса.
bool Queue::saveToFile(const string& fileName) const {
    Array data;
    data.PushBack("Q");
    for (Node* node = head; node != nullptr; node = node->next) data.PushBack(node->value);
    return data.saveToFile(fileName);
}

bool Queue::loadFromFile(const string& fileName) {
    Array data;
    if (!data.loadFromFile(fileName) || data.Size() < 1 || data.At(0) != "Q") return false;
    Queue loaded;
    for (int i = 1; i < data.Size(); ++i) loaded.push(data.At(i));
    swap(head, loaded.head);
    swap(tail, loaded.tail);
    return true;
}

bool Queue::saveBinary(const string& fileName) const {
    Array data;
    data.PushBack("Q");
    for (Node* node = head; node != nullptr; node = node->next) data.PushBack(node->value);
    return data.saveBinary(fileName);
}

bool Queue::loadBinary(const string& fileName) {
    Array data;
    if (!data.loadBinary(fileName) || data.Size() < 1 || data.At(0) != "Q") return false;
    Queue loaded;
    for (int i = 1; i < data.Size(); ++i) loaded.push(data.At(i));
    swap(head, loaded.head);
    swap(tail, loaded.tail);
    return true;
}
