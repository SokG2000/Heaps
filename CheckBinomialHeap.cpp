#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>
#include "BinomialHeap.hpp"

using std::vector;
using std::ifstream;
using std::ofstream;
using std::string;
using std::pair;
using std::make_pair;
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
        if (min_id == -1 || v[i].first < v[min_id].first) {
            min_id = i;
        }
    }
    if (min_id == -1) {
        throw std::out_of_range("Can not get minimum in empty heap.");
    }
    return v[min_id].first;
}

//template <typename T>
void delete_from_vector(vector <pair <int, int> > &v, int id) {
    if (id < 0 || id >= v.size()) {
        cout << "Wrong delete from vector\n";
        throw fatal_error();
    }
    for (int i = id; i < int(v.size()) - 1; i++) {
        v[i] = v[i + 1];
    }
    v.pop_back();
}

void change_vector(vector <pair <int, int> > &v, int id, int val) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i].second == id) {
            v[i].first = val;
            return;
        }
    }
    cout << "Wrong change vector\n";
    throw fatal_error();
}

void delete_vector(vector <pair <int, int> > &v, int id) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i].second == id) {
            delete_from_vector(v, i);
            return;
        }
    }
    cout << "Wrong delete vector\n" << "abacaba\n";
    throw fatal_error();
}

