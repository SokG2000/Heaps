#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>
#include "Vector.hpp"

using std::vector;
using std::ifstream;
using std::ofstream;
using std::string;
const int test_num = 30;

struct fatal_error{};

string to_string(int num) {
    string res;
    res = "";
    while(num > 0) {
        res += '0' + (num % 10);
        num /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

int get_vect(vector <int> &v, int i) {
    if (i >= 0 && i < v.size()) {
        return v[i];
    }
    throw std::out_of_range("Vector index is out of range.");
}

void set_vect(vector <int> &v, int i, int x) {
    if (i >= 0 && i < v.size()) {
        v[i] = x;
        return;
    }
    throw std::out_of_range("Vector index is out of range.");
}

void pop_vect(vector <int> &v) {
    if (v.size() == 0) {
        throw std::out_of_range("Can not delete from empty vector.");
    }
    v.pop_back();
}

void push(Vector <int> &v1, vector <int> &v2, int x) {
    v1.push_back(x);
    v2.push_back(x);
}

void pop(Vector <int> &v1, vector <int> &v2) {
    bool flag1, flag2;
    flag1 = false;
    flag2 = false;
    try {
        v1.pop_back();
    }  catch(std::out_of_range &e) {
        flag1 = true;
    } catch(...) {
        cout << "Vector error\n0\n";
        throw fatal_error();
    }
    try {
        pop_vect(v2);
    }  catch(std::out_of_range &e) {
        flag2 = true;
    } catch(...) {
        cout << "Test error\n0\n";
        throw fatal_error();
    }
    if (flag1 != flag2) {
        cout << "Pop error\n";
        throw fatal_error();
    }
}

void get(Vector <int> &v1, vector <int> &v2, int i) {
    bool flag1, flag2;
    flag1 = false;
    flag2 = false;
    int ans1, ans2;
    try {
        ans1 = v1[i];
    }  catch(std::out_of_range &e) {
        flag1 = true;
    } catch(...) {
        cout << "Get Vector error\n";
        throw fatal_error();
    }
    try {
        ans2 = get_vect(v2, i);
    }  catch(std::out_of_range &e) {
        flag2 = true;
    } catch(...) {
        cout << "Get test error\n";
        throw fatal_error();
    }
    if (flag1 != flag1) {
        cout << "Different get flags.\n";
        throw fatal_error();
    }
    if (!flag1 && ans1 != ans2) {
        cout << "Different get ans\n";
        cout << ans1 << ' ' << ans2 << '\n';
        throw fatal_error();
    }
}

void set(Vector <int> &v1, vector <int> &v2, int i, int x) {
    bool flag1, flag2;
    flag1 = false;
    flag2 = false;
    try {
        v1[i] = x;
    }  catch(std::out_of_range &e) {
        flag1 = true;
    } catch(...) {
        cout << "Set Vector error\n";
        throw fatal_error();
    }
    try {
        set_vect(v2, i, x);
        //v2[i] = x;
    }  catch(std::out_of_range &e) {
        flag2 = true;
    } catch(...) {
        cout << "Set test error\n";
        throw fatal_error();
    }
    if (flag1 != flag2) {
        cout << "Set error\n3\n";
        throw fatal_error();
    }
}

int main() {
    string file_name;
    int start_time, end_time, time;
    for (int i = 1; i <= test_num; i++) {
        file_name = "VectorTests\\" + to_string(i) + ".txt";
        ifstream fin(file_name);
        int n, typ, id, x;
        start_time = clock();
        Vector <int> MyVector;
        vector <int> v;
        fin >> n;
        for (int j = 0; j < n; j++) {
            fin >> typ;
            if (typ == 0) {
                pop(MyVector, v);
            } else if (typ == 1) {
                fin >> x;
                push(MyVector, v, x);
            } else if (typ == 2) {
                fin >> id;
                get(MyVector, v, id);
            } else if (typ == 3) {
                fin >> id >> x;
                set(MyVector, v, id, x);
            } else {
                cout << "Bad test\n";
                throw fatal_error();
            }
        }
        end_time = clock();
        time = end_time - start_time;
        cout << i << ' ' << n << ' ' << time << '\n';
        if (time > 2000) {
            cout << "Too long!\n";
            return 0;
        }
    }
    return 0;
}
