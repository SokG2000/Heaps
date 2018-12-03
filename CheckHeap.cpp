#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>
#include "Heap.hpp"

using std::vector;
using std::ifstream;
using std::ofstream;
using std::string;
const int test_num = 1;
ifstream fin;

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

int get_min_vect(vector <int> &v, vector <bool> &deleted) {
    int min_id = -1;
    for (int i = 0; i < v.size(); i++) {
        if (!deleted[i]) {
            if (min_id == -1 || v[i] < v[min_id]) {
                min_id = i;
            }
        }
    }
    if (min_id == -1) {
        throw std::out_of_range("Can not get minimum in empty heap.");
    }
    return v[min_id];
}

int extract_min_vect(vector <int> &v, vector <bool> &deleted) {
    int min_id = -1;
    for (int i = 0; i < v.size(); i++) {
        if (!deleted[i]) {
            if (min_id == -1 || v[i] < v[min_id]) {
                min_id = i;
            }
        }
    }
    if (min_id == -1) {
        throw std::out_of_range("Can not get minimum in empty heap.");
    }
    deleted[min_id] = true;
    return v[min_id];
}

bool is_empty_vect(vector <int> &v, vector <bool> &deleted) {
    for (int i = 0; i < v.size(); i++) {
        if (!deleted[i]) {
            return false;
        }
    }
    return true;
}

template <class Iterator>
void create_vect(vector <int> &v, vector <bool> &deleted, Iterator start, Iterator finish) {
    while (start != finish) {
        v.push_back(*start);
        deleted.push_back(false);
        start++;
    }
}

void insert_vect(vector <int> &v, vector <bool> &deleted, int x) {
    v.push_back(x);
    deleted.push_back(false);
}

void insert(vector <int> &v, vector <bool> &deleted, vector <THeap<int>::Pointer> &pointers, THeap <int> &h, int x) {
    insert_vect(v, deleted, x);
    pointers.push_back(h.insert(x));
}

void is_empty(vector <int> &v, vector <bool> &deleted, THeap <int> &h) {
    if (h.is_empty() != is_empty_vect(v, deleted)) {
        cout << "Different is_empty.\n";
        //cout << h.is_empty() << ' ' << is_empty_vect(v, deleted) << '\n';
        throw fatal_error();
    }
}

void get_min(vector <int> &v, vector <bool> &deleted, THeap <int> &h) {
    bool flag1, flag2;
    flag1 = false;
    flag2 = false;
    int ans1, ans2;
    try {
        ans1 = get_min_vect(v, deleted);
    } catch(std::out_of_range &e) {
        flag1 = true;
    } catch(...) {
        cout << "Get test error\n";
        throw fatal_error();
    }
    try {
        ans2 = h.get_min();
    }  catch(std::out_of_range &e) {
        flag2 = true;
    } catch(...) {
        cout << "Get heap error\n";
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

void extract_min(vector <int> &v, vector <bool> &deleted, THeap <int> &h) {
    bool flag1, flag2;
    flag1 = false;
    flag2 = false;
    int ans1, ans2;
    try {
        ans1 = extract_min_vect(v, deleted);
    } catch(std::out_of_range &e) {
        flag1 = true;
    } catch(...) {
        cout << "Extract test error\n";
        throw fatal_error();
    }
    try {
        ans2 = h.extract_min();
    }  catch(std::out_of_range &e) {
        flag2 = true;
    } catch(...) {
        cout << "Extract heap error\n";
        throw fatal_error();
    }
    if (flag1 != flag1) {
        cout << "Different extract flags.\n";
        throw fatal_error();
    }
    if (!flag1 && ans1 != ans2) {
        cout << "Different extract ans\n";
        cout << ans1 << ' ' << ans2 << '\n';
        throw fatal_error();
    }
}

void delete_(vector <int> &v, vector <bool> &deleted, THeap <int> &h, vector <THeap<int>::Pointer> &pointers, int id) { // delete element added in id insert
    if (id < 0 || id > v.size()) {
        cout << "Test delete error.\n";
        throw fatal_error();
    }
    if (deleted[id]) {
        return;
    }
    try {
        h.delete_(pointers[id]);
        deleted[id] = true;
    }  catch(std::out_of_range &e) {
        cout << "Delete heap error\n";
        throw fatal_error();
    } catch(...) {
        cout << "Delete heap error\n";
        throw fatal_error();
    }
}

void change(vector <int> &v, vector <bool> &deleted, THeap <int> &h, vector <THeap<int>::Pointer> &pointers, int id, int x) { // delete element added in id insert
    if (id < 0 || id > v.size()) {
        cout << "Test change error.\n";
        throw fatal_error();
    }
    if (deleted[id]) {
        return;
    }
    try {
        h.change(pointers[id], x);
        v[id] = x;
    }  catch(std::out_of_range &e) {
        cout << "Change heap error\n";
        throw fatal_error();
    } catch(...) {
        cout << "Change heap error\n";
        throw fatal_error();
    }
}

void check_correct(int test_num) {
    int n, typ, id, x;
    THeap <int> MyHeap;
    vector <int> v;
    vector <bool> deleted;
    vector <THeap<int>::Pointer> pointers;
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> typ;
        if (typ == 0) {
            is_empty(v, deleted, MyHeap);
        } else if (typ == 1) {
            fin >> x;
            insert(v, deleted, pointers, MyHeap, x);
        } else if (typ == 2) {
            get_min(v, deleted, MyHeap);
        } else if (typ == 3) {
            extract_min(v, deleted, MyHeap);
        } else if (typ == 4) {
            fin >> id;
            delete_(v, deleted, MyHeap, pointers, id);
        } else if (typ == 5) {
            fin >> id >> x;
            change(v, deleted, MyHeap, pointers, id, x);
        } else {
            cout << "Bad test\n";
            throw fatal_error();
        }
    }
    cout << test_num << '\n';
}

void check_time(int test_num) {
    int start_time, end_time, time;
    int n, typ, id, x, ans;
    bool ans1;
    start_time = clock();
    THeap <int> MyHeap;
    vector <THeap<int>::Pointer> pointers;
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> typ;
        if (typ == 0) {
            try {
                ans1 = MyHeap.is_empty();
            } catch (...) {}
        } else if (typ == 1) {
            fin >> x;
            try {
                pointers.push_back(MyHeap.insert(x));
            } catch (...) {}
        } else if (typ == 2) {
            try {
                ans = MyHeap.get_min();
            } catch (...) {}
        } else if (typ == 3) {
            try {
                ans = MyHeap.extract_min();
            } catch (...) {}
        } else if (typ == 4) {
            fin >> id;
            try {
                MyHeap.delete_(pointers[id]);
            } catch (...) {}
        } else if (typ == 5) {
            fin >> id >> x;
            try {
                MyHeap.change(pointers[id], x);
            } catch (...) {}
        } else {
            cout << "Bad test\n";
            throw fatal_error();
        }
    }
    end_time = clock();
    time = end_time - start_time;
    cout << test_num << ' ' << time << '\n';
    if (time > 10000) {
        cout << "Too slow!";
        throw fatal_error();
    }
}

int main() {
    string file_name;
    const int small_test_num = 19;
    const int test_num = 30;
    for (int i = 1; i <= small_test_num; i++) {
        file_name = "HeapTests\\" + to_string(i) + ".txt";
        fin.open(file_name);
        check_correct(i);
        fin.close();
    }
    for (int i = 1; i <= test_num; i++) {
        file_name = "HeapTests\\" + to_string(i) + ".txt";
        fin.open(file_name);
        check_time(i);
        fin.close();
    }
    return 0;
}
