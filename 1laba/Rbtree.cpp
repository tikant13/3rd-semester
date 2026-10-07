#include <iostream>
#include <string>
#include "Rbtree.h"

using namespace std;

RBTree::~RBTree() {
    clear(root);
}

void RBTree::clear(Node* node) {
    if (node == nullptr) {
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

bool RBTree::isRed(Node* node) {
    return (node != nullptr && node->color == 'R');
}

// Левый поворот вокруг x: правый ребёнок y поднимается на место x,
// x становится левым ребёнком y, а левое поддерево y переходит к x (вправо).
void RBTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RBTree::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void RBTree::Insert(int key) {
    // ищем место, как в обычном бинарном дереве поиска
    Node* parent = nullptr;
    Node* temp = root;
    while (temp != nullptr) {
        parent = temp;
        if (key == temp->key) {
            return; // такой ключ уже есть
        }
        if (key < temp->key) {
            temp = temp->left;
        } else {
            temp = temp->right;
        }
    }

    // новый узел всегда красный чёрная высота от этого не меняется
    Node* node = new Node;
    node->key = key;
    node->color = 'R';
    node->parent = parent;
    if (parent == nullptr) {
        root = node;
    } else if (key < parent->key) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    // могли получиться два красных подряд
    insertFixup(node);
}

void RBTree::insertFixup(Node* node) {
    while (node != root && node->parent->color == 'R') {
        Node* parent = node->parent;
        Node* grand = parent->parent;

        if (parent == grand->left) {
            Node* uncle = grand->right;
            if (isRed(uncle)) {
                parent->color = 'B';
                uncle->color = 'B';
                grand->color = 'R';
                node = grand;
            } else {
                if (node == parent->right) {
                    node = parent;
                    leftRotate(node);
                    parent = node->parent;
                }
                parent->color = 'B';
                grand->color = 'R';
                rightRotate(grand);
            }
        } else {
            Node* uncle = grand->left;
            if (isRed(uncle)) {
                parent->color = 'B';
                uncle->color = 'B';
                grand->color = 'R';
                node = grand;
            } else {
                if (node == parent->left) {
                    node = parent;
                    rightRotate(node);
                    parent = node->parent;
                }
                parent->color = 'B';
                grand->color = 'R';
                leftRotate(grand);
            }
        }
    }
    root->color = 'B'; // корень всегда чёрный
}

bool RBTree::Contains(int key) {
    Node* temp = root;
    while (temp != nullptr) {
        if (key == temp->key) {
            return true;
        }
        if (key < temp->key) {
            temp = temp->left;
        } else {
            temp = temp->right;
        }
    }
    return false;
}

void RBTree::transplant(Node* u, Node* v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

void RBTree::Remove(int key) {
    // ищем узел z с этим ключом
    Node* z = root;
    while (z != nullptr && z->key != key) {
        if (key < z->key) {
            z = z->left;
        } else {
            z = z->right;
        }
    }
    if (z == nullptr) {
        return; // такого ключа нет
    }

    Node* y = z; // узел, который уходит со своего места
    char yColor = y->color; // его цвет до удаления
    Node* x = nullptr; // узел, который встаёт на место y
    Node* xParent = nullptr; // родитель x храним отдельно, так как x может быть nullptr

    if (z->left == nullptr) {
        // нет левого ребёнка на место z ставим правого
        x = z->right;
        xParent = z->parent;
        transplant(z, z->right);
    } else if (z->right == nullptr) {
        // нет правого ребёнка на место z ставим левого
        x = z->left;
        xParent = z->parent;
        transplant(z, z->left);
    } else {
        y = z->right;
        while (y->left != nullptr) {
            y = y->left;
        }
        yColor = y->color;
        x = y->right;
        if (y->parent == z) {
            xParent = y;
        } else {
            // вынимаем y с его места, туда встаёт его правый ребёнок
            xParent = y->parent;
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        // ставим y на место z и красим в цвет z
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;

    // если ушёл чёрный узел, на части путей стало на один чёрный меньше исправляем
    if (yColor == 'B') {
        deleteFixup(x, xParent);
    }
}

void RBTree::deleteFixup(Node* x, Node* xParent) {
    while (x != root && !isRed(x)) {
        if (x == xParent->left) {
            Node* brother = xParent->right; // брат x
            if (isRed(brother)) {
                brother->color = 'B';
                xParent->color = 'R';
                leftRotate(xParent);
                brother = xParent->right;
            }
            if (!isRed(brother->left) && !isRed(brother->right)) {
                brother->color = 'R';
                x = xParent;
                xParent = x->parent;
            } else {
                if (!isRed(brother->right)) {
                    brother->left->color = 'B';
                    brother->color = 'R';
                    rightRotate(brother);
                    brother = xParent->right;
                }
                brother->color = xParent->color;
                xParent->color = 'B';
                brother->right->color = 'B';
                leftRotate(xParent);
                x = root; // выходим из цикла
            }
        } else {
            // то же самое зеркально: x — правый ребёнок
            Node* brother = xParent->left;
            if (isRed(brother)) {
                brother->color = 'B';
                xParent->color = 'R';
                rightRotate(xParent);
                brother = xParent->left;
            }
            if (!isRed(brother->left) && !isRed(brother->right)) {
                brother->color = 'R';
                x = xParent;
                xParent = x->parent;
            } else {
                if (!isRed(brother->left)) {
                    brother->right->color = 'B';
                    brother->color = 'R';
                    leftRotate(brother);
                    brother = xParent->left;
                }
                brother->color = xParent->color;
                xParent->color = 'B';
                brother->left->color = 'B';
                rightRotate(xParent);
                x = root;
            }
        }
    }
    if (x != nullptr) {
        x->color = 'B';
    }
}

void RBTree::printTree(Node* node, int depth) {
    if (node == nullptr) {
        return;
    }
    printTree(node->right, depth + 1);
    for (int i = 0; i < depth; ++i) {
        cout << "      ";
    }
    cout << node->key << "(" << node->color << ")" << endl;
    printTree(node->left, depth + 1);
}

void RBTree::Print() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    printTree(root, 0);
}

void RBTree::inorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    inorder(node->left);
    cout << node->key << " ";
    inorder(node->right);
}

void RBTree::preorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    cout << node->key << " ";
    preorder(node->left);
    preorder(node->right);
}

void RBTree::postorder(Node* node) {
    if (node == nullptr) {
        return;
    }
    postorder(node->left);
    postorder(node->right);
    cout << node->key << " ";
}

void RBTree::PrintInorder() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    inorder(root);
    cout << endl;
}

void RBTree::PrintPreorder() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    preorder(root);
    cout << endl;
}

