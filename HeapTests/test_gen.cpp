#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>

using std::vector;
using std::ifstream;
using std::ofstream;
using std::string;
using std::set;
std::mt19937 gen(1543);
ofstream fout;

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

void change_test_gen(int n) {
    int x, id, rest, typ, max_pointer;
    set <int> undeleted;
    max_pointer = 0;
    std::cout << "n = " << n << '\n';
    fout << n << '\n' << 0 << '\n';
    n--;
    rest = n;
    for (int i = 0; i <= n / 2; i++) {
        x = gen();
        fout << "1 " << x << '\n';
        rest--;
        undeleted.insert(max_pointer);
        max_pointer++;
    }
    n = rest;
    for (int i = 0; i < n / 3; i++) {
        id = gen();
        id = (id % (max_pointer + 1) + (max_pointer + 1)) % (max_pointer + 1);
        set<int>::iterator it;
        it = undeleted.lower_bound(id);
        while (it == undeleted.end()) {
            id = gen();
            id = (id % (max_pointer + 1) + (max_pointer + 1)) % (max_pointer + 1);
            it = undeleted.lower_bound(id);
        }
        undeleted.erase(it);
        fout << 4 << ' ' << *it << '\n';
        rest--;
    }
    n = rest;
    for (int i = 0; i < n; i++) {
        typ = (gen() % 5 + 5) % 5;
        if (typ == 3) {
            typ = 5;
        }
        if (typ == 0) {
            fout << "0\n";
        } else if (typ == 1) {
            x = gen();
            fout << "1 " << x << '\n';
            undeleted.insert(max_pointer);
            max_pointer++;
        } else if (typ == 2) {
            fout << "2\n";
        } else if (typ == 4) {
            id = gen();
            id = (id % (max_pointer + 1) + (max_pointer + 1)) % (max_pointer + 1);
            set<int>::iterator it;
            it = undeleted.lower_bound(id);
            while (it == undeleted.end()) {
                id = gen();
                id = (id % (max_pointer + 1) + (max_pointer + 1)) % (max_pointer + 1);
                it = undeleted.lower_bound(id);
            }
            undeleted.erase(it);
            fout << 4 << ' ' << *it << '\n';
        } else {
            x = gen();
            id = gen();
            id = (id % (max_pointer + 1) + (max_pointer + 1)) % (max_pointer + 1);
            set<int>::iterator it;
            it = undeleted.lower_bound(id);
            while (it == undeleted.end()) {
                id = gen();
                id = (id % (max_pointer + 1) + (max_pointer + 1)) % (max_pointer + 1);
                it = undeleted.lower_bound(id);
            }
            fout << 5 << ' ' << *it << ' ' << x << '\n';
       }
    }
}

void extract_test_gen(int n) {
    int x, id, rest, typ, max_pointer;
    max_pointer = 0;
    std::cout << "n = " << n << '\n';
    fout << n << '\n' << 0 << '\n';
    n--;
    rest = n;
    for (int i = 0; i <= n / 2; i++) {
        x = gen();
        fout << "1 " << x << '\n';
        rest--;
    }
    n = rest;
    for (int i = 0; i < n / 3; i++) {
        fout << "3\n";
        rest--;
    }
    n = rest;
    for (int i = 0; i < n; i++) {
        typ = (gen() % 4 + 4) % 4;
        if (typ == 0) {
            fout << "0\n";
        } else if (typ == 1) {
            x = gen();
            fout << "1 " << x << '\n';
        } else if (typ == 2) {
            fout << "2\n";
        } else {
            fout << "3\n";
        }
    }
}

int main() {
    const int test_num = 30;
    const int small_test_num = 20;
    const int very_small = 10;
    int n, x, id, rest, typ, len;
    string file_name;
    for (int i = 2; i <= test_num; i++) {
        std::cout << i << '\n';
        file_name = to_string(i) + string(".txt");
        fout.open(file_name);
        if (i < very_small) {
            n = 10;
        } else if (i < small_test_num) {
            n = 1000;
        } else {
            n = 1000000;
        }
        if (i % 2 == 0) {
            change_test_gen(n);
        } else {
            extract_test_gen(n);
        }
        fout.close();
    }
}
