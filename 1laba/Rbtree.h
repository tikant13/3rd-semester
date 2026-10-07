#pragma once
#include <iostream>
#include <string>

using namespace std;

struct RBTree {
private:
    struct Node {
        int key = 0;
        char color = 'R';
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
    };
    Node* root = nullptr;

    bool isRed(Node* node); // nullptr считается чёрным
    void leftRotate(Node* x); // левый поворот вокруг x
    void rightRotate(Node* x); // правый поворот вокруг x
    void insertFixup(Node* node); // восстановление свойств после вставки
    void deleteFixup(Node* x, Node* xParent); // восстановление свойств после удаления
    void transplant(Node* u, Node* v); // поставить поддерево v на место поддерева u
    void clear(Node* node);
    void printTree(Node* node, int depth);
    void inorder(Node* node);
    void preorder(Node* node);
    void postorder(Node* node);
    int countNodes(Node* node);
    void saveNode(Node* node, string& res);
public:
    ~RBTree();
    void Insert(int key); // добавление элемента
    void Remove(int key); // удаление элемента с сохранением свойств дерева
    bool Contains(int key); // поиск элемента
    void Print(); // вывод дерева боком
    void PrintInorder(); // обход в глубину: левое, корень, правое
    void PrintPreorder(); // обход в глубину: корень, левое, правое
    void PrintPostorder(); // обход в глубину: левое, правое, корень
    void PrintBFS(); // обход в ширину (по уровням)
    int Size(); // количество элементов
    void Load(int key, char color); // вставка при чтении файла
    string ToString(); // пары ключ цвет в прямом обходе для записи в файл
};