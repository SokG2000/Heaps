#include <iostream>
#include <string>
#include "vector.hpp"

using std::cout;
using std::cin;

template <typename Key>
class THeap {
    public:
        class Pointer{
            public:
                int* ptr; // указатель на индекс в куче
                Pointer(){}
                Pointer (int* i) {
                    ptr = i;
                }
        };
        THeap () {
            sz = 0;
            k = 2;
            is_optimised = false;
            rest_optimise = -1;
        }
        template <class Iterator>
        THeap(Iterator begin_, Iterator end_) {
            k = 2;
            is_optimised = false;
            rest_optimise = -1;
            Iterator it = begin_;
            while (it != end_) {
                v.push_back(*it);
                ++it;
            }
            sz = v.size();
            for (int i = 0; i < sz; i++) {
                int *tmp = new int;
                *tmp = i;
                back_indexes.push_back(tmp);
            }
            for (int i = sz - 1; i >= 0; i--) {
                down(i);
            }
        }
        int get_size() const {
            return sz;
        }

        bool is_empty() const {
            return sz == 0;
        }

        Key get_min() const {
            if (sz == 0) {
                throw std::out_of_range("Can not get minimum in empty heap.");
            }
            return v[0];
        }

        Pointer insert(Key num) {
            v.push_back(num);
            int *tmp = new int;
            *tmp = sz;
            back_indexes.push_back(tmp);
            sz++;
            up(sz - 1);
            if (is_optimised) {
                rest_optimise--;
                if (rest_optimise == 0) {
                    end_optimisation();
                }
            }
            return Pointer(tmp);
        }

        Key extract_min() {
            if (sz == 0) {
                throw std::out_of_range("Can not extract minimum from empty heap.");
            }
            Key res;
            res = v[0];
            swap_(0, sz - 1);
            pop_back();
            down(0);
            if (is_optimised) {
                rest_optimise--;
                if (rest_optimise == 0) {
                    end_optimisation();
                }
            }
            return res;
        }

        void delete_(const Pointer &p) {
            int i = *(p.ptr);
            bool flag;
            if (v[i] < v[sz - 1]) {
                flag = true;
            } else {
                flag = false;
            }
            swap_(i, sz - 1);
            pop_back();
            if (i == sz) {
                return;
            }
            if (flag) {
                down(i);
            } else {
                up(i);
            }
        }

        void change(Pointer &p, Key new_num) {
            delete_(p);
            p = insert(new_num);
        }

        void optimize(int insertCount, int extractCount) {
            if (sz > 0) {
                throw std::logic_error("Can not optimize heap when it is not empty.");
            }
            k = find_degree(insertCount, extractCount);
            if (k != 2) {
                is_optimised = true;
                rest_optimise = insertCount + extractCount;
            }
        }

    private:
        Vector <int*> back_indexes; // back_indexes[i] - указатель на элемент i
        Vector <Key> v;
        int sz, rest_optimise;
        int k = 2;
        bool is_optimised = false;
        void swap_(int i, int j) {
            back_indexes.swap_elements(i, j);
            v.swap_elements(i, j);
            *back_indexes[i] = i;
            *back_indexes[j] = j;
        }
        void pop_back() {
            v.pop_back();
            delete back_indexes[sz - 1];
            back_indexes.pop_back();
            sz--;
        }
        void up(int id) {
            Key tmp;
            while (id != 0 && v[id] <= v[(id - 1) / k]) {
                swap_(id, (id - 1) / k);
                id = (id - 1) / k;
            }
        }
        void down(int id) {
            int min_id;
            Key tmp;
            while (id * k + 1 < sz) {
                min_id = id;
                for (int i = id * k + 1; i < id * k + k + 1 && i < sz; i++) {
                    if (v[i] < v[min_id]) {
                        min_id = i;
                    }
                }
                if (min_id == id) {
                    return;
                } else {
                    swap_(id, min_id);
                    id = min_id;
                }
            }
        }
        void decrease(const Pointer &p, Key num) {
            int i = *(p.ptr);
            if (v[i] < num) {
                return;
            }
            v[i] = num;
            up(i);
        }
        int find_degree(int insertCount, int extractCount) {
            if (extractCount == 0) {
                extractCount++;
            }
            int res = insertCount / extractCount;
            if (res < 2) {
                res = 2;
            }
            return res;
        }
        int end_optimisation() {
            k = 2;
            is_optimised = false;
            rest_optimise = -1;
            for (int i = sz - 1; i >= 0; i--) {
                down(i);
            }
        }
    public:
        void print() {
            for (int i = 0; i < sz; i++) {
                cout << v[i] << ' ';
            }
            cout << '\n';
        }
};

