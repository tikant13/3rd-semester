#include <string>
#include "Fold.h"
using namespace std;

const int PART = 3; // сколько цифр в одной части

long long foldHash(const string& key, string& expr){
    // Шаг 1: ключ должен быть числом. Если в нём есть не цифры,
    // заменяем каждый символ его кодом: "ab" -> "97" + "98" = "9798"
    bool onlyDigits = !key.empty();
    for (int i = 0; i < (int)key.size(); ++i){
        if (key[i] < '0' || key[i] > '9'){
            onlyDigits = false;
        }
    }
    string digits;
    if (onlyDigits){
        digits = key;
    } else {
        for (int i = 0; i < (int)key.size(); ++i){
            digits += to_string((unsigned char)key[i]);
        }
    }

    // Шаг 2: режем на части по 3 цифры слева направо и складываем
    long long sum = 0;
    expr = "";
    for (int i = 0; i < (int)digits.size(); i += PART){
        string part = digits.substr(i, PART); // последняя часть может быть короче
        sum += stoll(part);
        if (!expr.empty()){
            expr += "+";
        }
        expr += part;
    }
    expr += " = " + to_string(sum);
    return sum;
}

long long foldHash(const string& key){
    string expr; // выражение не нужно, просто выбрасываем
    return foldHash(key, expr);
}
