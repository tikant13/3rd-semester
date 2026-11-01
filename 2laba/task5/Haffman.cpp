#include <iostream>
#include <string>
#include "Haffman.h" 
using namespace std; 

Huffman::~Huffman(){
    clear(root);
}

void Huffman::clear(HNode* node){
    if (node == nullptr){
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

void Huffman::Build(const string& text){
    // Шаг 1: считаем частоты символов
    for (int i = 0; i < (int)text.size(); ++i){
        unsigned char c = text[i];
        if (freq[c] == 0){
            order += text[i]; // запоминаем символы в порядке первого появления
        }
        ++freq[c];
    }

    // сортируем символы по убыванию частоты (вставками, при равенстве порядок не меняется)
    for (int i = 1; i < (int)order.size(); ++i){
        char cur = order[i];
        int j = i - 1;
        while (j >= 0 && freq[(unsigned char)order[j]] < freq[(unsigned char)cur]){
            order[j + 1] = order[j];
            --j;
        }
        order[j + 1] = cur;
    }

    // список свободных узлов. Узлы лежат в nodes, а в freeList - их номера
    HNode* nodes[511]; // максимум 256 листьев и 255 родителей
    int nodeCount = 0;
    Array freeList;
    for (int i = 0; i < (int)order.size(); ++i){
        HNode* leaf = new HNode;
        leaf->symbols = order[i];
        leaf->weight = freq[(unsigned char)order[i]];
        nodes[nodeCount] = leaf;
        freeList.PushBack(to_string(nodeCount));
        ++nodeCount;
    }

    // пока в списке больше одного узла, объединяем два самых лёгких
    while (freeList.Size() > 1){
        // Шаг 3: ищем позиции двух узлов с наименьшим весом
        int min1 = -1; // позиция самого лёгкого
        int min2 = -1; // позиция второго по лёгкости
        for (int i = 0; i < freeList.Size(); ++i){
            int w = nodes[stoi(freeList.At(i))]->weight;
            if (min1 == -1 || w < nodes[stoi(freeList.At(min1))]->weight){
                min2 = min1;
                min1 = i;
            } else if (min2 == -1 || w < nodes[stoi(freeList.At(min2))]->weight){
                min2 = i;
            }
        }
        HNode* a = nodes[stoi(freeList.At(min1))];
        HNode* b = nodes[stoi(freeList.At(min2))];

        // родитель - объединение символов, вес - сумма весов
        HNode* parent = new HNode;
        parent->symbols = a->symbols + b->symbols;
        parent->weight = a->weight + b->weight;
        parent->left = a; // Шаг 6: левой дуге бит 0
        parent->right = b; // правой дуге бит 1
        nodes[nodeCount] = parent;

        // родитель встаёт в список вместо двух своих потомков
        freeList.Set(min1, to_string(nodeCount));
        freeList.DelIndx(min2);
        ++nodeCount;
    }

    if (freeList.Size() == 0){
        return; // пустой текст
    }
    root = nodes[stoi(freeList.At(0))]; // оставшийся узел - корень дерева

    if (root->left == nullptr){
        codes[(unsigned char)root->symbols[0]] = "0"; // в тексте один различный символ, дерево из одного листа
    } else {
        makeCodes(root, "");
    }
}

// Идём от корня к листьям: влево дописываем 0, вправо 1. Дошли до листа - это его код
void Huffman::makeCodes(HNode* node, const string& code){
    if (node->left == nullptr && node->right == nullptr){
        codes[(unsigned char)node->symbols[0]] = code;
        return;
    }
    makeCodes(node->left, code + "0");
    makeCodes(node->right, code + "1");
}

string Huffman::Encode(const string& text){
    string res;
    for (int i = 0; i < (int)text.size(); ++i){
        res += codes[(unsigned char)text[i]]; // заменяем каждый символ его кодом
    }
    return res;
}

// Расшифровка: идём по дереву от корня, 0 - влево, 1 - вправо.
// Дошли до листа - записываем символ и возвращаемся в корень
bool Huffman::Decode(const string& code, string& result){
    result = "";
    if (root == nullptr){
        return false;
    }
    if (root->left == nullptr){ // дерево из одного листа, каждый 0 - это этот символ
        for (int i = 0; i < (int)code.size(); ++i){
            if (code[i] != '0'){
                return false;
            }
            result += root->symbols;
        }
        return true;
    }
    HNode* node = root;
    for (int i = 0; i < (int)code.size(); ++i){
        if (code[i] == '0'){
            node = node->left;
        } else if (code[i] == '1'){
            node = node->right;
        } else {
            return false; // в коде что-то кроме 0 и 1
        }
        if (node->left == nullptr && node->right == nullptr){
            result += node->symbols;
            node = root;
        }
    }
    return node == root; // если остановились не в корне, код оборвался на середине символа
}

void Huffman::PrintTable(){
    cout << "Символ  Частота  Код" << endl;
    for (int i = 0; i < (int)order.size(); ++i){
        unsigned char c = order[i];
        if (order[i] == ' '){
            cout << "пробел";
        } else {
            cout << "'" << order[i] << "'   ";
        }
        cout << "  " << freq[c] << "        " << codes[c] << endl;
    }
}