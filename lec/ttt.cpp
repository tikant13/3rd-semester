#include <iostream>
#include <string>

using namespace std;

class Animal {
public:
    string name;
    int age;
    Animal() {
        age = 0;
        name = "???";
    }

    Animal (string Name, int Age){
        name = Name;
        age = Age;
    }

    
};

class LionDota2 : public Animal {
public:
    int grivaSize;
    LionDota2(int GrivaSize, string Name, int Age) : Animal(Name, Age), grivaSize(GrivaSize) {}
    LionDota2 () : Animal(), grivaSize(0) {}
};

int main(){
    LionDota2 lion(5, "LION!!!!", 80);
    cout << "Griva " << lion.grivaSize<< endl;
    cout << "Age " << lion.age << endl;
    cout << "Name " << lion.name << endl;
    return 0;
}