int extract_min_vect(vector <pair <int, int> > &v) {
    int min_id, res;
    min_id = -1;
    for (int i = 0; i < v.size(); i++) {
        if (min_id == -1 || v[i].first < v[min_id].first) {
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
    v.push_back(make_pair(x, id));
}

void merge_vectors(vector <vector <pair <int, int> > > &v, int id1, int id2) {
    while (v[id2].size() > 0) {
        v[id1].push_back(v[id2][v[id2].size() - 1]);
        v[id2].pop_back();
    }
}


void insert(vector <vector <pair <int, int> > > &v, vector <BinomialHeap<int>::Pointer> &pointers, vector <BinomialHeap <int> > &heaps, int x, int heap_id) {
    if (heap_id >= heaps.size()) {
        cout << "Bad test\n";
        throw fatal_error();
    }
    insert_vect(v[heap_id], x, pointers.size());
    pointers.push_back(heaps[heap_id].insert(x));
}

void is_empty(vector <vector <pair <int, int> > > &v, vector <BinomialHeap <int> > &heaps, int heap_id) {
    if (heaps[heap_id].is_empty() != is_empty_vect(v[heap_id])) {
        cout << "Different is_empty.\n";
        throw fatal_error();
    }
}

void get_min(vector <vector <pair <int, int> > > &v, vector <BinomialHeap <int> > &heaps, int heap_id) {
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
        ans2 = heaps[heap_id].get_min();
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

void extract_min(vector <vector <pair <int, int> > > &v, vector <BinomialHeap <int> > &heaps, int heap_id) {
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
        ans2 = heaps[heap_id].extract_min();
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

void delete_(vector <vector <pair <int, int> > > &v, vector<BinomialHeap <int> > &heaps, vector <BinomialHeap<int>::Pointer> &pointers, int id, int heap_id) { // delete element from h_id heap added in id insert
    if (id < 0 || id >= pointers.size()) {
        cout << "Test delete error.\n";
        throw fatal_error();
    }
    try {
        heaps[heap_id].delete_(pointers[id]);
        delete_vector(v[heap_id], id);
    }  catch(std::out_of_range &e) {
        cout << "Delete heap error\n";
        throw fatal_error();
    } catch(...) {
        cout << "Delete heap error\n";
        throw fatal_error();
    }
}

void change(vector <vector <pair <int, int> > > &v, vector <BinomialHeap <int> > &heaps, vector <BinomialHeap<int>::Pointer> &pointers, int id, int heap_id, int x) { // change element from h_id heap added in id insert
    if (id < 0 || id > pointers.size()) {
        cout << "Test change error.\n";
        throw fatal_error();
    }
    //if (deleted[id]) {
    //    return;
    //}
    try {
        heaps[heap_id].change(pointers[id], x);
        change_vector(v[heap_id], id, x);
    }  catch(std::out_of_range &e) {
        cout << "Change heap error\n";
        throw fatal_error();
    } catch(...) {
        cout << "Change heap error\n";
        throw fatal_error();
    }
}

void new_heap(vector <vector <pair <int, int> > > &v, vector <BinomialHeap <int> > &heaps) {
    v.push_back(vector <pair <int, int> > ());
    BinomialHeap <int> tmp;
    heaps.push_back(tmp);
}

void merge_(vector <vector <pair <int, int> > > &v, vector <BinomialHeap <int> > &heaps, int id1, int id2) {
    if (id1 < 0 || id1 >= v.size() || id2 < 0 || id2 >= v.size() || id1 == id2) {
        cout << "Merge wrong test\n";
        throw fatal_error();
    } else {
        heaps[id1].merge(heaps[id2]);
        merge_vectors(v, id1, id2);
    }
}

void check_correct(int test_num) {
    int n, typ, id, x, heap_id, h_id1, h_id2;
    vector <BinomialHeap <int> > heaps;
    vector <vector <pair <int, int> > > v;
    vector <BinomialHeap <int>::Pointer> pointers;
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> typ;
        if (typ <= 5) {
            fin >> heap_id;
            if (heap_id >= heaps.size()) {
                cout << "Bad test\n";
                throw fatal_error();
            }
        }
        if (typ == 0) {
            is_empty(v, heaps, heap_id);
        } else if (typ == 1) {
            fin >> x;
            insert(v, pointers, heaps, x, heap_id);
        } else if (typ == 2) {
            get_min(v, heaps, heap_id);
        } else if (typ == 3) {
            extract_min(v, heaps, heap_id);
        } else if (typ == 4) {
            fin >> id;
            delete_(v, heaps, pointers, id, heap_id);
        } else if (typ == 5) {
            fin >> id >> x;
            change(v, heaps, pointers, id, heap_id, x);
        } else if (typ == 6) {
            new_heap(v, heaps);
        } else if (typ == 7) {
            fin >> h_id1 >> h_id2;
            merge_(v, heaps, h_id1, h_id2);
        } else {
            cout << "Bad test\n";
            throw fatal_error();
        }
    }
    cout  << test_num << '\n';
}

void check_time(int test_num) {
    int start_time, end_time, time, heap_id, h_id1, h_id2;
    int n, typ, id, x, ans;
    bool ans1;
    start_time = clock();
    vector<BinomialHeap <int> > heaps;
    vector <BinomialHeap<int>::Pointer> pointers;
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> typ;
        if (typ <= 5) {
            fin >> heap_id;
            if (heap_id >= heaps.size()) {
                cout << "Bad test\n";
                throw fatal_error();
            }
        }
        if (typ == 0) {
            try {
                ans1 = heaps[heap_id].is_empty();
            } catch (...) {}
        } else if (typ == 1) {
            fin >> x;
            try {
                pointers.push_back(heaps[heap_id].insert(x));
            } catch (...) {}
        } else if (typ == 2) {
            try {
                ans = heaps[heap_id].get_min();
            } catch (...) {}
        } else if (typ == 3) {
            try {
                ans = heaps[heap_id].extract_min();
            } catch (...) {}
        } else if (typ == 4) {
            fin >> id;
            try {
                heaps[heap_id].delete_(pointers[id]);
            } catch (...) {}
        } else if (typ == 5) {
            fin >> id >> x;
            try {
                heaps[heap_id].change(pointers[id], x);
            } catch (...) {}
        } else if (typ == 6) {
            heaps.push_back(BinomialHeap <int> ());
        } else if (typ == 7) {
            fin >> h_id1 >> h_id2;
            if (h_id1 == h_id2 || h_id1 >= heaps.size() || h_id2 >= heaps.size()) {
                cout << "Bad test\n";
                throw fatal_error();
            }
            heaps[h_id1].merge(heaps[h_id2]);
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
