#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "set.h"

using namespace std;

// Множество с именем. Все множества из файла хранятся цепочкой
struct NamedSet {
    string name;
    Set set;
    NamedSet* next = nullptr;
};

// Найти множество по имени, если нет - nullptr
NamedSet* findSet(NamedSet* head, const string& name){
    NamedSet* temp = head;
    while (temp != nullptr){
        if (temp->name == name){
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

// Найти множество, а если его нет - создать пустое и добавить в конец цепочки
NamedSet* getOrCreate(NamedSet*& head, const string& name){
    NamedSet* found = findSet(head, name);
    if (found != nullptr){
        return found;
    }
    NamedSet* newSet = new NamedSet;
    newSet->name = name;
    if (head == nullptr){
        head = newSet;
    } else {
        NamedSet* temp = head;
        while (temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = newSet; // в конец, чтобы порядок множеств в файле не менялся
    }
    return newSet;
}

// Чтение файла: каждая строка вида "SET A 5 7 9"
void Load(NamedSet*& head, const string& path){
    ifstream file(path);
    if (!file.is_open()){
        return; // файла ещё нет, значит множеств нет
    }
    string line;
    while (getline(file, line)){
        istringstream ss(line);
        string type, name, value;
        if (!(ss >> type >> name) || type != "SET"){
            continue; // пропускаем чужие строки
        }
        NamedSet* ns = getOrCreate(head, name);
        while (ss >> value){
            ns->set.add(value);
        }
    }
    file.close();
}

// Запись всех множеств обратно в файл
void Save(NamedSet* head, const string& path){
    ofstream file(path);
    NamedSet* temp = head;
    while (temp != nullptr){
        file << "SET " << temp->name;
        string elems = temp->set.ToString();
        if (!elems.empty()){
            file << " " << elems;
        }
        file << endl;
        temp = temp->next;
    }
    file.close();
}

// Выполняет запрос. Возвращает true, если множества изменились и файл нужно перезаписать
bool Execute(NamedSet*& head, const string& query){
    istringstream ss(query);
    string command, name, value;
    ss >> command >> name >> value;

    if (name.empty()){
        cout << "Ошибка: не указано имя множества" << endl;
        return false;
    }

    if (command == "PRINT"){
        NamedSet* ns = findSet(head, name);
        if (ns == nullptr){
            cout << "Множества " << name << " нет" << endl;
        } else {
            ns->set.Print();
        }
        return false;
    }

    if (value.empty()){
        cout << "Ошибка: не указан элемент" << endl;
        return false;
    }

    if (command == "SETADD"){
        if (getOrCreate(head, name)->set.add(value)){
            cout << "Элемент " << value << " добавлен" << endl;
            return true;
        }
        cout << "Элемент " << value << " уже есть во множестве" << endl;
        return false;
    }

    if (command == "SETDEL"){
        NamedSet* ns = findSet(head, name);
        if (ns != nullptr && ns->set.del(value)){
            cout << "Элемент " << value << " удалён" << endl;
            return true;
        }
        cout << "Элемента " << value << " нет во множестве" << endl;
        return false;
    }

    if (command == "SET_AT"){
        NamedSet* ns = findSet(head, name);
        if (ns != nullptr && ns->set.at(value)){
            cout << "TRUE" << endl;
        } else {
            cout << "FALSE" << endl;
        }
        return false;
    }

    cout << "Неизвестная команда: " << command << endl;
    return false;
}

// Запуск: ./set --file <путь до файла> --query "<КОМАНДА> <имя множества> <элемент>"
int main(int argc, char* argv[]){
    string path;
    string query;

    // Разбираем аргументы командной строки
    for (int i = 1; i < argc; ++i){
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc){
            path = argv[++i];
        } else if (arg == "--query"){
            // запрос может прийти одним аргументом ("SETADD A 5") или несколькими (SETADD A 5)
            while (i + 1 < argc && string(argv[i + 1]) != "--file"){
                if (!query.empty()){
                    query += " ";
                }
                query += argv[++i];
            }
        }
    }

    if (path.empty() || query.empty()){
        cout << "Использование: ./set --file <файл> --query \"<КОМАНДА> <имя> <элемент>\"" << endl;
        return 1;
    }

    NamedSet* head = nullptr;
    Load(head, path); // 1) прочитали все множества из файла
    if (Execute(head, query)){ // 2) выполнили запрос
        Save(head, path); // 3) сохранили, если что-то изменилось
    }

    // Освобождаем память
    while (head != nullptr){
        NamedSet* next = head->next;
        delete head;
        head = next;
    }
    return 0;
}