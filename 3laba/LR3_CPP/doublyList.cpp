#include "array.h"
#include <utility>
#include <iostream>
#include <string>
#include "doublyList.h"

using namespace std;

DoublyList::~DoublyList() {
    clear();
}

void DoublyList::clear() {
    Node* temp = head;
    while (temp != nullptr) {
        Node* next = temp->next;
        delete temp;
        temp = next;
    }
    head = tail = nullptr;
}

void DoublyList::pushFront(const string& val) {
    Node* newHead = new Node;
    newHead->value = val;
    if (head == nullptr) {
        head = tail = newHead;
        return;
    }
    newHead->next = head;
    head->prev = newHead;
    head = newHead;
}

void DoublyList::pushBack(const string& val) {
    Node* newTail = new Node;
    newTail->value = val;
    if (tail == nullptr) {
        head = tail = newTail;
        return;
    }
    newTail->prev = tail;
    tail->next = newTail;
    tail = newTail;
}

void DoublyList::pushBefore(const string& tar, const string& value) {
    Node* temp = head;
    while (temp != nullptr && temp->value != tar) {
        temp = temp->next;
    }
    if (temp == nullptr) {
        cout << "Error!" << endl;
        return;
    }
    if (temp == head) {
        pushFront(value);
        return;
    }
    Node* newNode = new Node;
    newNode->value = value;
    newNode->prev = temp->prev;
    newNode->next = temp;
    temp->prev->next = newNode;
    temp->prev = newNode;

}

void DoublyList::pushAfter(const string& tar, const string& value) {
    Node* temp = head;
    while (temp != nullptr && temp->value != tar) {
        temp = temp->next;
    }
    if (temp == nullptr) {
        cout << "Error!" << endl; // значения нет
        return;
    }
    if (temp == tail) {
        pushBack(value);
        return;
    }
 
    Node* newNode = new Node;
    newNode->value = value;
    newNode->prev = temp;
    newNode->next = temp->next;
    temp->next->prev = newNode;
    temp->next = newNode;

}

void DoublyList::GetHead() const {
    if (head == nullptr) {
        cout << "ПУСТОЙ" << endl;
        return;
    }
    cout << head->value << endl;
}

void DoublyList::GetTail() const {
    if (tail == nullptr) {
        cout << "ПУСТОЙ" << endl;
        return;
    }
    cout << tail->value << endl;
}

void DoublyList::GetIndx(const int& indx) const {
    if (indx < 0) {
        cout << "Error!" << endl;
        return;
    }

    Node* temp = head;
    for (int i = 0; i < indx && temp != nullptr; ++i) {
        temp = temp->next;
    }
    if (temp == nullptr) {
        cout << "Error!" << endl;
        return;
    }
    cout << temp->value << endl;
}

void DoublyList::Print() const {
    if (head == nullptr) {
        cout << "ПУСТОЙ" << endl;
        return;
    }
    Node* temp = head;
    while (temp != nullptr) {
        cout << temp->value;
        if (temp->next != nullptr) {
            cout << " <-> ";
        }
        temp = temp->next;
    }
    cout << endl;
}

void DoublyList::delFront() {
    if (head == nullptr) {
        cout << "Error!" << endl;
        return;
    }
    Node* temp = head;
    head = head->next;
    if (head == nullptr) {
        tail = nullptr;
    } else {
        head->prev = nullptr;
    }
    delete temp;
}

void DoublyList::delBack() {
    if (tail == nullptr) {
        cout << "Error!" << endl;
        return;
    }
    Node* temp = tail;
    tail = tail->prev;
    if (tail == nullptr) {
        head = nullptr;
    } else {
        tail->next = nullptr;
    }
    delete temp;
}

