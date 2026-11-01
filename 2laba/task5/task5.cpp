#include <iostream>
#include <string>
#include "Haffman.h"

using namespace std;

int main(){
    string text;
    cout << "Введите строку: ";
    getline(cin, text);
    if (text.empty()){
        cout << "Ошибка: пустая строка" << endl;
        return 1;
    }

    Huffman h;
    h.Build(text);

    // Строка -> код
    string code = h.Encode(text);
    cout << "Код: " << code << endl;
    cout << "Таблица:" << endl;
    h.PrintTable();
    cout << "Было " << text.size() * 8 << " бит, стало " << code.size() << " бит" << endl;

    // Код в строку
    string decoded;
    h.Decode(code, decoded);
    cout << "Восстановленная строка: " << decoded << endl;

    // Можно расшифровать любой другой код по этой же таблице
    string line;
    while (true){
        cout << "Введите код для расшифровки (пустая строка - выход): ";
        if (!getline(cin, line) || line.empty()){
            break;
        }
        string res;
        if (h.Decode(line, res)){
            cout << res << endl;
        } else {
            cout << "Ошибка" << endl;
        }
    }
    return 0;
}