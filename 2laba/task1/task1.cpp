#include <iostream>
#include <string>
#include "stack.h"

using namespace std;


string fixBrackets(const string& str){
    Stack st; // стек с индексами открытых (
    int open = 0; // сколько ( сейчас лежит в стеке
    string marks(str.size(), '0'); // по символу на каждую позицию: 1 - удалить, 0 - оставить

    //проходим по строке
    for (int i = 0; i < (int)str.size(); ++i){
        if (str[i] == '('){
            st.push(to_string(i)); // запоминаем позицию (
            ++open;
        } else if (str[i] == ')'){
            if (open == 0){
                marks[i] = '1'; // открытой скобки нет, значит ) лишняя
            } else {
                st.pop(); // ) закрывает последнюю (
                --open;
            }
        }
    }

    //в стеке остались ( без пары, ToString() отдает их индексы через пробел
    string rest = st.ToString();
    string num;
    for (int i = 0; i <= (int)rest.size(); ++i){
        if (i == (int)rest.size() || rest[i] == ' '){
            if (!num.empty()){
                marks[stoi(num)] = '1';  // помечаем ( на удаление
                num = "";
            }
        } else {
            num += rest[i];
        }
    }

    // собираем ответ из непомеченных символов
    string res;
    for (int i = 0; i < (int)str.size(); ++i){
        if (marks[i] == '0'){
            res += str[i];
        }
    }
    return res;
}

int main(){
    string str;
    getline(cin, str);
    cout << fixBrackets(str) << endl;
    return 0;
}