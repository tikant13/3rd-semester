#pragma once
#include <iostream>
#include <string>

using namespace std;

class RBTree {
private:
    struct Node {
        int key = 0;
        char color = 'R';
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
    };
    Node* root = nullptr;

    bool isRed(Node* node) const; // nullptr считается чёрным
    void leftRotate(Node* x); // левый поворот вокруг x
    void rightRotate(Node* x); // правый поворот вокруг x
    void insertFixup(Node* node); // восстановление свойств после вставки
    void deleteFixup(Node* x, Node* xParent); // восстановление свойств после удаления
    void transplant(Node* u, Node* v); // поставить поддерево v на место поддерева u
    void clear(Node* node);
    void printTree(Node* node, int depth) const;
    void inorder(Node* node) const;
    void preorder(Node* node) const;
    void postorder(Node* node) const;
    int countNodes(Node* node) const;
    void saveNode(Node* node, string& res) const;
public:
    RBTree() = default;
    // Объект сам владеет памятью, поверхностное копирование запрещено.
    RBTree(const RBTree&) = delete;
    RBTree& operator=(const RBTree&) = delete;
    ~RBTree();
    void Insert(int key); // добавление элемента
    void Remove(int key); // удаление элемента с сохранением свойств дерева
    bool Contains(int key) const; // поиск элемента
    void Print() const; // вывод дерева боком
    void PrintInorder() const; // обход в глубину: левое, корень, правое
    void PrintPreorder() const; // обход в глубину: корень, левое, правое
    void PrintPostorder() const; // обход в глубину: левое, правое, корень
    void PrintBFS() const; // обход в ширину (по уровням)
    int Size() const; // количество элементов
    void Load(int key, char color); // вставка при чтении файла
    string ToString() const; // пары ключ цвет в прямом обходе для записи в файл
};
