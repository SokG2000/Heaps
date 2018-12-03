#include <iostream>
//#pragmaous

using std::cout;
using std::cin;

template <typename Key>
class Vector {
    class Iterator {
        int index;
        Vector *v;
        public:
        Iterator (Vector <Key> *v, int i) {
            this->v = v;
            index = i;
        }
        Key operator * () const {
            if (index < v->sz) {
                return ((*v)[index]);
            }
            //throw std::out_of_range();
        }
        Iterator& operator ++ () {
            this->index++;
            return *this;
        }
        Iterator& operator ++ (int) {
            Iterator it = *this;
            this->index++;
            return it;
        }
        bool operator == (const Iterator &it) const {
            //Vector *tmp == this->v;
            if ((v == it.v) && (index == it.index)) {
                return true;
            }
            return false;
        }
        bool operator != (const Iterator &it) const {
            return !(*this == it);
        }
    };
    int sz;
    int max_sz;
    static const int small = 4, big = 2;
    Key* arr;
    public:
        Iterator begin() {
            return Iterator(this, 0);
        }
        Iterator end() {
            return Iterator(this, sz);
        }
        Vector(int n) {
            sz = n;
            max_sz = n;
            arr = new Key[n];
        }
        Vector () {
            sz = 0;
            max_sz = 1;
            arr = new Key[1];
        }
        ~Vector() {
            delete[] arr;
        }
        Key& operator[] (int i) {
            if (i < sz && i >= 0) {
                return *(arr + i);
            }
            throw std::out_of_range("Vector index is out of range.");
        }
        const Key& operator[] (int i) const {
            if (i < sz && i >= 0) {
                return *(arr + i);
            }
            throw std::out_of_range("Vector index is out of range.");
        }
        void push_back(Key x) {
            if (sz == max_sz) {
                reallocation(max_sz * big);
            }
            *(arr + sz) = x;
            sz++;
        }
        void pop_back() {
            if (sz == 0) {
                throw std::out_of_range("Can not delete from empty vector.");
            }
            sz--;
            if (sz < max_sz / small) {
                reallocation(sz * big);
            }
        }
        int size() const {
            return sz;
        }
        void swap_elements(int i, int j) {
            Key tmp = *(arr + i);
            *(arr + i) = *(arr + j);
            *(arr + j) = tmp;
        }
        bool is_empty() const {
            return (sz > 0);
        }
    private:
        void reallocation(int new_max_sz) {
            if (new_max_sz == 0) {
                new_max_sz = 1;
            }
            Key* arr1 = new Key[new_max_sz];
            if (sz > new_max_sz) {
                sz = new_max_sz;
            }
            for (int i = 0; i < sz; i++) {
                *(arr1 + i) = *(arr + i);
            }
            delete[] arr;
            arr = arr1;
            max_sz = new_max_sz;
        }
};

template <typename Key>
std::ostream& operator << (std::ostream &out, const Vector <Key> &v) {
    for (int i = 0; i < v.size(); i++) {
        out << v[i] << ' ';
    }
    out << '\n';
}

