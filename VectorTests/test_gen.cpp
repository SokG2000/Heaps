#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using std::vector;
using std::ifstream;
using std::ofstream;
using std::string;
std::mt19937 gen(1543);

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

int main() {
    const int test_num = 30;
    const int small_test_num = 20;
    const int very_small = 10;
    int n, x, id, rest, typ, len;
    ofstream fout;
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
        fout << n << '\n';
        rest = n;
        len = 0;
        for (int i = 0; i < n / 3; i++) {
            x = gen();
            fout << "1 " << x << '\n';
            rest--;
            len++;
        }
        n = rest;
        for (int i = 0; i < n / 3; i++) {
            fout << "0\n";
            rest--;
            len--;
        }
        n = rest;
        for (int i = 0; i < n; i++) {
            typ = (gen() % 4 + 4) % 4;
            fout << typ;
            if (typ == 0) {
                fout << '\n';
            } else if (typ == 1) {
                x = gen();
                fout << ' ' << x << '\n';
            } else if (typ == 2) {
                id = gen();
                if (id % 2 == 0) {
                    id = (id % len + len) % len;
                }
                fout << ' ' << id << '\n';
            } else if (typ == 3) {
                id = gen();
                if (id % 2 == 0) {
                    id = (id % len + len) % len;
                }
                x = gen();
                fout << ' ' << id << ' ' << x << '\n';
             }
        }
        fout.close();
    }
}