void RBTree::PrintPostorder() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    postorder(root);
    cout << endl;
}

void RBTree::PrintBFS() {
    if (root == nullptr) {
        cout << "Пустое" << endl;
        return;
    }
    int n = Size();
    Node** q = new Node*[n];
    int head = 0;
    int tail = 0;
    q[tail] = root;
    ++tail;
    while (head < tail) {
        Node* node = q[head];
        ++head;
        cout << node->key << " ";
        if (node->left != nullptr) {
            q[tail] = node->left;
            ++tail;
        }
        if (node->right != nullptr) {
            q[tail] = node->right;
            ++tail;
        }
    }
    cout << endl;
    delete[] q;
}

int RBTree::countNodes(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
}

int RBTree::Size() {
    return countNodes(root);
}

void RBTree::Load(int key, char color) {
    Node* parent = nullptr;
    Node* temp = root;
    while (temp != nullptr) {
        parent = temp;
        if (key < temp->key) {
            temp = temp->left;
        } else {
            temp = temp->right;
        }
    }
    Node* node = new Node;
    node->key = key;
    node->color = color;
    node->parent = parent;
    if (parent == nullptr) {
        root = node;
    } else if (key < parent->key) {
        parent->left = node;
    } else {
        parent->right = node;
    }
}

void RBTree::saveNode(Node* node, string& res) {
    if (node == nullptr) {
        return;
    }
    if (!res.empty()) {
        res += " ";
    }
    res += to_string(node->key);
    res += " ";
    res += node->color;
    saveNode(node->left, res);
    saveNode(node->right, res);
}

string RBTree::ToString() {
    string res;
    saveNode(root, res);
    return res;
}