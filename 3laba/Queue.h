#pragma once
#include <iostream>
#include <string>

class Queue {
private:
    struct Node {
        std::string value;
        Node* next = nullptr;
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    void clear();
public:
    Queue() = default;
    // Объект сам владеет памятью, поверхностное копирование запрещено.
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    ~Queue();
    void push(const std::string& value); // добавить в конец
    void pop(); // удалить из начала
    void peek() const; // прочитать первый элемент
    void Print() const;
    int Size() const; // количество элементов
    std::string ToString() const; // элементы через пробел от начала к концу для записи в файл
};
