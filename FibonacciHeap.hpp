#include <iostream>
#include "Vector.hpp"
using std::cout;
using std::cerr;

template <typename Key>
class FibonacciHeap {
    class Node{
        public:
            int x;
            Node *parent;
            Node *child;
            Node *left_brother;
            Node *right_brother;
            Node **back_pointer;
            Key value;
            int rang;
            bool marked;
            Node(Key num) {
                parent = nullptr;
                child = nullptr;
                left_brother = this;
                right_brother = this;
                value = num;
                rang = 0;
                back_pointer = nullptr;
                marked = false;
            }
    };
    int sz;
    Node* min_node;
    public:
        class Pointer{
            public:
                Node* node;
                Pointer() {}
                Pointer(Node* node_) {
                    node = node_;
                }
                Key operator * () const {
                    return node->value;
                }
                void print() const {
                    cout << node << ' ' << node->value << '\n';
                }
        };
        bool is_empty() const {
            return min_node == nullptr;
        }
        int size() const {
            return sz;
        }
        Key get_min() const {
            if (min_node == nullptr) {
                throw std::out_of_range("Can not get minimum in empty heap.");
            }
            return min_node->value;
        }
        FibonacciHeap (Key num) {
            sz = 1;
            min_node = new Node(num);
        }
        FibonacciHeap () {
            sz = 0;
            min_node = nullptr;
        }
        void merge(FibonacciHeap &other_heap) {
            if (other_heap.is_empty()) {
                return;
            }
            if (is_empty()) {
                min_node = other_heap.min_node;
                sz = other_heap.size();
                other_heap.make_empty();
                return;
            }
            merge_lists(min_node, other_heap.min_node);
            if (min_node->value > other_heap.min_node->value) {
                min_node = other_heap.min_node;
            }
            sz += other_heap.size();
            other_heap.make_empty();
            return;
        }
        Pointer insert(Key num) {
            FibonacciHeap<Key> tmpHeap = FibonacciHeap(num);
            Pointer new_node(tmpHeap.min_node);
            merge(tmpHeap);
            return new_node;
        }
        Key extract_min() {
            Key answer;
            if (min_node == nullptr) {
                throw std::out_of_range("Can not extract minimum from empty heap.");
            }
            answer = min_node->value;
            extract_root(min_node);
            return answer;
        }
        void decrease(Pointer p, Key new_value) {
            Node* changing = p.node;
            Node *parent, *current;
            if (changing->value < new_value) {
                throw std::logic_error("Wrong decrease\n");
            }
            changing->value = new_value;
            if (changing->parent != nullptr) {
                changing->marked = true;
            }
            current = changing;
            while (current != nullptr && current->parent != nullptr && current->marked == true) {
                parent = current->parent;
                current->parent = nullptr;
                current->marked = false;
                if (parent->child == current) {
                    if (current->right_brother == current) {
                        parent->child = nullptr;
                        merge_lists(min_node, current);
                    } else {
                        parent->child = current->right_brother;
                        extract_from_list(current);
                        merge_lists(min_node, current);
                    }
                } else {
                    extract_from_list(current);
                    merge_lists(min_node, current);
                }
                parent->rang--;
                current = parent;
            }
            if (current != nullptr) {
                current->marked = true;
            }
            if (changing->value < min_node->value) {
                min_node = changing;
            }
        }
        void delete_(Pointer p) {
            decrease(p, *p);
            extract_root(p.node);
        }
        ~FibonacciHeap() {
            destroy_tree(min_node);
        }
    private:
        void extract_root(Node* root) {
            if (root == nullptr) {
                cerr << "Can not extract null node.\n";
                throw std::logic_error("Can not extract null node.");
            }
            if (root->parent != nullptr) {
                cerr << "Can extract only root.\n";
                //cout << root << ' ' << root->parent << ' ' << root->value << ' ' << root->left_brother << ' ' << root->right_brother << '\n';
                throw std::logic_error("Can extract only root.");
            }
            if (root->child != nullptr) {
                Node* root_child = root->child;
                while(root_child->parent != nullptr) {
                    root_child->parent = nullptr;
                    root_child->marked = false;
                    root_child = root_child->right_brother;
                }
                root->child = nullptr;
                merge_lists(min_node, root_child);
            }
            sz--;
            if (min_node->right_brother == min_node) {
                delete root;
                min_node = nullptr;
            } else {
                if (root == min_node) {
                    Node* last_min = min_node;
                    min_node = min_node->right_brother;
                    extract_from_list(last_min);
                    delete last_min;
                } else {
                    extract_from_list(root);
                    delete root;
                }
                consolidate();
            }
        }
        void consolidate() {
            int max_rang, now_num, prev_num, tmp;
            Node *current_node;
            max_rang = 1;
            now_num = 1;
            prev_num = 0;
            while (now_num < sz) {
                tmp = now_num;
                now_num = now_num + prev_num;
                prev_num = tmp;
                max_rang++;
            }
            Node* roots[max_rang];
            for (int i = 0; i < max_rang; i++) {
                roots[i] = nullptr;
            }
            while(min_node != nullptr) {
                current_node = min_node;
                if (min_node->right_brother == min_node) {
                    min_node = nullptr;
                } else {
                    min_node = min_node->right_brother;
                }
                extract_from_list(current_node);
                while(roots[current_node->rang] != nullptr) {
                    current_node = merge_nodes(current_node, roots[current_node->rang]);
                    roots[current_node->rang - 1] = nullptr;
                }
                roots[current_node->rang] = current_node;
            }
            min_node = nullptr;
            for (int i = 0; i < max_rang; i++) {
                if (roots[i] != nullptr) {
                    if (min_node == nullptr || min_node->value > roots[i]->value) {
                        min_node = roots[i];
                    }
                }
            }
            for (int i = 0; i < max_rang; i++) {
                if (roots[i] != nullptr && roots[i] != min_node) {
                    merge_lists(roots[i], min_node);
                }
            }
        }
        void extract_from_list(Node *node) {
            node->left_brother->right_brother = node->right_brother;
            node->right_brother->left_brother = node->left_brother;
            node->left_brother = node;
            node->right_brother = node;
        }
        void make_empty() {
            sz = 0;
            min_node = nullptr;
        }
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
        void node_push_back(Node *node1, Node *node2) {
            if (node1->child == nullptr) {
                node1->child = node2;
                node2->parent = node1;
                node1->rang++;
            } else {
                merge_lists(node1->child, node2);
                node2->parent = node1;
                node1->rang++;
            }
        }
        void merge_lists(Node *node1, Node* node2) {
            if (node1 == nullptr || node2 == nullptr) {
                cerr << "Can not merge empty list\n";
                throw(0);
            }
            Node *left1, *right2;
            left1 = node1->left_brother;
            right2 = node2->right_brother;
            node1->left_brother = node2;
            node2->right_brother = node1;
            left1->right_brother = right2;
            right2->left_brother = left1;
        }
        void destroy_tree(Node* root) {
            if (root == nullptr) {
                return;
            }
            if (root->child != nullptr) {
                Vector<Node*> children;
                children.push_back(root->child);
                Node* p = root->child->right_brother;
                while (p != children[0]) {
                    children.push_back(p);
                    p = p->right_brother;
                }
                for (int i = 0; i < children.size(); i++) {
                    destroy_tree(children[i]);
                }
            }
            delete root;
        }
    public:
        void print_roots(){
            cout << "roots:\n";
            print_list(min_node);
            cout << "end_roots\n";
        }
        void print_all() {
            cout << "\nHeap information:\n";
            print_tree(min_node);
            cout << "End heap information\n\n";
        }
    private:
        void print_tree(Node* node) {
            print_list(node);
            if (node->child != nullptr) {
                print_tree(node->child);
            }
            for (Node* cur = node->right_brother; cur != node; cur = cur->right_brother) {
                if (cur->child != nullptr) {
                    print_tree(cur->child);
                }
            }
        }
        void print_list(Node* node) {
            cout << node << ' ' << node->left_brother->value << '-' << node->value << '-' << node->right_brother->value << ' ';
            cout << node->parent << ' ';
            if (node->parent != nullptr) {
                cout << node->parent->value << ' ';
            }
            cout << node->child << ' ';
            if (node->child != nullptr) {
                cout << node->child->value << ' ';
            }
            cout << "    ";
            for (Node* cur = node->right_brother; cur != node; cur = cur->right_brother) {
                cout << cur << ' ' << cur->left_brother->value << '-' << cur->value << '-' << cur->right_brother->value << ' ';
                cout << cur->parent << ' ';
                if (cur->parent != nullptr) {
                    cout << cur->parent->value << ' ';
                }
                cout << cur->child << ' ';
                if (cur->child != nullptr) {
                    cout << cur->child->value << ' ';
                }
                cout << "    ";
            }
            cout << '\n';
        }
};
