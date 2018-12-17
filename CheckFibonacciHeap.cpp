#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>
#include "FibonacciHeap.hpp"

using std::pair;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::string;
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

int get_min_vect(vector <pair <int, int> > &v) {
    int min_id = -1;
    for (int i = 0; i < v.size(); i++) {
        if (min_id == -1 || v[i] < v[min_id]) {
            min_id = i;
        }
    }
    if (min_id == -1) {
        throw std::out_of_range("Can not get minimum in empty heap.");
    }
    return v[min_id].first;
}

void delete_from_vector(vector <pair <int, int> > &v, int id) {
    for (int i = id; i < int(v.size()) - 1; i++) {
        v[i] = v[i + 1];
    }
    v.pop_back();
}

int extract_min_vect(vector <pair <int, int> > &v) {
    int min_id = -1;
    int res;
    for (int i = 0; i < v.size(); i++) {
        if (min_id == -1 || v[i] < v[min_id]) {
            min_id = i;
        }
    }
    if (min_id == -1) {
        throw std::out_of_range("Can not get minimum in empty heap.");
    }
    res = v[min_id].first;
    delete_from_vector(v, min_id);
    return res;
}

bool is_empty_vect(vector <pair <int, int> > &v) {
    return v.size() == 0;
}

void insert_vect(vector <pair <int, int> > &v, int x, int id) {
    v.push_back(std::make_pair(x, id));
}

void insert(vector <vector <pair <int, int> > > &v, vector <FibonacciHeap<int>::Pointer> &pointers, vector<FibonacciHeap <int> > &h, int x, int heap_id) {
    if (heap_id >= h.size()) {
        cout << "insert test error\n";
        throw fatal_error();
    }
    insert_vect(v[heap_id], x, pointers.size());
    pointers.push_back(h[heap_id].insert(x));
}

void is_empty(vector <vector <pair <int, int> > > &v, vector<FibonacciHeap <int> > &h, int heap_id) {
    if (h[heap_id].is_empty() != is_empty_vect(v[heap_id])) {
        cout << "Different is_empty.\n";
        throw fatal_error();
    }
}

void get_min(vector <vector <pair <int, int> > > &v, vector<FibonacciHeap <int> > &h, int heap_id) {
    bool flag1, flag2;
    flag1 = false;
    flag2 = false;
    int ans1, ans2;
    try {
        ans1 = get_min_vect(v[heap_id]);
    } catch(std::out_of_range &e) {
        flag1 = true;
    } catch(...) {
        cout << "Get test error\n";
        throw fatal_error();
    }
    try {
        ans2 = h[heap_id].get_min();
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

void extract_min(vector <vector <pair <int, int> > > &v, vector<FibonacciHeap <int> > &h, int heap_id) {
    bool flag1, flag2;
    flag1 = false;
    flag2 = false;
    int ans1, ans2;
    try {
        ans1 = extract_min_vect(v[heap_id]);
    } catch(std::out_of_range &e) {
        flag1 = true;
    } catch(...) {
        cout << "Extract test error\n";
        throw fatal_error();
    }
    try {
        ans2 = h[heap_id].extract_min();
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

void vector_delete(vector <pair <int, int> > &v, int id) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i].second == id) {
            delete_from_vector(v, i);
            return;
        }
    }
    cout << "Bad del\n";
}

void delete_(vector <vector <pair <int, int> > > &v, vector <FibonacciHeap <int> > &h, vector <FibonacciHeap<int>::Pointer> &pointers, int id, int heap_id) { // delete element added in id insert
    if (id < 0 || id > pointers.size()) {
        cout << "Test delete error.\n";
        throw fatal_error();
    }
    try {
        h[heap_id].delete_(pointers[id]);
        vector_delete(v[heap_id], id);
    }  catch(std::out_of_range &e) {
        cout << "Delete heap error1\n";
        throw fatal_error();
    } catch(...) {
        cout << "Delete heap error\n";
        throw fatal_error();
    }
}

void vector_decrease(vector <pair <int, int> > &v, int id, int x) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i].second == id) {
            if (v[i].first < x) {
                throw std::logic_error("Wrong decrease\n");
            }
            v[i].first = x;
            return;
        }
    }
}

void decrease(vector <vector <pair <int, int> > > &v, vector <FibonacciHeap <int> > &h, vector <FibonacciHeap<int>::Pointer> &pointers, int id, int x, int heap_id) { // decrease element added in id insert
    bool flag1, flag2;
    flag1 = false;
    flag2 = false;
    try {
        vector_decrease(v[heap_id], id, x);
    } catch(std::logic_error &e) {
        flag1 = true;
    } catch(...) {
        cout << "Decrease test error\n";
        throw fatal_error();
    }
    try {
        h[heap_id].decrease(pointers[id], x);
    }  catch(std::logic_error &e) {
        flag2 = true;
    } catch(...) {
        cout << "Decrease heap error\n";
        throw fatal_error();
    }
    if (flag1 != flag1) {
        cout << "Different decrease flags.\n";
        throw fatal_error();
    }
}

