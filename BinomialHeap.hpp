#include <iostream>
#include "vector.hpp"
//using std::cin;
using std::cout;
using std::cerr;

template <typename Key>
class BinomialHeap {
    class Node{
        public:
            Node *parent, *left_child, *right_child, *left_brother, *right_brother;
            Node **back_pointer;
            Key value;
            int rang;
            Node(Key num) {
                parent = nullptr;
                left_child = nullptr;
                right_child = nullptr;
                left_brother = nullptr;
                right_brother = nullptr;
                value = num;
                rang = 0;
                back_pointer = nullptr;
            }
    };
    public:
        class Pointer{
            public:
                Node** node;
                Pointer() {}
                Pointer(Node* node_) {
                    node = new Node*(node_);
                }
                Key operator * () const {
                    return (*node)->value;
                }
                void print() const {
                    cout << node << ' ' << *node << ' ' << (*node)->value << '\n';
                }
        };
        bool is_empty() const {
            return sz == 0;
        }
        int size() const {
            return sz;
        }
        Key get_min() const {
            if (sz == 0) {
                throw std::out_of_range("Can not get minimum in empty heap.");
            }
            return minimum;
        }
        BinomialHeap (Key num) {
            sz = 1;
            max_rang = 0;
            minimum = num;
            Node* tmp = new Node(num);
            roots.push_back(tmp);
            min_id = 0;
        }
        BinomialHeap () {
            sz = 0;
            max_rang = -1;
        }
        BinomialHeap(Node* node) {
            if (node == nullptr) {
                //cerr << "Impossible build\n";
                throw std::logic_error("Impossible build\n");
            }
            if (node->rang == 0) {
                sz = 0;
                max_rang = -1;
                return;
            }
            max_rang = node->rang - 1;
            Node* current = node->left_child;
            roots.push_back(current);
            sz = 2;
            minimum = current->value;
            current = current->right_brother;
            while (current != nullptr) {
                roots.push_back(current);
                minimum = std::min(current->value, minimum);
                sz *= 2;
                current = current->right_brother;
            }
            for (int i = 0; i < roots.size(); i++) {
                roots[i]->left_brother = nullptr;
                roots[i]->right_brother = nullptr;
                roots[i]->parent = nullptr;
            }
            --sz;
        }
        void merge(BinomialHeap &other_heap) {
            while(other_heap.max_rang >= roots.size()) {
                roots.push_back(nullptr);
            }
            roots.push_back(nullptr);
            Node *current, *removal;
            current = nullptr;
            removal = nullptr;
            for (int i = 0; i < roots.size(); i++) {
                current = removal;
                removal = nullptr;
                if (roots[i] != nullptr) {
                    if (current == nullptr) {
                        current = roots[i];
                    } else {
                        removal = merge_nodes(current, roots[i]);
                        current = nullptr;
                    }
                }
                if (other_heap.max_rang >= i && other_heap.roots[i] != nullptr) {
                    if (current == nullptr) {
                        current = other_heap.roots[i];
                    } else {
                        removal = merge_nodes(current, other_heap.roots[i]);
                        current = nullptr;
                    }
                }
                roots[i] = current;
            }
            if (roots[roots.size() - 1] == nullptr) {
                roots.pop_back();
            }
            max_rang = roots.size() - 1;
            sz += other_heap.sz;
            other_heap.make_empty();
            if (sz > 0) {
                find_minimum();
            }
        }
        Pointer insert(Key num) {
            BinomialHeap <Key> adding_heap(num);
            Node* tmp = adding_heap.roots[0];
            Pointer p(tmp);
            (adding_heap.roots[0])->back_pointer = p.node;
            merge(adding_heap);
            return p;
        }
        Key extract_min() {
            if (sz == 0) {
                throw std::out_of_range("Can not extract minimum from empty heap.");
            }
            Key result = minimum;
            BinomialHeap <Key> minimum_children(roots[min_id]);
            delete roots[min_id];
            roots[min_id] = nullptr;
            sz -= minimum_children.size();
            sz--;
            while(roots.size() > 0 && roots[roots.size() - 1] == nullptr) {
                roots.pop_back();
            }
            max_rang = roots.size() - 1;
            merge(minimum_children);
            return result;
        }
        void delete_ (const Pointer &p) {
            Node* cur_node = *(p.node);
            while(cur_node->parent != nullptr) {
                swap_values(cur_node, cur_node->parent);
                cur_node = cur_node->parent;
            }
            for (int i = 0; i < roots.size(); i++) {
                if (roots[i] == cur_node) {
                    min_id = i;
                }
            }
            extract_min();
        }
        void change(Pointer &p, Key new_num) {
            delete_(p);
            p = insert(new_num);
        }
        ~BinomialHeap() {
            for (int i = 0; i < roots.size(); i++) {
                if (roots[i] != nullptr) {
                    destroy_tree(roots[i]);
                }
            }
        }
    private:
        int sz;
        int max_rang;
        Key minimum;
        int min_id;
        Vector <Node*> roots;
        Node* merge_nodes(Node* node1, Node* node2) {
            if (node1 == nullptr) {
                return node2;
            }
            if (node2 == nullptr) {
                return node1;
            }
            if (node1->rang != node2->rang) {
                throw std::logic_error("Impossible node merge. ");
            }
            if (node1->value <= node2->value) {
                node_push_back(node1, node2);
                return node1;
            } else {
                node_push_back(node2, node1);
                return node2;
            }
        }
        void make_empty() {
            while(roots.size() > 0) {
                roots.pop_back();
            }
            sz = 0;
            max_rang = -1;
        }
        template <typename T> void swap(T& a, T& b) {
            T tmp;
            tmp = a;
            a = b;
            b = tmp;
        }
        void swap_values(Node* node1, Node* node2) {
            if (node1 == nullptr || node2 == nullptr) {
                cerr << "Impossible swap\n";
                throw ("Impossible swap");
            }
            swap(node1->value, node2->value);
            swap(node1->back_pointer, node2->back_pointer);
            if (node1->back_pointer != nullptr) {
                *(node1->back_pointer) = node1;
            }
            if (node2->back_pointer != nullptr) {
                *(node2->back_pointer) = node2;
            }
        }
        void find_minimum() {
            if (max_rang == -1) {
                cerr << "Can not find minimum in empty heap";
                throw ("Can not find minimum in empty heap");
            }
            min_id = max_rang;
            minimum = roots[min_id]->value;
            for (int i = 0; i < max_rang; i++) {
                if (roots[i] != nullptr && roots[i]->value < minimum) {
                    min_id = i;
                    minimum = roots[min_id]->value;
                }
            }
        }
        void node_push_back(Node* node1, Node* node2) {
            if (node1 == node2) {
                cerr << "Can not push_back equal node\n";
                throw ("Can not push_back equal node");
            }
            node2->parent = node1;
            if (node1->left_child == nullptr) {
                node1->left_child = node2;
                node1->right_child = node2;
            } else {
                node1->right_child->right_brother = node2;
                node2->left_brother = node1->right_child;
                node1->right_child = node2;
            }
            node1->rang++;
        }
        void destroy_tree(Node* root) {
            if (root == nullptr) {
                return;
            }
            Node* p = root->left_child;
            Node* next;
            while (p != nullptr) {
                next = p->right_brother;
                destroy_tree(p);
                p = next;
            }
            destroy_node(root);
        }
        void destroy_node(Node* node) {
            if (node == nullptr) {
                return;
            }
            if (node->back_pointer != nullptr) {
                delete node->back_pointer;
            }
            delete node;
        }
    public:
        void print_roots() {
            for (int i = 0; i < roots.size(); i++) {
                cout << roots[i];
                if (roots[i] != nullptr) {
                    cout << ' ' << (roots[i])->value;
                }
                cout << "   ";
            }
            cout << '\n';
        }
        void print() {
            cout << "\ntree_inf: size = " << sz << ", max_rang = " << max_rang;
            if (sz > 0) {
                cout << ", minimum = " << minimum;
            }
            cout <<  "\nroots:";
            print_roots();
            for (int i = 0; i < roots.size(); i++) {
                if (roots[i] != nullptr) {
                    print_tree(roots[i]);
                    cout << '\n';
                }
            }
            cout << "end tree\n";
        }
        void print_values() {
            for (int i = 0; i < roots.size(); i++) {
                if (roots[i] != nullptr) {
                    print_tree_values(roots[i]);
                }
            }
            cout << '\n';
        }
    private:
        void print_node_inf(Node* node) {
            cout << "adress = " << node;
            if (node == nullptr) {
                cout << "\n";
                return;
            }
            cout << ", rang = " << node->rang << ", value = " << node->value << ";  ";
            cout << "parent = " << node->parent << ", left_son = " << node->left_child << ", right_son = " << node->right_child << ", left_br = " << node->left_brother << ", rigth_br = " << node->right_brother << '\n';
        }
        void print_tree(Node* node) {
            print_node_inf(node);
            for(Node* p = node->left_child; p != nullptr; p = p->right_brother) {
                print_tree(p);
            }
        }
        void print_tree_values(Node* node) {
            cout << node->value << ' ';
            for(Node* p = node->left_child; p != nullptr; p = p->right_brother) {
                print_tree_values(p);
            }
        }
};
