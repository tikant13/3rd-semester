#include <iostream>
#include <string>
#include <vector>
#include <memory>

//   чтобы заполнить перем, нужно написать коструктор
using namespace std;

struct S {
    int x : 10 ; //4
    double f; //8
    char c; //1
    // padding
    // standart layout
    // память выравнивается по значению большего элемента
    // от правильного расположения зависит сколко памяти занимает 
    // иногда в структурах применяются битовые поля
    // int x : 4;
    // есть риск когда элементы помещаются в 1 ячейку
};

struct Child : Parent

int main(){
    S s;
    cout << sizeof(S);
    return 0;
}
/*
class History {
public:
string str;
    History (const string& s) : str(s) {
        cout << str << " created " << endl;
    }
    ~History(){
        cout << str << " destroy " << endl;
    }
};

class Creatures {
public:
    string name = "creatures";
    Creatures( const string& s) : h(s + " creatures ") {}
    History h;
};

class Ogr : public Creatures {
public:
    Ogr() : Creatures("ogr"){}
    History h = History("ogr");
};

class King : public Creatures {
public:
    King(const string& str) : Creatures("king"), h("king"), nameKing(str) {}
    string nameKing;
    History h;
    //vector <Treasu>;
    string title;
};

*/


/* 
Полиморфизм 
Сигнатура - это манглетнное имя ...

 vtabdle pointer
 таблица виртуальных функ находится на уровне смешения 0
*/


