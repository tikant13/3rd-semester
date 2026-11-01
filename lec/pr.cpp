#include <iostream>

using namespace std;

class Vector {
public:
    size_t sz;
    int* data;
    Vector(){
        sz = 0;
        data = nullptr;
    }
    Vector(size_t s){
        sz = s;
        data = new int [sz];
    }
    Vector (size_t s, int value){
        sz = s;
        data = new int [sz];
        for (size_t i = 0; i < sz; ++i){
            data[i] = value;
        }
    }
    void Print(){
        for (int i = 0; i < sz; ++i){
            cout << data[i] << " ";
        }
        cout << endl;
    }

    int operator [] (int x) const{
        return *(data+x);
    };

    int Sum(){
        int sum = 0;
        for (int i = 0; i < sz; ++i){
            sum +=data[i];
        }
        return sum;
    }

    int Max(){
        int mx = data[0];
        for (int i = 0; i < sz; ++i){
            if (mx < data[i]){
                mx = data[i];
            }
        }
        return mx;
    }

    int Min(){
        int mn = data[0];
        for (int i = 0; i < sz; ++i){
            if (mn > data[i]){
                mn = data[i];
            }
        }
        return mn;
    }

    ~Vector(){
        clear();
    }

    void clear(){
        cout << "destr " << sz ;
        delete[] data;
    }

    Vector operator +(const Vector& v){
        Vector result [sz];
        for (size_t i = 0; i < sz; ++i){
            //result.data[i] = data[i] +v.data[i];
        }
        return result[sz];
    }
};

ostream& operator<<(ostream& os, const Vector& v){
    for (int i = 0; i < v.sz; ++i){
        os << v.data[i] << " ";
    }
    return os;
}

int main(){
    Vector v1(10, 1);
    Vector v2(10, 2);
    Vector v3 = v1 + v2;
    //v3.Print();
    //cout << v3[5];
    //cout << v3;
    cout << v3.Sum();
    cout << endl;
    cout << v3.Max();
    cout << endl;
    cout << v3.Min();
    cout << endl;

    return 0;

}