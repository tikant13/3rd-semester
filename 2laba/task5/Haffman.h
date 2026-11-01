#pragma once
#include <iostream>
#include <string>
#include "array.h"
using namespace std;

// Узел дерева Хаффмана
struct HNode {
    string symbols; // символы этого узла (у листа - один символ)
    int weight = 0; // вес = сколько раз эти символы встретились в тексте
    HNode* left = nullptr; // дуга с битом 0
    HNode* right = nullptr; // дуга с битом 1
};

struct Huffman {
private:
    HNode* root = nullptr;
    int freq[256] = {0}; // частота каждого символа
    string codes[256]; // код каждого символа
    string order; // символы текста по убыванию частоты (для таблицы)
    void makeCodes(HNode* node, const string& code); // обход дерева, собираем коды
    void clear(HNode* node);
public:
    ~Huffman();
    void Build(const string& text); // строим дерево по тексту
    string Encode(const string& text); // строка -> двоичный код
    bool Decode(const string& code, string& result); // двоичный код -> строка, false если код неверный
    void PrintTable();
};