void DoublyList::delBefore(const string& tar) {
    Node* temp = head;
    while (temp != nullptr && temp->value != tar) {
        temp = temp->next;
    }
    if (temp == nullptr || temp == head) {
        cout << "Error!" << endl; 
        return;
    }
    Node* delNode = temp->prev;
    if (delNode == head) {
        delFront();
        return;
    }
    delNode->prev->next = temp;
    temp->prev = delNode->prev;
    delete delNode;
}

void DoublyList::delAfter(const string& tar) {
    Node* temp = head;
    while (temp != nullptr && temp->value != tar) {
        temp = temp->next;
    }
    if (temp == nullptr || temp->next == nullptr) {
        cout << "Error!" << endl;
        return;
    }
    Node* delNode = temp->next;
    if (delNode == tail) {
        delBack();
        return;
    }
    temp->next = delNode->next;
    delNode->next->prev = temp;
    delete delNode;

}

void DoublyList::DelVal(const string& value) {
    Node* temp = head;
    while (temp != nullptr && temp->value != value) {
        temp = temp->next;
    }
    if (temp == nullptr) {
        cout << "Error!" << endl;
        return;
    }
    if (temp == head) {
        delFront();
        return;
    }
    if (temp == tail) {
        delBack();
        return;
    }

    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    delete temp;
}

void DoublyList::GetVal(const string& value) const {
    int indx = Find(value);
    if (indx == -1) {
        cout << "ЗНАЧЕНИЕ НЕ НАЙДЕНО" << endl;
    } else {
        cout << "ЗНАЧЕНИЕ \"" << value << "\" ИНДЕКС " << indx << endl;
    }
}

int DoublyList::Size() const {
    int count = 0;
    Node* temp = head;
    while (temp != nullptr) {
        ++count;
        temp = temp->next;
    }
    return count;
}

string DoublyList::At(const int& indx) const {
    if (indx < 0) {
        cout << "Error!" << endl;
        return "";
    }

    Node* temp = head;
    for (int i = 0; i < indx && temp != nullptr; ++i) {
        temp = temp->next;
    }
    if (temp == nullptr) {
        cout << "Error!" << endl;
        return "";
    }
    return temp->value;
}

int DoublyList::Find(const string& value) const {
    Node* temp = head;
    int indx = 0;
    while (temp != nullptr) {
        if (temp->value == value) {
            return indx;
        }
        temp = temp->next;
        ++indx;
    }
    return -1;
}

string DoublyList::ToString() const {
    string result;
    Node* temp = head;
    while (temp != nullptr) {
        result += temp->value;
        if (temp->next != nullptr) {
            result += " ";
        }
        temp = temp->next;
    }
    return result;
}

// Сохраняем элементы в том же порядке, в котором ими пользуется структура.
// Первый элемент — обозначение типа, чтобы не загрузить файл другого класса.
bool DoublyList::saveToFile(const string& fileName) const {
    Array data;
    data.PushBack("L");
    for (Node* node = head; node != nullptr; node = node->next) data.PushBack(node->value);
    return data.saveToFile(fileName);
}

bool DoublyList::loadFromFile(const string& fileName) {
    Array data;
    if (!data.loadFromFile(fileName) || data.Size() < 1 || data.At(0) != "L") return false;
    DoublyList loaded;
    for (int i = 1; i < data.Size(); ++i) loaded.pushBack(data.At(i));
    swap(head, loaded.head);
    swap(tail, loaded.tail);
    return true;
}

bool DoublyList::saveBinary(const string& fileName) const {
    Array data;
    data.PushBack("L");
    for (Node* node = head; node != nullptr; node = node->next) data.PushBack(node->value);
    return data.saveBinary(fileName);
}

bool DoublyList::loadBinary(const string& fileName) {
    Array data;
    if (!data.loadBinary(fileName) || data.Size() < 1 || data.At(0) != "L") return false;
    DoublyList loaded;
    for (int i = 1; i < data.Size(); ++i) loaded.pushBack(data.At(i));
    swap(head, loaded.head);
    swap(tail, loaded.tail);
    return true;
}