void add_heap(vector <vector <pair <int, int> > > &v, vector <FibonacciHeap <int> > &h) {
    v.push_back(vector <pair <int, int> > ());
    h.push_back(FibonacciHeap <int> ());
}

void merge_vectors(vector <vector <pair <int, int> > > &v, int id1, int id2) {
    while (v[id2].size() > 0) {
        v[id1].push_back(v[id2][v[id2].size() - 1]);
        v[id2].pop_back();
    }
}

void merge(vector <vector <pair <int, int> > > &v, vector <FibonacciHeap <int> > &h, int id1, int id2) {
    int max_id;
    max_id = v.size();
    if (id1 < 0 || id1 >= max_id || id2 < 0 || id2 >= max_id || id1 == id2) {
        return;
    }
    merge_vectors(v, id1, id2);
    h[id1].merge(h[id2]);
}

void print_vectors(vector <vector <pair <int, int> > > &v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            cout << v[i][j].first << ' ' << v[i][j].second << "   ";
        }
        cout << '\n';
    }
    cout << '\n';
}

void check_correct(int test_num) {
    int n, typ, id, x, heap_id, h_id1, h_id2;
    vector <FibonacciHeap <int> > MyHeap;
    vector <vector <pair <int, int> > > v;
    //vector <bool> deleted;
    vector <FibonacciHeap<int>::Pointer> pointers;
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> typ;
        if (typ <= 5) {
            fin >> heap_id;
            if (heap_id >= v.size()) {
                cout << "Check correct out of range\n";
                throw fatal_error();
            }
        }
        if (typ == 0) {
            is_empty(v, MyHeap, heap_id);
        } else if (typ == 1) {
            fin >> x;
            insert(v, pointers, MyHeap, x, heap_id);
        } else if (typ == 2) {
            get_min(v, MyHeap, heap_id);
        } else if (typ == 3) {
            extract_min(v, MyHeap, heap_id);
        } else if (typ == 4) {
            fin >> id;
            delete_(v, MyHeap, pointers, id, heap_id);
        } else if (typ == 5) {
            fin >> id >> x;
            decrease(v, MyHeap, pointers, id, x, heap_id);
        } else if (typ == 6) {
            add_heap(v, MyHeap);
        } else if (typ == 7) {
            fin >> h_id1 >> h_id2;
            merge(v, MyHeap, h_id1, h_id2);
        } else {
            cout << "Bad test\n";
            throw fatal_error();
        }
    }
    cout << test_num << '\n';
}

void check_time(int test_num) {
    int start_time, end_time, time, heap_id, h_id1, h_id2;
    int n, typ, id, x, ans;
    bool ans1;
    start_time = clock();
    vector <FibonacciHeap <int> > MyHeap;
    vector <FibonacciHeap<int>::Pointer> pointers;
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> typ;
        if (typ <= 5) {
            fin >> heap_id;
            if (heap_id >= MyHeap.size()) {
                cout << "Check time out of range\n";
                throw fatal_error();
            }
        }
        if (typ == 0) {
            try {
                ans1 = MyHeap[heap_id].is_empty();
            } catch (...) {}
        } else if (typ == 1) {
            fin >> x;
            try {
                pointers.push_back(MyHeap[heap_id].insert(x));
            } catch (...) {}
        } else if (typ == 2) {
            try {
                ans = MyHeap[heap_id].get_min();
            } catch (...) {}
        } else if (typ == 3) {
            try {
                ans = MyHeap[heap_id].extract_min();
            } catch (...) {}
        } else if (typ == 4) {
            fin >> id;
            try {
                MyHeap[heap_id].delete_(pointers[id]);
            } catch (...) {}
        } else if (typ == 5) {
            fin >> id >> x;
            try {
                MyHeap[heap_id].decrease(pointers[id], x);
            } catch (...) {}
        } else if (typ == 6) {
            MyHeap.push_back(FibonacciHeap <int> ());
        } else if (typ == 7) {
            fin >> h_id1 >> h_id2;
            MyHeap[h_id1].merge(MyHeap[h_id2]);
        }
        else {
            cout << "Bad test\n";
            throw fatal_error();
        }
    }
    end_time = clock();
    time = end_time - start_time;
    cout << test_num << ' ' << time << '\n';
    if (time > 30000) {
        cout << "Too slow!";
        throw fatal_error();
    }
}

int main() {
    string file_name;
    const int small_test_num = 10;
    const int test_num = 30;
    for (int i = 1; i <= small_test_num; i++) {
        file_name = "BinomialHeapTests\\" + to_string(i) + ".txt";
        fin.open(file_name);
        check_correct(i);
        fin.close();
    }
    for (int i = 1; i <= test_num; i++) {
        file_name = "BinomialHeapTests\\" + to_string(i) + ".txt";
        fin.open(file_name);
        check_time(i);
        fin.close();
    }
    return 0;
}
