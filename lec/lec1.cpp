#include <iostream>
#include <string>
#include <memory>

//   чтобы заполнить перем, нужно написать коструктор
using namespace std;

struct Food {
    int clr = 0;
    string name = "food";
};


struct Eye : public Food{
    int clr = 100;
    string name = "Eye";
};

struct Soup : public Food {
    Soup(){
        int clr = 150;
        string name = "soup";
    }
};

struct AngrPeople : public Food {
    int clr = 500;
};

struct Pumpkee : public Food{
    Pumpkee(){
        int clr = 80;
        string name = "Pumpkee";
    }
};

struct Bugs : public Food {
    int clr = 50;

};

class Donkey {
public:
    void Eat(const Food& food){
        cout << food.clr << endl;
    }

};

class Creatures {
public:
    string name;
    void Eat(const Food& food){
        cout << name << food.name<< " " << food.clr << endl;
    }
    Creatures(const string s) : name(s) {}
    virtual void Say (){

    }

};

class Ogr : public Creatures {
public:
    Ogr () : Creatures("Ogr"){}
    void Say() override{
        cout << "Hello"<< endl;
    }
};

class King : public Creatures {
public:
    King() : Creatures("King") {}
    King (const string& s) : Creatures("King"), str(s) {}
    string str;
};

int main(){
    Ogr Shrek;
    Pumpkee p;
    Shrek.Eat(p);
    King king;


    shared_ptr<Creatures> crr;
    crr = make_shared<Ogr>;
    crr = make_shared<King> ("Name");
    crr->Say();

    //!!! потом глянуть 
    //vector<shared_ptr<Creatures>> manyCreatures = {
    //    make_shared<Ogr> (), make_shared<King> ("Name");
    //}

    
    return 0;
}


/* 
наследование = это способ выразить связи между классами

один класс является подтипом другого класса реализуя часть его функциональности, и может использоваться в связке с ним

Преимущества
1)Иерархия между классами
2) решение проблем с дублиров кода
3) возможность написания универсальных функций


Базово class Child : public Parent {};

методы которые работают уникально в каждом потоке называется виртувльными (virtual) по ссылке обязательно

в классе потомков надо писать overrid
*/


