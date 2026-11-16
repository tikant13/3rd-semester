#pragma once
#include <iostream>
#include <string>

class Stack {
private:
    struct Node {
        std::string value;
        Node* next = nullptr;
    };
    Node* top = nullptr;
    void clear();
public:
    Stack() = default;
    // Объект сам владеет памятью, поверхностное копирование запрещено.
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
    ~Stack();
    void push(const std::string& value);
    void pop();
    void Print() const;
    void peek() const; 
    int Size() const;
    std::string ToString() const;
};
